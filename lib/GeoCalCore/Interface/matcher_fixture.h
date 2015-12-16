#ifndef MATCHER_FIXTURE_H
#define MATCHER_FIXTURE_H
#include "global_fixture.h"
#include "memory_raster_image.h"
#include <fstream>

using namespace GeoCal;

namespace GeoCal {
class MatcherFixture: public GlobalFixture {
public:
// Read in the binary test data files. These files work with unsigned char
// data, so there are no platform endian issues.
  MatcherFixture() 
    : ref_img(64, 64), new_img(64, 64), ref_ic(33.688, 32.060),
      new_ic(33.500, 31.880)
  {
  // Size is hard coded.
    boost::multi_array<char, 2> ref_d(boost::extents[64][64]);
    boost::multi_array<char, 2> new_d(boost::extents[64][64]);
    std::ifstream ref((test_data_dir() + 
		       "matcher_test_refimage.bin").c_str(),
		      std::ios::binary);
    std::ifstream nw((test_data_dir() + 
		      "matcher_test_newimage.bin").c_str(),
		     std::ios::binary);
    ref.read((char*) ref_d.data(), 64 * 64);
    nw.read((char*) new_d.data(), 64 * 64);
    for(int i = 0; i < ref_img.number_line(); ++i)
      for(int j = 0; j < ref_img.number_sample(); ++j) {
	ref_img.write(i, j, ref_d[i][j]);
	new_img.write(i, j, new_d[i][j]);
      }
  }
  MemoryRasterImage ref_img, new_img;
  ImageCoordinate ref_ic, new_ic;
};
}
#endif
