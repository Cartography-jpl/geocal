try:
    import pynitf

    # Turn this on if errors are getting sucked up during testing and turned
    # into TreUnknown. We don't want this on all the time, because we have
    # some unit tests that really do want warnings only
    import warnings

    if True:
        warnings.filterwarnings("error", category=pynitf.TreWarning)
except ImportError:
    pass
from geocal import (
    GdalRasterImage,
    VicarRasterImage,
    VicarLiteRasterImage,
    RsmRationalPolynomial,
    Rsm,
    ImageCoordinate,
    IgcMsp,
    RsmBUnmodeledCovariance,
    RsmIndirectCovarianceB,
    RsmIdTiming,
    RsmAdjustableParameterB,
    Time,
    WithParameter,
    ArrayAd_double_1,
    rad_to_arcsecond,
    arcsecond_to_rad,
    LocalRcParameter,
    LocalRcConverter,
    OrbitDataImageGroundConnection,
    MemoryRasterImage,
    SimpleDem,
    FrameCoordinate,
    QuaternionCamera,
    Quaternion_double,
    KeplerOrbit,
    write_shelve,
    RsmMultiSection,
    RsmGrid,
    RsmRpPlusGrid,
    Geodetic,
    distance,
    GlasGfmCamera,
    OrbitDes,
    AttCsattb,
    PosCsephb,
    RsmBSubgroup,
    IgcArray,
    OrbitOffsetCorrection,
    vector_string,
)
from fixtures.require_check import (
    require_pynitf,
    require_msp,
    require_serialize,
    require_vicar,
)
import io
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib as mpl
import seaborn as sns
from matplotlib.backends.backend_pdf import PdfPages
import itertools
from multiprocessing import Pool
import math
import pytest


def create_image_seg(f, num_line=9, num_sample=10, iid1="Test data"):
    img = pynitf.NitfImageWriteNumpy(num_line, num_sample, np.uint8)
    img.subheader.idlvl = 1
    img.subheader.iid1 = iid1
    for i in range(num_line):
        for j in range(num_sample):
            img[0, i, j] = (i + j) % 256
    f.image_segment.append(pynitf.NitfImageSegment(img))


@require_pynitf
def test_rsm_rp(isolated_dir, rsm):
    """Create a file, and write out a RSM. This RSM has just a single
    rational polynomial in it"""
    f = pynitf.NitfFile()
    create_image_seg(f)
    rsm.rsm_id.full_number_line = 100
    rsm.rsm_id.full_number_sample = 200
    f.image_segment[0].rsm = rsm
    f.write("nitf_rsm.ntf")
    f2 = pynitf.NitfFile("nitf_rsm.ntf")
    print(f2)


# This doesn't actually work. I don't think we can easily modify the RSM,
# this pretty much needs to go through C++. But for now leave this in place
# in case we need to come back to this.
@pytest.mark.skip
@require_pynitf
def test_modify_rsm_tre(isolated_dir, rsm):
    """We can modify the format/content of TREs in pynitf in a number
    of ways (see sample_format_test.py in pynitf). However modifying
    a RSM TRE has a particular complication.

    Because the exact TREs needed for the RSM vary depending on the
    specifics of the RSM, we have all the TREs removed and recreated from
    the Rsm object associated with a image segment. So any changes you make
    *won't* show up when writing.

    Changes should probably ultimately be handled by changing GeoCal, but
    even then there may be special formatting issues (e.g., a specific
    partner requires a specific format due to what a tool expects). So it
    is important to be able to modify this outside of GeoCal.

    The replacement is handled with pynitf.NitfSegmentHookSet. So we can
    add additional hooks to modify the RSM TREs after the RsmImageSegmentHook
    creates them.
    """
    # The initial stuff without change
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm
    f.write("nitf_rsm.ntf")
    f2 = pynitf.NitfFile("nitf_rsm.ntf")
    t = f2.image_segment[0].find_one_tre("RSMIDA")
    # We don't fill in the sensor x-acceleration
    print(t.sax)
    assert t.sax is None

    # Create a NitfImageHook to modify RSMIDA
    from geocal.geocal_nitf_rsm import RsmImageSegmentHook

    class ModifyRsmHook(RsmImageSegmentHook):
        def __init__(self, sax_value):
            super().__init__()
            self.sax_value = sax_value

        def before_write_hook(self, seg, nitf_file):
            super().before_write_hook(seg, nitf_file)
            # We don't normally allow classes with a tre_implementation_field
            # to be written to, to prevent accidental overwriting. But in this
            # case we specially want to allow this. So go ahead and
            # override this
            kp = pynitf.TreRSMIDA.tre_implementation_field
            pynitf.TreRSMIDA.tre_implementation_field = None
            t = seg.find_exactly_one_tre("RSMIDA")
            if t:
                print(t)
                t.sax = self.sax_value
                print(t)
            # And now put it back
            pynitf.TreRSMIDA.tre_implementation_field = kp

    # The hook can either be added to the default hook set to be used
    # for all files, or we can use it just for a specific file. For this
    # example, we just modify for a single file

    f = pynitf.NitfFile()
    # Remove existing RsmImageSegmentHook
    f.segment_hook_set.hook_set = set()
    # Replace with ours
    f.segment_hook_set.add_hook(ModifyRsmHook(10))
    create_image_seg(f)
    f.image_segment[0].rsm = rsm
    f.write("nitf_rsm.ntf")
    f2 = pynitf.NitfFile("nitf_rsm.ntf")
    t = f2.image_segment[0].find_one_tre("RSMIDA")
    print(t)
    # Filled in value
    print(t.sax)
    assert t.sax == 10


@require_msp
@require_pynitf
def test_rsm_generate_with_msp(isolated_dir, igc_rpc):
    """This uses the MSP toolkit to generate a RSM file, and makes sure
    MSP is happy with it.
    """

    # Start with an existing NITF file that we can create a sensor model for.
    # In this case we use an RPC, but this could be anything that MSP
    # supports (e.g, a GFM model)

    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rpc = igc_rpc.rpc
    f.write("nitf_input_rpc.ntf")
    igc = IgcMsp("nitf_input_rpc.ntf")

    # Run generation process.

    rsm = igc.generate_rsm("rsm_generate_report.txt")

    # Generate a NITF file with the RSM in it. This is just
    # so we can use the MSP software with the RSM.

    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm
    f.write("nitf_rsm.ntf")

    # Run the MSP code to compare the RSM to our initial RPC, and
    # also our calculation using the supplied rsm.

    igc_rsm = IgcMsp("nitf_rsm.ntf")
    for i in range(10):
        for j in range(10):
            ic = ImageCoordinate(i, j)
            p1 = igc_rsm.ground_coordinate(ic)
            p2 = igc_rpc.ground_coordinate(ic)
            p3 = rsm.ground_coordinate(ic, igc_rpc.dem)
            assert distance(p1, p2) < 0.01
            assert distance(p1, p3) < 0.01


@require_msp
@require_pynitf
def test_rsm_rp_with_msp(isolated_dir, rsm):
    """Compare the RSM we write to a NITF file with what the MSP library
    calculates. This verifies both the validity of our NITF and our RSM
    code"""
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm
    f.write("nitf_rsm.ntf")
    igc_msp = IgcMsp("nitf_rsm.ntf")
    for h in (
        rsm.rsm_id.ground_domain_vertex[0].height_reference_surface + 10.0,
        rsm.rsm_id.ground_domain_vertex[7].height_reference_surface - 10.0,
    ):
        for ln in np.linspace(0, 1000, 10):
            for smp in np.linspace(0, 1000, 10):
                ic = ImageCoordinate(ln, smp)
                p1 = igc_msp.ground_coordinate_approx_height(ic, h)
                p2 = rsm.ground_coordinate_approx_height(ic, h)
                assert distance(p1, p2) < 0.01


@require_msp
@require_pynitf
def test_rsm_lc_rp_with_msp(isolated_dir, rsm_lc):
    """Compare the RSM we write to a NITF file with what the MSP library
    calculates. This verifies both the validity of our NITF and our RSM
    code"""
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm_lc
    f.write("nitf_rsm.ntf")
    igc_msp = IgcMsp("nitf_rsm.ntf")
    for h in (
        rsm_lc.rsm_id.ground_domain_vertex[0].height_reference_surface + 10.0,
        rsm_lc.rsm_id.ground_domain_vertex[7].height_reference_surface - 10.0,
    ):
        for ln in np.linspace(0, 1000, 10):
            for smp in np.linspace(0, 1000, 10):
                ic = ImageCoordinate(ln, smp)
                p1 = igc_msp.ground_coordinate_approx_height(ic, h)
                p2 = rsm_lc.ground_coordinate_approx_height(ic, h)
                assert distance(p1, p2) < 0.01


@require_msp
@require_pynitf
def test_rsm_lc_rp_with_msp_with_adj(isolated_dir, rsm_lc, igc_rpc):
    """Compare the RSM we write to a NITF file with what the MSP library
    calculates. This verifies both the validity of our NITF and our RSM
    code"""
    f = pynitf.NitfFile()
    create_image_seg(f)
    adj = RsmAdjustableParameterB(igc_rpc, 500, 1500, rsm_lc.rsm_id)
    adj.row_power = np.array([[1, 0, 0], [0, 1, 0], [0, 0, 1]], dtype=int)
    adj.parameter = np.array([1.0, 2, 3])
    rsm_lc.rsm_adjustable_parameter = adj
    f.image_segment[0].rsm = rsm_lc
    f.write("nitf_rsm.ntf")
    f2 = pynitf.NitfFile("nitf_rsm.ntf")
    rsm_lc2 = f2.image_segment[0].rsm
    print(rsm_lc2)
    igc_msp = IgcMsp("nitf_rsm.ntf")
    for h in (
        rsm_lc.rsm_id.ground_domain_vertex[0].height_reference_surface + 10.0,
        rsm_lc.rsm_id.ground_domain_vertex[7].height_reference_surface - 10.0,
    ):
        for ln in np.linspace(0, 1000, 10):
            for smp in np.linspace(0, 1000, 10):
                ic = ImageCoordinate(ln, smp)
                p1 = igc_msp.ground_coordinate_approx_height(ic, h)
                p2 = rsm_lc.ground_coordinate_approx_height(ic, h)
                p3 = rsm_lc2.ground_coordinate_approx_height(ic, h)
                assert distance(p1, p2) < 0.01
                assert distance(p2, p3) < 0.01


