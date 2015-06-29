#include "unit_test_support.h"
#include "feature_detector.h"
#include "forstner_feature_detector.h"
#include <algorithm>
#include <boost/foreach.hpp>
#include "gdal_raster_image.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(feature_detector, GlobalFixture)

BOOST_AUTO_TEST_CASE(interest_point)
{
  InterestPoint ip(ImageCoordinate(1, 2), 3);
  BOOST_CHECK_EQUAL(ip.image_coordinate, ImageCoordinate(1, 2));
  BOOST_CHECK_CLOSE(ip.weight, 3.0, 1e-4);
  InterestPoint ip2(ImageCoordinate(1, 2), 4);
  BOOST_CHECK(ip <= ip);
  BOOST_CHECK(ip >= ip);
  BOOST_CHECK(ip < ip2);
  BOOST_CHECK(ip2 > ip);
  BOOST_CHECK_EQUAL(ip.print_to_string(), 
		    "Interest Point: (1, 2) Weight: 3");
}

#ifdef HAVE_GDAL
BOOST_AUTO_TEST_CASE(forstner_feature_detector)
{
  std::string fname = test_data_dir() + "cib_sample.img";
  ForstnerFeatureDetector fd;
  GdalRasterImage img(fname);
  std::vector<InterestPoint> res = fd.interest_point_detect(img);
  BOOST_CHECK_EQUAL(res.size(), (std::vector<InterestPoint>::size_type) 184);
  InterestPoint& ip_max = *std::max_element(res.begin(), res.end());
  BOOST_CHECK_EQUAL(ip_max.image_coordinate, ImageCoordinate(55.7685, 51.705));
  BOOST_CHECK_CLOSE(ip_max.weight, 9.71407806, 1e-4);
  CombinedGroundMask m;
  std::vector<ImageCoordinate> g = fd.interest_point_grid(img, m, 3, 3, 2);
  BOOST_CHECK_EQUAL(g.size(), std::vector<ImageCoordinate>::size_type(9));
  BOOST_CHECK_EQUAL(g[0], ImageCoordinate(42.2927, 7.52101));
}

BOOST_AUTO_TEST_CASE(serialization_fd)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<FeatureDetector> 
    fd(new ForstnerFeatureDetector());
  std::string d = serialize_write_string(fd);
  if(false)
    std::cerr << d;
  boost::shared_ptr<ForstnerFeatureDetector> fdr = 
    serialize_read_string<ForstnerFeatureDetector>(d);

  std::string fname = test_data_dir() + "cib_sample.img";
  GdalRasterImage img(fname);
  std::vector<InterestPoint> res = fdr->interest_point_detect(img);
  BOOST_CHECK_EQUAL(res.size(), (std::vector<InterestPoint>::size_type) 184);
  InterestPoint& ip_max = *std::max_element(res.begin(), res.end());
  BOOST_CHECK_EQUAL(ip_max.image_coordinate, ImageCoordinate(55.7685, 51.705));
  BOOST_CHECK_CLOSE(ip_max.weight, 9.71407806, 1e-4);
  CombinedGroundMask m;
  std::vector<ImageCoordinate> g = fdr->interest_point_grid(img, m, 3, 3, 2);
  BOOST_CHECK_EQUAL(g.size(), std::vector<ImageCoordinate>::size_type(9));
  BOOST_CHECK_EQUAL(g[0], ImageCoordinate(42.2927, 7.52101));

}

#endif

BOOST_AUTO_TEST_CASE(serialization_ip)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<InterestPoint> 
    ip(new InterestPoint(ImageCoordinate(1, 2), 3));
  std::string d = serialize_write_string(ip);
  if(false)
    std::cerr << d;
  boost::shared_ptr<InterestPoint> ipr = 
    serialize_read_string<InterestPoint>(d);
  BOOST_CHECK_EQUAL(ipr->image_coordinate, ImageCoordinate(1, 2));
  BOOST_CHECK_CLOSE(ip->weight, 3.0, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
