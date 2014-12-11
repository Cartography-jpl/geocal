// Need to have declaration of serialize outside of inclusion of
// polymorphic_text_iarchive etc, due to really convoluted template
// voodoo that the boost library uses. I don't think this is a general
// issue, just for this specific unit test because we are doing fairly
// complicated stuff here.

#include "geocal_serialize_support.h"
#include "generic_object.h"
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
namespace GeoCal {
class FakeClass : public GeoCal::GenericObject {
public:
  FakeClass(int I1, int I2) : i1(I1), i2(I2) {}
  virtual ~FakeClass() {}
  int i1, i2;
private:
  FakeClass() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(FakeClass);

template<class Archive>
void FakeClass::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(FakeClass);
  ar & GEOCAL_NVP(i1) & GEOCAL_NVP(i2);
}

GEOCAL_IMPLEMENT(FakeClass);

#endif
