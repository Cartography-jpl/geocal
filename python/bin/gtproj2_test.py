from geocal import *
from nose.tools import *
from nose.plugins.skip import Skip, SkipTest
import subprocess
import os
import sys
import re
from vicar_test_support import *

# Setup and teardown called automatically by nosetest for whole module
original_env = None
prefix = "gtproj2_xxx"
def setup():
    check_vicarb()
    add_tae_path(os.path.dirname(__file__), original_env)

def teardown():
    set_original_env(original_env)
    # Clean up the various temporary files
    subprocess.run("rm %s*" % prefix, shell=True,
                   stdout=subprocess.PIPE,
                   stderr=subprocess.PIPE)

def gtgen(filename, label):
    '''This runs the gtgen program. This is used in the original gtproj unit
    tests to generate files with the right projection information. We could
    generate these files using geocal MapInfo etc code, but for traceability
    it is nice to match what the original tests did.'''
    label=re.sub(r'\+?\n',"", label)
    res = subprocess.run("gtgen out='%s' geotiff='%s'" % (filename,  label),
                   shell=True,
                   stdout=subprocess.PIPE,
                   stderr=subprocess.STDOUT)
    # Somewhat oddly vicar stuff exits with a status of 1 on success, not 0
    if(res.returncode != 1):
        raise RuntimeError("Call to gtgen failed with:\n%s" % res.stdout)

def test_gtproj2_1():
    '''This corresponds to the test labeled 
    "basic case, geogr-projective, lambert" in the original tstgtproj'''
    f1 = prefix+"lab1"
    f2 = prefix+"lab2"
    ib = prefix+"ibis"
    gtgen(f1, 
''' ("GTModelTypeGeoKey=2(ModelTypeGeographic)", +
 "GeogGeodeticDatumGeoKey=39990(NIMA:NAS-C)", +
 "GeogPrimeMeridianGeoKey=40.0")
''')
    gtgen(f2, 
'''("ProjectionGeoKey=8(CT_LambertConfConic_2sp)", +
          "PROJCENTERLATGEOKEY=40", +
          "PROJCENTERLONGGEOKEY=30.5", +
          "PROJSTDPARALLEL1GEOKEY=33.0", +
          "PROJSTDPARALLEL2GEOKEY=45", +
          "PROJFALSENORTHINGGEOKEY=0", +
          "PROJFALSEEASTINGGEOKEY=0", +
          "GeogGeodeticDatumGeoKey=39990(NIMA:NAS-C)")
''')
    d = np.array([[31.0,40.0,333.33, 0.0, 0.0, 0.0],
                  [32.0,40.0,333.33, 0.0, 0.0, 0.0]])
    IbisFile.create(ib, d)
    vicarb_run("gtproj2 \"(%s,%s,%s)\" incol=\"(1,2,3)\" outcol=\"(4,5,6)\"" % (ib, f1, f2))
    f = IbisFile(ib)
    print(f[:,:])
    
