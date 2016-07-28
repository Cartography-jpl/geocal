#ifndef SWIG_TYPE_MAPPER_BASE_H
#define SWIG_TYPE_MAPPER_BASE_H
#include <Python.h>
#include <boost/shared_ptr.hpp>
#include <map>
#include <iostream>
#include "generic_object.h"
#include "geocal_type_index.h"


namespace GeoCal {

/****************************************************************//**
  This is the base class that is used by fp_shared_ptr.i to map a
  shared_ptr to a python object.
*******************************************************************/
class SwigTypeMapperBase {
public:
  virtual PyObject* to_python(const boost::shared_ptr<GeoCal::GenericObject>& V) = 0;
  virtual ~SwigTypeMapperBase() {}
};

//-----------------------------------------------------------------------
/// Map to go from type_index to the SwigTypeMapperBase object
/// associated with it.
//-----------------------------------------------------------------------

  extern std::map<type_index, boost::shared_ptr<SwigTypeMapperBase> > swig_type_map;

}
#endif
