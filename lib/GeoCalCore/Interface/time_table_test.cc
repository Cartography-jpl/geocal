#include "unit_test_support.h"
#include "time_table.h"
#include <cmath>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(time_table, GlobalFixture)

BOOST_AUTO_TEST_CASE(constant_spacing_time_table)
{
  Time t = Time::parse_time("2003-01-01T10:30:00Z");
  Time t2 = t + 100.4 * 40.8e-3;
  ConstantSpacingTimeTable tt(t, t2);
  BOOST_CHECK(fabs(tt.min_time() - t) < 1e-4);
  BOOST_CHECK(fabs(tt.max_time() - ( t + 100 * 40.8e-3)) < 1e-4);
  BOOST_CHECK_EQUAL(tt.min_line(), 0);
  BOOST_CHECK_EQUAL(tt.max_line(), 100);
  Time t_expect = t + 40.5 * 40.8e-3;
  FrameCoordinate f_expect(0, 35);
  ImageCoordinate ic_expect(40.5, 35);
  ImageCoordinate ic = tt.image_coordinate(t_expect, f_expect);
  BOOST_CHECK_CLOSE(ic.line, ic_expect.line, 1e-4);
  BOOST_CHECK_CLOSE(ic.sample, ic_expect.sample, 1e-4);
  Time tres;
  FrameCoordinate fres;
  tt.time(ic, tres, fres);
  BOOST_CHECK(fabs(tres - t_expect) < 1e-4);
  BOOST_CHECK_CLOSE(fres.line, f_expect.line, 1e-4);
  BOOST_CHECK_CLOSE(fres.sample, f_expect.sample, 1e-4);
}

BOOST_AUTO_TEST_CASE(measured_time_table)
{
  Time t = Time::parse_time("2003-01-01T10:30:00Z");
  Time t2 = t + 100.4 * 40.8e-3;
  std::vector<Time> tlist;
  for(int i = 10; i <= 100; ++i)
    tlist.push_back(t + 40.8e-3 * i);
  MeasuredTimeTable tt(tlist, 10);
  BOOST_CHECK(fabs(tt.min_time() - (t + 10 * 40.8e-3)) < 1e-4);
  BOOST_CHECK(fabs(tt.max_time() - ( t + 100 * 40.8e-3)) < 1e-4);
  BOOST_CHECK_EQUAL(tt.min_line(), 10);
  BOOST_CHECK_EQUAL(tt.max_line(), 100);
  Time t_expect = t + 40.5 * 40.8e-3;
  FrameCoordinate f_expect(0, 35);
  ImageCoordinate ic_expect(40.5, 35);
  ImageCoordinate ic = tt.image_coordinate(t_expect, f_expect);
  BOOST_CHECK_CLOSE(ic.line, ic_expect.line, 1e-4);
  BOOST_CHECK_CLOSE(ic.sample, ic_expect.sample, 1e-4);
  Time tres;
  FrameCoordinate fres;
  tt.time(ic, tres, fres);
  BOOST_CHECK(fabs(tres - t_expect) < 1e-4);
  BOOST_CHECK_CLOSE(fres.line, f_expect.line, 1e-4);
  BOOST_CHECK_CLOSE(fres.sample, f_expect.sample, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()

