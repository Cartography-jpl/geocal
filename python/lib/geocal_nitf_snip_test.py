try:
    import pynitf
except ImportError:
    pass
from test_support import *
from .geocal_nitf_des import *
from .glas_gfm import *
from geocal_swig import (IgcMsp, SimpleDem, ImageCoordinate, distance,
                         OrbitDataImageGroundConnection, PosCsephb, AttCsattb,
                         KeplerOrbit, Time, OrbitDes)
import os
import math

def create_image_seg(f):
    img = pynitf.NitfImageWriteNumpy(9, 10, np.uint8,
                                     idatim="20030101111100")
    img.subheader.idlvl = 1
    for i in range(9):
        for j in range(10):
            img[0,i,j] = i + j
    f.image_segment.append(pynitf.NitfImageSegment(img))

def create_gfm_igc():
    '''Sample IGC that can be used to write GLAS/GFM'''
    tdata = Time.parse_time("2003-01-01T11:11:00Z")
    orb = KeplerOrbit()
    porb = PosCsephb(orb, tdata-10.0,tdata+10.0, 0.5, PosCsephb.LAGRANGE,
                     PosCsephb.LAGRANGE_5, PosCsephb.EPHEMERIS_QUALITY_GOOD,
                     PosCsephb.ACTUAL, PosCsephb.CARTESIAN_FIXED)
    aorb = AttCsattb(orb, tdata-10.0,tdata+10.0, 0.5, AttCsattb.LAGRANGE,
                     AttCsattb.LAGRANGE_7, AttCsattb.ATTITUDE_QUALITY_GOOD,
                     AttCsattb.ACTUAL, AttCsattb.CARTESIAN_FIXED)
    orb = OrbitDes(porb,aorb)
    cam = GlasGfmCamera(2048, 2048)
    cam.focal_length = 123.8e-3
    cam.focal_length_time = orb.min_time
    cam.first_line_block = [0]
    cam.first_sample_block = [0]
    cam.delta_line_block = [2048]
    cam.delta_sample_block = [2048]
    fa = np.empty((1,1,2,2,2))
    fa[0,0,:,0,0] = -1024 * 21e-6
    fa[0,0,:,1,0] = 1024 * 21e-6
    fa[0,0,0,:,1] = 1024 * 21e-6
    fa[0,0,1,:,1] = -1024 * 21e-6
    cam.field_alignment_block(0,fa)
    igc = OrbitDataImageGroundConnection(orb, tdata, cam, SimpleDem(),
                                         None)
    igc.platform_id = "FAKEPL"
    igc.payload_id = "FAKEPY"
    igc.sensor_id = "FAKESN"
    return igc

@require_msp    
@require_pynitf
def test_snip_example(isolated_dir, nitf_sample_rip):
    igc1 = IgcMsp(nitf_sample_rip, SimpleDem(), 1, "GLAS", "GLAS")
    igc2 = IgcMsp(nitf_sample_rip, SimpleDem(), 1, "RSM", "RSM")
    # Not sure why, but default IgcMsp is not what we would expect. Neither
    # GLAS nor RSM is selected
    igc3 = IgcMsp(nitf_sample_rip, SimpleDem(), 1)
    print(igc1)
    print(igc2)
    print(igc3)
    pt1 = igc1.ground_coordinate_approx_height(ImageCoordinate(100,100), 0)
    pt2 = igc2.ground_coordinate_approx_height(ImageCoordinate(100,100), 0)
    pt3 = igc3.ground_coordinate_approx_height(ImageCoordinate(100,100), 0)
    print(distance(pt1, pt2))
    print(distance(pt1, pt3))

# Fails, we'll want to look at this but turn off for now.    
@skip    
@require_msp    
@require_pynitf
def test_snip_create(isolated_dir):
    '''Basic creation test. Right now, we just copy things'''
    pynitf.register_des_class(pynitf.NitfDesCopy, priority_order=-999)
    fname = "/home/smyth/Local/SNIP NITF Example/07APR2005_Hyperion_331405N0442002E_SWIR172_001_L1R.ntf"
    # Ok if data isn't available. We can only run test if we have the
    # test data.
    if(not os.path.exists(fname)):
        raise SkipTest
    f = NitfFile(fname)
    tcsexrb = f.image_segment[1].find_exactly_one_tre("CSEXRB")
    f2 = NitfFile()
    create_image_seg(f2)
    f2.image_segment[0].tre_list.append(tcsexrb)
    # All 4 of these seem to be needed. Not clear if this is just because
    # they are all listed in the associated tre stuff. We might be able
    # to remove the covariance if it is not listed, or perhaps this really
    # is needed.
    csattb = [d for d in f.des_segment if d.subheader.desid == "CSATTB"][0]
    f2.des_segment.append(csattb)
    csephb = [d for d in f.des_segment if d.subheader.desid == "CSEPHB"][0]
    f2.des_segment.append(csephb)
    cssfab = [d for d in f.des_segment if d.subheader.desid == "CSSFAB"][0]
    f2.des_segment.append(cssfab)
    cscsdb = [d for d in f.des_segment if d.subheader.desid == "CSCSDB"][0]
    f2.des_segment.append(cscsdb)
    f2.write("test.ntf")
    igc1 = IgcMsp(fname, SimpleDem(), 1, "GLAS", "GLAS")
    igc2 = IgcMsp("test.ntf", SimpleDem(), 0, "GLAS", "GLAS")
    pt1 = igc1.ground_coordinate_approx_height(ImageCoordinate(100,100), 0)
    pt2 = igc2.ground_coordinate_approx_height(ImageCoordinate(100,100), 0)
    print(distance(pt1, pt2))
    pynitf.unregister_des_class(pynitf.NitfDesCopy)
    
