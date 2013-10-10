#!/bin/sh
# You can edit this on your local copy to run a single test or a selected set
# of tests. This can be useful when you are working in one area and want a
# fast turn around. You should however always have all the tests on
# when you check this in.

echo "==========================================================="
echo "Can specify run_test=<exp> on command line to run subset of"
echo "unit tests. See boost unit test documentation for details"
echo "==========================================================="

if [ ${log_test} ] ; then
    ./geocal_test_all --log_level=test_suite --run_test=${run_test}
else
    ./geocal_test_all --show_progress --run_test=${run_test}
fi

# Valgrind version of test run. 
#valgrind --suppressions=${srcdir}/test_data/valgrind.suppressions ./lib/test_all --log_level=test_suite

