try:
    import pynitf
except ImportError:
    pass
from test_support import *
from geocal.geocal_nitf_image import *

# This requires hard coded data, so don't normally run.
@skip
@require_pynitf
def test_nitf_gdal_image():
    '''Try reading SNIP data using GDAL'''
    f = pynitf.NitfFile("/home/smyth/Local/SNIP NITF Example/07APR2005_Hyperion_331405N0442002E_SWIR172_001_L1R.ntf")
    print(f.image_segment[1].data[:,:,:])
    print(f.image_segment[1].data[:,:,:].max())
    
