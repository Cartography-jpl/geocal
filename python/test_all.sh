#!/bin/sh
# This gets run by the makefile. If you just want to run the tests yourself,
# you can source the setup_afids_env.sh file and then run nosetests

export PYTHONPATH=${python_test_path}
source ${AFIDS_DATA}/setup_afids_data.sh
export PATH=${python_exec_path}
echo "Hi there"
echo ${PATH}
nosetests ${srcdir}/afids_python/python/*/ 

