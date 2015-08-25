from nose.tools import *
from nose.plugins.skip import Skip, SkipTest
from geocal_swig import *
import cPickle

def _new_from_serialization(data):
    return geocal_swig.serialize_read_binary(data)

# Try having a class derived from Orbit, and test out the handling
# of all the director stuff.

def _boost_serialize_load(cls, version, d):
    if(cls.__serialize_format_version__() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__dict__ = d
    inst.__init_base__()
    return inst

class PythonOrbit(Orbit):
    def __init__(self):
        tm = Time.time_j2000(100.0)
        Orbit.__init__(self, tm, tm + 10)
        self.korb = KeplerOrbit()

    def __init_base__(self):
        print "In init_base"
        Orbit.__init__(self)
        print self.min_time

    def __str__(self):
        return "PythonOrbit"
    @classmethod
    def __serialize_format_version__(cls):
        return 1

    def __boost_serialize_save__(self):
        d = self.__dict__.copy()
        del d['this']
        return cPickle.dumps((_boost_serialize_load, (self.__class__, self.__serialize_format_version__(), d)))

    def __reduce__(self):
        return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

def test_director_serialize():
    if(not have_serialize_supported()):
        raise SkipTest
    orb = PythonOrbit()
    t = serialize_write_string(orb)
    print t
    orb2 = serialize_read_generic_string(t)
    print orb2.min_time

def test_director_pickle():
    if(not have_serialize_supported()):
        raise SkipTest
    orb = PythonOrbit()
    t = cPickle.dumps(orb)
    print t
    orb2 = cPickle.loads(t)
    print orb2.min_time


