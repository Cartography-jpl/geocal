import os
import sqlite3

class JobDatabase(object):
    '''This handles a simple database used for job management, 
    checking and starting jobs. This is a very simple job management 
    software, it is meant to be something like PBS but much simpler.

    We may well replace this at some point in the future with actual
    job management software, this is primarily meant for use as a quick 
    an dirty system for use with the ABCD system.'''
    def __init__(self, db_name = None):
        '''This create a handle for the JobDatabase. You can pass in the 
        name of the sqlite file to use, or if you leave this as 'None' we
        look for the environment variable 'AFIDS_JOB_STATUS_DATABASE'.

        If the database file doesn't already exist, we create it. If
        the table 'job_status' doesn't exist, we create it.'''
        if(db_name is None):
            db_name = os.environ["AFIDS_JOB_STATUS_DATABASE"]
        self.db = sqlite3.connect(db_name)
        self.db.execute('''
CREATE TABLE IF NOT EXISTS job_status
(job_id INTEGER PRIMARY_KEY, working_directory text not null, creation_time datetime not null, modify_time datetime not null, job_status text not null, pid INTEGER, job_start_time datetime, job_to_run BLOB)
''')
        
    def __del__(self):
        self.close()

    def close(self):
        '''Commit changes and close the file'''
        if self.db is not None:
            self.db.commit()
            self.db.close()
            self.db = None