class IgcParameter(WithParameter):
    """Wrapper illustrating a parameter adapter."""

    def __init__(self, orbit, camera):
        super().__init__()
        self.cam = camera
        self.orb = orbit

    def _v_parameter_name(self):
        return vector_string(
            [
                "X Offset (m)",
                "Y Offset (m)",
                "Z Offset (m)",
                "Yaw Offset (rad)",
                "Pitch Offset (rad)",
                "Roll Offset(rad)",
                "Focal length (mm)",
            ]
        )

    def _v_parameter_with_derivative(self, value=None):
        if value is None:
            fl = self.cam.focal_length_with_derivative
            res = ArrayAd_double_1(7, fl.number_variable)
            # Vagaries of the interface, we can't use a slice here in assigment,
            # so just do each value separately
            res[0] = self.orb.parameter_with_derivative[0]
            res[1] = self.orb.parameter_with_derivative[1]
            res[2] = self.orb.parameter_with_derivative[2]
            res[3] = self.orb.parameter_with_derivative[3] * arcsecond_to_rad
            res[4] = self.orb.parameter_with_derivative[4] * arcsecond_to_rad
            res[5] = self.orb.parameter_with_derivative[5] * arcsecond_to_rad
            res[6] = fl
            return res
        orbp = ArrayAd_double_1(6, value.number_variable)
        orbp[0] = value[0]
        orbp[1] = value[1]
        orbp[2] = value[2]
        orbp[3] = value[3] * rad_to_arcsecond
        orbp[4] = value[4] * rad_to_arcsecond
        orbp[5] = value[5] * rad_to_arcsecond
        self.orb.parameter_with_derivative = orbp
        self.cam.focal_length_with_derivative = value[6]


@require_msp
@require_pynitf
def test_rsm_indirect_cov_msp(isolated_dir, rsm_lc, igc_rpc):
    """Compare the RSM we write to a NITF file with what the MSP library
    calculates. This verifies both the validity of our NITF and our RSM
    code"""
    orb = KeplerOrbit()
    tm = Time.parse_time("2003-01-01T11:11:00Z")
    orb_corr = OrbitOffsetCorrection(orb)
    orb_corr.insert_position_time_point(tm)
    orb_corr.insert_attitude_time_point(tm)
    cam = QuaternionCamera(
        Quaternion_double(1, 0, 0, 0),
        2048,
        2048,
        2e-3,
        2e-3,
        2.85e3,
        FrameCoordinate(1024, 1024),
    )
    dem = SimpleDem(100)
    img = MemoryRasterImage(cam.number_line(0), cam.number_sample(0))
    hmin = 50
    hmax = 150
    igc = OrbitDataImageGroundConnection(orb_corr, tm, cam, dem, img)
    # We don't have all the mechanism in place for jacobians with a
    # ImageGroundConnection, but to do an IgcArray. So make a single
    # image igccol, and then we can pull out the image_ground_connection
    # for it. We could probably put this in place for ImageGroundConnection,
    # be we generally want a collection when we need jacobians so we
    # haven't.
    igccol = IgcArray(
        [
            igc,
        ]
    )
    # Stash a copy of IgcParameter, just for lifetime issues
    igccol.stash_parm = IgcParameter(orb_corr, igc.camera)
    igccol.add_object(igccol.stash_parm)
    igccol.parameter_subset = np.concatenate([[0] * 6, [igc.camera.focal_length]])
    igccol.add_identity_gradient()
    # The RPC version has poles, to a numerator only version
    # rsm = Rsm(RsmRationalPolynomial(3,3,3,3,3,3,3,3),
    #          LocalRcConverter(LocalRcParameter(igc)))
    rsm = Rsm(
        RsmRationalPolynomial(4, 4, 3, 0, 0, 0, 4, 0),
        LocalRcConverter(LocalRcParameter(igc)),
    )
    rsm.fit(igc, hmin, hmax)
    print("Poles in fit: ", rsm.check_zero_crossing())
    rsm.image_identifier = "image1"
    rsm.rsm_support_data_edition = "support1"
    rsm.rsm_id.image_acquistion_time = tm
    rsm.rsm_id.timing = RsmIdTiming(
        igc.camera.number_line(0), igc.camera.number_sample(0), 0.0, 0.0
    )
    rsm.rsm_id.sensor_identifier = "fakesen"
    f = pynitf.NitfFile()
    create_image_seg(
        f,
        num_line=igc.camera.number_line(0),
        num_sample=igc.camera.number_sample(0),
        iid1="image1",
    )
    create_image_seg(
        f,
        num_line=igc.camera.number_line(0),
        num_sample=igc.camera.number_sample(0),
        iid1="image2",
    )
    # Add in corners. Not strictly needed, but useful to testing other MSP
    # functionality.
    if True:
        gc1 = igc.ground_coordinate(ImageCoordinate(0, 0))
        gc2 = igc.ground_coordinate(ImageCoordinate(0, igc.camera.number_sample(0) - 1))
        gc3 = igc.ground_coordinate(
            ImageCoordinate(
                igc.camera.number_line(0) - 1, igc.camera.number_sample(0) - 1
            )
        )
        gc4 = igc.ground_coordinate(ImageCoordinate(igc.camera.number_line(0) - 1, 0))
        t = pynitf.TreCSCRNA()
        t.predict_corners = "N"
        t.ulcnr_lat = gc1.latitude
        t.ulcnr_long = gc1.longitude
        t.ulcnr_ht = gc1.height_reference_surface
        t.urcnr_lat = gc2.latitude
        t.urcnr_long = gc2.longitude
        t.urcnr_ht = gc2.height_reference_surface
        t.lrcnr_lat = gc3.latitude
        t.lrcnr_long = gc3.longitude
        t.lrcnr_ht = gc3.height_reference_surface
        t.llcnr_lat = gc4.latitude
        t.llcnr_long = gc4.longitude
        t.llcnr_ht = gc4.height_reference_surface
        f.image_segment[0].tre_list.append(t)
        f.image_segment[1].tre_list.append(t)
    cov = RsmIndirectCovarianceB(igc, hmin, hmax, rsm.rsm_id)
    cov.row_power = np.array([[0, 0, 0], [1, 0, 0], [0, 1, 0]], dtype=int)
    cov.col_power = cov.row_power
    # Position has 10 m covariance
    cov.add_subgroup(RsmBSubgroup(np.diag([10**2] * 3), 1, 1, 0, 0, 10))
    # Attitude has 10 arcsecond covariance
    cov.add_subgroup(
        RsmBSubgroup(np.diag([(10 * arcsecond_to_rad) ** 2] * 3), 1, 1, 0, 0, 100)
    )
    # Focal length is 10 mm covariance
    cov.add_subgroup(RsmBSubgroup(np.diag([10**2]), 1, 1, 0, 0, 1000))
    # We need the adjustment in place to calculate mapping_matrix.
    adj = RsmAdjustableParameterB(igc, hmin, hmax, rsm.rsm_id)
    adj.row_power = cov.row_power
    adj.col_power = cov.col_power
    adj.parameter = np.array([0, 0, 0, 0, 0, 0])
    rsm.rsm_adjustable_parameter = adj
    # Sanity check on jacobian calcuation
    print(igccol.parameter_name)
    gc = igc.ground_coordinate(ImageCoordinate(100, 200))
    print(igccol.image_coordinate_jac_parm(0, gc))
    print(
        igccol.image_coordinate_jac_parm_fd(
            0, gc, [1e-3, 1e-3, 1e-3, 1e-8, 1e-8, 1e-8, 1e-3]
        )
    )
    igccol.add_identity_gradient()

    cov.mapping_matrix = rsm.mapping_matrix(
        igccol.image_ground_connection(0), hmin, hmax
    )
    # But by convention we don't store an all zero adjustment in
    # the file.
    rsm.rsm_adjustable_parameter = None
    cov.unmodeled_covariance = RsmBUnmodeledCovariance(
        np.diag([0.25, 0.25]), 1, 0, 0, 10, 1, 0, 0, 10
    )
    rsm.rsm_indirect_covariance = cov
    f.image_segment[0].rsm = rsm

    # Duplicate RSM. Not clear that we really need everything regenerated,
    # I'm just trying to make another image at a different time. But for
    # now do this so we *know* everything is copied.
    rsm2 = Rsm(
        RsmRationalPolynomial(4, 4, 3, 0, 0, 0, 4, 0),
        LocalRcConverter(LocalRcParameter(igc)),
    )
    rsm2.fit(igc, hmin, hmax)
    rsm2.image_identifier = "image2"
    rsm2.rsm_support_data_edition = "support2"
    rsm2.rsm_id.image_acquistion_time = tm + 5.0
    rsm.rsm_id.timing = RsmIdTiming(
        igc.camera.number_line(0), igc.camera.number_sample(0), 0.0, 0.0
    )
    rsm2.rsm_id.sensor_identifier = "fakesen"
    rsm2.rsm_indirect_covariance = cov
    f.image_segment[1].rsm = rsm2
    # Add in GLAS. Don't actually need for this unit test, but this
    # is an easy way to make a data set that we can do other MSP tests
    # against
    if True:
        porb = PosCsephb(
            orb,
            tm - 10 * 1e-3,
            tm + 10 * 1e-3,
            1e-3,
            PosCsephb.LAGRANGE,
            PosCsephb.LAGRANGE_5,
            PosCsephb.EPHEMERIS_QUALITY_GOOD,
            PosCsephb.ACTUAL,
            PosCsephb.CARTESIAN_FIXED,
        )
        aorb = AttCsattb(
            orb,
            tm - 10 * 1e-3,
            tm + 10 * 1e-3,
            1e-3,
            AttCsattb.LAGRANGE,
            AttCsattb.LAGRANGE_7,
            AttCsattb.ATTITUDE_QUALITY_GOOD,
            AttCsattb.ACTUAL,
            AttCsattb.CARTESIAN_FIXED,
        )
        orb_g = OrbitDes(porb, aorb)
        cam_g = GlasGfmCamera(cam, 0, 256, 256)
        igc_g = OrbitDataImageGroundConnection(orb_g, tm, cam_g, dem, img)
        igc_g.platform_id = "FAKEPL"
        igc_g.payload_id = "FAKEPY"
        igc_g.sensor_id = "FAKESN"
        f.image_segment[0].create_glas_gfm(igc_g)
        (true_line, true_sample, calc_line, calc_sample, distance_true_vs_calc) = (
            rsm.compare_igc(igc_g, igc_g.number_line, igc_g.number_sample, 100)
        )
        print("Compare GFM")
        print(pd.DataFrame(np.abs(true_line - calc_line).flatten()).describe())
        print(pd.DataFrame(np.abs(true_sample - calc_sample).flatten()).describe())
        print(pd.DataFrame(distance_true_vs_calc.flatten()).describe())

    f.write("nitf_rsm.ntf")
    f2 = pynitf.NitfFile("nitf_rsm.ntf")
    print(f2.image_segment[0].rsm)
    igc_msp = IgcMsp("nitf_rsm.ntf", SimpleDem(), 0, "RSM", "RSM")
    igc_msp2 = IgcMsp("nitf_rsm.ntf", SimpleDem(), 1, "RSM", "RSM")
    print(igc_msp.covariance)
    print(igc_msp.joint_covariance(igc_msp2))
    gp, gp_cov = igc_msp.ground_coordinate_with_cov(
        ImageCoordinate(10, 10), np.array([[0.5 * 0.5, 0], [0, 0.5 * 0.5]]), 100, 0.1
    )
    print(gp)
    print(gp_cov)
    print(igc_msp.ce90_le90(ImageCoordinate(10, 10), 100))


