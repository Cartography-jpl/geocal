#ifndef GENERIC_OBJECT_H
#define GENERIC_OBJECT_H
#include "geocal_config.h"

namespace boost {
  namespace serialization {
    class access;
  }
}

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
#include <boost/serialization/export.hpp>
#define GEOCAL_EXPORT_KEY(NAME) BOOST_CLASS_EXPORT_KEY(GeoCal::NAME)
#else
#define GEOCAL_EXPORT_KEY(x) /* Noop */
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
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(GenericObject);

#endif
