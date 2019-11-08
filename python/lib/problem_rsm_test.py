try:
    import pynitf
except ImportError:
    pass
from test_support import *
import geocal.geocal_nitf_rsm
from geocal.geocal_nitf_rsm import *
from geocal_swig import *

def create_image_seg(f):
    img = pynitf.NitfImageWriteNumpy(9, 10, np.uint8)
    img.image_subheader.idlvl = 1
    for i in range(9):
        for j in range(10):
            img[0,i,j] = i + j
    f.image_segment.append(pynitf.NitfImageSegment(img))

def compare_rsm(igc_msp, rsm):
    for h in (rsm.rsm_id.ground_domain_vertex[0].height_reference_surface + 10.0,
              rsm.rsm_id.ground_domain_vertex[7].height_reference_surface - 10.0):
        for ln in np.linspace(0, 2000, 10):
            for smp in np.linspace(0, 2000, 10):
                ic = ImageCoordinate(ln,smp)
                p1 = igc_msp.ground_coordinate_approx_height(ic, h)
                p2 = rsm.ground_coordinate_approx_height(ic, h)
                assert(geocal_swig.distance(p1, p2) < 0.1)
    
@pytest.fixture(scope="function")
def igc_test():
    cam = QuaternionCamera(Quaternion_double(1,0,0,0),
                           2000, 2000, 9.7e-7,9.7e-7, 1,
                           FrameCoordinate(1000,1000))
    od = AircraftOrbitData(Time.parse_time("2012-06-12T03:30:00Z"),
                           Ecr(Geodetic(40,125,520e3)),
                           [-1600,5000,-5500], 0, 0, -101)
    igc = OrbitDataImageGroundConnection(od, cam, SimpleDem(), None)
    return igc
    
@require_msp
@require_pynitf
def test_rsm_geodetic(isolated_dir, igc_test):
    '''Compare the RSM we write to a NITF file with what the MSP library 
    calculates. This verifies both the validity of our NITF and our RSM 
    code'''
    rsm = Rsm(RsmRationalPolynomial(3,3,3,3,3,3,3,3),
              GeodeticRadianConverter())
    rsm.fit(igc_test, -1000, 1000)
    rsm.fill_in_ground_domain_vertex(-1000,1000)
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm
    f.write("nitf_rsm.ntf")
    igc_msp = IgcMsp("nitf_rsm.ntf")
    compare_rsm(igc_msp, rsm)

@require_msp
@require_pynitf
def test_rsm_local(isolated_dir, igc_test):
    '''Compare the RSM we write to a NITF file with what the MSP library 
    calculates. This verifies both the validity of our NITF and our RSM 
    code'''
    rsm = Rsm(RsmRationalPolynomial(3,3,3,3,3,3,3,3),
              LocalRcConverter(LocalRcParameter(igc_test)))
    rsm.fit(igc_test, -1000, 1000)
    rsm.fill_in_ground_domain_vertex(-1000,1000)
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm
    f.write("nitf_rsm.ntf")
    igc_msp = IgcMsp("nitf_rsm.ntf")
    compare_rsm(igc_msp, rsm)
    