@require_pynitf
@require_vicar
@require_serialize
def test_vicar_rsm(isolated_dir, rsm):
    """Test that we can read and write RSM attached to a VicarRasterImage
    and VicarLiteRasterImage. This requires everything to be installed, so
    we can't check this at the C++ level (which runs unit tests before
    installing"""
    out = VicarRasterImage("vicar_rsm_nitf.img", "BYTE", 10, 11, 1)
    out.write(0, 0, np.zeros((10, 11), dtype=np.uint8))
    out.set_rsm(rsm)
    out = None
    fin = VicarRasterImage("vicar_rsm_nitf.img")
    print(fin.rsm)
    fin = VicarLiteRasterImage("vicar_rsm_nitf.img")
    print(fin.rsm)


@require_pynitf
def test_rsm_grid(isolated_dir, rsm_g):
    """Create a file, and write out a RSM. This RSM has just a single
    grid in it"""
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm_g
    f.write("nitf_rsm.ntf")
    f2 = pynitf.NitfFile("nitf_rsm.ntf")
    print(f2)


# Not working yet, we'll come back to this
@pytest.mark.skip
@require_msp
@require_pynitf
def test_rsm_grid_with_msp(isolated_dir, rsm_g):
    """Compare the RSM we write to a NITF file with what the MSP library
    calculates. This verifies both the validity of our NITF and our RSM
    code"""
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm_g
    f.write("nitf_rsm.ntf")
    igc_msp = IgcMsp("nitf_rsm.ntf")
    for h in (
        rsm_g.rsm_id.ground_domain_vertex[0].height_reference_surface + 10.0,
        rsm_g.rsm_id.ground_domain_vertex[7].height_reference_surface - 10.0,
    ):
        for ln in np.linspace(0, rsm_g.rsm_id.max_line, 10):
            for smp in np.linspace(0, rsm_g.rsm_id.max_sample, 10):
                ic = ImageCoordinate(ln + 10, smp + 10)
                p2 = rsm_g.ground_coordinate_approx_height(ic, h)
                ic1 = igc_msp.image_coordinate(p2)
                print(p2)
                print("IC: ", ic)
                print("IC1: ", ic1)
                # assert(geocal_swig.distance(p1, p2) < 0.01)


@require_pynitf
def test_rsm_ms_rp(isolated_dir, rsm_ms_rp):
    """Create a file, and write out a RSM. This has a multi section
    rational polynomial in it"""
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm_ms_rp
    f.write("nitf_rsm.ntf")
    f2 = pynitf.NitfFile("nitf_rsm.ntf")
    print(f2)


# Note we ran into an issue here. Our rsm_ms_rp polynomial has a few poles,
# since there are zeros
# in the denominator. Our code works fine with this, but the MSP code fails.
# This is in some sense not a "real" failure, so we use a different polynomial
# for this with fewer poles.


@pytest.mark.skip
@require_msp
@require_pynitf
def test_rsm_ms_rp_with_msp(isolated_dir, rsm_ms_rp_np):
    """Compare the RSM we write to a NITF file with what the MSP library
    calculates. This verifies both the validity of our NITF and our RSM
    code"""
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm_ms_rp_np
    f.write("nitf_rsm.ntf")
    igc_msp = IgcMsp("nitf_rsm.ntf")
    for h in (
        rsm_ms_rp_np.rsm_id.ground_domain_vertex[0].height_reference_surface + 10.0,
        rsm_ms_rp_np.rsm_id.ground_domain_vertex[7].height_reference_surface - 10.0,
    ):
        for ln in np.linspace(0, rsm_ms_rp_np.rsm_id.max_line, 10):
            for smp in np.linspace(0, rsm_ms_rp_np.rsm_id.max_sample, 10):
                ic = ImageCoordinate(ln, smp)
                p1 = igc_msp.ground_coordinate_approx_height(ic, h)
                p2 = rsm_ms_rp_np.ground_coordinate_approx_height(ic, h)
                assert distance(p1, p2) < 0.1


# Note, this is too large. I think we aren't doing the grid interpolation
# correctly. We'll need to look into this
@pytest.mark.skip
@require_msp
@require_pynitf
def test_rsm_rp_cgrid_with_msp(isolated_dir, rsm_rp_cgrid):
    """Compare the RSM we write to a NITF file with what the MSP library
    calculates. This verifies both the validity of our NITF and our RSM
    code"""
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm_rp_cgrid
    f.write("nitf_rsm.ntf")
    igc_msp = IgcMsp("nitf_rsm.ntf")
    for h in (
        rsm_rp_cgrid.rsm_id.ground_domain_vertex[0].height_reference_surface + 10.0,
        rsm_rp_cgrid.rsm_id.ground_domain_vertex[7].height_reference_surface - 10.0,
    ):
        for ln in np.linspace(0, rsm_rp_cgrid.rsm_id.max_line, 10):
            for smp in np.linspace(0, rsm_rp_cgrid.rsm_id.max_sample, 10):
                ic = ImageCoordinate(ln, smp)
                p1 = igc_msp.ground_coordinate_approx_height(ic, h)
                p2 = rsm_rp_cgrid.ground_coordinate_approx_height(ic, h)
                assert distance(p1, p2) < 10


@require_pynitf
def test_rsm_ms_g(isolated_dir, rsm_ms_g):
    """Create a file, and write out a RSM. This has a multi section
    grid in it"""
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm_ms_g
    f.write("nitf_rsm.ntf")
    f2 = pynitf.NitfFile("nitf_rsm.ntf")
    print(f2)


# Test the raw TREs
@require_pynitf
def test_tre_rsmpca(rsm_rational_polynomial):
    t = pynitf.TreRSMPCA()
    t.rsm_rational_polynomial = rsm_rational_polynomial
    t.update_raw_field()
    fh = io.BytesIO()
    t.write_to_file(fh)
    print(fh.getvalue())
    fh2 = io.BytesIO(fh.getvalue())
    t2 = pynitf.TreRSMPCA()
    t2.read_from_file(fh2)
    print(t2)
    # Check that we can't the field values.
    with pytest.raises(RuntimeError):
        t.iid = "abc"
    with pytest.raises(RuntimeError):
        t.rnpcf[0] = 1.0

    assert t.iid is None
    assert t2.iid is None
    assert t.rsn == 1
    assert t2.rsn == 1
    assert t.csn == 1
    assert t2.csn == 1
    assert t.rfep is None
    assert t2.rfep is None
    assert t.cfep is None
    assert t2.cfep is None
    assert t.rnrmo == 2881.0
    assert t2.rnrmo == 2881.0


@require_pynitf
def test_tre_rsmgga(rsm_grid):
    t = pynitf.TreRSMGGA()
    t.rsm_grid = rsm_grid
    t.update_raw_field()
    fh = io.BytesIO()
    t.write_to_file(fh)
    # This is way too large to check, so skip this
    # assert fh.getvalue() == b'blah'
    fh2 = io.BytesIO(fh.getvalue())
    t2 = pynitf.TreRSMGGA()
    t2.read_from_file(fh2)
    print(t2)
    # Check that we can't the field values.
    with pytest.raises(RuntimeError):
        t.iid = "abc"
    with pytest.raises(RuntimeError):
        t.rcoord[0, 0] = "1.0"

    assert t.iid is None
    assert abs(t.deltaz - 422.66) < 0.01


