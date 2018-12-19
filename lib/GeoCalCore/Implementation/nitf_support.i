// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geocal_autoderivative_quaternion.h"
#include "nitf_support.h"
%}

%import "geocal_quaternion.i"

namespace GeoCal {

boost::math::quaternion<double>
  nitf_to_quaternion(const blitz::Array<double, 1>& Data);
  
blitz::Array<double, 1>
  quaternion_to_nitf(const boost::math::quaternion<double>& Q);
  
blitz::Array<double, 1>
  quaternion_to_nitf(const blitz::Array<double, 1>& Q);

blitz::Array<double, 2>
  quaternion_to_nitf(const blitz::Array<double, 2>& Q);
  
}

// List of things "import *" will include
%python_export("nitf_to_quaternion", "quaternion_to_nitf")
