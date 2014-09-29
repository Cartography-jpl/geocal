import os
import pickle
import sqlite3
import time 
import signal
import subprocess
from misc import pid_exists, makedirs_p
import logging
import datetime

db_save = None
job_id_save = None
def _signal_handle(signum, stack):
    db_save.execute('''
update job_status SET status='interrupted', 
modify_time=strftime('%Y-%m-%d %H:%M:%f', 'now'),
pid=null,
job_start_time=null where job_id=?
''', (job_id_save,))
    db_save.commit()
    
class JobDatabase(object):
    '''This handles a simple database used for job management, 
    checking and starting jobs. This is a very simple job management 
    software, it is meant to be something like PBS but much simpler.

    We may well replace this at some point in the future with actual
    job management software, this is primarily meant for use as a quick 
    an dirty system for use with the ABCD system.'''
    def __init__(self, db_name):
        '''This create a handle for the JobDatabase. You pass in the 
        name of the sqlite file to use.

        If the database file doesn't already exist, we create it. If
        the table 'job_status' doesn't exist, we create it.'''
        self.db_name = db_name
        self.db = sqlite3.connect(db_name)
        self.db.row_factory = sqlite3.Row        
        self.db.execute('''
CREATE TABLE IF NOT EXISTS job_status
(job_id INTEGER PRIMARY KEY ASC, working_directory text not null, creation_time datetime not null, modify_time datetime not null, status text not null, pid INTEGER, job_start_time datetime, job_to_run BLOB)
''')
        
    def __del__(self):
        self.close()

    def close(self):
        '''Commit changes and close the file'''
        if self.db is not None:
            self.db.commit()
            self.db.close()
            self.db = None

    def add_job(self, working_dir, log_file, job_to_run, 
                start_if_available = True):
        '''Submit a job to run. You supply the working directory (can
        be relative if desired, we call os.path.abspath on this), and
        a job to run. The job should be an array like what can be
        passed to subprocess.check_call.

        Note that by default we check and see if the job can be run, and
        if so the job is started (asynchronously, so we don't wait for
        it to finish). You can turn is off is desired by passing
        'start_if_available' as False.

        This return the job id of the submitted job.'''
        cur = self.db.execute('''
INSERT INTO job_status VALUES(null, ?, strftime('%Y-%m-%d %H:%M:%f', 'now'),
strftime('%Y-%m-%d %H:%M:%f', 'now'), 'queued', null, null, ?)''', 
                        (os.path.abspath(working_dir), 
                         pickle.dumps(job_to_run)))
        self.db.commit()
        with open(log_file, "a") as fh:
            print >>fh, "%s INFO:geocal-python.job_database Added job %d to the job queue" % (datetime.datetime.now().isoformat(), cur.lastrowid)
        if(start_if_available):
            subprocess.call("job_database_check_and_run %s %s &" %
                            (self.db_name, log_file), shell = True)
        return cur.lastrowid

    def abcd_job(self, working_dir, log_file, 
                 pre_pan, post_pan,
                 pre_ms = None, post_ms = None, subset = None,
                 resolution = 0.5, number_process=1, dem_file = None,
                 start_if_available = True):
        '''Submit an abachd job. 

        You need to supply a working direction, and a pre and post 
        panchromatic image. You can optionally also supply 
        multispectral pre and/or post. You can optionally specify 
        a subset to use, this should be an array of 4 values - start line, 
        start sample, number line, number sample.

        You can give the resolution (in meters) for the output. The default
        is 0.5 meter. You can supply the number of processors to use,
        the default is 1. You can supply a DEM file to use, the default is 
        to use the SRTM.
        '''
        cmd = ["abachd"]
        args = [ pre_pan ]
        if(post_ms is not None):
            args.extend([post_pan, post_ms])
        elif(pre_ms is not None):
            cmd.append("--pre-ms")
            args.extend([pre_ms, post_pan])
        else:
            cmd.append("--no-ms")
            args.append(post_pan)
        if(subset is not None):
            cmd.extend(["--subset", str(subset[0]), str(subset[1]),
                        str(subset[2]), str(subset[3])])
        args.append(working_dir)
        cmd.append("--diagnostic")
        cmd.append("--verbose")
        cmd.append("--resolution=%f" % resolution)
        cmd.append("--number-process=%d" % number_process)
        if(dem_file is not None):
            cmd.append("--dem-file=%s" % dem_file)
        cmd.extend(args)
        return self.add_job(working_dir, log_file, cmd, 
                            start_if_available = start_if_available)

    def __getitem__(self, job_id):
        '''Return information about a job with the given job_id.'''
        r = self.db.execute("select * from job_status where job_id=?",
                            (job_id,)).fetchone()
        if(r is None):
            raise KeyError(job_id)
        return r

    def keys(self):
        '''Return the list of jobs ids.'''
        return [row[0] for row in
                self.db.execute("select job_id from job_status")]

    def __delitem__(self, job_id):
        self.db.execute("DELETE FROM job_status WHERE job_id=?", [job_id])
        log = logging.getLogger("geocal-python.job_database")
        log.info("Removing job %d from the job queue" % job_id)

    def set_status(self, job_id, status):
        '''Set a job id to the given status.'''
        if(status == "running"):
            self.db.execute('''
update job_status SET status='running', 
modify_time=strftime('%Y-%m-%d %H:%M:%f', 'now'),
pid=?,
job_start_time=strftime('%Y-%m-%d %H:%M:%f', 'now') where job_id=?
''', (os.getpid(), job_id))
        else:
            self.db.execute('''
update job_status SET status=?, 
modify_time=strftime('%Y-%m-%d %H:%M:%f', 'now'),
pid=null,
job_start_time=null where job_id=?
''', (status, job_id))

        self.db.commit()
        
    def check_and_run(self):
        '''This checks if:

        1. There is currently no running job
        2. There is a job in the queue read to run.

        If so, this starts the job, and waits for it to finish before
        returning. We then update the state of the job to:

        1. "success"
        2. "failure" (error occurred)
        3. "interrupted" (if a signal occurred during the run)

        Return true if a job was run, false otherwise.
        '''
        log = logging.getLogger("geocal-python.job_database")
        for r in self.db.execute('''
select job_id, pid from job_status where status = 'running'
'''):
            if(pid_exists(r["pid"])):
                # Job running, so don't start another one
                return False
            log.info("Job %d had a run status, but the process %d is not on the system. Apparently this died, so marking job as 'interrupted" % (r["job_id"], r["pid"]))
        # We may have had the job disappear (e.g., a SIGKILL occurred).
            self.db.execute('''
update job_status SET status='interrupted', 
modify_time=strftime('%Y-%m-%d %H:%M:%f', 'now'),
pid=null,
job_start_time=null where job_id=?
''', (r["job_id"],))
            self.db.commit()
        
            
        # Find next job (if any) that can be run
        r = self.db.execute('''
select job_id, job_to_run, working_directory from job_status 
where status = 'queued' 
order by job_id
''').fetchone()
        if(r is None):
            return False
        old_signal = {}
        global db_save
        global job_id_save
        db_save = self.db
        job_id_save = r["job_id"]
        siglist = [signal.SIGHUP, signal.SIGINT, signal.SIGQUIT, signal.SIGTERM]
        try:
            for s in siglist:
                old_signal[s] = signal.signal(s, _signal_handle)
            self.set_status(r["job_id"], "running")
            makedirs_p(r["working_directory"])
            with open(r["working_directory"] + "/job.log", "w") as joblog:
                log.info("Running job %d" % r["job_id"])
                subprocess.check_call(pickle.loads(r["job_to_run"]),
                                      stdout=joblog,
                                      stderr=subprocess.STDOUT)
            self.set_status(r["job_id"], "success")
            log.info("Job %d ran successfully" % r["job_id"])
        except subprocess.CalledProcessError:
            self.set_status(r["job_id"], "failure")
            log.info("Job %d failed" % r["job_id"])
        finally:
            for s in siglist:
                if(s in old_signal):
                    signal.signal(s, old_signal[s])
        return True
