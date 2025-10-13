from geocal import JobDatabase
import pytest


def test_job_database(isolated_dir):
    jdb = JobDatabase("test.db")
    for jid in list(jdb.keys()):
        del jdb[jid]
    jid = jdb.add_job(
        "job_database_dir",
        "user",
        "key",
        "job_database.log",
        ["ls"],
        start_if_available=False,
    )
    assert jdb.check_and_run()
    assert jdb[jid]["status"] == "success"
    assert jdb.get_by_key("key")["status"] == "success"


def test_handle_running_job(isolated_dir):
    jdb = JobDatabase("test.db")
    for jid in list(jdb.keys()):
        del jdb[jid]
    jid_running = jdb.add_job(
        "job_database_dir",
        "user",
        "key",
        "job_database.log",
        ["ls"],
        start_if_available=False,
    )
    jdb.set_status(jid_running, "running")
    _ = jdb.add_job(
        "job_database_dir",
        "user",
        "key",
        "job_database.log",
        ["ls"],
        start_if_available=False,
    )
    assert not jdb.check_and_run()


def test_handle_interrupted_job(isolated_dir):
    jdb = JobDatabase("test.db")
    for jid in list(jdb.keys()):
        del jdb[jid]
    jid_running = jdb.add_job(
        "job_database_dir",
        "user",
        "key",
        "job_database.log",
        ["ls"],
        start_if_available=False,
    )
    jdb.set_status(jid_running, "running")
    # Fake a PID no longer running
    jdb.db.execute("update job_status set pid=-1 where job_id=?", (jid_running,))
    jdb.db.commit()
    jid_could_run = jdb.add_job(
        "job_database_dir",
        "user",
        "key",
        "job_database.log",
        ["ls"],
        start_if_available=False,
    )
    assert jdb.check_and_run()
    assert jdb[jid_running]["status"] == "interrupted"
    assert jdb[jid_could_run]["status"] == "success"


def test_handle_failed_job(isolated_dir):
    jdb = JobDatabase("test.db")
    for jid in list(jdb.keys()):
        del jdb[jid]
    jid = jdb.add_job(
        "job_database_dir",
        "user",
        "key",
        "job_database.log",
        ["ls", "--bad-argument"],
        start_if_available=False,
    )
    assert jdb.check_and_run()
    assert jdb[jid]["status"] == "failure"


@pytest.mark.skip
def test_abcd_job(isolated_dir):
    jdb = JobDatabase("test.db")
    jdb.abcd_job(
        "mali_cosi_sub",
        "user",
        "key",
        "job_database.log",
        "/raid10/sba_gold/mali_cosi/12MAR10105443-P1BS-052683561010_04_P007.NTF",
        "/raid10/sba_gold/mali_cosi/13FEB26104346-P1BS-500058150010_01_P002.NTF",
        number_process=24,
        subset=[10000, 10000, 1000, 1000],
    )


@pytest.mark.skip
def test_abcd_no_imd_job(isolated_dir):
    """copied data local, but don't have IMD file. Make sure we still work."""
    jdb = JobDatabase("test.db")
    jdb.abcd_job(
        "mali_cosi_sub",
        "user",
        "key",
        "job_database.log",
        "12MAR10105443-P1BS-052683561010_04_P007.NTF",
        "13FEB26104346-P1BS-500058150010_01_P002.NTF",
        number_process=24,
        subset=[10000, 10000, 1000, 1000],
    )
