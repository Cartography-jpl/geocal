#!/bin/sh
# This gets run by the makefile. If you just want to run the tests yourself,
# you can source the setup_afids_env.sh file and then run nosetests

source ${python_setup}
nosetests ${srcdir}/src/python/*/ ${srcdir}/src/vdev/*/

