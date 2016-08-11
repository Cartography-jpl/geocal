import shutil
import time
import subprocess
import sys
import os
from queue import Queue

class Task(object):
    '''This class is a interface class describing a 'Task'. This is inspired
    by the Luigi package (see https://luigi.readthedocs.io/en/stable/index.html
    However we don't actually use the luigi packages, it turns out that we 
    run differently enough that there is no real advantage to using that 
    library.

    A Task is anything that can be run to generate output. We specify other
    tasks that it requires to provide input before we can run.'''

    def __init__(self, local_dir=None, force=False):
        '''Initialize with the given local directory used in the final
        cleanup.

        You can pass force=True to force the task to run, even if the output
        already exists.'''
        if(local_dir is not None):
            self.local_dir = os.path.abspath(local_dir)
        else:
            self.local_dir = None
        self.force = force
        
    def run(self):
        '''Do the work of the Task. Note you don't need to worry about doing 
        any cleanup here if an error occurs, just raise an exception and the 
        rest of the Task infrastructure will handle this.

        Most tasks will produce output files (given by output). However, some
        tasks just calculate other tasks to run. For this second category, run
        can update what is returned by "run_after_tasks"'''
        pass

    def tasks_to_run_after(self):
        '''Return list of tasks to run after this one. May be calculated by
        "run".  

        Note that in some cases we may want to *update* a later task already in
        the task list. So if this returns a task with the same 
        task_unique_name as an existing task, we update the existing task.
        This can be useful if we for example calculate additional requires()
        tasks for an existing task.
        '''
        return []

    def requires(self):
        '''Return a list of Tasks we depend on. Note that these are *not*
        Targets, instead this is the Tasks that can generate the Targets we 
        require as input.'''
        return []

    def output(self):
        '''Return a list of output Targets we generate when we successfully
        run. 

        Note that by default we use the first listed output to match
        tasks that appear twice in a dependency try (e.g., task D
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
        '''
        return []

    def remove_output(self):
        '''Remove all output.'''
        for f in self.output():
            f.remove()

    def remove_process_level(self, level):
        '''Remove all output from a given process level on. Currently this
        just supports 'sba', which cleans the results of the sba and 
        everything that follows. This is really just meant as a way to redo
        the sba process after something changes (e.g, removing a tiepoint),
        but we could potential find other uses for this in the future.

        Default is to do nothing.'''
        if(level != "sba"):
            raise RuntimeError("Right now remove_process_level only supports level='sba'")
        # Do nothing else

    def task_unique_name(self):
        '''Return a unique name that we can use to identify this task.

        By default we return file name for the first output. This can often be 
        used to uniquely identify a task. There may be special cases where
        this doesn't work (e.g., there is no output), so you may need to
        override this function in derived classes.'''
        return self.output()[0].filename()

    def output_exists(self):
        '''Return True if all the output exists, False otherwise'''
        for target in self.output():
            if not target.exists():
                return False
        return True

    def output_error_exists(self):
        '''Return True if any of the possible output exists in error form, 
        False otherwise'''
        for target in self.output():
            if target.error_exists():
                return True
        return False

    def task_need_run(self):
        '''Return True if the task needs to be run. Default is just to
        run this unless the output_exists'''
        return self.force or not self.output_exists()

    def run_pipeline(self, pool = None, skip_cleanup_on_error=False):
        '''Run all the precursors etc. to generate the output for this task.

        Normally we remove the temporary local directory in all cases. If you
        want this left in place if an error occurs specify 
        skip_cleanup_on_error=True. This is useful for being able to debug
        problems.'''
        Task.run_pipeline_arr([self], pool=pool, 
                              skip_cleanup_on_error=skip_cleanup_on_error)

    @staticmethod
    def run_pipeline_arr(task_list, pool = None, 
                         skip_cleanup_on_error=False):
        '''Run a pipeline over a list of Tasks. The advantage of this
        function vs. just calling run_pipeline on each task is that we
        can keep the multiprocessing.Pool fully used, starting a second task
        before the first has completed.'''
        tr = TaskRunner(task_list, pool, 
                        skip_cleanup_on_error=skip_cleanup_on_error)
        tr.run()
        
    def _run_with_finish(self):
        '''This is an internal function. We run the task, then make sure
        all the output Targets have "finish" or "finish_error" called.'''
        try:
            self.run()
            for f in self.output():
                f.finish()
            return self.task_unique_name(), self.tasks_to_run_after()
        except BaseException as e:
            for f in self.output():
                try:
                    f.finish_error()
                except BaseException:
                    pass        # Ignore errors in cleaning up error
            e.task_unique_name = self.task_unique_name()
            raise e

