#include "unit_test_support.h"
#include "landsat7_global.h"
#include "vicar_file.h"
#include <boost/filesystem.hpp>
#include <iostream>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(landsat7_global, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  if(!VicarFile::vicar_available())
    return;
  if(!boost::filesystem::is_directory("/raid22/band62_VICAR"))
    return;
  Landsat7Global f("/raid22", Landsat7Global::BAND62);
  // We just found this value empirical, and check that we get the
  // same results back.
  BOOST_CHECK_CLOSE(f.interpolate(f.coordinate(Geodetic(42, -77))), 136.0,
		    1e-4);
}

BOOST_AUTO_TEST_CASE(create_subset_file)
{
  if(!VicarFile::vicar_available())
    return;
  if(!boost::filesystem::is_directory("/raid22/band62_VICAR"))
    return;
  Landsat7Global f("/raid22", Landsat7Global::BAND62);
  std::vector<boost::shared_ptr<GroundCoordinate> > pt;
  pt.push_back(boost::make_shared<Geodetic>(64.9, 69.9));
  pt.push_back(boost::make_shared<Geodetic>(65.1, 70.1));
  auto msub = boost::make_shared<MapInfo>(f.map_info().cover(pt));
  std::vector<boost::shared_ptr<GroundCoordinate> > pt2;
  //f.create_subset_file("subset_file.img", "VICAR", pt2, msub, "", "", 0, true);
  f.create_subset_file("subset_file.img", "VICAR", pt2, msub);
}

BOOST_AUTO_TEST_CASE(create_subset_file_dateline)
{
  if(!VicarFile::vicar_available())
    return;
  if(!boost::filesystem::is_directory("/raid22/band62_VICAR"))
    return;
  Landsat7Global f("/raid22", Landsat7Global::BAND62);
  f.change_to_geodetic360();
  std::vector<boost::shared_ptr<GroundCoordinate> > pt;
  pt.push_back(boost::make_shared<Geodetic360>(64.9, 179.9));
  pt.push_back(boost::make_shared<Geodetic360>(65.1, 180.1));
  auto msub = boost::make_shared<MapInfo>(f.map_info().cover(pt));
  std::vector<boost::shared_ptr<GroundCoordinate> > pt2;
  //f.create_subset_file("subset_file_dateline.img", "VICAR", pt2, msub, "", "", 0, true);
  f.create_subset_file("subset_file_dateline.img", "VICAR", pt2, msub);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!VicarFile::vicar_available())
    return;
  if(!have_serialize_supported())
    return;
  if(!boost::filesystem::is_directory("/raid22/band62_VICAR"))
    return;
  boost::shared_ptr<RasterImage> img;
  img.reset(new Landsat7Global("/raid22", Landsat7Global::BAND62));
  std::string d = serialize_write_string(img);
  if(false)
    std::cerr << d;
  boost::shared_ptr<RasterImage> imgr = 
    serialize_read_string<RasterImage>(d);
  BOOST_CHECK_CLOSE(imgr->interpolate(imgr->coordinate(Geodetic(42, -77))), 
		    136, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
