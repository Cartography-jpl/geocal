import pytest

from geocal import (
    Geodetic,
    IgcArray,
    ImageCoordinate,
    RayIntersect2,
    RayIntersect3,
    SimpleDem,
    TiePoint,
    VicarImageGroundConnection,
    VicarLiteDem,
    distance,
)


def test_ray_intersect(stereo_unit_test_data):
    initial_height = 1291
    dem = SimpleDem(initial_height)
    igc1 = VicarImageGroundConnection(str(stereo_unit_test_data / "10MAY21-1.img"), dem)
    igc2 = VicarImageGroundConnection(str(stereo_unit_test_data / "10MAY21-2.img"), dem)
    igc_coll = IgcArray([igc1, igc2])
    ri = RayIntersect2(igc_coll)
    ic1 = ImageCoordinate(550, 550)
    ic2 = ImageCoordinate(553.659, 553.99)
    p, dist = ri.two_ray_intersect(ic1, ic2)
    assert dist == pytest.approx(0.359094, abs=1e-4)
    assert distance(p, Geodetic(36.7754303197, -116.118981291, 1290.64)) < 0.01


def test_tp(stereo_unit_test_data):
    demin = VicarLiteDem(str(stereo_unit_test_data / "nevada_elv_aoi.img"), True)
    igc1 = VicarImageGroundConnection(
        str(stereo_unit_test_data / "10MAY21-1.img"), demin
    )
    igc2 = VicarImageGroundConnection(
        str(stereo_unit_test_data / "10MAY21-2.img"), demin
    )
    igc3 = VicarImageGroundConnection(
        str(stereo_unit_test_data / "10MAY21-3.img"), demin
    )
    igc_coll = IgcArray([igc1, igc2, igc3])
    ri = RayIntersect2(igc_coll)
    tp = TiePoint(3)
    tp.image_coordinate(0, ImageCoordinate(944, 916), 0.1, 0.1)
    tp.image_coordinate(1, ImageCoordinate(975.65, 934.365), 0.1, 0.1)
    tp.image_coordinate(2, ImageCoordinate(1007.83, 950.128), 0.1, 0.1)
    ri.ray_intersect(tp)
    assert (
        distance(tp.ground_location, Geodetic(36.7731550801, -116.116742412, 1276.05))
        < 1
    )


def test_ray_intersect3(stereo_unit_test_data):
    demin = VicarLiteDem(str(stereo_unit_test_data / "nevada_elv_aoi.img"), True)
    igc1 = VicarImageGroundConnection(
        str(stereo_unit_test_data / "10MAY21-1.img"), demin
    )
    igc2 = VicarImageGroundConnection(
        str(stereo_unit_test_data / "10MAY21-2.img"), demin
    )
    igc3 = VicarImageGroundConnection(
        str(stereo_unit_test_data / "10MAY21-3.img"), demin
    )
    igc_coll = IgcArray([igc1, igc2, igc3])
    ri = RayIntersect3(igc_coll)
    tp = TiePoint(3)
    tp.image_coordinate(0, ImageCoordinate(944, 916), 0.1, 0.1)
    tp.image_coordinate(1, ImageCoordinate(975.65, 934.365), 0.1, 0.1)
    tp.image_coordinate(2, ImageCoordinate(1007.83, 950.128), 0.1, 0.1)
    ri.ray_intersect(tp)
    assert (
        distance(tp.ground_location, Geodetic(36.7731550801, -116.116742412, 1276.05))
        < 1
    )
