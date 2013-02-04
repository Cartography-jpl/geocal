#include "unit_test_support.h"
#include "vicar_ogr.h"

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(vicar_ogr, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic)
{
  if(!VicarFile::vicar_available())
    return;
  VicarOgr v;
  boost::shared_ptr<OgrCoordinateConverter> 
    cv(new OgrCoordinateConverter(OgrWrapper::from_epsg(32612)));
  MapInfo mi(cv, 10000, 20000, 10000 + 100 * 20, 20000 + 200 * 30, 100, 200);
  {
    VicarFile f("test_out/vicar_ogr_write.img", 200, 100);
    v.to_vicar(mi, f);
  } // Destructor forces file to be flushed.
  VicarFile f2("test_out/vicar_ogr_write.img");
  MapInfo mi2 = v.from_vicar(f2);
  OgrCoordinateConverter& ogr = *(dynamic_cast<OgrCoordinateConverter*>(mi.coordinate_converter_ptr().get()));
  BOOST_CHECK(ogr.ogr().ogr().IsSame(&cv->ogr().ogr()));
}

BOOST_AUTO_TEST_CASE(vicar_ogr_point_vs_area)
{
  // Check correct handling of pixel area vs point.
  
  // These 2 files have the same tiepoint, but in one case this is 
  // for area and in the other point. For area, the geotiff (0,0)
  // corresponds to GeoCal (-0.5,-0.5) (since they use a different
  // convention of (0,0) - for geotiff this is the ulc of the first pixel,
  // for geocal this is the cneter of the first pixel). For pixel is
  // point, geotiff and Geocal both have (0,0) as the pixel.
  std::string pixel_is_area = test_data_dir() + "test_pixel_is_area.img";
  std::string pixel_is_point = test_data_dir() + "test_pixel_is_point.img";
  VicarFile fpixel_is_area(pixel_is_area);
  VicarFile fpixel_is_point(pixel_is_point);
  Geodetic ulc(20, 10);
  VicarOgr vogr;
  BOOST_CHECK_CLOSE(distance
      (*vogr.from_vicar(fpixel_is_area).ground_coordinate(-0.5, -0.5),
       ulc), 0, 1e-4);
  BOOST_CHECK_CLOSE(distance
      (*vogr.from_vicar(fpixel_is_point).ground_coordinate(0.0, 0.0),
       ulc), 0, 1e-4);

  // Check that we can force pixel as point to pixel as area. This is
  // really meant just as a workaround for the SRTM erroneously
  // marking the files as pixel is point when really it is pixel as
  // area, see Srtm class for details.
  VicarFile fpixel_is_point_make_area
    (pixel_is_point, VicarFile::READ, true);
  BOOST_CHECK_CLOSE(distance
      (*vogr.from_vicar(fpixel_is_point_make_area).ground_coordinate(-0.5, -0.5),
       ulc), 0, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