@require_pynitf
def test_illumb(isolated_dir):
    '''Test creating a ILLUMB tre.'''
    # The sample SNIP file doesn't actually have a a ILLUMB TRE in it.
    # So we just create a placeholder Igc as a sample. This is a GLAS/GFM,
    # but this is just for convenience. ILLUMB has nothing directly to do
    # with GLAS/GFM other then that they both appear in the SNIP
    igc = create_gfm_igc()
    f = pynitf.NitfFile()
    create_image_seg(f)
    iseg = f.image_segment[0]
    iseg.create_glas_gfm(igc)
    t = iseg.glas_gfm.tre_csexrb
    pt = igc.ground_coordinate(ImageCoordinate(1024, 1024))
    t.ground_ref_point_x = pt.position[0]
    t.ground_ref_point_y = pt.position[1]
    t.ground_ref_point_z = pt.position[2]
    t.atm_refr_flag = 0

    t = pynitf.TreILLUMB()

    # This is Band 2 of ECOSTRESS, just as an example
    # Center is 8.78, FWHM is 0.310
    t.num_bands = 1
    t.band_unit = "micrometer"
    t.lbound[0] = 8.78 - 0.310 / 2
    t.ubound[0] = 8.78 + 0.310 / 2
    t.num_coms = 1
    t.comment[0] = "Using Band 2 of ECOSTRESS as example"

    # Defaults for TRE fill in datum etc, so we don't need to change
    # t.geo_datum = "World Geodetic System 1984" etc.

    # Don't include any other natural light sources
    t.num_others = 0

    # One illumination set for the middle of image segment
    t.num_illum_sets = 1
    t.datetime[0] = iseg.subheader.idatim
    ic_center = ImageCoordinate(igc.number_line / 2.0, igc.number_sample / 2.0)
    gc_center = igc.ground_coordinate(ic_center)
    t.target_lat[0] = gc_center.latitude
    # This is -180 to 180. Assume that is right, but might need to check
    # spec
    t.target_lon[0] = gc_center.longitude
    t.target_hgt[0] = gc_center.height_reference_surface

    # Solar angles
    slv = LnLookVector.solar_look_vector(igc.pixel_time(ic_center), gc_center)
    t.existence_mask |= 0x400000
    # We happen to have zenith angle in geocal, not elevation. Convert since
    # tre wants the elevation
    t.sun_elev[0] = 90.0 - slv.view_zenith
    t.sun_azimuth[0] = slv.view_azimuth

    # Moon angles
    mlv = LnLookVector.body_look_vector(Ecr.MOON_NAIF_CODE, igc.pixel_time(ic_center), gc_center)
    t.existence_mask |= 0x200000
    # We happen to have zenith angle in geocal, not elevation. Convert since
    # tre wants the elevation
    t.moon_elev[0] = 90.0 - mlv.view_zenith
    t.moon_azimuth[0] = mlv.view_azimuth

    # Phase angle, I'm pretty sure this is right but should verify if it
    # matters
    t1 = np.array([*slv.direction])
    t2 = np.array([*mlv.direction])
    t.existence_mask |= 0x100000
    t.moon_phase_angle[0] = math.degrees(math.acos(np.dot(t1, t2)))
    
    # View angles
    vlv = LnLookVector(igc.cf_look_vector_lv(ic_center), gc_center)
    t.existence_mask |= 0x020000
    t.sensor_elev[0] = 90.0 - vlv.view_zenith
    t.sensor_azimuth[0] = vlv.view_azimuth

    # Could probably calculate cats_angle if needed, would need to figure
    # out defintion
    
    iseg.tre_list.append(t)
    f.write("illumb_test.ntf")


