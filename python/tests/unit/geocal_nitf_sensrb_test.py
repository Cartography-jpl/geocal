try:
    import pynitf
except ImportError:
    pass
from geocal import (
    Time,
    KeplerOrbit,
    FrameCoordinate,
    SimpleDem,
    SensrbCamera,
    AircraftOrbitData,
    distance,
    ImageCoordinate,
    Geodetic,
    OrbitDataImageGroundConnection,
    IgcMsp,
)
from fixtures.require_check import require_pynitf, require_msp
import numpy as np
import io


def create_image_seg(f):
    img = pynitf.NitfImageWriteNumpy(9, 10, np.uint8)
    img.subheader.idlvl = 1
    for i in range(9):
        for j in range(10):
            img[0, i, j] = i + j
    f.image_segment.append(pynitf.NitfImageSegment(img))


def igc_compare(od, cam):
    """Given orbit data and camera, create a normal igc and a MSP igc. We
    can then compare between them."""
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].orbit_data_and_camera(od, cam)
    t = f.image_segment[0].find_exactly_one_tre("SENSRB")
    t.sensor = "TEST"
    t.platform = "fake_platform"
    t.operation_domain = "Spaceborne"
    t.generation_date = 20000101
    t.generation_time = 0
    # Temp
    # t.principal_point_offset_x = 1.8e-5
    # t.principal_point_offset_x = 21e-6 * 1e3
    f.write("sensrb_test.ntf")
    # MSP doesn't do aberration of light correction, so to match
    # it we need to fake a 0 velocity to directly compare
    od2 = AircraftOrbitData(
        od.time, od.position_cf, [0, 0, 0], od.roll, od.pitch, od.heading
    )
    igc = OrbitDataImageGroundConnection(od2, cam, SimpleDem(), None)
    igc_msp = IgcMsp("sensrb_test.ntf", SimpleDem())
    return igc, igc_msp


@require_pynitf
def test_sensrb_cam_od(isolated_dir):
    """Create a file, and write out a SENSRB, then make sure we can read it."""
    f = pynitf.NitfFile()
    create_image_seg(f)
    create_image_seg(f)
    t = Time.parse_time("1998-06-30T10:51:28.32Z")
    korb = KeplerOrbit(t, t + 100.0)
    od = korb.orbit_data(t + 5)
    cam = SensrbCamera(
        SensrbCamera.sensor_angle_to_quaternion(10, 70, 20),
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        0.0,
        3000,
        2048,
        1024,
        18e-6,
        21e-6,
        123.8e-3,
        FrameCoordinate(2048 / 2, 1024 / 2),
    )
    f.image_segment[0].orbit_data_and_camera(od, cam)
    f.image_segment[1].orbit_data_only(od, integration_time=1.0)
    f.write("sensrb_test.ntf")
    f2 = pynitf.NitfFile("sensrb_test.ntf")
    od2 = f2.image_segment[0].orbit_data_sensrb
    od3 = f2.image_segment[1].orbit_data_sensrb
    cam2 = f2.image_segment[0].camera_sensrb
    fc = FrameCoordinate(0, 752)
    dem = SimpleDem()
    assert (
        distance(
            od.surface_intersect(cam, fc, dem), od2.surface_intersect(cam2, fc, dem)
        )
        < 1.0
    )
    assert (
        distance(
            od.surface_intersect(cam, fc, dem), od3.surface_intersect(cam, fc, dem)
        )
        < 1.0
    )


@require_msp
@require_pynitf
def test_sensrb_msp(isolated_dir):
    """Compare SENSRB computionation with MSP library"""
    t = Time.parse_time("1998-06-30T10:51:28.32Z")
    korb = KeplerOrbit(t, t + 100.0)
    od = korb.orbit_data(t + 5)
    od_ac = AircraftOrbitData(od)
    # Add some roll and pitch to make sure this gets accounted for correctly.
    od = AircraftOrbitData(
        od_ac.time, od_ac.position_cf, [0, 0, 0], 10, 20, od_ac.heading
    )

    nline = 2048
    nsamp = 1024
    cam = SensrbCamera(
        SensrbCamera.sensor_angle_to_quaternion(10, 70, 20),
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        3000,
        nline,
        nsamp,
        18e-6,
        21e-6,
        123.8e-3,
        FrameCoordinate(nline / 2, nsamp / 2),
    )
    igc, igc_msp = igc_compare(od, cam)
    # Compare also with igc we directly read from pynitf. Should be
    # the same as igc, up to round off, but we want to check that.
    f = pynitf.NitfFile("sensrb_test.ntf")
    # Write out tre as string
    fh2 = io.BytesIO()
    t = f.image_segment[0].find_one_tre("SENSRB")
    t.write_to_file(fh2)
    print(fh2.getvalue())
    print(f.image_segment[0].attitude_quaternion_sensrb)
    # MSP doesn't do aberration of light correction, so to match
    # it we need to fake a 0 velocity to directly compare
    od2 = f.image_segment[0].orbit_data_sensrb
    od2 = AircraftOrbitData(
        od2.time, od2.position_cf, [0, 0, 0], od2.roll, od2.pitch, od2.heading
    )
    igc2 = OrbitDataImageGroundConnection(
        od2, f.image_segment[0].camera_sensrb, SimpleDem(), None
    )
    h = 0
    if True:
        ic = ImageCoordinate(0, 0)
        p1 = igc_msp.ground_coordinate_approx_height(ic, h)
        p2 = igc.ground_coordinate_approx_height(ic, h)
        # p3 = igc2.ground_coordinate_approx_height(ic, h)
        print(f.image_segment[0].find_exactly_one_tre("SENSRB"))
        print(Geodetic(od.position_cf))
        print(Geodetic(p2))
        print(Geodetic(p1))
        print(distance(p1, p2))
        # print(distance(p1, p3))
        print(igc.resolution_meter())
        print(igc.image_coordinate(p1))
        # print(igc2.image_coordinate(p1))
    if True:
        for ln in np.linspace(0, 2048, 10):
            for smp in np.linspace(0, 1024, 10):
                ic = ImageCoordinate(ln, smp)
                p1 = igc_msp.ground_coordinate_approx_height(ic, h)
                p2 = igc.ground_coordinate_approx_height(ic, h)
                p3 = igc2.ground_coordinate_approx_height(ic, h)
                # Pixel size is about 100m, so difference here is small
                assert distance(p1, p2) < 5.0
                assert distance(p2, p3) < 5.0