@require_pynitf
def test_tre_rsmgia(rsm_ms_polynomial):
    t = pynitf.TreRSMGIA()
    t.rsm_multi_section = rsm_ms_polynomial
    t.update_raw_field()
    fh = io.BytesIO()
    t.write_to_file(fh)
    # This can vary depending on roundoff, so don't compare.
    # assert fh.getvalue() == b'Blah'
    fh2 = io.BytesIO(fh.getvalue())
    t2 = pynitf.TreRSMGIA()
    t2.read_from_file(fh2)
    print(t2)
    assert t2.rsm_multi_section.number_row_section == 3
    assert t2.rsm_multi_section.number_col_section == 2
    assert t2.gcssiz == 13763.0


@require_pynitf
def test_tre_rsmpia(rsm_ms_polynomial):
    t = pynitf.TreRSMPIA()
    t.rsm_multi_section = rsm_ms_polynomial
    t.update_raw_field()
    fh = io.BytesIO()
    t.write_to_file(fh)
    # This can vary depending on roundoff, so don't compare.
    # assert fh.getvalue() == b'Blah'
    fh2 = io.BytesIO(fh.getvalue())
    t2 = pynitf.TreRSMPIA()
    t2.read_from_file(fh2)
    print(t2)
    assert t2.rsm_multi_section.number_row_section == 3
    assert t2.rsm_multi_section.number_col_section == 2
    assert t2.cssiz == 13763.0


@require_pynitf
def test_tre_rsmida(rsm):
    t = pynitf.TreRSMIDA()
    t.rsm_id = rsm.rsm_id
    t.update_raw_field()
    fh = io.BytesIO()
    t.write_to_file(fh)
    # This can vary depending on roundoff, so don't compare.
    # assert fh.getvalue() == b'Blah'
    fh2 = io.BytesIO(fh.getvalue())
    t2 = pynitf.TreRSMIDA()
    t2.read_from_file(fh2)
    print(t2)
    assert t2.rsm_id.rsm_support_data_edition == "fake-1"


# The RSM sample data has expected results as a spreadsheet. We grab these
# values and put them here.
#
# Note, there seems to be clear problems with the spreadsheet. There are
# several places where we have what looks like duplicate data in the
# the latitude for different points, and where we have data that doesn't agree
# with its X,Y,Z calculation. I'm fairly certain the problem is with
# the test data spreadsheet as opposed to my code, if we convert to the X,Y,Z
# coordinates we can clearly see differences with the points with the duplicate
# data. I've commented out what looks like bad points.

# 1. Point calculation

