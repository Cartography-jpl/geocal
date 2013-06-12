#include "unit_test_support.h"
#include "ground_mask.h"
#include "geodetic.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(ground_mask, GlobalFixture)

class EmptyMask : public GroundMask {
public:
  EmptyMask() {}
  virtual ~EmptyMask() {}
  virtual bool mask(const GroundCoordinate& Gc) const {return false;}
  virtual bool region_masked(const GroundCoordinate& Ulc, 
			     const GroundCoordinate& Lrc) const
  {return false;}
  virtual void print(std::ostream& Os) const
  {Os << "Empty mask\n";}
};
class AlwaysMask : public GroundMask {
public:
  AlwaysMask() {}
  virtual ~AlwaysMask() {}
  virtual bool mask(const GroundCoordinate& Gc) const {return true;}
  virtual bool region_masked(const GroundCoordinate& Ulc, 
			     const GroundCoordinate& Lrc) const
  {return true;}
  virtual void print(std::ostream& Os) const
  {Os << "Always mask\n";}
};

BOOST_AUTO_TEST_CASE(combine_ground_mask)
{
  boost::shared_ptr<GroundMask> empty(new EmptyMask);
  boost::shared_ptr<GroundMask> always(new AlwaysMask);
  Geodetic g1(10, 20);
  Geodetic g2(11, 21);
  CombinedGroundMask cmask;
  BOOST_CHECK(!cmask.mask(g1));
  BOOST_CHECK(!cmask.region_masked(g1, g2));
  BOOST_CHECK_EQUAL(cmask.print_to_string(), "Empty CombinedGroundMask\n");
  cmask.mask_list.push_back(empty);
  BOOST_CHECK(!cmask.mask(g1));
  BOOST_CHECK(!cmask.region_masked(g1, g2));
  BOOST_CHECK_EQUAL(cmask.print_to_string(), "CombinedGroundMask: \n  Mask 1:\nEmpty mask\n");
  cmask.mask_list.push_back(always);
  BOOST_CHECK(cmask.mask(g1));
  BOOST_CHECK(cmask.region_masked(g1, g2));
  BOOST_CHECK_EQUAL(cmask.print_to_string(), "CombinedGroundMask: \n  Mask 1:\nEmpty mask\n  Mask 2:\nAlways mask\n");
}

BOOST_AUTO_TEST_SUITE_END()
