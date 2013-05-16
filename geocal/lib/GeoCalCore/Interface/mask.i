// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "common.i"

%{
#include "mask.h"
%}
%base_import(generic_object)
%import "ground_coordinate.i"
%geocal_shared_ptr(GeoCal::Mask);
%geocal_shared_ptr(GeoCal::CombinedMask);
namespace GeoCal {
class Mask: public GenericObject {
public:
  virtual bool mask(const GroundCoordinate& Gc) const = 0;
  virtual bool region_masked(const GroundCoordinate& Ulc, 
			     const GroundCoordinate& Lrc) const = 0;
  std::string print_to_string() const;
};

class CombinedMask: public Mask {
public:
  CombinedMask();
  CombinedMask(const std::vector<boost::shared_ptr<Mask> >& Ml);
  virtual bool mask(const GroundCoordinate& Gc) const;
  virtual bool region_masked(const GroundCoordinate& Ulc, 
			     const GroundCoordinate& Lrc) const;
  std::vector<boost::shared_ptr<Mask> > mask_list;
  %pickle_init(1, self.mask_list)
};

}
%template(Vector_Mask) std::vector<boost::shared_ptr<GeoCal::Mask> >;
