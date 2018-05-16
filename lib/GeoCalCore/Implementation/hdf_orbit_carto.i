// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"
// We only include this if we have the carto library which includes
// the eci_tod_burl class.
%{
#include "hdf_orbit.h"
#include "eci_tod.h"
#include "eci_tod_burl.h"
%}
%import "hdf_orbit.i"
%import "eci_tod_burl.i"
%geocal_shared_ptr(GeoCal::HdfOrbit<GeoCal::EciTodBurl, GeoCal::TimeAcsCreator>);
%template(HdfOrbit_EciTodBurl_TimeAcs) GeoCal::HdfOrbit<GeoCal::EciTodBurl, GeoCal::TimeAcsCreator>;
// List of things "import *" will include
%python_export("HdfOrbit_EciTodBurl_TimeAcs")