class TaskRunner(object):
    '''This is a helper class for Task that actually handled running it
    and the pipeline needed to create it.'''
    def __init__(self, task_list, pool, 
                 skip_cleanup_on_error=False):
        self.skip_cleanup_on_error = skip_cleanup_on_error
        self.task_list = task_list
        self.tasks_to_run = {}
        # We have to be a little careful about new tasks, since these
        # Can come asynchronously, so we don't want to directly modify
        # task_to_run (imagine looping over this dictionary when we get
        # interrupted by a callback that adds new tasks). We stick these
        # into a Queue, which is then processed in our main loop.
        self.new_or_updated_task = Queue()
        for task in task_list:
            self.process_task(task)
        self.pool = pool
        # How often we check if we can run a new job
        self.polling_time = 0.1
        self.error = None

    def requires_satisfied(self, task):
        '''True if all the required input has been generated, False 
        otherwise. We check both that the required task successfully ran (if
        it is in the list of tasks to run) and that all the output exists.'''
        for rtsk in task.requires():
            if rtsk.task_unique_name() in self.tasks_to_run:
                t = self.tasks_to_run[rtsk.task_unique_name()]
                if(t["state"] != "done"):
                    return False
        for rtsk in task.requires():
            if not rtsk.output_exists():
                return False
        return True
    
    def process_task(self, task, force_update=False):
        if(not task.task_need_run()):
            return
        if not force_update and task.task_unique_name() in self.tasks_to_run:
            return
        self.tasks_to_run[task.task_unique_name()] = {"task": task,
                                                     "state" : "not_started"}
        for tsk in task.requires():
            self.process_task(tsk)

    def number_task_left(self):
        '''Return the number of processes we have left to finish up with.'''
        return sum(1 for k,t in self.tasks_to_run.items()
                   if not t["state"] in ("done", "error"))

    def number_task_running(self):
        '''Return the number of processes we have running.'''
        return sum(1 for k,t in self.tasks_to_run.items()
                   if t["state"] == "running")
        
    def next_task_to_process(self):
        '''Find the next task that hasn't been run yet, but has all the 
        required input ready.'''
        for k, t in self.tasks_to_run.items():
            if(t["state"] == "not_started" and
               self.requires_satisfied(t["task"])):
                return t["task"]
        return None

    def run_callback(self, x):
        ofname, tsklist = x
        for tsk in tsklist:
            self.new_or_updated_task.put(tsk)
        self.tasks_to_run[ofname]["state"] = "done"

    def run_callback_error(self, e):
        self.tasks_to_run[e.task_unique_name]["state"] = "error"
        self.tasks_to_run[e.task_unique_name]["exception"] = e
        self.error = e
        
    def run_next_tasks(self):
        task = self.next_task_to_process()
        # Make sure we have at least one task to process, or still
        # running
        if(task is None and self.number_task_running() == 0):
            #raise RuntimeError("Pipeline can't be completed. Task_to_run: %s" % self.tasks_to_run)
            raise RuntimeError("Pipeline can't be completed.")

        # Just run the task, if we don't have a pool
        if(self.pool is None):
            try:
                res = task._run_with_finish()
                self.run_callback(res)
            except BaseException as e:
                self.run_callback_error(e)
            return

        # Otherwise, start as many tasks as we can
        while(task is not None):
            self.tasks_to_run[task.task_unique_name()]["state"] = "running"
            self.pool.apply_async(task._run_with_finish, (), {},
                                  self.run_callback, self.run_callback_error)
            task = self.next_task_to_process()
        
    def run(self):
        try:
            # Optimization to avoid polling run_text_task constantly,
            # Only check when the number of tasks left has changed, e.g,
            # something has finished since the last time through the
            # loop.
            nleft = -1
            while(not self.number_task_left() ==0):
                if(nleft != self.number_task_left()):
                    nleft = self.number_task_left()
                    if(self.error):
                        if(self.number_task_running() == 0):
                            raise self.error
                    self.run_next_tasks()
                time.sleep(self.polling_time)
                while(not self.new_or_updated_task.empty()):
                    self.process_task(self.new_or_updated_task.get(),
                                      force_update=True)
                    nleft = -1
            if(self.error):
                raise self.error
        finally:
            for task in self.task_list:
                if(task.local_dir is not None):
                    # Leave in place if we end on an error and user
                    # requests this not to be cleaned up
                    if(not self.skip_cleanup_on_error or self.error is None):
                        shutil.rmtree(task.local_dir, True)
