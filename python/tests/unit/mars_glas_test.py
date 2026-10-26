import numpy as np
import pytest
from fixtures.require_check import require_mars_spice, require_pynitf, require_spice
from numpy.testing import assert_almost_equal

from geocal import (
    AttCsattb,
    ConstantSpacingTimeTable,
    FrameCoordinate,
    GlasGfmCamera,
    ImageCoordinate,
    Ipi,
    IpiImageGroundConnection,
    OrbitDes,
    PlanetConstant,
    PosCsephb,
    igc_mex_hrsc,
    read_shelve,
    rsm_hrsc,
    rsm_plot_diff,
    write_shelve,
)

try:
    import pynitf
except ImportError:
    pass


# Skip for now, this isn't so much a test as a canned example. We can
# perhaps clean this up
@pytest.mark.skip(
    reason="Not so much a test as a canned example of creating GLAS/GFM from HRSC data. Kept for reference"
)
@require_pynitf
@require_spice
@require_mars_spice
def test_glas_mex_hrsc(mars_test_data):
    """Create a Glass/GFM igc from  HRSC data"""
    igc = igc_mex_hrsc(str(mars_test_data / "h1326_0000_nd2.img"))
    print(igc)
    tt = igc.ipi.time_table
    tspace = (
        igc.ipi.time_table.time(ImageCoordinate(1000, 0))[0]
        - igc.ipi.time_table.time(ImageCoordinate(0, 0))[0]
    ) / 1000
    tstart = tt.min_time
    tend = tt.min_time + tspace * 99900
    tt = ConstantSpacingTimeTable(tstart, tend, tspace)
    # The time table in HRSC seems to not have constant rate, I'm guessing
    # there are missing lines. We'll need to look into this, but might explain
    # why a number of the RSM fits aren't very good
    igc3 = IpiImageGroundConnection(
        Ipi(igc.ipi.orbit, igc.ipi.camera, 0, tstart, tend, tt, 5, 30, 1e-10),
        igc.dem,
        igc.image,
    )
    write_shelve("test_igc.xml", igc3)
    qcam = igc.ipi.camera
    cam = GlasGfmCamera(1, qcam.number_sample(0))
    cam.delta_sample_pair = qcam.number_sample(0)
    cam.focal_length = qcam.focal_length * 1e-3
    # Should probably have a automatic generation of this.
    slc1 = qcam.sc_look_vector(FrameCoordinate(0, 0), 0)
    slc2 = qcam.sc_look_vector(FrameCoordinate(0, qcam.number_sample(0)), 0)
    t1 = slc1.look_vector[2] / cam.focal_length
    t2 = slc2.look_vector[2] / cam.focal_length
    fa = np.array(
        [
            [
                slc1.look_vector[0] / t1,
                slc1.look_vector[1] / t1,
                slc2.look_vector[0] / t2,
                slc2.look_vector[1] / t2,
            ]
        ]
    )
    cam.field_alignment = fa
    assert cam.number_band == 1
    assert cam.number_line(0) == 1
    assert cam.number_sample(0) == qcam.number_sample(0)
    assert_almost_equal(cam.focal_length, qcam.focal_length * 1e-3)
    for s in range(0, qcam.number_sample(0), 100):
        fc = qcam.frame_coordinate(cam.sc_look_vector(FrameCoordinate(0, s), 0), 0)
        assert_almost_equal(fc.line, 0)
        assert_almost_equal(fc.sample, s)
    # Right now, we are limited to 99,999 points. We could probably
    # interpolate, or use multiple DES. But just punt and look at only
    # roughly half of the data. Truncate slightly in case there are any
    # edge things we need. May not be necessary.
    aorb = AttCsattb(
        igc.ipi.orbit,
        tstart,
        tend,
        tspace,
        AttCsattb.LAGRANGE,
        AttCsattb.LAGRANGE_7,
        AttCsattb.ATTITUDE_QUALITY_GOOD,
        AttCsattb.ACTUAL,
        AttCsattb.CARTESIAN_FIXED,
    )
    porb = PosCsephb(
        igc.ipi.orbit,
        tstart,
        tend,
        tspace,
        PosCsephb.LAGRANGE,
        PosCsephb.LAGRANGE_5,
        PosCsephb.EPHEMERIS_QUALITY_GOOD,
        PosCsephb.ACTUAL,
        PosCsephb.CARTESIAN_FIXED,
    )
    # Temp, we need to actually put the planet in. But short circuit for
    # now so we can proceed to the other steps
    dorb = OrbitDes(porb, aorb, PlanetConstant.MARS_NAIF_CODE)
    igc2 = IpiImageGroundConnection(
        Ipi(dorb, cam, 0, tstart, tend, tt, 5, 30, 1e-10), igc.dem, igc.image
    )
    write_shelve("test_igc2.bin", igc2)

    igc2.platform_id = "MEX"
    igc2.payload_id = "HRSC"
    igc2.sensor_id = "Nadir"
    f = pynitf.NitfFile()
    img = pynitf.NitfImageWriteNumpy(1, 1, np.uint8, idlvl=2)
    img[0, 0, 0] = 0
    f.image_segment.append(pynitf.NitfImageSegment(img))
    f.image_segment[0].create_glas_gfm(igc2)
    f.write("test_mars_glas.ntf")


@pytest.mark.skip(
    reason="Depends on files written by test_glas_mex_hrsc, a one time examination"
)
def test_plot():
    igc = read_shelve("test_igc.xml")
    igc2 = read_shelve("test_igc2.bin")
    if False:
        rsm = rsm_hrsc(igc)
        write_shelve("test_rsm.xml", rsm)
    else:
        rsm = read_shelve("test_rsm.xml")
    rsm_plot_diff(rsm, igc, "diff.pdf")
    rsm_plot_diff(igc2, igc, "diff2.pdf")


@pytest.mark.skip(
    reason="Depends on files written by test_glas_mex_hrsc, a one time examination"
)
def test_quick():
    igc2 = read_shelve("test_igc2.bin")
    print(igc2)
    igc2.platform_id = "MEX"
    igc2.payload_id = "HRSC"
    igc2.sensor_id = "Nadir"
    f = pynitf.NitfFile()
    img = pynitf.NitfImageWriteNumpy(1, 1, np.uint8, idlvl=2)
    img[0, 0, 0] = 0
    f.image_segment.append(pynitf.NitfImageSegment(img))
    f.image_segment[0].create_glas_gfm(igc2)
    f.write("test_mars_glas.ntf")
