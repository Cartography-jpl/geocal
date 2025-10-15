from geocal import (
    SimpleDem,
    RpcImageGroundConnection,
    MemoryRasterImage,
    IpiImageGroundConnection,
    OrbitQuaternionList,
    ImageCoordinate,
    Ipi,
    ConstantSpacingTimeTable,
    cib01_mapinfo,
    distance,
    LnLookVector,
    CartesianFixedLookVector,
    FrameCoordinate,
    Quaternion_double,
    QuaternionCamera,
    Time,
    KeplerOrbit,
    OrbitDataImageGroundConnection,
    GlasGfmCamera,
    OrbitDes,
    AttCsattb,
    PosCsephb,
)
import numpy as np
import pytest


@pytest.fixture(scope="function")
def igc_half_meter_pushbroom():
    """Pushbroom camera with a resolution of 0.5 m (i.e., approximately
    WV-2). This is 2048 x 2048 sized image"""
    orb = KeplerOrbit()
    tm = Time.parse_time("2015-02-03T10:00:00Z")
    # Camera that has a roughly 0.5 meter resolution nadir looking, i.e.,
    # about the resolution of WV-2
    cam = QuaternionCamera(
        Quaternion_double(1, 0, 0, 0),
        1,
        2048,
        2e-3,
        2e-3,
        2.85e3,
        FrameCoordinate(0, 1024),
    )
    # Time delta that is roughly 0.5 meter apart
    tdelta = 7.5e-5
    dem = SimpleDem()
    tt = ConstantSpacingTimeTable(tm, tm + tdelta * 2048, tdelta)
    # "Real" igc
    ipi = Ipi(orb, cam, 0, tt.min_time, tt.max_time, tt)
    igc = IpiImageGroundConnection(ipi, dem, None)
    if False:
        ic = ImageCoordinate(igc.number_line / 2, igc.number_sample / 2)
        gc1 = igc.ground_coordinate(ic)
        gc2 = igc.ground_coordinate(ImageCoordinate(ic.line + 1, ic.sample))
        gc3 = igc.ground_coordinate(ImageCoordinate(ic.line, ic.sample + 1))
        print("resolution line dir: ", distance(gc1, gc2))
        print("resolution sample dir: ", distance(gc1, gc3))
    return igc


@pytest.fixture(scope="function")
def igc_two_meter_pushbroom():
    """Pushbroom camera with a resolution of 2.0 m .
    This is 1024 x 1024 sized image"""
    orb = KeplerOrbit()
    tm = Time.parse_time("2015-02-03T10:00:00Z")
    # Camera that has a roughly 0.5 meter resolution nadir looking, i.e.,
    # about the resolution of WV-2
    nline = 1024
    nsamp = 1024
    cam = QuaternionCamera(
        Quaternion_double(1, 0, 0, 0),
        1,
        nsamp,
        2e-3,
        2e-3,
        2.85e3 / 4,
        FrameCoordinate(0, nsamp / 2),
    )
    # Time delta that is roughly 0.5 meter apart
    tdelta = 7.5e-5 * 4
    dem = SimpleDem()
    tt = ConstantSpacingTimeTable(tm, tm + tdelta * nline, tdelta)
    # "Real" igc
    ipi = Ipi(orb, cam, 0, tt.min_time, tt.max_time, tt)
    igc = IpiImageGroundConnection(ipi, dem, None)
    if False:
        ic = ImageCoordinate(igc.number_line / 2, igc.number_sample / 2)
        gc1 = igc.ground_coordinate(ic)
        gc2 = igc.ground_coordinate(ImageCoordinate(ic.line + 1, ic.sample))
        gc3 = igc.ground_coordinate(ImageCoordinate(ic.line, ic.sample + 1))
        print("resolution line dir: ", distance(gc1, gc2))
        print("resolution sample dir: ", distance(gc1, gc3))
    return igc