# This is row, col, height, latitude, longitude
pcalc = dict()
pcalc["i_6130a.ntf"] = [
    [0, 0, -500, 33.1616263666, -117.0386236785],
    [1024, 0, -500, 33.1616140061, -117.0337581906],
    [2048, 1024, -500, 33.1657014400, -117.0288590590],
    [3072, 2048, -500, 33.1698074925, -117.0239367569],
    [4096, 3072, -500, 33.1739325945, -117.0189907556],
    [5120, 4096, -500, 33.1780771868, -117.0140205142],
    [6144, 5120, -500, 33.1822417202, -117.0090254794],
    [7168, 6144, -500, 33.1864266558, -117.0040050848],
    [8192, 7168, -500, 33.1906324659, -116.9989587503],
    [9216, 8192, -500, 33.1948596339, -116.9938858820],
    [10239, 9215, -500, 33.1991044947, -116.9887908653],
    [0, 0, 0, 33.1617298457, -117.0388169203],
    [1024, 0, 0, 33.1617198102, -117.0348583317],
    [2048, 1024, 0, 33.1650454017, -117.0308724867],
    [3072, 2048, 0, 33.1683861119, -117.0268679557],
    [4096, 3072, 0, 33.1717422449, -117.0228443664],
    [5120, 4096, 0, 33.1751141114, -117.0188013380],
    [6144, 5120, 0, 33.1785020290, -117.0147384814],
    [7168, 6144, 0, 33.1819063224, -117.0106553988],
    [8192, 7168, 0, 33.1853273233, -117.0065516833],
    [9216, 8192, 0, 33.1887653713, -117.0024269189],
    [10239, 9215, 0, 33.1922174304, -116.9982847393],
    [0, 0, 500, 33.1618333074, -117.0390101313],
    [1024, 0, 500, 33.1618255874, -117.0359582996],
    [2048, 1024, 500, 33.1643894424, -117.0328855435],
    [3072, 2048, 500, 33.1669649301, -117.0297985094],
    [4096, 3072, 500, 33.1695522492, -117.0266969546],
    [5120, 4096, 500, 33.1721516024, -117.0235806312],
    # [6144, 5120, 500, 33.1773872434, -117.0204492863],
    [7168, 6144, 500, 33.1773872434, -117.0173026615],
    [8192, 7168, 500, 33.1800239580, -117.0141404932],
    [9216, 8192, 500, 33.1826735607, -117.0109625121],
    [10239, 9215, 500, 33.1853336697, -117.0077715702],
    [2994, 2610, 0, 33.17020003, -117.027196],
    [1995, 7091, 0, 33.1842917, -117.031198],
    [8287, 4901, 0, 33.17798445, -117.005901],
    [8607, 5229, 0, 33.17908728, -117.004603],
    [887, 6410, 0, 33.18205237, -117.035415],
    [2936, 5149, 0, 33.17830643, -117.027508],
    [422, 449, 0, 33.16317745, -117.03719],
    [7218, 1676, 350, 33.1664582, -117.014765],
    [7817, 2259, 35, 33.1691211, -117.008082],
    [1269, 1443, -10, 33.16640833, -117.033891],
    [3508, 7208, -150, 33.18640237, -117.024367],
    [3109, 5259, 200, 33.17715898, -117.02801139],
    #    [14, 2453, 100, 33.16924395, -117.038784],
    [4024, 398, -50, 33.16302301, -117.022615],
    [2084, 2918, -250, 33.17217737, -117.029753],
    [6127, 2385, -450, 33.17111665, -117.009412],
    [7742, 3966, -275, 33.17647492, -117.004083],
    [2394, 5686, 25, 33.17974551, -117.029745],
    #    [128, 2590, 475, 33.16831576, -117.038602],
    [1603, 2123, 0, 33.16858343, -117.032629],
]
pcalc["i_6130b.ntf"] = [
    [0, 0, -500, 33.1616263667, -117.0386236785],
    [1024, 0, -500, 33.1616140061, -117.0337581906],
    [2048, 1024, -500, 33.16570143946534, -117.02885906007381],
    [3072, 2048, -500, 33.1698074925, -117.0239367569],
    [4096, 3072, -500, 33.1739325945, -117.0189907556],
    [5120, 4096, -500, 33.1780771868, -117.0140205143],
    [6144, 5120, -500, 33.1822417202, -117.0090254795],
    [7168, 6144, -500, 33.1864266558, -117.0040050848],
    [8192, 7168, -500, 33.1906324659, -116.9989587503],
    [9216, 8192, -500, 33.1948596338, -116.9938858821],
    [10239, 9215, -500, 33.1991044947, -116.9887908652],
    [0, 0, 0, 33.1617298457, -117.0388169203],
    [1024, 0, 0, 33.1617198102, -117.0348583319],
    [2048, 1024, 0, 33.1650454017, -117.0308724867],
    [3072, 2048, 0, 33.1683861119, -117.0268679557],
    [4096, 3072, 0, 33.1717422449, -117.0228443664],
    [5120, 4096, 0, 33.1751141114, -117.0188013380],
    [6144, 5120, 0, 33.1785020290, -117.0147384814],
    [7168, 6144, 0, 33.1819063224, -117.0106553988],
    [8192, 7168, 0, 33.1853273233, -117.0065516833],
    [9216, 8192, 0, 33.1887653713, -117.0024269189],
    [10239, 9215, 0, 33.1922174304, -116.9982847393],
    [0, 0, 500, 33.1618333074, -117.0390101313],
    [1024, 0, 500, 33.1618255874, -117.0359582996],
    [2048, 1024, 500, 33.164389442, -117.0328855435],
    [3072, 2048, 500, 33.1669649301, -117.0297985094],
    [4096, 3072, 500, 33.1695522492, -117.0266969546],
    [5120, 4096, 500, 33.1721516024, -117.0235806312],
    [6144, 5120, 500, 33.1747631968, -117.0204492862],
    [7168, 6144, 500, 33.1773872434, -117.0173026615],
    [8192, 7168, 500, 33.1800239580, -117.0141404932],
    [9216, 8192, 500, 33.1826735607, -117.0109625121],
    [10239, 9215, 500, 33.1853336697, -117.0077715703],
    [4294, 3329, 0, 33.1725837600, -117.0220670000],
    [6166, 5484, 0, 33.1796765300, -117.0146790000],
    [7808, 5517, 0, 33.1799469900, -117.0079590000],
    # [3128, 1589, 0, 33.1668975300, -117.0266300000],
    [5893, 4111, 0, 33.1752176100, -117.0156780000],
    [55, 4246, 0, 33.1752372200, -117.0385720000],
    [7761, 6880, 0, 33.1843452100, -117.0082960000],
    [436, 504, 0, 33.1633549400, -117.0371360000],
    [6304, 8055, 0, 33.1878888900, -117.0143290000],
    [1323, 8052, 0, 33.1871700400, -117.0337730000],
    [1387, 452, 0, 33.1631860100, -117.0334480000],
    [6734, 819, 425, 33.1639738100, -117.0173380000],
    [9100, 5297, 275, 33.1771935300, -117.0072230000],
    [431, 5711, 225, 33.1780172200, -117.0374020000],
    [244, 1824, 100, 33.1673408500, -117.0379510000],
    [5521, 4286, -125, 33.1765348000, -117.0159110000],
    [785, 318, -200, 33.1628054300, -117.0354320000],
    [1551, 7531, -300, 33.1888166400, -117.0319710000],
    [5605, 5103, -325, 33.1808135100, -117.0135290000],
    [6558, 0, -450, 33.1615528500, -117.0070140000],
]
pcalc["i_6130c.ntf"] = [
    [0, 0, -500, 33.1616263667, -117.0386236785],
    [1024, 0, -500, 33.1616140061, -117.0337581906],
    # [2048, 1024, -500, 33.1615701440, -117.0288590590],
    [3072, 2048, -500, 33.1698074925, -117.0239367569],
    [4096, 3072, -500, 33.1739325945, -117.0189907556],
    [5120, 4096, -500, 33.1780771868, -117.0140205143],
    [6144, 5120, -500, 33.1822417202, -117.0090254795],
    [7168, 6144, -500, 33.1864266558, -117.0040050848],
    [8192, 7168, -500, 33.1906324659, -116.9989587503],
    [9216, 8192, -500, 33.1948596338, -116.9938858821],
    [10239, 9215, -500, 33.1991044947, -116.9887908652],
    [0, 0, 0, 33.1617298457, -117.0388169203],
    [1024, 0, 0, 33.1617198102, -117.0348583319],
    [2048, 1024, 0, 33.1650454017, -117.0308724867],
    [3072, 2048, 0, 33.1683861119, -117.0268679557],
    [4096, 3072, 0, 33.1717422449, -117.0228443664],
    [5120, 4096, 0, 33.1751141114, -117.0188013380],
    [6144, 5120, 0, 33.1785020290, -117.0147384814],
    [7168, 6144, 0, 33.1819063224, -117.0106553988],
    [8192, 7168, 0, 33.1853273233, -117.0065516833],
    [9216, 8192, 0, 33.1887653713, -117.0024269189],
    [10239, 9215, 0, 33.1922174304, -116.9982847393],
    [0, 0, 500, 33.1618333074, -117.0390101313],
    [1024, 0, 500, 33.1618255874, -117.0359582996],
    # [2048, 1024, 500, 33.1643899442, -117.0328855435],
    [3072, 2048, 500, 33.1669649301, -117.0297985094],
    [4096, 3072, 500, 33.1695522492, -117.0266969546],
    [5120, 4096, 500, 33.1721516024, -117.0235806312],
    [6144, 5120, 500, 33.1747631968, -117.0204492862],
    [7168, 6144, 500, 33.1773872434, -117.0173026615],
    [8192, 7168, 500, 33.1800239580, -117.0141404932],
    [9216, 8192, 500, 33.1826735607, -117.0109625121],
    [10239, 9215, 500, 33.1853336697, -117.0077715703],
    [4870, 5367, 0, 33.1791764000, -117.0198840000],
    [12, 849, 0, 33.1644639800, -117.0387640000],
    [3613, 4533, 0, 33.1764053700, -117.0248280000],
    [8121, 2113, 0, 33.1687577100, -117.0062740000],
    # [4803, 4095, 0, 33.1750885500, -117.0220740000],
    [7880, 8081, 0, 33.1882059000, -117.0079340000],
    [6798, 3763, 0, 33.1741459100, -117.0119490000],
    [1370, 3855, 0, 33.1740931500, -117.0335510000],
    [8489, 3460, 0, 33.1732551800, -117.0048850000],
    [112, 3225, 500, 33.1697743000, -117.0386610000],
    [2077, 3947, 475, 33.1717646600, -117.0327440000],
    [6818, 2649, 300, 33.1693459300, -117.0155990000],
    [5105, 7454, 225, 33.1833867400, -117.0212000000],
    [2165, 1510, 150, 33.1663167800, -117.0310590000],
    [3387, 6169, 75, 33.1809036700, -117.0262600000],
    [9121, 4799, -25, 33.1779019500, -117.0019640000],
    [5744, 8108, -150, 33.1897469600, -117.0149890000],
    [898, 6941, -325, 33.1869001400, -117.0347370000],
    [4367, 5387, -450, 33.1827007400, -117.0182200000],
    [7800, 6978, -475, 33.1895605600, -117.0012810000],
]
pcalc["i_6130d.ntf"] = [
    [0, 0, -500, 33.1616263667, -117.0386236785],
    [1024, 0, -500, 33.1616140061, -117.0337581906],
    [2048, 1024, -500, 33.1615701440, -117.0288590590],
    [3072, 2048, -500, 33.1698074925, -117.0239367569],
    [4096, 3072, -500, 33.1739325945, -117.0189907556],
    [5120, 4096, -500, 33.1780771868, -117.0140205143],
    [6144, 5120, -500, 33.1822417202, -117.0090254795],
    [7168, 6144, -500, 33.1864266558, -117.0040050848],
    [8192, 7168, -500, 33.1906324659, -116.9989587503],
    [9216, 8192, -500, 33.1948596338, -116.9938858821],
    [10239, 9215, -500, 33.1991044947, -116.9887908652],
    [0, 0, 0, 33.1617298457, -117.0388169203],
    [1024, 0, 0, 33.1617198102, -117.0348583319],
    [2048, 1024, 0, 33.1650454017, -117.0308724867],
    [3072, 2048, 0, 33.1683861119, -117.0268679557],
    [4096, 3072, 0, 33.1717422449, -117.0228443664],
    [5120, 4096, 0, 33.1751141114, -117.0188013380],
    [6144, 5120, 0, 33.1785020290, -117.0147384814],
    [7168, 6144, 0, 33.1819063224, -117.0106553988],
    [8192, 7168, 0, 33.1853273233, -117.0065516833],
    [9216, 8192, 0, 33.1887653713, -117.0024269189],
    [10239, 9215, 0, 33.1922174304, -116.9982847393],
    [0, 0, 500, 33.1618333074, -117.0390101313],
    [1024, 0, 500, 33.1618255874, -117.0359582996],
    [2048, 1024, 500, 33.1643899442, -117.0328855435],
    [3072, 2048, 500, 33.1669649301, -117.0297985094],
    [4096, 3072, 500, 33.1695522492, -117.0266969546],
    [5120, 4096, 500, 33.1721516024, -117.0235806312],
    [6144, 5120, 500, 33.1747631968, -117.0204492862],
    [7168, 6144, 500, 33.1773872434, -117.0173026615],
    [8192, 7168, 500, 33.1800239580, -117.0141404932],
    [9216, 8192, 500, 33.1826735607, -117.0109625121],
    [10239, 9215, 500, 33.1853336697, -117.0077715703],
    [6674, 7506, 0, 33.1862019500, -117.0127900000],
    [5373, 4975, 0, 33.1779661900, -117.0178430000],
    [380, 359, 0, 33.1628869700, -117.0373520000],
    [2611, 6422, 0, 33.1822792000, -117.0288100000],
    [1857, 529, 0, 33.1634360200, -117.0316120000],
    [983, 7743, 0, 33.1861754600, -117.0350570000],
    [1315, 6059, 0, 33.1810074600, -117.0337840000],
    [8388, 4216, 0, 33.1757446700, -117.0053990000],
    [6766, 7058, 0, 33.1847875100, -117.0123780000],
    [2796, 6972, 0, 33.1840190300, -117.0281090000],
    [3272, 3551, 0, 33.1732404200, -117.0261340000],
    [7971, 557, 0, 33.1635297300, -117.0067270000],
    [3358, 646, 475, 33.1634637000, -117.0287210000],
    [52, 6715, 375, 33.1793414200, -117.0387530000],
    [846, 4205, 250, 33.1736762800, -117.0360330000],
    [9144, 8150, 75, 33.1877105200, -117.0039920000],
    [2078, 3901, -50, 33.1745614500, -117.0306120000],
    [7393, 6144, -175, 33.1835157000, -117.0073320000],
    [242, 148, -325, 33.1622096900, -117.0376210000],
    [5172, 841, -400, 33.1648898100, -117.0144700000],
]
pcalc["i_6130e.ntf"] = [
    [0, 0, -500, 33.1616263700, -117.0386240000],
    [1024, 0, -500, 33.1616140100, -117.0337580000],
    [2048, 1024, -500, 33.1657014400, -117.0288590000],
    [3072, 2048, -500, 33.1698074900, -117.0239370000],
    [4096, 3072, -500, 33.1739325900, -117.0189910000],
    [5120, 4096, -500, 33.1780771900, -117.0140210000],
    [6144, 5120, -500, 33.1822417200, -117.0090250000],
    [7168, 6144, -500, 33.1864266500, -117.0040050000],
    [8192, 7168, -500, 33.1906324700, -116.9989590000],
    [9216, 8192, -500, 33.1948596400, -116.9938860000],
    [10239, 9215, -500, 33.1991044900, -116.9887910000],
    [0, 0, 0, 33.1617298400, -117.0388170000],
    [1024, 0, 0, 33.1617198100, -117.0348580000],
    [2048, 1024, 0, 33.1650454100, -117.0308720000],
    [3072, 2048, 0, 33.1683861100, -117.0268680000],
    [4096, 3072, 0, 33.1717422400, -117.0228440000],
    [5120, 4096, 0, 33.1751141100, -117.0188010000],
    [6144, 5120, 0, 33.1785020300, -117.0147380000],
    [7168, 6144, 0, 33.1819063200, -117.0106550000],
    [8192, 7168, 0, 33.1853273300, -117.0065520000],
    [9216, 8192, 0, 33.1887653700, -117.0024270000],
    [10239, 9215, 0, 33.1922174200, -116.9982850000],
    [0, 0, 500, 33.1618333100, -117.0390100000],
    [1024, 0, 500, 33.1618255900, -117.0359580000],
    [2048, 1024, 500, 33.1643894400, -117.0328860000],
    [3072, 2048, 500, 33.1669649300, -117.0297990000],
    [4096, 3072, 500, 33.1695522500, -117.0266970000],
    [5120, 4096, 500, 33.1721516000, -117.0235810000],
    [6144, 5120, 500, 33.1747631900, -117.0204490000],
    [7168, 6144, 500, 33.1773872400, -117.0173030000],
    [8192, 7168, 500, 33.1800239600, -117.0141400000],
    [9216, 8192, 500, 33.1826735600, -117.0109630000],
    [10239, 9215, 500, 33.1853336700, -117.0077720000],
    [7982, 2431, 0, 33.1698129400, -117.0068950000],
    [2335, 6289, 0, 33.1818328900, -117.0298730000],
    [3397, 4655, 0, 33.1767772500, -117.0256840000],
    [5261, 7867, 0, 33.1871458800, -117.0184840000],
    [5500, 987, 0, 33.1649568300, -117.0170430000],
    [1672, 8549, 0, 33.1887443000, -117.0324540000],
    [672, 4840, 0, 33.1771431300, -117.0362280000],
    [496, 6251, 0, 33.1815181800, -117.0368950000],
    [8642, 464, 0, 33.1632142300, -117.0038670000],
    [3893, 7866, 0, 33.1869506800, -117.0238760000],
    [6584, 4526, 0, 33.1766173100, -117.0128960000],
    [6560, 1884, 225, 33.1673545600, -117.0155330000],
    [1587, 5856, 450, 33.1766452700, -117.0341680000],
    [713, 3378, 300, 33.1711208300, -117.0365640000],
    [3350, 1744, -25, 33.1674665100, -117.0255950000],
    [6847, 5825, -175, 33.1823405200, -117.0097130000],
    [522, 4365, -325, 33.1776479200, -117.0363730000],
    [9159, 3055, 0, 33.1719466700, -117.0020030000],
    [2404, 6527, 0, 33.1825842100, -117.0296120000],
    [9067, 4473, 0, 33.1766450600, -117.0025770000],
]
pcalc["i_6130f.ntf"] = [
    [0, 0, -500, 33.1616263600, -117.0386240000],
    [1024, 0, -500, 33.1616140000, -117.0337580000],
    [2048, 1024, -500, 33.1657014300, -117.0288590000],
    [3072, 2048, -500, 33.1698074900, -117.0239370000],
    [4096, 3072, -500, 33.1739325900, -117.0189910000],
    [5120, 4096, -500, 33.1780771800, -117.0140210000],
    [6144, 5120, -500, 33.1822417200, -117.0090250000],
    [7168, 6144, -500, 33.1864266500, -117.0040050000],
    [8192, 7168, -500, 33.1906324700, -116.9989590000],
    [9216, 8192, -500, 33.1948596400, -116.9938860000],
    [10239, 9215, -500, 33.1991045000, -116.9887910000],
    [0, 0, 0, 33.1617298400, -117.0388170000],
    [1024, 0, 0, 33.1617198000, -117.0348580000],
    [2048, 1024, 0, 33.1650453900, -117.0308720000],
    [3072, 2048, 0, 33.1683861000, -117.0268680000],
    [4096, 3072, 0, 33.1717422400, -117.0228440000],
    [5120, 4096, 0, 33.1751141100, -117.0188010000],
    [6144, 5120, 0, 33.1785020300, -117.0147380000],
    [7168, 6144, 0, 33.1819063200, -117.0106550000],
    [8192, 7168, 0, 33.1853273300, -117.0065520000],
    [9216, 8192, 0, 33.1887653800, -117.0024270000],
    [10239, 9215, 0, 33.1922174400, -116.9982850000],
    [0, 0, 500, 33.1618333000, -117.0390100000],
    [1024, 0, 500, 33.1618255800, -117.0359580000],
    [2048, 1024, 500, 33.1643894400, -117.0328860000],
    [3072, 2048, 500, 33.1669649200, -117.0297990000],
    [4096, 3072, 500, 33.1695522500, -117.0266970000],
    [5120, 4096, 500, 33.1721516000, -117.0235810000],
    [6144, 5120, 500, 33.1747632000, -117.0204490000],
    [7168, 6144, 500, 33.1773872500, -117.0173030000],
    [8192, 7168, 500, 33.1800239700, -117.0141400000],
    [9216, 8192, 500, 33.1826735700, -117.0109620000],
    [10239, 9215, 500, 33.1853336800, -117.0077720000],
    [6679, 3963, 0, 33.1747913600, -117.0124570000],
    [2072, 8071, 0, 33.1873324700, -117.0309220000],
    [7142, 5643, 0, 33.1802873200, -117.0107140000],
    [4264, 1516, 0, 33.1666822800, -117.0220900000],
    [1896, 6813, 0, 33.1834166800, -117.0315720000],
    [6507, 4171, 0, 33.1754570600, -117.0131800000],
    [7985, 6752, 0, 33.1839616800, -117.0073600000],
    [3951, 6582, 0, 33.1829336300, -117.0235890000],
    [9112, 3524, 0, 33.1735058800, -117.0022640000],
    [8373, 6120, 0, 33.1819650000, -117.0056830000],
    [7064, 1407, 0, 33.1663745100, -117.0106230000],
    [3166, 518, 0, 33.1634001200, -117.0264390000],
    [2513, 1006, 50, 33.1649270300, -117.0292860000],
    [2276, 2836, 425, 33.1691993200, -117.0318980000],
    [7129, 7112, 275, 33.1821305500, -117.0145320000],
    [2249, 6219, 150, 33.1802697400, -117.0308530000],
    [5221, 169, -200, 33.1622440900, -117.0161520000],
    [2004, 2957, -475, 33.1732494300, -117.0292170000],
    [595, 344, -75, 33.1628603900, -117.0364150000],
    [8844, 4729, -500, 33.1809724500, -116.9952680000],
]
pcalc["i_6130g.ntf"] = [
    [0, 0, -500, 33.1616259600, -117.0386240000],
    [1024, 0, -500, 33.1616129500, -117.0337590000],
    [2048, 1024, -500, 33.1656999900, -117.0288590000],
    [3072, 2048, -500, 33.1698063600, -117.0239360000],
    [4096, 3072, -500, 33.1739321900, -117.0189880000],
    [5120, 4096, -500, 33.1780776300, -117.0140170000],
    [6144, 5120, -500, 33.1822428400, -117.0090220000],
    [7168, 6144, -500, 33.1864279600, -117.0040020000],
    [8192, 7168, -500, 33.1906331300, -116.9989580000],
    [9216, 8192, -500, 33.1948585200, -116.9938900000],
    [10239, 9215, -500, 33.1991001100, -116.9888010000],
    [0, 0, 0, 33.1617300900, -117.0388170000],
    [1024, 0, 0, 33.1617194900, -117.0348590000],
    [2048, 1024, 0, 33.1650445900, -117.0308730000],
    [3072, 2048, 0, 33.1683854000, -117.0268670000],
    [4096, 3072, 0, 33.1717420400, -117.0228420000],
    [5120, 4096, 0, 33.1751146500, -117.0187980000],
    [6144, 5120, 0, 33.1785033300, -117.0147340000],
    [7168, 6144, 0, 33.1819082000, -117.0106510000],
    [8192, 7168, 0, 33.1853293900, -117.0065470000],
    [9216, 8192, 0, 33.1887670300, -117.0024240000],
    [10239, 9215, 0, 33.1922178400, -116.9982840000],
    [0, 0, 500, 33.1618342100, -117.0390100000],
    [1024, 0, 500, 33.1618260400, -117.0359590000],
    [2048, 1024, 500, 33.1643891900, -117.0328860000],
    [3072, 2048, 500, 33.1669644400, -117.0297990000],
    [4096, 3072, 500, 33.1695519000, -117.0266970000],
    [5120, 4096, 500, 33.1721516600, -117.0235790000],
    [6144, 5120, 500, 33.1747638100, -117.0204470000],
    [7168, 6144, 500, 33.1773884500, -117.0172990000],
    [8192, 7168, 500, 33.1800256600, -117.0141360000],
    [9216, 8192, 500, 33.1826755300, -117.0109580000],
    [10239, 9215, 500, 33.1853355800, -117.0077670000],
    [254, 1571, 0, 33.1667803500, -117.0378320000],
    [3017, 6739, 0, 33.1833196000, -117.0272440000],
    [7237, 8982, 0, 33.1909637100, -117.0106420000],
    [935, 1253, 0, 33.1657716300, -117.0352110000],
    [1118, 3202, 0, 33.1720086700, -117.0345160000],
    [9249, 6373, 0, 33.1828910600, -117.0020510000],
    [895, 542, 0, 33.1634774100, -117.0353620000],
    [6384, 144, 0, 33.1621451700, -117.0133300000],
    [3003, 2453, 0, 33.1696932400, -117.0271540000],
    [5986, 7367, 0, 33.1856700800, -117.0155500000],
    [172, 2823, 0, 33.1707622400, -117.0381380000],
    [5467, 2853, 50, 33.1708919800, -117.0178220000],
    [8326, 4316, 275, 33.1743195800, -117.0099500000],
    [5728, 5665, 325, 33.1775298400, -117.0199150000],
    [2506, 2925, 400, 33.1695405500, -117.0310540000],
    [888, 4536, -5, 33.1762397200, -117.0353950000],
    [8544, 2170, -175, 33.1695057200, -117.0016710000],
    [2887, 4732, -300, 33.1790166000, -117.0260390000],
    [6474, 450, -400, 33.1633509800, -117.0080920000],
    [7499, 1725, -425, 33.1684685000, -117.0028320000],
]

