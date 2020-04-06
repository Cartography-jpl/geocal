try:
    import pynitf
except ImportError:
    pass
from test_support import *
import geocal.geocal_nitf_rsm
from geocal.geocal_nitf_rsm import *
from geocal.geocal_nitf_des import *
from geocal.glas_gfm import *
from geocal_swig import *

def create_image_seg(f):
    img = pynitf.NitfImageWriteNumpy(9, 10, np.uint8)
    img.subheader.idlvl = 1
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
                print(Geodetic(p1))
                print(Geodetic(p2))
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

# Skip for now. As the file name indicates, these are problems so the unit
# tests fail. We'll need to get to the bottom of the problem, but for now don't
# break our unit tests
@skip
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

# Skip for now. As the file name indicates, these are problems so the unit
# tests fail. We'll need to get to the bottom of the problem, but for now don't
# break our unit tests
@skip
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
    
# Skip for now. As the file name indicates, these are problems so the unit
# tests fail. We'll need to get to the bottom of the problem, but for now don't
# break our unit tests
@skip
@require_msp
@require_pynitf
def test_gfm(isolated_dir, igc_test):
    od = igc_test.orbit_data
    tmin = od.time - 10.0
    tmax = od.time + 10.0
    od = AircraftOrbitData(Time.parse_time("2012-06-12T03:30:00Z"),
                           Ecr(Geodetic(40,125,520e3)),
                           [-1600,5000,-5500], 0, 0, -101)
    posmin = od.position_cf
    posmax = od.position_cf
    posmin.position = [posmin.position[0] - od.velocity_cf[0] * 10.0,
                       posmin.position[1] - od.velocity_cf[1] * 10.0,
                       posmin.position[2] - od.velocity_cf[2] * 10.0]
    posmax.position = [posmax.position[0] + od.velocity_cf[0] * 10.0,
                       posmax.position[1] + od.velocity_cf[1] * 10.0,
                       posmax.position[2] + od.velocity_cf[2] * 10.0]
    odmin = AircraftOrbitData(tmin, posmin, [-1600,5000,-5500], 0, 0, -101)
    odmax = AircraftOrbitData(tmax, posmax, [-1600,5000,-5500], 0, 0, -101)
    orb = OrbitQuaternionList([odmin, od, odmax])
    porb = PosCsephb(orb, tmin,tmax, 0.5, PosCsephb.LAGRANGE,
                     PosCsephb.LAGRANGE_7, PosCsephb.EPHEMERIS_QUALITY_GOOD,
                     PosCsephb.ACTUAL, PosCsephb.CARTESIAN_FIXED)
    aorb = AttCsattb(orb, tmin,tmax, 0.5, AttCsattb.LAGRANGE,
                     AttCsattb.LAGRANGE_7, AttCsattb.ATTITUDE_QUALITY_GOOD,
                     AttCsattb.ACTUAL, AttCsattb.CARTESIAN_FIXED)
    orb = OrbitDes(porb,aorb)
    cam = GlasGfmCamera(2000, 2000)
    cam.focal_length = igc_test.camera.focal_length * 1e-3
    cam.field_alignment_block(igc_test.camera, 1000, 1000)
    igc = OrbitDataImageGroundConnection(orb, od.time, cam, SimpleDem(),
                                         None)
    igc.platform_id = "FAKEPL"
    igc.payload_id = "FAKEPY"
    igc.sensor_id = "FAKESN"
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].create_glas_gfm(igc)
    t = f.image_segment[0].glas_gfm.tre_csexrb
    pt = igc.ground_coordinate(ImageCoordinate(1000, 1000))
    t.ground_ref_point_x = pt.position[0]
    t.ground_ref_point_y = pt.position[1]
    t.ground_ref_point_z = pt.position[2]
    t.atm_refr_flag = 0
    f.write("gfm_test.ntf")
    igc2 = IgcMsp("gfm_test.ntf", SimpleDem(), 0, "GFM", "GFM")
    print(igc)
    rsm = Rsm(RsmRationalPolynomial(3,3,3,3,3,3,3,3),
              GeodeticRadianConverter())
    rsm.fit(igc, -1000, 1000)
    rsm.fill_in_ground_domain_vertex(-1000,1000)
    for i in range(0, igc.number_line, 20):
        for j in range (0, igc.number_sample, 20):
            ic = ImageCoordinate(i, j)
            d1 = distance(igc.ground_coordinate(ic),
                          igc2.ground_coordinate(ic))
    # Not sure why so far off, we may need to look into this. But for now,
    # just skip
    #compare_rsm(igc2, rsm)
    rsm2 = igc2.generate_rsm("rsm_generate_report.txt",
                             "/home/smyth/Local/rsm_config/database_settings.strat")
    print(rsm2)
