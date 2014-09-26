from nose.tools import *
from job_database import *
from misc import *
import logging

def test_job_database():
    jdb = JobDatabase("test.db")
    for jid in jdb.keys():
        del jdb[jid]
    jid = jdb.add_job("job_database_dir", ["ls"])
#    assert jdb.check_and_run()
#    assert jdb[jid]["status"] == "success"

def test_handle_running_job():
    jdb = JobDatabase("test.db")
    for jid in jdb.keys():
        del jdb[jid]
    jid_running = jdb.add_job("job_database_dir", ["ls"])
    jdb.set_status(jid_running, "running")
    jid_could_run = jdb.add_job("job_database_dir", ["ls"])
    assert not jdb.check_and_run()

def test_handle_interrupted_job():
    jdb = JobDatabase("test.db")
    for jid in jdb.keys():
        del jdb[jid]
    jid_running = jdb.add_job("job_database_dir", ["ls"])
    jdb.set_status(jid_running, "running")
    # Fake a PID no longer running
    jdb.db.execute("update job_status set pid=-1 where job_id=?", (jid_running,))
    jdb.db.commit()
    jid_could_run = jdb.add_job("job_database_dir", ["ls"])
    assert jdb.check_and_run()
    assert jdb[jid_running]["status"] == "interrupted"
    assert jdb[jid_could_run]["status"] == "success"

def test_handle_failed_job():
    jdb = JobDatabase("test.db")
    for jid in jdb.keys():
        del jdb[jid]
    jid = jdb.add_job("job_database_dir", ["ls", "--bad-argument"])
    assert jdb.check_and_run()
    assert jdb[jid]["status"] == "failure"
