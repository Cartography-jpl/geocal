from geocal.pds_label import *
from test_support import *
import os
import glob

def test_pds_label_text():
    fname = unit_test_data + "mars_kernel/mro_kernel/spk/mro_psp6_ssd_mro110c.lbl"
    txt = pds_label_text(fname)
    assert txt == '''PDS_VERSION_ID               = PDS3
RECORD_TYPE                  = FIXED_LENGTH
RECORD_BYTES                 = 1024
^SPICE_KERNEL                = "mro_psp6_ssd_mro110c.bsp"
MISSION_NAME                 = "MARS RECONNAISSANCE ORBITER"
SPACECRAFT_NAME              = "MARS RECONNAISSANCE ORBITER"
DATA_SET_ID                  = "MRO-M-SPICE-6-V1.0"
KERNEL_TYPE_ID               = SPK
PRODUCT_ID                   = "mro_psp6_ssd_mro110c.bsp"
PRODUCT_CREATION_TIME        = 2015-08-27T11:21:49
PRODUCER_ID                  = "SSD/JPL"
MISSION_PHASE_NAME           = "PRIMARY SCIENCE"
PRODUCT_VERSION_TYPE         = ACTUAL
PLATFORM_OR_MOUNTING_NAME    = "N/A"
START_TIME                   = 2008-01-01T00:00:00
STOP_TIME                    = 2008-04-01T01:00:00
SPACECRAFT_CLOCK_START_COUNT = "N/A"
SPACECRAFT_CLOCK_STOP_COUNT  = "N/A"
TARGET_NAME                  = MARS
INSTRUMENT_NAME              = "N/A"
NAIF_INSTRUMENT_ID           = "N/A"
SOURCE_PRODUCT_ID            = "N/A"
NOTE                         = "See comments in the file for details"
OBJECT                       = SPICE_KERNEL
  INTERCHANGE_FORMAT         = BINARY
  KERNEL_TYPE                = EPHEMERIS
  DESCRIPTION                = "MRO SPICE SPK file providing MRO spacecraft
trajectory, determined using Mars gravity field MRO110C, for a part of the
Primary Science phase of the mission, created by the Solar System Dynamics
group, JPL. "
END_OBJECT                   = SPICE_KERNEL
END
'''
    
def test_pds_label():
    fname = unit_test_data + "mars_kernel/mro_kernel/spk/mro_psp6_ssd_mro110c.lbl"
    d = pds_label(fname)
    assert d['KERNEL_TYPE'] == 'EPHEMERIS'

def test_spice_kernel_by_time():
    bdir = unit_test_data + "mars_kernel/mro_kernel/spk/"
    kdata = SpiceKernelByTime(glob.glob(bdir + "mro_*_ssd_mro110c.lbl"))
    tstart = Time.parse_time("2008-02-23T03:36:10.894Z")
    assert kdata.kernel(tstart) == bdir + "mro_psp6_ssd_mro110c.bsp"
    
    
