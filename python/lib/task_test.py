from __future__ import print_function
from geocal.task import *
from geocal.local_target import *
from test_support import *
import shutil
from multiprocessing import Pool

class TaskA(Task):
    def __init__(self, fname, local_dir):
        super().__init__(local_dir)
        self.output_list = [ OutLocalTarget(fname, local_dir), ]

    def run(self):
        for target in self.output_list:
            with open(target.local_filename(), "w") as fh:
                print("Hi there A", file=fh)

    def output(self):
        return self.output_list

class TaskAFailed(Task):
    def __init__(self, fname, local_dir):
        super().__init__(local_dir)
        self.output_list = [ OutLocalTarget(fname, local_dir), ]

    def run(self):
        for target in self.output_list:
            with open(target.local_filename(), "w") as fh:
                print("Hi there A", file=fh)
        raise RuntimeError("Oops, an error")

    def output(self):
        return self.output_list
    
class TaskB(Task):
    '''Task that requires a TaskA to run first'''
    def __init__(self, fname_in, fname, local_dir):
        super().__init__(local_dir)
        self.output_list = [ OutLocalTarget(fname, local_dir), ]
        self.require_list = [ TaskA(fname_in, local_dir) ]
        self.fname_in = fname_in

    def requires(self):
        return self.require_list

    def run(self):
        ft = InLocalTarget(self.fname_in, self.local_dir)
        t = open(ft.local_filename(), "r").read()
        for target in self.output_list:
            with open(target.local_filename(), "w") as fh:
                print(t, file=fh)
                print("Hi there B", file=fh)

    def output(self):
        return self.output_list

class TaskC(Task):
    '''Task that requires 2 TaskB and 3rd task A to run first. This is 
    complicated enough that we should be able to fully test everything.'''
    def __init__(self):
        super().__init__("./local_dir")
        self.require_list = []
        self.output_list = [OutLocalTarget("./output/c.out", self.local_dir)]
        self.require_list.append(TaskA("./output/a1.out", self.local_dir))
        self.require_list.append(TaskB("./output/a2.out", "./output/b1.out",
                                       self.local_dir))
        self.require_list.append(TaskB("./output/a1.out", "./output/b2.out",
                                       self.local_dir))
        

    def requires(self):
        return self.require_list

    def run(self):
        ft = InLocalTarget("./output/a1.out", self.local_dir)
        t1 = open(ft.local_filename(), "r").read()
        ft = InLocalTarget("./output/b1.out", self.local_dir)
        t2 = open(ft.local_filename(), "r").read()
        ft = InLocalTarget("./output/b2.out", self.local_dir)
        t3 = open(ft.local_filename(), "r").read()
        for target in self.output_list:
            with open(target.local_filename(), "w") as fh:
                print(t1, file=fh)
                print(t2, file=fh)
                print(t3, file=fh)
                print("Hi there C", file=fh)

    def output(self):
        return self.output_list

class TaskDFailed(Task):
    '''Task that has lots of inputs, 1 of which will fail.'''
    def __init__(self):
        super().__init__("./local_dir")
        self.require_list = []
        self.output_list = [OutLocalTarget("./output/c.out", self.local_dir)]
        for i in range(20):
            self.require_list.append(TaskA("./output/a%d.out" % i,
                                           self.local_dir))
        self.require_list.append(TaskAFailed("./output/a_failed.out",
                                             self.local_dir))
        self.require_list.append(TaskAFailed("./output/a_failed2.out",
                                             self.local_dir))

    def requires(self):
        return self.require_list

    def run(self):
        # Never actually runs
        pass

    def output(self):
        return self.output_list
        
        
@require_rsync
@require_python3
def test_1task_pipeline(isolated_dir):
    '''Test a pipeline with a single task in it.'''
    task = TaskA("./output/a_out.txt", "./local_dir")
    task.run_pipeline(skip_cleanup_on_error=True)
    assert task.output_exists()
    assert not os.path.exists("./local_dir")

@require_rsync
@require_python3
def test_2task_pipeline(isolated_dir):
    '''Test a pipeline with a task that depends on another task in it.'''
    task = TaskB("./output/a_out.txt", "./output/b_out.txt", "./local_dir")
    task.run_pipeline(skip_cleanup_on_error=True)
    assert task.output_exists()
    assert not os.path.exists("./local_dir")
    
@require_rsync
@require_python3
def test_complicated_pipeline(isolated_dir):
    '''Test a more complicated task with a longer dependency chain, including
    duplicate tasks.'''
    task = TaskC()
    # Look in more detail at TaskRunner. Normally don't use this directly,
    # but we'll do that for testing purposes.
    tr = TaskRunner([task], None)
    assert tr.number_task_left() == 5
    tr.run()
    assert tr.number_task_left() == 0
    assert task.output_exists()
    assert not os.path.exists("./local_dir")

@require_rsync
@require_python3
def test_pool(isolated_dir):
    '''Test a more complicated task with a longer dependency chain, including
    duplicate tasks, using a pool to do the processing.'''
    task = TaskC()
    pool = Pool(2)
    task.run_pipeline(pool = pool,skip_cleanup_on_error=True)
    assert task.output_exists()
    assert not os.path.exists("./local_dir")


@require_rsync
@require_python3
def test_pool_failed(isolated_dir):
    '''Test where we have lots of tasks, one of which fails. Make sure
    we catch that failure.'''
    task = TaskDFailed()
    pool = Pool(3)
    with pytest.raises(RuntimeError) as e_info:
        task.run_pipeline(pool = pool)
    assert not os.path.exists("./local_dir")
    assert os.path.exists("./output/a_failed.out.error")

@require_rsync
@require_python3
def test_pool_failed2(isolated_dir):
    '''Test where we have lots of tasks, one of which fails. Make sure
    we catch that failure. This variation uses skip_cleanup_on_error to
    *not* cleanup the test data'''
    task = TaskDFailed()
    pool = Pool(3)
    pool = None
    with pytest.raises(RuntimeError) as e_info:
        task.run_pipeline(pool = pool, skip_cleanup_on_error = True)
    assert os.path.exists("./local_dir")
    assert os.path.exists("./output/a_failed.out.error")
    
    
