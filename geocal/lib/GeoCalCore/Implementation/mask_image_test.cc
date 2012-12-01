#include "unit_test_support.h"
#include "mask_image.h"
#include "geodetic.h"
#ifdef FALSE
#include "vicar_lite_file.h"
#endif
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(mask_image, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  // Don't run this normally, since we don't want to depend on the
  // rather large water mask file in the unit tests. But we do want to
  // test out this functionality at least the first time
#ifdef FALSE
  boost::shared_ptr<RasterImage> 
  lw(new VicarLiteMapProjectedImage("/Users/smyth/AlStuff/world_30as_lwm.img"));
  MaskImage mi(lw);
  // Looking through the data, we find 1000, 10573 is land, with
  // points to the left and below water.
  BOOST_CHECK(!mi.mask(Geodetic(81.6667, -91.8333)));
  BOOST_CHECK(mi.mask(Geodetic(81.6583, -91.8917)));
  BOOST_CHECK(mi.region_masked(Geodetic(81.6583, -91.8917), 
			       Geodetic(81.6667, -91.8333)));
#endif
}

BOOST_AUTO_TEST_SUITE_END()
