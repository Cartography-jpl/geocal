// Forward declaration
template<typename P_type> class TestMemoryBlockReference;
// Very evil kludge. We use one of the function names in MemoryBlockReference
// to sneak in a friend declaration so we can access the internal block_
// variable. This is because we don't want to edit the actual blitz header.
#define blockLength() _fake() {return 0;}	 \
  friend class TestMemoryBlockReference<T_type>; \
  sizeType blockLength()
#include <blitz/memblock.h>
#undef blockLength
#include "unit_test_support.h"
#include "time_table.h"
#include <cmath>
#include <blitz/array.h>

using namespace GeoCal;

template<typename P_type>
class TestMemoryBlock : public blitz::MemoryBlock<P_type> {
public:
  typedef P_type T_type;
  TestMemoryBlock(blitz::sizeType length, T_type* data)
    : blitz::MemoryBlock<P_type>(length, data)
  {
    std::cerr << "Hi, in TestMemoryBlock\n";
  }
  virtual ~TestMemoryBlock()
  {
    blitz::MemoryBlock<P_type>::dataBlockAddress() = 0;
    std::cerr << "Hi, in ~TestMemoryBlock\n";
  }
};


template<typename P_type>
class TestMemoryBlockReference : public blitz::MemoryBlockReference<P_type> {
public:
  typedef P_type T_type;
  template<int N_rank>
  TestMemoryBlockReference(blitz::Array<T_type, N_rank>& a)
    : blitz::MemoryBlockReference<T_type>(0, a.data(),
					  blitz::neverDeleteData)
  {
    blitz::MemoryBlockReference<T_type>::block_ =
      new TestMemoryBlock<T_type>(0, a.data());
    a.changeBlock(*this);
  }
};

// Perhaps use https://bloglitb.blogspot.com/2010/07/access-to-private-members-thats-easy.html

BOOST_FIXTURE_TEST_SUITE(time_table, GlobalFixture)

BOOST_AUTO_TEST_CASE(temp)
{
  blitz::Array<double, 1> raw_data(5);
  raw_data = 1, 2, 3, 4, 5;
  {
    blitz::Array<double, 1> a(raw_data.data(), blitz::shape(5),
			      blitz::neverDeleteData);
    TestMemoryBlockReference<double> br(a);
    std::cerr << a << "\n";
    std::cerr << "Going out of scope for a\n";
  }
  std::cerr << "Going out of scope for raw_data\n";
}

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

  BOOST_CHECK(fabs(ttr->min_time() - t) < 1e-4);
  BOOST_CHECK(fabs(ttr->max_time() - ( t + 100 * 40.8e-3)) < 1e-4);
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
  BOOST_CHECK(fabs(ttr->min_time() - (t + 10 * 40.8e-3)) < 1e-4);
  BOOST_CHECK(fabs(ttr->max_time() - ( t + 100 * 40.8e-3)) < 1e-4);
  BOOST_CHECK_EQUAL(ttr->min_line(), 10);
  BOOST_CHECK_EQUAL(ttr->max_line(), 100);
}

BOOST_AUTO_TEST_SUITE_END()

