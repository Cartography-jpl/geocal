from geocal import (
    VicarLiteDem,
    VicarImageGroundConnection,
    VicarLiteRasterImage,
    IgcMapProjected,
    VicarRasterImage,
    copy_raster,
)
from fixtures.require_check import require_vicar


@require_vicar
def test_igc_write(isolated_dir, stereo_unit_test_data):
    dem = VicarLiteDem(str(stereo_unit_test_data / "nevada_elv_aoi.img"), True)
    igc1 = VicarImageGroundConnection(str(stereo_unit_test_data / "10MAY21-1.img"), dem)
    gaoi = VicarLiteRasterImage(str(stereo_unit_test_data / "aoi.img"))
    mproj = IgcMapProjected(gaoi.map_info, igc1)
    out = VicarRasterImage("igc_test.out", gaoi.map_info, "HALF")
    copy_raster(mproj, out)
