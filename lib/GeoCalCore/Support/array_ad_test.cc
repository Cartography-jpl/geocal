#include "array_ad.h"
#include "unit_test_support.h"

using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(array_ad, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  ArrayAd<double, 2> ad(2,3,4);
  ad(0, 1) = AutoDerivative<double>(3, 1, 4);
  ad(0, 2) = 5;
  BOOST_CHECK_CLOSE(ad.value()(0,1), 3.0, 1e-8);
  BOOST_CHECK_CLOSE(ad.value()(0,2), 5.0, 1e-8);
  Array<double, 1> grad_expect(4);
  grad_expect = 0, 1, 0, 0;
  BOOST_CHECK_MATRIX_CLOSE(ad.jacobian()(0,1,Range::all()), grad_expect);
  grad_expect = 0;
  BOOST_CHECK_MATRIX_CLOSE(ad.jacobian()(0,2,Range::all()), grad_expect);
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<ArrayAd<double, 1> > ad(new ArrayAd<double, 1>(2, 4));
  (*ad)(0) = AutoDerivative<double>(3, 1, 4);
  (*ad)(1) = AutoDerivative<double>(5, 2, 4);
  std::string d = serialize_write_string(ad);
  if(false)
    // Can dump to screen, if we want to see the text
    std::cerr << d;
  boost::shared_ptr<ArrayAd<double, 1> > adr =
    serialize_read_string<ArrayAd<double, 1> >(d);
  BOOST_CHECK_MATRIX_CLOSE(adr->value(), ad->value());
  BOOST_CHECK_MATRIX_CLOSE(adr->jacobian(), ad->jacobian());
}

template<typename T> class SwigValueWrapper {
public:
  struct SwigSmartPointer {
    T *ptr;
    SwigSmartPointer(T *p) : ptr(p) { }
    ~SwigSmartPointer() { delete ptr; }
    SwigSmartPointer& operator=(SwigSmartPointer& rhs) { T* oldptr = ptr; ptr = 0; delete oldptr; ptr = rhs.ptr; rhs.ptr = 0; return *this; }
    void reset(T *p) { T* oldptr = ptr; ptr = 0; delete oldptr; ptr = p; }
  } pointer;
  SwigValueWrapper& operator=(const SwigValueWrapper<T>& rhs);
  SwigValueWrapper(const SwigValueWrapper<T>& rhs);
  SwigValueWrapper() : pointer(0) { }
  SwigValueWrapper& operator=(const T& t) { SwigSmartPointer tmp(new T(t)); pointer = tmp; return *this; }
#if __cplusplus >=201103L
  SwigValueWrapper& operator=(T&& t) { SwigSmartPointer tmp(new T(std::move(t))); pointer = tmp; return *this; }
  operator T&&() const { return std::move(*pointer.ptr); }
#else
  operator T&() const { return *pointer.ptr; }
#endif
  T *operator&() const { return pointer.ptr; }
  static void reset(SwigValueWrapper& t, T *p) { t.pointer.reset(p); }
};

BOOST_AUTO_TEST_CASE(move_test)
{
  // swig uses the newer std::move semantics. We initially ran into
  // problems with this, have a unit test which try this out.
  //
  // Turns outs the move works fine, the problem is a bug with
  // swig. See below. We'll need to fix this, but set it aside for
  // now. The fix might just be using a newer version of SWIG, we'll
  // have to investigate this further when we have a little more time.
  ArrayAd<double, 1> ad(2,4);
  ad(0) = AutoDerivative<double>(3, 1, 4);
  ad(1) = 5;
  BOOST_CHECK_CLOSE(ad.value()(0), 3.0, 1e-8);
  BOOST_CHECK_CLOSE(ad.value()(1), 5.0, 1e-8);
  Array<double, 1> grad_expect(4);
  grad_expect = 0, 1, 0, 0;
  BOOST_CHECK_MATRIX_CLOSE(ad.jacobian()(0,Range::all()), grad_expect);
  grad_expect = 0;
  BOOST_CHECK_MATRIX_CLOSE(ad.jacobian()(1,Range::all()), grad_expect);

  SwigValueWrapper< GeoCal::ArrayAd< double,1 > > result;
  result = ad;

  BOOST_CHECK_CLOSE(ad.value()(0), 3.0, 1e-8);
  BOOST_CHECK_CLOSE(ad.value()(1), 5.0, 1e-8);
  grad_expect = 0, 1, 0, 0;
  BOOST_CHECK_MATRIX_CLOSE(ad.jacobian()(0,Range::all()), grad_expect);
  grad_expect = 0;
  BOOST_CHECK_MATRIX_CLOSE(ad.jacobian()(1,Range::all()), grad_expect);

  // Swig breaks
  //auto admove = new GeoCal::ArrayAd< double,1 >((GeoCal::ArrayAd<
  //double,1 > &)result);
  // Correct
  auto admove = new GeoCal::ArrayAd< double,1 >((GeoCal::ArrayAd< double,1 > &&)result);
  BOOST_CHECK_CLOSE(admove->value()(0), 3.0, 1e-8);
  BOOST_CHECK_CLOSE(admove->value()(1), 5.0, 1e-8);
  grad_expect = 0, 1, 0, 0;
  BOOST_CHECK_MATRIX_CLOSE(admove->jacobian()(0,Range::all()), grad_expect);
  grad_expect = 0;
  BOOST_CHECK_MATRIX_CLOSE(admove->jacobian()(1,Range::all()), grad_expect);
}


BOOST_AUTO_TEST_SUITE_END()
