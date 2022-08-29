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
  BOOST_CHECK(fabs(tt.min_time() - (t - 40.8e-3)) < 1e-4);
  BOOST_CHECK(fabs(tt.max_time() - ( t + 101 * 40.8e-3)) < 1e-4);
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
  Time t3, t4;
  tt.time_acquisition(t + 40.5 * 40.8e-3, FrameCoordinate(0,10), t3, t4);
  BOOST_CHECK(fabs(t3 -  (t + 40 * 40.8e-3)) <  1e-4);
  BOOST_CHECK(fabs(t4 -  (t + 41 * 40.8e-3)) <  1e-4);
}

BOOST_AUTO_TEST_CASE(framelet_time_table)
{
  Time t = Time::parse_time("2003-01-01T10:30:00Z");
  Time t2 = t + 100.4 * 40.8e-3;
  int framelet_size = 10;
  ConstantSpacingFrameletTimeTable tt(t, t2, framelet_size, 40.8e-3);
  BOOST_CHECK(fabs(tt.min_time() - (t - 40.8e-3)) < 1e-4);
  BOOST_CHECK(fabs(tt.max_time() - ( t + 101 * 40.8e-3)) < 1e-4);
  BOOST_CHECK_EQUAL(tt.min_line(), 0);
  BOOST_CHECK_EQUAL(tt.max_line(), 100*framelet_size);
  Time t_expect = t + 40.5 * 40.8e-3;
  FrameCoordinate f_expect(0, 35);
  ImageCoordinate ic_expect(405, 35);
  ImageCoordinate ic = tt.image_coordinate(t_expect, f_expect);
  BOOST_CHECK_CLOSE(ic.line, ic_expect.line, 1e-4);
  BOOST_CHECK_CLOSE(ic.sample, ic_expect.sample, 1e-4);
  Time tres;
  FrameCoordinate fres;
  tt.time(ic, tres, fres);
  BOOST_CHECK(fabs(tres - (t + 40 * 40.8e-3)) < 1e-4);
  BOOST_CHECK_CLOSE(fres.line, 5, 1e-4);
  BOOST_CHECK_CLOSE(fres.sample, f_expect.sample, 1e-4);
  Time t3, t4;
  tt.time_acquisition(t + 40.5 * 40.8e-3, FrameCoordinate(0,10), t3, t4);
  BOOST_CHECK(fabs(t3 -  (t + 40 * 40.8e-3)) <  1e-4);
  BOOST_CHECK(fabs(t4 -  (t + 41 * 40.8e-3)) <  1e-4);
}

BOOST_AUTO_TEST_CASE(measured_time_table)
{
  Time t = Time::parse_time("2003-01-01T10:30:00Z");
  Time t2 = t + 100.4 * 40.8e-3;
  std::vector<Time> tlist;
  for(int i = 10; i <= 100; ++i)
    tlist.push_back(t + 40.8e-3 * i);
  MeasuredTimeTable tt(tlist, 10);
  BOOST_CHECK(fabs(tt.min_time() - (t + 9 * 40.8e-3)) < 1e-4);
  BOOST_CHECK(fabs(tt.max_time() - ( t + 101 * 40.8e-3)) < 1e-4);
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

BOOST_AUTO_TEST_CASE(serialization_constant_time_table)
{
  if(!have_serialize_supported())
    return;
  Time t = Time::parse_time("2003-01-01T10:30:00Z");
  Time t2 = t + 100.4 * 40.8e-3;
  boost::shared_ptr<TimeTable> tt(new ConstantSpacingTimeTable(t, t2));
  std::string d = serialize_write_string(tt);
  if(false)
    std::cerr << d;
  boost::shared_ptr<ConstantSpacingTimeTable> ttr = 
    serialize_read_string<ConstantSpacingTimeTable>(d);

  BOOST_CHECK(fabs(ttr->min_time() - (t - 40.8e-3)) < 1e-4);
  BOOST_CHECK(fabs(ttr->max_time() - ( t + 101 * 40.8e-3)) < 1e-4);
  BOOST_CHECK_EQUAL(ttr->min_line(), 0);
  BOOST_CHECK_EQUAL(ttr->max_line(), 100);

}

BOOST_AUTO_TEST_CASE(serialization_measured_time_table)
{
  if(!have_serialize_supported())
    return;
  Time t = Time::parse_time("2003-01-01T10:30:00Z");
  Time t2 = t + 100.4 * 40.8e-3;
  std::vector<Time> tlist;
  for(int i = 10; i <= 100; ++i)
    tlist.push_back(t + 40.8e-3 * i);
  boost::shared_ptr<TimeTable> tt(new MeasuredTimeTable(tlist, 10));
  std::string d = serialize_write_string(tt);
  if(false)
    std::cerr << d;
  boost::shared_ptr<MeasuredTimeTable> ttr = 
    serialize_read_string<MeasuredTimeTable>(d);
  BOOST_CHECK(fabs(ttr->min_time() - (t + 9 * 40.8e-3)) < 1e-4);
  BOOST_CHECK(fabs(ttr->max_time() - ( t + 101 * 40.8e-3)) < 1e-4);
  BOOST_CHECK_EQUAL(ttr->min_line(), 10);
  BOOST_CHECK_EQUAL(ttr->max_line(), 100);
}

BOOST_AUTO_TEST_SUITE_END()

