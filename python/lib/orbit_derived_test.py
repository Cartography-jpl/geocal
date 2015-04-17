from nose.tools import *
from nose.plugins.skip import Skip, SkipTest
from geocal_swig import *
import cPickle

# Try having a class derived from Orbit, and test out the handling
# of all the director stuff.

def _new_from_dict(cls, version, d, bc):
    '''For use with pickle, covers common case where we just store the
    arguments needed to create an object. See for example HdfFile'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__dict__ = d
    Orbit.__init__(inst)
    #inst.serialize_base_class(bc)
    return inst

class PythonOrbit(Orbit):
    def __init__(self):
        tm = Time.time_j2000(100.0)
        Orbit.__init__(self, tm, tm + 10)
        self.korb = KeplerOrbit()
    def __str__(self):
        return "PythonOrbit"
    @classmethod
    def pickle_format_version(cls):
        return 1
    def __reduce__(self):
        d = self.__dict__.copy()
        del d['this']
        #return _new_from_dict, (self.__class__, 1, d, self.serialize_base_class())
        return _new_from_dict, (self.__class__, 1, d, "fake")


def test_director_stuff():
    orb = PythonOrbit()
    print orb.min_time
    t = serialize_write_string(orb)
    orb2 = serialize_read_generic_string(t)
    print orb2.min_time


