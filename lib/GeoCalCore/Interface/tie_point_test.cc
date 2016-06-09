#include "unit_test_support.h"
#include "tie_point.h"
#include "ecr.h"
#include <boost/make_shared.hpp>
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(tie_point, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  TiePoint tp(5);
  tp.ground_location(boost::shared_ptr<GroundCoordinate>(new Ecr(1, 2, 3)));
  tp.id(10);
  tp.is_gcp(true);
  tp.image_coordinate(3, boost::make_shared<ImageCoordinate>(1, 2), 0.2, 0.3);
  tp.image_coordinate(1, boost::make_shared<ImageCoordinate>(3, 4), 0.25, 0.35);
  BOOST_CHECK_EQUAL(tp.id(), 10);
  BOOST_CHECK(tp.is_gcp());
  BOOST_CHECK(distance(*tp.ground_location(), Ecr(1,2,3)) < 0.1);
  BOOST_CHECK_EQUAL(tp.number_image(), 5);
  BOOST_CHECK_EQUAL(tp.number_image_location(), 2);
  BOOST_CHECK(!tp.image_coordinate(0));
  BOOST_CHECK(*tp.image_coordinate(1) == ImageCoordinate(3, 4));
  BOOST_CHECK(*tp.image_coordinate(3) == ImageCoordinate(1, 2));
  BOOST_CHECK_CLOSE(tp.line_sigma(3), 0.2, 1e-4);
  BOOST_CHECK_CLOSE(tp.line_sigma(1), 0.25, 1e-4);
  BOOST_CHECK_CLOSE(tp.sample_sigma(3), 0.3, 1e-4);
  BOOST_CHECK_CLOSE(tp.sample_sigma(1), 0.35, 1e-4);
}

BOOST_AUTO_TEST_CASE(tiepoint_collection_test)
{
  TiePointCollection tpcol;
  TiePoint tp(5);
  tp.ground_location(boost::shared_ptr<GroundCoordinate>(new Ecr(1, 2, 3)));
  tp.id(10);
  tp.is_gcp(true);
  tp.image_coordinate(3, boost::make_shared<ImageCoordinate>(1, 2), 0.2, 0.3);
  tp.image_coordinate(1, boost::make_shared<ImageCoordinate>(3, 4), 0.25, 0.35);
  tpcol.push_back(boost::make_shared<TiePoint>(tp));
  tp.id(11);
  tpcol.push_back(boost::make_shared<TiePoint>(tp));
  tp.id(12);
  tp.is_gcp(false);
  tpcol.push_back(boost::make_shared<TiePoint>(tp));
  BOOST_CHECK_EQUAL((int) tpcol.size(), 3);
  BOOST_CHECK_EQUAL(tpcol.number_gcp(), 2);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<TiePoint> tp(new TiePoint(5));
  tp->ground_location(boost::shared_ptr<GroundCoordinate>(new Ecr(1, 2, 3)));
  tp->id(10);
  tp->is_gcp(true);
  tp->image_coordinate(3, boost::make_shared<ImageCoordinate>(1, 2), 0.2, 0.3);
  tp->image_coordinate(1, boost::make_shared<ImageCoordinate>(3, 4), 0.25,
		       0.35);
  std::string d = serialize_write_string(tp);
  if(false)
    std::cerr << d;
  boost::shared_ptr<TiePoint> tp2 = 
    serialize_read_string<TiePoint>(d);
  BOOST_CHECK_EQUAL(tp2->id(), 10);
  BOOST_CHECK(tp2->is_gcp());
  BOOST_CHECK(distance(*tp2->ground_location(), Ecr(1,2,3)) < 0.1);
  BOOST_CHECK_EQUAL(tp2->number_image(), 5);
  BOOST_CHECK_EQUAL(tp2->number_image_location(), 2);
  BOOST_CHECK(!tp2->image_coordinate(0));
  BOOST_CHECK(*tp2->image_coordinate(1) == ImageCoordinate(3, 4));
  BOOST_CHECK(*tp2->image_coordinate(3) == ImageCoordinate(1, 2));
  BOOST_CHECK_CLOSE(tp2->line_sigma(3), 0.2, 1e-4);
  BOOST_CHECK_CLOSE(tp2->line_sigma(1), 0.25, 1e-4);
  BOOST_CHECK_CLOSE(tp2->sample_sigma(3), 0.3, 1e-4);
  BOOST_CHECK_CLOSE(tp2->sample_sigma(1), 0.35, 1e-4);
}

BOOST_AUTO_TEST_CASE(serialization_coll)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<TiePointCollection> tpcol(new TiePointCollection);
  TiePoint tp(5);
  tp.ground_location(boost::shared_ptr<GroundCoordinate>(new Ecr(1, 2, 3)));
  tp.id(10);
  tp.is_gcp(true);
  tp.image_coordinate(3, boost::make_shared<ImageCoordinate>(1, 2), 0.2, 0.3);
  tp.image_coordinate(1, boost::make_shared<ImageCoordinate>(3, 4), 0.25, 0.35);
  tpcol->push_back(boost::make_shared<TiePoint>(tp));
  tp.id(11);
  tpcol->push_back(boost::make_shared<TiePoint>(tp));
  tp.id(12);
  tp.is_gcp(false);
  tpcol->push_back(boost::make_shared<TiePoint>(tp));
  std::string d = serialize_write_string(tpcol);
  if(false)
    std::cerr << d;
  boost::shared_ptr<TiePointCollection> tpcol2 = 
    serialize_read_string<TiePointCollection>(d);
  BOOST_CHECK_EQUAL((int) tpcol2->size(), 3);
  BOOST_CHECK_EQUAL(tpcol2->number_gcp(), 2);
}

BOOST_AUTO_TEST_SUITE_END()
