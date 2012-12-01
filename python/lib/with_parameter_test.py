from nose.tools import *
from with_parameter import *

class WithParameterTest(WithParameter):
    def __init__(self):
        self._parameter = [1, 2, 3]
    @property
    def parameter(self):
        return self._parameter
    
    @parameter.setter
    def parameter(self, value):
        self._parameter = value

    @property
    def parameter_subset_mask(self):
        return [False, True, False]

def test_parameter_name():
    '''Test parameter_name.'''
    t = WithParameterTest()
    assert t.parameter_name == ["Parameter 0", "Parameter 1", "Parameter 2"]

def test_parameter_subset():
    '''Test parameters subsetting.'''
    t = WithParameterTest()
    assert t.parameter == [1,2,3]
    assert t.parameter_subset == [2]
    t.parameter_subset = [4]
    assert t.parameter == [1,4,3]
    assert t.parameter_subset == [4]
    
def test_parameter_subset_name():
    '''Test parameter_subset_name.'''
    t = WithParameterTest()
    assert t.parameter_name == ["Parameter 0", "Parameter 1", "Parameter 2"]
    assert t.parameter_subset_name == ["Parameter 1"]
