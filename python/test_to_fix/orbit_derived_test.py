from geocal_swig import Orbit
from .sqlite_shelf import read_shelve, write_shelve
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

@require_serialize
def test_director_serialize(isolated_dir):
    orb = PythonOrbit()
    print(orb)
    write_shelve("orb.xml", orb)
    orb2 = read_shelve("orb.xml")
    print(orb2.min_time)

@require_serialize
def test_director_pickle():
    orb = PythonOrbit()
    t = pickle.dumps(orb)
    orb2 = pickle.loads(t)
    print(orb2.min_time)


