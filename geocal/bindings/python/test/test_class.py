import geocal as gc
import unittest
import os
from numpy.testing import *

class TestClass(unittest.TestCase):
    # Set up to compare numpy arrays.
    def data_test(self):
        t = os.environ.get("UNIT_TEST_DATA")
        if(t == None):
            raise Exception("Need to set the environment variable 'UNIT_TEST_DATA' before running tests")
        return t + "/"
