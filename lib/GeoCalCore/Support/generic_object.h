#ifndef GENERIC_OBJECT_H
#define GENERIC_OBJECT_H
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

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
  friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive & ar, const unsigned int version)
  {
    // Nothing to do
  }
};
}
#endif
