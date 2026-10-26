from __future__ import annotations

import os
import shutil
import sys
import time
import traceback
import typing
from io import StringIO
from threading import RLock
from typing import Any

if typing.TYPE_CHECKING:
    from multiprocessing.pool import Pool

    from .target import Target


class Task:
    """This class is a interface class describing a 'Task'. This is inspired
    by the Luigi package (see https://luigi.readthedocs.io/en/stable/index.html
    However we don't actually use the luigi packages, it turns out that we
    run differently enough that there is no real advantage to using that
    library.

    A Task is anything that can be run to generate output. We specify other
    tasks that it requires to provide input before we can run."""

    def __init__(self, local_dir: str | None = None, force: bool = False) -> None:
        """Initialize with the given local directory used in the final
        cleanup.

        You can pass force=True to force the task to run, even if the output
        already exists."""
        self.local_dir: str | None = None
        if local_dir is not None:
            self.local_dir = os.path.abspath(local_dir)
        self.force = force

    def run(self) -> None:
        '''Do the work of the Task. Note you don't need to worry about doing
        any cleanup here if an error occurs, just raise an exception and the
        rest of the Task infrastructure will handle this.

        Most tasks will produce output files (given by output). However, some
        tasks just calculate other tasks to run. For this second category, run
        can update what is returned by "run_after_tasks"'''

    def tasks_to_run_after(self) -> list[Task]:
        """Return list of tasks to run after this one. May be calculated by
        "run".

        Note that in some cases we may want to *update* a later task already in
        the task list. So if this returns a task with the same
        task_unique_name as an existing task, we update the existing task.
        This can be useful if we for example calculate additional requires()
        tasks for an existing task.
        """
        return []

    def requires(self) -> list[Task]:
        """Return a list of Tasks we depend on. Note that these are *not*
        Targets, instead this is the Tasks that can generate the Targets we
        require as input."""
        return []

    def output(self) -> list[Target]:
        """Return a list of output Targets we generate when we successfully
        run.

        Note that by default we use the first listed output to match
        tasks that appear twice in a dependency tree (e.g., task D
        depends on both B and C, which each depend on A. A naive
        dependency tree would try to run A twice. So the first output
        file should be unique. And in addition, it should return the
        same value each time it is called. Alternatively, you can just
        override task_unique_name to give a different unique name.

        Also, for some tasks we don't know the full output list until
        after we have run the task. It is ok for the output list to
        get extended by run, however we should have at least one file
        in the list to use for the task_unique_name (or alternatively
        you can just override task_unique_name to generate a unique
        name is a different manner).
        """
        return []

    def remove_output(self) -> None:
        """Remove all output."""
        for f in self.output():
            f.remove()

    def remove_process_level(self, level: str) -> None:
        """Remove all output from a given process level on. Currently this
        just supports 'sba', which cleans the results of the sba and
        everything that follows. This is really just meant as a way to redo
        the sba process after something changes (e.g, removing a tiepoint),
        but we could potential find other uses for this in the future.

        Default is to do nothing."""
        if level != "sba":
            raise RuntimeError(
                "Right now remove_process_level only supports level='sba'"
            )
        # Do nothing else

    def task_unique_name(self) -> str:
        """Return a unique name that we can use to identify this task.

        By default we return file name for the first output. This can often be
        used to uniquely identify a task. There may be special cases where
        this doesn't work (e.g., there is no output), so you may need to
        override this function in derived classes."""
        return self.output()[0].filename()

    def output_exists(self) -> bool:
        """Return True if all the output exists, False otherwise"""
        for target in self.output():
            if not target.exists():
                return False
        return True

    def output_error_exists(self) -> bool:
        """Return True if any of the possible output exists in error form,
        False otherwise"""
        for target in self.output():
            if target.error_exists():
                return True
        return False

    def task_need_run(self) -> bool:
        """Return True if the task needs to be run. Default is just to
        run this unless the output_exists"""
        return self.force or not self.output_exists()

    def run_pipeline(
        self, pool: Pool | None = None, skip_cleanup_on_error: bool = False
    ) -> None:
        """Run all the precursors etc. to generate the output for this task.

        Normally we remove the temporary local directory in all cases. If you
        want this left in place if an error occurs specify
        skip_cleanup_on_error=True. This is useful for being able to debug
        problems."""
        Task.run_pipeline_arr(
            [self], pool=pool, skip_cleanup_on_error=skip_cleanup_on_error
        )

    @staticmethod
    def run_pipeline_arr(
        task_list: list[Task],
        pool: Pool | None = None,
        skip_cleanup_on_error: bool = False,
    ) -> None:
        """Run a pipeline over a list of Tasks. The advantage of this
        function vs. just calling run_pipeline on each task is that we
        can keep the multiprocessing.Pool fully used, starting a second task
        before the first has completed."""
        tr = TaskRunner(task_list, pool, skip_cleanup_on_error=skip_cleanup_on_error)
        tr.run()

    def _run_with_finish(self) -> tuple[str, list[Task]]:
        """This is an internal function. We run the task, then make sure
        all the output Targets have "finish" or "finish_error" called."""
        try:
            self.run()
            for f in self.output():
                f.finish()
            return self.task_unique_name(), self.tasks_to_run_after()
        except BaseException as e:
            tp, v, tb = sys.exc_info()
            for f in self.output():
                try:
                    f.finish_error()
                except BaseException:  # noqa: BLE001, S110
                    pass  # Ignore errors in cleaning up error
            fh = StringIO()
            traceback.print_exception(tp, v, tb, file=fh)
            # Attach information to the exception, so the error callback
            # can report it (this also passes through a multiprocessing Pool)
            e.task_unique_name = self.task_unique_name()  # type: ignore[attr-defined]
            # For some reason, __traceback__ doesn't go through when called
            # by pool. Save this information
            e.print_exception_string = fh.getvalue()  # type: ignore[attr-defined]
            raise