def _zenith_angle(orb, tm, pt):
    """Zenith angle, 180 degrees looks straight up"""
    clv = CartesianFixedLookVector(orb.position_cf(tm), pt)
    llv = LnLookVector(clv, pt)
    return llv.view_zenith


@pytest.fixture(scope="function")
def igc_staring(igc_half_meter_pushbroom):
    """This is a pushbroom camera where we adjust the gimbal angle that we view
    at to stare near a fixed point (i.e., like Freebird)"""
    igc = igc_half_meter_pushbroom
    mi = cib01_mapinfo(0.5)
    mi = mi.rotated_map(igc)
    img = MemoryRasterImage(mi)
    center = ImageCoordinate(img.number_line / 2, img.number_sample / 2)
    uedge = ImageCoordinate(center.line - 2048 / 2, center.sample)
    ledge = ImageCoordinate(center.line + 2048 / 2, center.sample)
    orb = igc.ipi.orbit
    # 90 seconds goes from roughly +- 45 degree angle with surface, double
    # this to go from about +- 66 degrees, This gives a pretty extreme
    # bowtie, which is good for testing create a RSM to handle this
    tstart = igc.ipi.min_time - 90 * 2
    tend = igc.ipi.max_time + 90 * 2
    if False:
        print(180 - _zenith_angle(orb, tstart, img.ground_coordinate(uedge)))
        print(180 - _zenith_angle(orb, tend, img.ground_coordinate(ledge)))
    tt = ConstantSpacingTimeTable(tstart, tend, (tend - tstart) / 2047)
    odlist = []
    # Little extra at ends, so orbit it larger than needed
    for i in range(-10, 2048 + 10):
        tm = tstart + (tend - tstart) / 2047 * i
        ic = ImageCoordinate(uedge.line + i, uedge.sample)
        od = orb.orbit_data(tm)
        od.quat_from_principal_gic(ic, img, igc.dem)
        odlist.append(od)
    orb2 = OrbitQuaternionList(odlist)
    ipi = Ipi(orb2, igc.ipi.camera, 0, tt.min_time, tt.max_time, tt)
    igc2 = IpiImageGroundConnection(ipi, igc.dem, None)
    if False:
        ic = ImageCoordinate(0, igc2.number_sample / 2)
        gc1 = igc2.ground_coordinate(ic)
        gc2 = igc2.ground_coordinate(ImageCoordinate(ic.line + 1, ic.sample))
        gc3 = igc2.ground_coordinate(ImageCoordinate(ic.line, ic.sample + 1))
        print("resolution upper edge line dir: ", distance(gc1, gc2))
        print("resolution upper edge sample dir: ", distance(gc1, gc3))
        ic = ImageCoordinate(igc2.number_line / 2, igc2.number_sample / 2)
        gc1 = igc2.ground_coordinate(ic)
        gc2 = igc2.ground_coordinate(ImageCoordinate(ic.line + 1, ic.sample))
        gc3 = igc2.ground_coordinate(ImageCoordinate(ic.line, ic.sample + 1))
        print("resolution middle line dir: ", distance(gc1, gc2))
        print("resolution middle sample dir: ", distance(gc1, gc3))
        ic = ImageCoordinate(igc2.number_line - 1, igc2.number_sample / 2)
        gc1 = igc2.ground_coordinate(ic)
        gc2 = igc2.ground_coordinate(ImageCoordinate(ic.line - 1, ic.sample))
        gc3 = igc2.ground_coordinate(ImageCoordinate(ic.line, ic.sample + 1))
        print("resolution bottom edge line dir: ", distance(gc1, gc2))
        print("resolution bottom edge sample dir: ", distance(gc1, gc3))
    return igc2


