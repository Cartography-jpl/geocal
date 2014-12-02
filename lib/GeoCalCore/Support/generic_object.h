#ifndef GENERIC_OBJECT_H
#define GENERIC_OBJECT_H
// Optional boost serialization information. This is entirely done
// through header files, so code using geocal can decide to use this
// or not.
//
// Users of this class that want Boost serializations should:
// 1. Define "USE_BOOST_SERIALIZATON" before including headers
// 2. Include all supported archive types before including any
//    GeoCal headers.
// 3. Either directly use BOOST_CLASS_EXPORT for supported classes,
//    or the appropriate header files that define this (e.g.,
//    geocal_serialize.h for everything, geocal_serialize_camera.h for
//    Camera and derived classes, etc.).
//
// See the image_coordinate_test.cc unit test for an example of this,
// we use this particularly simple class as a test of the
// serialization. 
//
// Note that if you add serialization to a class, you should make sure
// it gets added to one of the files in Serialize directory.
#ifdef USE_BOOST_SERIALIZATON
#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/shared_ptr.hpp>
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
#ifdef USE_BOOST_SERIALIZATON
  friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive & ar, const unsigned int version)
  {
    // Nothing to do
  }
#endif
};

}
#endif
