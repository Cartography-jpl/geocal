#include "unit_test_support.h"
#include "gdal_multi_file.h"
#include "gdal_map_projected_image.h"

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(gdal_multi_file, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  if(false)			// Doesn't work yet
  try {
    std::vector<std::string> fname;
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_00_00_w007_30_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_00_00_w007_37_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_00_00_w007_45_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_07_30_w007_30_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_07_30_w007_37_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_07_30_w007_45_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_15_00_w007_37_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_15_00_w007_45_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_15_00_w007_52_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_15_00_w008_00_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_15_00_w008_07_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_15_00_w008_15_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_15_00_w008_22_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_22_30_w007_52_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_22_30_w008_00_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_22_30_w008_07_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_22_30_w008_15_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_22_30_w008_22_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_30_00_w007_45_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_30_00_w007_52_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_30_00_w008_00_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_30_00_w008_07_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_30_00_w008_15_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_30_00_w008_22_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_30_00_w008_30_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_37_30_w007_52_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_37_30_w008_00_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_37_30_w008_07_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_37_30_w008_15_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_37_30_w008_22_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_37_30_w008_30_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_37_30_w008_37_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_45_00_w007_52_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_45_00_w008_00_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_45_00_w008_07_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_45_00_w008_15_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_45_00_w008_22_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_45_00_w008_30_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_45_00_w008_37_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_52_30_w008_00_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_52_30_w008_07_30.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_52_30_w008_15_00.tif");
    fname.push_back("/Users/smyth/mali/20090511_V1/mosaic/q_n12_52_30_w008_22_30.tif");
    
    GdalMultiFile mf(fname, 1, false, 256);
    GdalMapProjectedImage gf("/Users/smyth/mali/20090511_V1/mosaic/q_n12_15_00_w008_00_00.tif");
    std::cerr << mf;
    ImageCoordinate ic = 
      mf.coordinate(*(gf.ground_coordinate(ImageCoordinate(10,20))));
    BOOST_CHECK_EQUAL(mf((int) round(ic.line), (int) round(ic.sample)),
		      gf(10,20));
    BOOST_CHECK_EQUAL(mf(10,20), 0);
    std::vector<std::string> out_list;
    std::vector<MapInfo> out_mapinfo;
//     GdalMultiFile::tile_for_google_earth(fname, 
// 		 "/Users/smyth/mali/20090511_V1/google_earth/",
// 		 out_list, out_mapinfo);
    std::cerr << "GdalMultiFile test ran successfully\n";
  } catch(const Exception&) {
    // Don't worry if we can't find the data.
  }
}

BOOST_AUTO_TEST_SUITE_END()