@pytest.fixture(scope="function")
def igc_staring2(igc_two_meter_pushbroom):
    """This is a pushbroom camera where we adjust the gimbal angle that we view
    at to stare near a fixed point (i.e., like Freebird)"""
    igc = igc_two_meter_pushbroom
    mi = cib01_mapinfo(2)
    mi = mi.rotated_map(igc)
    img = MemoryRasterImage(mi)
    center = ImageCoordinate(img.number_line / 2, img.number_sample / 2)
    uedge = ImageCoordinate(center.line - igc.number_line / 2, center.sample)
    orb = igc.ipi.orbit
    # 90 seconds goes from roughly +- 45 degree angle with surface. This
    # gives a pretty extreme bowtie, which is good for testing create a
    # RSM to handle this
    tstart = igc.ipi.min_time - 90
    tend = igc.ipi.max_time + 90
    tt = ConstantSpacingTimeTable(tstart, tend, (tend - tstart) / (igc.number_line - 1))
    odlist = []
    # Little extra at ends, so orbit it larger than needed
    for i in range(-100, igc.number_line + 100):
        tm = tstart + (tend - tstart) / (igc.number_line - 1) * i
        ic = ImageCoordinate(uedge.line + i, uedge.sample)
        od = orb.orbit_data(tm)
        od.quat_from_principal_gic(ic, img, igc.dem)
        odlist.append(od)
    orb2 = OrbitQuaternionList(odlist)
    # ipi = Ipi(orb2, igc.ipi.camera, 0, orb2.min_time, orb2.max_time, tt)
    ipi = Ipi(
        orb2,
        igc.ipi.camera,
        0,
        tt.min_time,
        #              tt.max_time + tt.time_space * 2,
        tt.max_time,
        tt,
    )
    igc2 = IpiImageGroundConnection(ipi, igc.dem, None)
    return igc2


@pytest.fixture(scope="function")
def igc_rpc(rpc):
    image = MemoryRasterImage(int(rpc.line_offset * 2), int(rpc.sample_offset * 2), 0)
    return RpcImageGroundConnection(rpc, SimpleDem(), image)


@pytest.fixture(scope="function")
def igc_gfm():
    """Sample GFM IGC"""
    tdata = Time.parse_time("2003-01-01T11:11:00Z")
    orb = KeplerOrbit()
    porb = PosCsephb(
        orb,
        tdata - 10.0,
        tdata + 10.0,
        0.5,
        PosCsephb.LINEAR,
        PosCsephb.NO_LAGRANGE,
        PosCsephb.EPHEMERIS_QUALITY_GOOD,
        PosCsephb.ACTUAL,
        PosCsephb.CARTESIAN_FIXED,
    )
    aorb = AttCsattb(
        orb,
        tdata - 10.0,
        tdata + 10.0,
        0.5,
        AttCsattb.LINEAR,
        AttCsattb.NO_LAGRANGE,
        AttCsattb.ATTITUDE_QUALITY_GOOD,
        AttCsattb.ACTUAL,
        AttCsattb.CARTESIAN_FIXED,
    )
    orb = OrbitDes(porb, aorb)
    cam = GlasGfmCamera(2048, 2048)
    cam.focal_length = 123.8e-3
    cam.focal_length_time = orb.min_time
    cam.first_line_block = [0]
    cam.first_sample_block = [0]
    cam.delta_line_block = [2048]
    cam.delta_sample_block = [2048]
    fa = np.empty((1, 1, 2, 2, 2))
    fa[0, 0, :, 0, 0] = -1024 * 21e-7
    fa[0, 0, :, 1, 0] = 1024 * 21e-7
    fa[0, 0, 0, :, 1] = 1024 * 21e-7
    fa[0, 0, 1, :, 1] = -1024 * 21e-7
    cam.field_alignment_block(0, fa)

    igc = OrbitDataImageGroundConnection(orb, tdata, cam, SimpleDem(), None)
    igc.platform_id = "FAKEPL"
    igc.payload_id = "FAKEPY"
    igc.sensor_id = "FAKESN"
    return igc
