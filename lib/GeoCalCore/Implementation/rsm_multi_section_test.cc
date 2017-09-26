#include "unit_test_support.h"
#include "rsm_multi_section.h"
#include "rsm_fixture.h"
using namespace GeoCal;
using namespace blitz;

  
BOOST_FIXTURE_TEST_SUITE(rsm_multi_section, RsmFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  RsmRationalPolynomial rp(3,3,3,3,3,3,3,3);
  RsmMultiSection r(igc->number_line(), igc->number_sample(), 3, 2,
		    rp);
  double hmin = rpc.height_offset - rpc.height_scale;
  double hmax = rpc.height_offset + rpc.height_scale;
  r.fit(*igc, *cconv, hmin, hmax, 0, igc->number_line(), 0,
	igc->number_sample());
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic = r.image_coordinate(gp.longitude(), gp.latitude(),
					  gp.height_reference_surface());
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-2);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-2);
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  RsmRationalPolynomial rp(3,3,3,3,3,3,3,3);
  boost::shared_ptr<RsmMultiSection> r = boost::make_shared<RsmMultiSection>
    (igc->number_line(), igc->number_sample(), 3, 2, rp);
  double hmin = rpc.height_offset - rpc.height_scale;
  double hmax = rpc.height_offset + rpc.height_scale;
  r->fit(*igc, *cconv, hmin, hmax, 0, igc->number_line(), 0,
	igc->number_sample());
  std::string d = serialize_write_string(r);
  if(false)
    std::cerr << d;
  boost::shared_ptr<RsmMultiSection> rr = 
    serialize_read_string<RsmMultiSection>(d);
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic = rr->image_coordinate(gp.longitude(), gp.latitude(),
					    gp.height_reference_surface());
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-2);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-2);
}
BOOST_AUTO_TEST_SUITE_END()