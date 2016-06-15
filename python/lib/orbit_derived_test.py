from __future__ import print_function
from future import standard_library
standard_library.install_aliases()
from test_support import *
import pickle

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
        print("In init_base")
        Orbit.__init__(self)
        print(self.min_time)

    def __str__(self):
        return "PythonOrbit"
    @classmethod
    def __serialize_format_version__(cls):
        return 1

    def __boost_serialize_save__(self):
        d = self.__dict__.copy()
        del d['this']
        return pickle.dumps((_boost_serialize_load, (self.__class__, self.__serialize_format_version__(), d)))

    def __reduce__(self):
        return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

# Temporary, parallel and pickle stuff doesn't seem to work yet
@skip
@require_serialize
def test_director_serialize():
    orb = PythonOrbit()
    t = serialize_write_string(orb)
    print(t)
    orb2 = serialize_read_generic_string(t)
    print(orb2.min_time)

# Temporary, parallel and pickle stuff doesn't seem to work yet
@skip
@require_serialize
def test_director_pickle():
    orb = PythonOrbit()
    t = pickle.dumps(orb)
    print(t)
    orb2 = pickle.loads(t)
    print(orb2.min_time)