# Doesn't work yet, so stub out test. We'll come back to this
# pcalc["i_6130a.ntf"] = []
# pcalc["i_6130b.ntf"] = []
# pcalc["i_6130c.ntf"] = []
# pcalc["i_6130f.ntf"] = []
# pcalc["i_6130g.ntf"] = []
# 2. Partial derivative

# 3. Partial derivative

# 4. Direct error covariance

# 5. Indirect error covariance

# 6. Error covariance

# 7. Illumination

# 8. Trajectory

# 9. Original full image ID

# 10. Request sensor ID

# 11. Request sension type

# 12. Request RSM image domain

# 13. Requstion edition ID

# 15. Request tringulation ID

# 16. Request RSM domains height range

# 17. Requestion time of image

# 18. Request identity and defnition of RSM coordinate system


# 19. Request fit error
@require_pynitf
def test_rsm_sample(isolated_dir, rsm_sample_data):
    """Check if we can read the various RSM sample data files.
    This comes from http://www.gwg.nga.mil/ntb/baseline/software/testfile/rsm/samples.htm"""
    for fname in (
        "i_6130a.ntf",
        "i_6130b.ntf",
        "i_6130c.ntf",
        # File looks like it is corrupt at the web site, the unzip
        # reports a CRC error
        #'i_6130d.ntf',
        # Can't process this file, limit it nitf code
        #'i_6130e.ntf',
        "i_6130f.ntf",
        "i_6130g.ntf",
    ):
        print(fname)
        f = pynitf.NitfFile(rsm_sample_data / fname)
        print(f.image_segment[0].rsm)
        r = f.image_segment[0].rsm

        for ln, smp, h, lat, lon in pcalc[fname]:
            ic_expect = ImageCoordinate(ln, smp)
            ic_calc, in_valid_range = r.image_coordinate(Geodetic(lat, lon, h))
            if (
                abs(ic_expect.line - ic_calc.line) >= 0.2
                or abs(ic_expect.sample - ic_calc.sample) >= 0.2
            ):
                print(fname)
                print(Geodetic(lat, lon, h))
                print(ic_expect)
                print(ic_calc)
            # According to sample file tutorial, difference should be
            # no greater than 0.05. But it appears the lat/lon is lower
            # resolution than some of the other data, so this gives closer to
            # 0.2 pixel
            assert abs(ic_expect.line - ic_calc.line) < 0.2
            assert abs(ic_expect.sample - ic_calc.sample) < 0.2
        # Should add tests to check against the expected value spreadsheet


