#ifndef SWIG_TYPE_MAPPER_BASE_H
#define SWIG_TYPE_MAPPER_BASE_H
#include <boost/shared_ptr.hpp>
#include <map>
#include <iostream>
#include "generic_object.h"
#include "geocal_type_index.h"
#include "geocal_exception.h"

namespace GeoCal {

/****************************************************************//**
  One of the limitations of SWIG is that you can "lose" type
  information. If you pass a boost::shared_ptr<DerivedType> to an
  object holding a boost::shared_ptr<BaseType> p, then when you access
  that object as a boost::shared_ptr<BaseType> p python will then treat
  the object as "BaseType" only (so type(p) will be
  boost::shared_ptr<BaseType>, *not* boost::shared_ptr<DerivedType>).

  Note C++ has no trouble tracking the type, so typeid(*p) in C++ will
  be the typeid for DerivedType. The only problem is mapping this same
  type information back into python.

  We have set up special rules for geocal_shared_ptr.i to convert this
  back to boost::shared_ptr<DerivedType>. This depends on being able
  to map typeid of the C++ object into the python type.
  
  This class handles that mapping.

  Note that unless you are working with low level SWIG code, you will
  not use this class directly. This is all handled behind the scenes
  with the standard SWIG infrastructure we have set up.
*******************************************************************/
class SwigTypeMapperBase {
public:
  virtual void* to_python(const boost::shared_ptr<GeoCal::GenericObject>& V) = 0;
  virtual ~SwigTypeMapperBase() {}

//-----------------------------------------------------------------------
/// Add a handler for the given type info. Normally this is called in
/// the SWIG initialization of the class.
//-----------------------------------------------------------------------
  
  static void add(std::type_info const& id,
		  const boost::shared_ptr<SwigTypeMapperBase>& v)
  { swig_type_map[type_index(id)] = v; }

//-----------------------------------------------------------------------
/// Return a python object (if we can convert), or 0 if we don't find
/// the type.
//-----------------------------------------------------------------------

  static void*
  map_to_python(const boost::shared_ptr<GenericObject>& V)
  {
    GenericObject& t(*V.get());
    type_index tid(typeid(t));
    if(swig_type_map.count(tid) != 0)
      return swig_type_map[tid]->to_python(V);
    return 0;
  }

//-----------------------------------------------------------------------
/// Return a python object. This variation explicitly gives the type
/// to use.
//-----------------------------------------------------------------------

  static void*
  map_to_python(const boost::shared_ptr<GenericObject>& V, std::type_info const& id)
  {
    type_index tid(id);
    if(swig_type_map.count(tid) != 0)
      return swig_type_map[tid]->to_python(V);
    throw Exception("Unknown type");
  }
private:
//-----------------------------------------------------------------------
/// Map to go from type_index to the SwigTypeMapperBase object
/// associated with it.
//-----------------------------------------------------------------------

  static std::map<type_index, boost::shared_ptr<SwigTypeMapperBase> >
  swig_type_map;
};
}
#endif
