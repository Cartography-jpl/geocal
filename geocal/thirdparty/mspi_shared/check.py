import sys
import re

fail_count = 0
fail_list = list()

max_len = 0
for filename in sys.argv[1:]:
    if (len(filename) > max_len):
        max_len = len(filename)
    
for file in sys.argv[1:]:
    all_tests_ok = False
    unexpected_output = False
    f = open(file,'r')
    for line in f:
        if (re.search("^All tests succeeded.$",line)):
            all_tests_ok = True
            continue
        unexpected_output = True
    f.close()

    if (unexpected_output or (not all_tests_ok)):
        print "%-*s fail" % (max_len+8,file)
        fail_count += 1
        fail_list.append(file)
    else:
        print "%-*s ok" % (max_len+8,file)

print
if (fail_count > 0):
    print fail_count,"tests failed:"
    for t in fail_list:
        print "  ",t
else:
    print "All tests passed."


