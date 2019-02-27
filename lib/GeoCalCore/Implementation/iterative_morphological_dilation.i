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
public:
  enum FillOrder {C_ORDER=0};
  IterativeMorphologicalDilation(const blitz::Array<double, 2>& Image,
				 const blitz::Array<bool, 2>& Mask,
				 const blitz::Array<double, 2>& Kernel,
				 FillOrder Fill_order = C_ORDER);
  std::string print_to_string() const;
  %python_attribute(filled_image, blitz::Array<double, 2>);
  %python_attribute(filled_mask, blitz::Array<bool, 2>);
  %python_attribute(kernel, blitz::Array<double, 2>);
  %python_attribute(fill_order, FillOrder);
  %python_attribute(iteration_count, int);
  void fill_missing_data();
  double neighborhood_average(int i, int j) const;
  blitz::Array<unsigned short int, 2> masked_neighbor_count() const;
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("IterativeMorphologicalDilation")

