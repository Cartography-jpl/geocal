// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "geocal_common.i"

%{
#include "rsm_nitf.h"
#include "image_ground_connection.h"
%}
%import "rsm.i"

namespace GeoCal {
void rsm_write_nitf(const std::string& Fname,
		    const boost::shared_ptr<Rsm>& R);
boost::shared_ptr<Rsm> rsm_read_nitf(const std::string& Fname);
}

// List of things "import *" will include
%python_export("rsm_write_nitf", "rsm_read_nitf")
