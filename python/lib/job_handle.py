import multiprocessing as mp
import time 
import os
class JobHandle(mp.Process):
    '''This adds some basic handling of a mp.Process. This gives a
    unix make like dependency, which checks the status of children
    when they finish and trigger an error, and also uses a queue to
    notify other interested classes when a child finishs (to allow for
    make like dependencies between objects).'''

    pool = dict()
    maxpool = mp.cpu_count()
    next_id = 0
    def __init__(self):
        '''Initialize job. This waits if too many jobs are already running'''
        mp.Process.__init__(self)
        # We want to kill this if the parent dies
        self.daemon = True
        self.job_id = JobHandle.next_id
        self.done_observer = []
        self.is_done = False
        JobHandle.next_id += 1
        while(len(JobHandle.pool) >= JobHandle.maxpool):
            JobHandle.wait()

    def start(self):
        while(len(JobHandle.pool) >= JobHandle.maxpool):
            JobHandle.wait()
        JobHandle.pool[self.job_id] = self
        mp.Process.start(self)
        
    # Version of join that checks exit status
    def join(self):
        if(not self.is_done):
            mp.Process.join(self)
            self.is_done = True
            if(self.exitcode != 0):
                raise RuntimeError("Child process failed")

    @staticmethod
    def wait_all_done():
        '''Wait until all jobs are done'''
        while(len(JobHandle.pool) > 0):
            JobHandle.wait()

    # Wait for the next process to finish
    @staticmethod
    def wait():
        '''Wait until the next job finishes.'''
        while(True):
            for id, obj in JobHandle.pool.iteritems():
                if(not obj.is_alive()):
                    del JobHandle.pool[id]
                    if(obj.exitcode != 0):
                        raise RuntimeError("Child process failed")
                    obj.notify_done()
                    return
            time.sleep(0.1)

    def add_done_observer(self, obj):
        '''Add an object as interested when this object finishs processing'''
        self.done_observer.append(obj)

    def process_is_done(self, process):
        '''Called when a process we are watching is done'''
        pass # Default is do nothing.

    def notify_done(self):
        '''Called to notify observers when we are done with this process.'''
        self.is_done = True
        for i in self.done_observer:
            i.process_is_done(self)

class JobDependent(JobHandle):
    '''A common scenario is a job that needs to wait on the output of other
    jobs. This class handles this common class, it takes a list of other
    JobHandle that must finish before this one starts.

    Derived classes may want to call process_is_done is their constructor,
    to handle the case of all the dependencies already being satisfied.'''
    def __init__(self, depend_on):
        JobHandle.__init__(self)
        self.depend_on = depend_on
        for i in self.depend_on:
            i.add_done_observer(self)

    def process_is_done(self, process):
        ready_run = True
        for i in self.depend_on:
            if(not i.is_done):
                ready_run = False
        if(ready_run):
            self.start()

    def run_if_needed(self, file):
        '''This checks to see if the given file exists, and if not then it
        starts this process to generate the file'''
        if(file and os.path.exists(file)):
            self.is_done = True
        else:
            self.process_is_done(None)
        
