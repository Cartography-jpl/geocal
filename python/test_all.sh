#!/bin/sh
# This gets run by the makefile. If you just want to run the tests yourself,
# you can source the setup_afids_env.sh file and then run nosetests

export PYTHONPATH=${python_test_path}
export SPICEDATA=$(AFIDS_DATA)/cspice
nosetests ${srcdir}/afids_python/python/*/ 

