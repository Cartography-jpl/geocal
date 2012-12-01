# This runs all the unit tests we find in this directory.

import os
import glob
import unittest
for i in glob.glob(os.path.dirname(__file__) + "/*_test.py"):
    exec 'from ' + os.path.basename(i).split('.')[0] + ' import *'

if __name__ == '__main__':
    unittest.main()
