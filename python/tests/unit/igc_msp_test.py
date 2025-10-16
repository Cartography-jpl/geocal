from fixtures.require_check import require_msp
from geocal import IgcMsp, ImageCoordinate

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
        print(igc.sensor_velocity(ImageCoordinate(10,20)))
