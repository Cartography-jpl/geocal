// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "iterative_morphological_dilation.h"
%}
%base_import(generic_object)
%geocal_shared_ptr(GeoCal::IterativeMorphologicalDilation);
namespace GeoCal {
class IterativeMorphologicalDilation : public GenericObject {
  IterativeMorphologicalDilation(const blitz::Array<double, 2>& Image,
				 const blitz::Array<bool, 2>& Mask);
  std::string print_to_string() const;
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("IterativeMorphologicalDilation")

