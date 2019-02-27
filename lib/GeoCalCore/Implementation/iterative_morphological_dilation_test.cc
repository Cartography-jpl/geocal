#include "iterative_morphological_dilation.h"
#include "unit_test_support.h"
#include <math.h>
using namespace GeoCal;
using namespace blitz;

class MissingDataFixture : public GlobalFixture {
public:
  MissingDataFixture()
    : data(100, 110),
      mask(100, 110),
      kernel(5,5)
  {
    double s = 10.0 / (data.rows() - 1);
    for(int i = 0; i < data.rows(); ++i)
      for(int j = 0; j < data.cols(); ++j)
	data(i,j) = i * s + j * s;
    mask = false;
    for(int i = 10; i <= 15; ++i)
      for(int j = 20; j <= 30; ++j) {
	data(i,j) = -9999; 
	mask(i,j) = true;
      }
    int krhs = 2;
    int kchs = 2;
    double sigma = 1.0;
    for(int i = -krhs; i <= krhs; ++i)
      for(int j = -kchs; j <= kchs; ++j)
	kernel(i+krhs,j+kchs) = exp(-(i*i + j*j) / (2*sigma*sigma));
  }
  virtual ~MissingDataFixture() {}
  blitz::Array<double, 2> data;
  blitz::Array<bool, 2> mask;
  blitz::Array<double, 2> kernel;
};

BOOST_FIXTURE_TEST_SUITE(iterative_morphological_dilation, MissingDataFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  IterativeMorphologicalDilation m(data, mask, kernel);
  // Check masked_neighbor_count. Should only be nonzero in our data
  // hole, and should have a count of 5 neighbors at the corners, 3 at
  // the edges, and zero elsewhere.
  blitz::Array<unsigned short int, 2> ncount = m.masked_neighbor_count();
  for(int i = 0; i < mask.rows(); ++i)
    for(int j = 0; j < mask.cols(); ++j)
      if(!mask(i,j))
	BOOST_CHECK_EQUAL(ncount(i,j), 0);
  blitz::Array<unsigned short int, 2> hole_count = ncount(blitz::Range(10,15),blitz::Range(20,30));
  for(int i = 0; i < hole_count.rows(); ++i)
    for(int j = 0; j < hole_count.cols(); ++j) {
      if(i == 0 || i == hole_count.rows() - 1) {
	if(j == 0 || j == hole_count.cols() - 1)
	  BOOST_CHECK_EQUAL(hole_count(i,j), 5);
	else
	  BOOST_CHECK_EQUAL(hole_count(i,j), 3);
      } else if(j == 0 || j == hole_count.cols() - 1)
	BOOST_CHECK_EQUAL(hole_count(i,j), 3);
      else 
	BOOST_CHECK_EQUAL(hole_count(i,j), 0);
    }
  BOOST_CHECK_CLOSE(m.neighborhood_average(10,20), 2.931835, 1e-2);
  BOOST_CHECK(m.fill_iteration());
  BOOST_CHECK_CLOSE(m.filled_image()(10,20), 2.931835, 1e-2);
  BOOST_CHECK(m.filled_mask()(10,20) == false);
  BOOST_CHECK_EQUAL(m.iteration_count(), 1);

  IterativeMorphologicalDilation m2(data, mask, kernel);
  m2.fill_missing_data();
  int nmask = count(m2.filled_mask() == true);
  BOOST_CHECK_EQUAL(nmask, 0);
  BOOST_CHECK_CLOSE(m2.filled_image()(10,20), 2.931835, 1e-2);
  BOOST_CHECK_EQUAL(m2.iteration_count(), 3);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<IterativeMorphologicalDilation> m =
    boost::make_shared<IterativeMorphologicalDilation>(data, mask, kernel);
  std::string d = serialize_write_string(m);
  if(false)
    std::cerr << d;
  boost::shared_ptr<IterativeMorphologicalDilation> mr = 
    serialize_read_string<IterativeMorphologicalDilation>(d);
}

BOOST_AUTO_TEST_SUITE_END()

  
