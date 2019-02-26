#include "iterative_morphological_dilation.h"
#include "unit_test_support.h"

using namespace GeoCal;
using namespace blitz;

class MissingDataFixture : public GlobalFixture {
public:
  MissingDataFixture()
    : data(100, 110),
      mask(100, 110)
  {
    double s = 10.0 / (data.rows() - 1);
    for(int i = 0; i < data.rows(); ++i)
      for(int j = 0; j < data.cols(); ++j)
	data(i,j) = i * s + j * s;
    mask = false;
    for(int i = 10; i < 15; ++i)
      for(int j = 20; j < 30; ++j) {
	data(i,j) = -9999; 
	mask(i,j) = true;
      }
  }
  virtual ~MissingDataFixture() {}
  blitz::Array<double, 2> data;
  blitz::Array<bool, 2> mask;
};

BOOST_FIXTURE_TEST_SUITE(iterative_morphological_dilation, MissingDataFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  IterativeMorphologicalDilation m(data, mask);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<IterativeMorphologicalDilation> m =
    boost::make_shared<IterativeMorphologicalDilation>(data, mask);
  std::string d = serialize_write_string(m);
  if(false)
    std::cerr << d;
  boost::shared_ptr<IterativeMorphologicalDilation> mr = 
    serialize_read_string<IterativeMorphologicalDilation>(d);
}

BOOST_AUTO_TEST_SUITE_END()

  