class TaskRunner:
    """This is a helper class for Task that actually handled running it
    and the pipeline needed to create it."""

    def __init__(
        self,
        task_list: list[Task],
        pool: Pool | None,
        skip_cleanup_on_error: bool = False,
    ) -> None:
        # Lock needed to avoid race conditions
        self.lock = RLock()
        self.skip_cleanup_on_error = skip_cleanup_on_error
        self.task_list = task_list
        self.tasks_to_run: dict[str, dict[str, Any]] = {}
        for task in task_list:
            self.process_task(task)
        self.pool = pool
        # How often we check if we can run a new job
        self.polling_time = 0.1
        # Exceptions from Task._run_with_finish, which have task_unique_name
        # and print_exception_string attached
        self.error: list[Any] = []

    def requires_satisfied(self, task: Task) -> bool:
        """True if all the required input has been generated, False
        otherwise. We check both that the required task successfully ran (if
        it is in the list of tasks to run) and that all the output exists."""
        with self.lock:
            for rtsk in task.requires():
                if rtsk.task_unique_name() in self.tasks_to_run:
                    t = self.tasks_to_run[rtsk.task_unique_name()]
                    if t["state"] != "done":
                        return False
            for rtsk in task.requires():
                if not rtsk.output_exists():
                    return False
            return True

    def process_task(self, task: Task, force_update: bool = False) -> None:
        with self.lock:
            if not task.task_need_run():
                return
            if not force_update and task.task_unique_name() in self.tasks_to_run:
                return
            self.tasks_to_run[task.task_unique_name()] = {
                "task": task,
                "state": "not_started",
            }
            for tsk in task.requires():
                self.process_task(tsk)

    def number_task_left(self) -> int:
        """Return the number of processes we have left to finish up with."""
        with self.lock:
            return sum(
                1
                for k, t in self.tasks_to_run.items()
                if t["state"] not in ("done", "error")
            )

    def number_task_running(self) -> int:
        """Return the number of processes we have running."""
        with self.lock:
            return sum(
                1 for k, t in self.tasks_to_run.items() if t["state"] == "running"
            )

    def next_task_to_process(self) -> Task | None:
        """Find the next task that hasn't been run yet, but has all the
        required input ready."""
        with self.lock:
            for t in self.tasks_to_run.values():
                if t["state"] == "not_started" and self.requires_satisfied(t["task"]):
                    return t["task"]
            return None

    def run_callback(self, x: tuple[str, list[Task]]) -> None:
        with self.lock:
            self.task_has_finished = True
            ofname, tsklist = x
            for tsk in tsklist:
                self.process_task(tsk, force_update=True)
            self.tasks_to_run[ofname]["state"] = "done"

    def run_callback_error(self, e: Any) -> None:
        with self.lock:
            self.task_has_finished = True
            self.tasks_to_run[e.task_unique_name]["state"] = "error"
            self.tasks_to_run[e.task_unique_name]["exception"] = e
            self.error.append(e)
            print("======== Task failed =======")
            print(f"Failure while trying to generate {e.task_unique_name}")
            print("Error:")
            print(e.print_exception_string)
            print("======= Waiting for pipeline to finish =======")

    def run_next_tasks(self) -> None:
        with self.lock:
            task = self.next_task_to_process()
            # Make sure we have at least one task to process, or still
            # running
            if task is None and self.number_task_running() == 0:
                # raise RuntimeError("Pipeline can't be completed. Task_to_run: %s" % self.tasks_to_run)
                print("======= Pipeline can't be completed =======")
                raise RuntimeError("Pipeline can't be completed.")

            # Just run the task, if we don't have a pool
            if self.pool is None:
                # Without a pool nothing is running in the background, so
                # we raised an error above if there is no task
                assert task is not None
                try:
                    res = task._run_with_finish()
                    self.run_callback(res)
                except BaseException as e:  # noqa: BLE001
                    # Pass any error on, like a pool would
                    self.run_callback_error(e)
                return

            # Otherwise, start as many tasks as we can
            while task is not None:
                self.tasks_to_run[task.task_unique_name()]["state"] = "running"
                self.pool.apply_async(
                    task._run_with_finish,
                    (),
                    {},
                    self.run_callback,
                    self.run_callback_error,
                )
                task = self.next_task_to_process()

    def run(self) -> None:
        try:
            # Optimization to avoid polling run_text_task constantly,
            # Only check when the number of tasks left has changed, e.g,
            # something has finished since the last time through the
            # loop.
            self.task_has_finished = True
            while self.number_task_left() != 0:
                with self.lock:
                    if self.task_has_finished:
                        self.task_has_finished = False
                        self.run_next_tasks()
                time.sleep(self.polling_time)
        finally:
            if len(self.error) > 0:
                print("======= Pipeline failed =======")
                print("Summary of errors:")
                for i, e in enumerate(self.error):
                    print(f"Error {i + 1:d}")
                    print("------------------------")
                    print(f"Failure while trying to generate {e.task_unique_name}")
                    print(e.print_exception_string)
                    print("------------------------")
            for task in self.task_list:
                # Leave in place if we end on an error and user
                # requests this not to be cleaned up
                if task.local_dir is not None and (
                    not self.skip_cleanup_on_error or len(self.error) == 0
                ):
                    shutil.rmtree(task.local_dir, True)
            if len(self.error) > 0:
                raise RuntimeError("Pipeline failed")


__all__ = ["Task", "TaskRunner"]
