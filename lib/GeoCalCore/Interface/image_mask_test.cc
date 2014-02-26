#include "unit_test_support.h"
#include "image_mask.h"
#include "geodetic.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(image_mask, GlobalFixture)

class EmptyMask : public ImageMask {
public:
  EmptyMask() {}
  virtual ~EmptyMask() {}
  virtual bool mask(int Line, int Sample) const {return false;}
  virtual bool area_any_masked(int Line, int Sample, int Number_line,
			       int Number_sample) const
  {return false;}
  virtual void print(std::ostream& Os) const
  {Os << "Empty mask\n";}
};
class AlwaysMask : public ImageMask {
public:
  AlwaysMask() {}
  virtual ~AlwaysMask() {}
  virtual bool mask(int Line, int Sample) const {return true;}
  virtual bool area_any_masked(int Line, int Sample, int Number_line,
			       int Number_sample) const
  {return true;}
  virtual void print(std::ostream& Os) const
  {Os << "Always mask\n";}
};

BOOST_AUTO_TEST_CASE(combine_image_mask)
{
  boost::shared_ptr<ImageMask> empty(new EmptyMask);
  boost::shared_ptr<ImageMask> always(new AlwaysMask);
  CombinedImageMask cmask;
  BOOST_CHECK(!cmask.mask(10, 20));
  BOOST_CHECK(!cmask.area_any_masked(10, 20, 100, 100));
  BOOST_CHECK_EQUAL(cmask.print_to_string(), "Empty CombinedImageMask\n");
  cmask.mask_list.push_back(empty);
  BOOST_CHECK(!cmask.mask(10, 20));
  BOOST_CHECK(!cmask.area_any_masked(10, 20, 100, 100));
  BOOST_CHECK_EQUAL(cmask.print_to_string(), "CombinedImageMask: \n  Mask 1:\nEmpty mask\n");
  cmask.mask_list.push_back(always);
  BOOST_CHECK(cmask.mask(10, 20));
  BOOST_CHECK(cmask.area_any_masked(10, 20, 100, 100));
  BOOST_CHECK_EQUAL(cmask.print_to_string(), "CombinedImageMask: \n  Mask 1:\nEmpty mask\n  Mask 2:\nAlways mask\n");
}

BOOST_AUTO_TEST_SUITE_END()
