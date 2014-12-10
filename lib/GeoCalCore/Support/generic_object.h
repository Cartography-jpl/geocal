#ifndef GENERIC_OBJECT_H
#define GENERIC_OBJECT_H
// Add boost serialization, if available. We support the polymorphic
// archives only, so you need to use an archive derived from
// this. Note that all the standard boost archives supplied by boost
// have polymorphic versions.
// Temp
//#define HAVE_BOOST_SERIALIZATON2
#ifdef HAVE_BOOST_SERIALIZATON2
namespace boost {
namespace archive {

class polymorphic_iarchive;
class polymorphic_oarchive;

} // namespace archive
} // namespace boost
#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/shared_ptr.hpp>

// Do this a lot, so give a shortcut for this
#define GEOCAL_NVP(x) BOOST_SERIALIZATION_NVP(x)
#define GEOCAL_NVP_(x) boost::serialization::make_nvp(BOOST_PP_STRINGIZE(x), x ## _)
#define GEOCAL_NVP2(x, y) boost::serialization::make_nvp(BOOST_PP_STRINGIZE(x), y)

#endif


namespace GeoCal {
/****************************************************************//**
  For use with SWIG, it is useful to have a base class that 
  everything can be cast to. This class doesn't provide any
  functionality, other than allowing casts.
*******************************************************************/
class GenericObject {
public:
  // Have a virtual member function, which forces RTTI information to
  // be available.
  virtual ~GenericObject() {}
#ifdef HAVE_BOOST_SERIALIZATON2
  friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive & ar, const unsigned int version);
#endif
};

}

#ifdef HAVE_BOOST_SERIALIZATON2
BOOST_CLASS_EXPORT_KEY(GeoCal::GenericObject)
#endif

#endif
