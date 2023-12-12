from geocal_swig import Orbit
from test_support import *
import pickle

class PythonOrbit(Orbit):
    def __init__(self):
        tm = Time.time_j2000(100.0)
        super().__init__(tm, tm + 10)
        self.korb = KeplerOrbit()

    def orbit_data(self, T):
        return self.korb.orbit_data(T)

    def desc(self):
        return "PythonOrbit"

@skip    
@require_serialize
def test_director_serialize():
    orb = PythonOrbit()
    print(orb)
    t = serialize_write_string(orb)
    print(t)
    orb2 = serialize_read_generic_string(t)
    print(orb2.min_time)

@skip    
@require_serialize
def test_director_pickle():
    orb = PythonOrbit()
    t = pickle.dumps(orb)
    orb2 = pickle.loads(t)
    print(orb2.min_time)


