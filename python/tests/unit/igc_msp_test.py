from fixtures.require_check import require_msp
from geocal import IgcMsp, ImageCoordinate, distance, GdalRasterImage
import pytest

@require_msp
def test_igc_msp(unit_test_data):
    IgcMsp.print_plugin_list()
    print(IgcMsp.plugin_list)
    print(IgcMsp.model_dict)
    igc = IgcMsp(str(unit_test_data / "rpc.ntf"))
    assert igc.file_name == str(unit_test_data / "rpc.ntf")
    assert igc.image_index == 0
    assert igc.family == "GeometricRaster"
    # This values are probably dependent on the specific MSP library we have,
    # so just print them out.
    print(igc.version)
    print(igc.model_name)
    print(igc.pedigree)
    print(igc.image_identifer)
    print(igc.sensor_identifer)
    print(igc.platform_identifer)
    print(igc.collection_identifer)
    print(igc.trajectory_identifer)
    print(igc.sensor_type)
    print(igc.sensor_mode)
    print(igc.reference_date_time)

    # This isn't support by RPC
    if False:
        print(igc.sensor_velocity(ImageCoordinate(10, 20)))

    # Compare calculations from MSP to direct calculations with the rpc
    rpc = GdalRasterImage(str(unit_test_data / "rpc.ntf")).rpc
    for i in range(10):
        for j in range(10):
            gp1 = igc.ground_coordinate(ImageCoordinate(i, j))
            gp2 = rpc.ground_coordinate(ImageCoordinate(i, j), 0)
            assert distance(gp1, gp2) < 0.01

    for i in range(10):
        for j in range(10):
            gp1 = igc.ground_coordinate_approx_height(ImageCoordinate(i, j), 100)
            gp2 = rpc.ground_coordinate(ImageCoordinate(i, j), 100)
            assert distance(gp1, gp2) < 0.01
            
    for i in range(10):
        for j in range(10):
            ic1 = ImageCoordinate(i, j)
            gp1 = igc.ground_coordinate(ic1)
            ic2 = igc.image_coordinate(gp1)
            assert ic2.line == pytest.approx(ic1.line, abs = 0.01)
            assert ic2.sample == pytest.approx(ic1.sample, abs = 0.01)

    #Time texpect = Time::parse_time("2002-12-16T15:16:29.000000Z");
    #BOOST_CHECK(fabs(igc.pixel_time(ImageCoordinate(10,10)) - texpect) < 1e-6);
            