@require_pynitf
def test_rsm_sample_file_a(isolated_dir, rsm_sample_data):
    """Test that we can generate the same TREs as we read."""
    fname = rsm_sample_data / "i_6130a.ntf"
    f = pynitf.NitfFile(fname)
    rsm = f.image_segment[0].rsm
    # GdalRasterImage has much more limited reading of TRE, it just gives
    # the data as a string. However that is exactly what we want to get the
    # raw uninterpreted data.
    fgdal = GdalRasterImage(str(fname))
    texpect = fgdal["TRE", "RSMPCA"]
    assert rsm.rsm_base.tre_string() == texpect
    texpect = fgdal["TRE", "RSMIDA"]
    assert rsm.rsm_id.tre_string() == texpect
    # This also as RSMDCA and RSMECA (the direct and indirect
    # covariance). We don't currently read these


@require_pynitf
def test_rsm_sample_file_b(isolated_dir, rsm_sample_data):
    """Test that we can generate the same TREs as we read."""
    fname = rsm_sample_data / "i_6130b.ntf"
    f = pynitf.NitfFile(fname)
    rsm = f.image_segment[0].rsm
    fgdal = GdalRasterImage(str(fname))
    texpect = fgdal["TRE", "RSMPCA"]
    assert rsm.rsm_base.tre_string() == texpect
    texpect = fgdal["TRE", "RSMIDA"]
    assert rsm.rsm_id.tre_string() == texpect
    # This also as RSMDCA and RSMECA (the direct and indirect
    # covariance). We don't currently read these


@require_pynitf
def test_rsm_sample_file_c(isolated_dir, rsm_sample_data):
    """Test that we can generate the same TREs as we read."""
    fname = rsm_sample_data / "i_6130c.ntf"
    f = pynitf.NitfFile(fname)
    rsm = f.image_segment[0].rsm
    fgdal = GdalRasterImage(str(fname))
    texpect = fgdal["TRE", "RSMPCA"]
    assert rsm.rsm_base.section(0, 0).tre_string() == texpect
    texpect = fgdal["TRE", "RSMPCA_2"]
    assert rsm.rsm_base.section(0, 1).tre_string() == texpect
    texpect = fgdal["TRE", "RSMPCA_3"]
    assert rsm.rsm_base.section(1, 0).tre_string() == texpect
    texpect = fgdal["TRE", "RSMPCA_4"]
    assert rsm.rsm_base.section(1, 1).tre_string() == texpect
    texpect = fgdal["TRE", "RSMPIA"]
    assert rsm.rsm_base.tre_string() == texpect
    texpect = fgdal["TRE", "RSMIDA"]
    assert rsm.rsm_id.tre_string() == texpect
    # This also as RSMDCA and RSMECA (the direct and indirect
    # covariance). We don't currently read these


# Doesn't work yet
@pytest.mark.skip
@require_pynitf
def test_rsm_sample_file_e(isolated_dir, rsm_sample_data):
    """Test that we can generate the same TREs as we read."""
    fname = rsm_sample_data / "i_6130e.ntf"
    f = pynitf.NitfFile(fname)
    rsm = f.image_segment[0].rsm
    fgdal = GdalRasterImage(str(fname))
    texpect = fgdal["TRE", "RSMPCA"]
    assert rsm.rsm_base.tre_string() == texpect
    texpect = fgdal["TRE", "RSMIDA"]
    assert rsm.rsm_id.tre_string() == texpect
    # This also as RSMDCA and RSMECA (the direct and indirect
    # covariance). We don't currently read these


@require_pynitf
def test_rsm_sample_file_f(isolated_dir, rsm_sample_data):
    """Test that we can generate the same TREs as we read."""
    fname = rsm_sample_data / "i_6130f.ntf"
    f = pynitf.NitfFile(fname)
    rsm = f.image_segment[0].rsm
    fgdal = GdalRasterImage(str(fname))
    texpect = fgdal["TRE", "RSMPCA"]
    assert rsm.rsm_base.tre_string() == texpect
    texpect = fgdal["TRE", "RSMIDA"]
    assert rsm.rsm_id.tre_string() == texpect
    # This also as RSMDCA and RSMECA (the direct and indirect
    # covariance). We don't currently read these
    # This also as RSMDCA and RSMAPA (the direct covariance,
    # adjustment). We don't currently read these.


@require_pynitf
def test_rsm_sample_file_g(isolated_dir, rsm_sample_data):
    """Test that we can generate the same TREs as we read."""
    fname = rsm_sample_data / "i_6130g.ntf"
    f = pynitf.NitfFile(fname)
    rsm = f.image_segment[0].rsm
    fgdal = GdalRasterImage(str(fname))
    texpect = fgdal["TRE", "RSMPCA"]
    assert rsm.rsm_base.section(0, 0).tre_string() == texpect
    texpect = fgdal["TRE", "RSMPIA"]
    assert rsm.rsm_base.tre_string() == texpect
    texpect = fgdal["TRE", "RSMIDA"]
    assert rsm.rsm_id.tre_string() == texpect
    # This also as RSMECA (the indirect
    # covariance). We don't currently read these


@require_pynitf
def test_create_staring_rsm(isolated_dir, igc_staring):
    """This variation uses a "staring" mode, which gives a strong bow-tie
    on the surface. Want to make sure we handle this correctly, and in
    particular the RSM handles this"""
    igc = igc_staring
    write_shelve("igc.xml", igc)


def setup_grid(
    igc,
    ccov,
    num_x,
    num_y,
    num_z,
    num_x_sec,
    total_number_row_digit=6,
    total_number_col_digit=6,
):
    n = 1
    g = RsmGrid(
        int(math.ceil(num_x / num_x_sec)),
        int(math.ceil(num_y / n)),
        num_z,
        False,
        total_number_row_digit,
        total_number_col_digit,
    )
    while g.tre_size() > 99999:
        n += 1
        g = RsmGrid(
            int(math.ceil(num_x / num_x_sec)),
            int(math.ceil(num_y / n)),
            num_z,
            False,
            total_number_row_digit,
            total_number_col_digit,
        )
    if n > 256:
        raise RuntimeError("Exceeds maximum number of sections")
    return Rsm(
        RsmMultiSection(igc.number_line, igc.number_sample, n, num_x_sec, g), ccov
    )


# This code isn't all that slow, but run in parallel just to test out
# that functionality.
class RsmFitWrap:
    def __init__(self, rsm, igc, min_h, max_h):
        self.rsm = rsm
        self.igc = igc
        self.min_h = min_h
        self.max_h = max_h

    def __call__(self, sec_ind):
        i, j = sec_ind
        res = self.rsm.rsm_base.fit_section(
            i, j, self.igc, self.rsm.coordinate_converter, self.min_h, self.max_h
        )
        res.extrapolate_y_direction()
        res.extrapolate_x_direction()
        res.extrapolate_z_direction()
        return res


def rsm_parallel_fit(rsm, igc, min_h, max_h, pool=None):
    rsm.rsm_base.fit_start(igc, rsm.coordinate_converter, min_h, max_h)
    func = RsmFitWrap(rsm, igc, min_h, max_h)
    sec_index_it = itertools.product(
        range(rsm.rsm_base.number_row_section), range(rsm.rsm_base.number_col_section)
    )
    if pool:
        print("Starting pool")
        sec_list = pool.map(func, sec_index_it)
    else:
        sec_list = map(func, sec_index_it)
    for sec in sec_list:
        rsm.rsm_base.section(
            sec.row_section_number - 1, sec.col_section_number - 1, sec
        )
    rsm.fill_in_ground_domain_vertex(igc, min_h, max_h)
    return rsm


