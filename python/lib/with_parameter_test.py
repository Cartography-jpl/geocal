from geocal_swig import *
from numpy.testing import assert_almost_equal

class WithParameterTest(WithParameter):
    def __init__(self):
        self._parameter = [1, 2, 3]
        WithParameter.__init__(self)
        self.parameter_mask = [False, True, False]
        
    def _v_parameter(self, *args):
        # Awkward interface, but this matches what the C++ needs. If we have
        # no arguments, then we are returning the parameters. Otherwise,
        # we are setting them.
        if(len(args) == 0):
            return self._parameter
        else:
            self._parameter = args[0]
    
    def _v_parameter_name(self):
        return ["Parameter 0", "Parameter 1", "Parameter 2"]

    def _v_parameter_mask(self):
        return self.parameter_mask_

    @property
    def parameter_mask(self):
        return self._v_parameter_mask()

    @parameter_mask.setter
    def parameter_mask(self, value):
        self.parameter_mask_ = value

def test_parameter_name():
    '''Test parameter_name.'''
    t = WithParameterTest()
    assert t.parameter_name == ["Parameter 0", "Parameter 1", "Parameter 2"]
    assert np.array(t.parameter_name_subset) == ["Parameter 1"]


def test_parameter_subset():
    '''Test parameters subsetting.'''
    t = WithParameterTest()
    assert_almost_equal(t.parameter, [1,2,3])
    assert_almost_equal(t.parameter_subset, [2])
    assert t.parameter_subset == [2]
    t.parameter_subset = [4]
    assert_almost_equal(t.parameter, [1,4,3])
    assert_almost_equal(t.parameter_subset, [4])
    