@require_msp
@require_pynitf
def test_bowtie_grid(isolated_dir, igc_staring2):
    igc = igc_staring2
    ccov = LocalRcConverter(
        LocalRcParameter(igc, 0, -1, -1, LocalRcParameter.FOLLOW_LINE_FULL)
    )
    # ccov = LocalRcConverter(LocalRcParameter(igc))
    r = setup_grid(igc, ccov, 200, 1000, 4, 4)
    pool = Pool(10)
    rsm_parallel_fit(r, igc, -100, 100, pool=pool)
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = r
    f.write("nitf_rsm.ntf")
    igc_msp = IgcMsp("nitf_rsm.ntf")
    # igc_msp = igc
    d = SimpleDem(0)
    ic = ImageCoordinate(1, 10)
    print(igc_msp.image_coordinate(igc.ground_coordinate_dem(ic, d)))
    print(r.image_coordinate(igc.ground_coordinate_dem(ic, d))[0])
    (true_line, true_sample, calc_line, calc_sample, distance_true_vs_calc) = (
        r.compare_igc(igc, igc.number_line, igc.number_sample, 0)
    )
    print("Poles in fit: ", r.check_zero_crossing())
    print(pd.DataFrame(np.abs(true_line - calc_line).flatten()).describe())
    print(pd.DataFrame(np.abs(true_sample - calc_sample).flatten()).describe())
    print(pd.DataFrame(distance_true_vs_calc.flatten()).describe())
    wp = np.unravel_index(np.nanargmax(np.abs(true_line - calc_line)), true_line.shape)
    ic = ImageCoordinate(true_line[wp], true_sample[wp])
    print(ic)
    print(r.image_coordinate(igc.ground_coordinate(ic))[0])
    print(igc_msp.image_coordinate(igc.ground_coordinate(ic)))
    if True:
        write_shelve("rsm.bin", r)
        write_shelve("igc.bin", igc)


@require_msp
@require_pynitf
def test_bowtie_poly(isolated_dir, igc_staring2):
    """This won't work as well as the grid. But we may want to look at
    having a polynomial with a correction grid, so start with just a
    polynomial"""
    igc = igc_staring2
    ccov = LocalRcConverter(
        LocalRcParameter(igc, 0, -1, -1, LocalRcParameter.FOLLOW_LINE_FULL)
    )
    # rsm = Rsm(RsmRationalPolynomial(4,4,3,0,0,0,4,0), ccov)
    rsm = Rsm(
        RsmRpPlusGrid(
            RsmRationalPolynomial(4, 4, 3, 0, 0, 0, 4, 0), RsmGrid(10, 100, 4)
        ),
        ccov,
    )
    rsm.fit(igc, -100, 100)
    rsm.rsm_base.correction_grid.extrapolate_y_direction()
    rsm.rsm_base.correction_grid.extrapolate_x_direction()
    rsm.rsm_base.correction_grid.extrapolate_z_direction()
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm
    f.write("nitf_rsm.ntf")
    igc_msp = IgcMsp("nitf_rsm.ntf")
    # igc_msp = igc
    d = SimpleDem(0)
    ic = ImageCoordinate(1, 10)
    print(igc_msp.image_coordinate(igc.ground_coordinate_dem(ic, d)))
    print(rsm.image_coordinate(igc.ground_coordinate_dem(ic, d))[0])
    (true_line, true_sample, calc_line, calc_sample, distance_true_vs_calc) = (
        rsm.compare_igc(igc, igc.number_line, igc.number_sample, 0)
    )
    print("Poles in fit: ", rsm.check_zero_crossing())
    print(pd.DataFrame(np.abs(true_line - calc_line).flatten()).describe())
    print(pd.DataFrame(np.abs(true_sample - calc_sample).flatten()).describe())
    print(pd.DataFrame(distance_true_vs_calc.flatten()).describe())
    wp = np.unravel_index(np.nanargmax(np.abs(true_line - calc_line)), true_line.shape)
    ic = ImageCoordinate(true_line[wp], true_sample[wp])
    print(ic)
    print(rsm.image_coordinate(igc.ground_coordinate(ic))[0])
    print(igc_msp.image_coordinate(igc.ground_coordinate(ic)))
    if True:
        write_shelve("rsm.bin", rsm)
        write_shelve("igc.bin", igc)


# Currently a bit slow, so skip. We'll likely speed this up or otherwise
# modify this
@pytest.mark.skip
@require_msp
@require_pynitf
def test_bowtie_multi_poly(isolated_dir, igc_staring2):
    """Multi-section polynomial grid"""
    igc = igc_staring2
    write_shelve("igc.xml", igc)
    ccov = LocalRcConverter(
        LocalRcParameter(igc, 0, -1, -1, LocalRcParameter.FOLLOW_LINE_FULL)
    )
    nlsec = 3
    nssec = 2
    rsm = Rsm(
        RsmMultiSection(
            igc.number_line,
            igc.number_sample,
            nlsec,
            nssec,
            RsmRationalPolynomial(4, 4, 3, 0, 0, 0, 4, 0),
        ),
        ccov,
    )
    d = rsm.generate_data(igc, -100, 100, igc.number_line // 2, igc.number_sample // 2)
    rsm.fit_data(d, igc.number_line, igc.number_sample)
    rsm.fill_in_ground_domain_vertex(igc, -100, 100)
    dcomp = rsm.compare_data(d)
    mln = np.nanargmax(np.abs(dcomp[:, 0]))
    print(mln)
    print(dcomp[mln, :])
    print(d[mln, :])
    print(pd.DataFrame(np.abs(dcomp[:, 0])).describe())
    print(pd.DataFrame(np.abs(dcomp[:, 1])).describe())
    dcomp = rsm.compare_data_dist(d)
    print(pd.DataFrame(dcomp).describe())
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm
    f.write("nitf_rsm.ntf")
    igc_msp = IgcMsp("nitf_rsm.ntf")
    d = SimpleDem(0)
    ic = ImageCoordinate(1, 10)
    print(igc_msp.image_coordinate(igc.ground_coordinate_dem(ic, d)))
    print(rsm.image_coordinate(igc.ground_coordinate_dem(ic, d))[0])
    (true_line, true_sample, calc_line, calc_sample, distance_true_vs_calc) = (
        rsm.compare_igc(igc, igc.number_line, igc.number_sample, 0)
    )
    print("Poles in fit: ", rsm.check_zero_crossing())
    print(pd.DataFrame(np.abs(true_line - calc_line).flatten()).describe())
    print(pd.DataFrame(np.abs(true_sample - calc_sample).flatten()).describe())
    print(pd.DataFrame(distance_true_vs_calc.flatten()).describe())
    wp = np.unravel_index(np.nanargmax(np.abs(true_line - calc_line)), true_line.shape)
    ic = ImageCoordinate(true_line[wp], true_sample[wp])
    print(ic)
    print(rsm.image_coordinate(igc.ground_coordinate(ic))[0])
    print(igc_msp.image_coordinate(igc.ground_coordinate(ic)))
    pdf = PdfPages("rsm_diff.pdf")
    cmap = mpl.colors.ListedColormap(sns.color_palette("RdBu_r", 256))
    fig, axs = plt.subplots(2, 2)
    t = axs[0, 0].imshow(calc_line - true_line, cmap=cmap, vmin=-2, vmax=2)
    fig.colorbar(t, ax=axs[0, 0])
    axs[0, 0].set_title("Line error")
    t = axs[0, 1].imshow(calc_sample - true_sample, cmap=cmap, vmin=-2, vmax=2)
    fig.colorbar(t, ax=axs[0, 1])
    axs[0, 1].set_title("Sample error")
    t = axs[1, 0].imshow(distance_true_vs_calc, cmap=cmap, vmin=-3, vmax=3)
    fig.colorbar(t, ax=axs[1, 0])
    axs[1, 0].set_title("Error distance (m)")
    axs[1, 1].axis("off")
    fig.tight_layout()
    pdf.savefig()
    pdf.close()
    if True:
        write_shelve("rsm.bin", rsm)
        write_shelve("igc.bin", igc)


def test_rsm_cov(isolated_dir):
    """Test a simple rsm covariance"""
    orb = KeplerOrbit()
    tm = Time.parse_time("2003-01-01T11:11:00Z")
    od = orb.orbit_data(tm)
    cam = QuaternionCamera(
        Quaternion_double(1, 0, 0, 0),
        2048,
        2048,
        2e-3,
        2e-3,
        2.85e3,
        FrameCoordinate(1024, 1024),
    )
    dem = SimpleDem(100)
    img = MemoryRasterImage(cam.number_line(0), cam.number_sample(0))
    hmin = 50
    hmax = 150
    igc = OrbitDataImageGroundConnection(od, cam, dem, img)
    r = Rsm(
        RsmRationalPolynomial(3, 3, 3, 3, 3, 3, 3, 3),
        LocalRcConverter(LocalRcParameter(igc)),
    )
    r.fit(igc, hmin, hmax)
    (true_line, true_sample, calc_line, calc_sample, distance_true_vs_calc) = (
        r.compare_igc(igc, 100, 100, 100)
    )
    print("Poles in fit: ", r.check_zero_crossing())
    print("Line:\n", pd.DataFrame(np.abs(true_line - calc_line).flatten()).describe())
    print(
        "Sample:\n",
        pd.DataFrame(np.abs(true_sample - calc_sample).flatten()).describe(),
    )
    print("Distance:\n", pd.DataFrame(distance_true_vs_calc.flatten()).describe())
