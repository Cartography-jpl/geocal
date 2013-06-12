// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "common.i"

%{
#include "ground_mask.h"
%}
%base_import(generic_object)
%import "ground_coordinate.i"
%geocal_shared_ptr(GeoCal::GroundMask);
%geocal_shared_ptr(GeoCal::CombinedGroundMask);
namespace GeoCal {
class GroundMask: public GenericObject {
public:
  virtual bool mask(const GroundCoordinate& Gc) const = 0;
  virtual bool region_masked(const GroundCoordinate& Ulc, 
			     const GroundCoordinate& Lrc) const = 0;
  std::string print_to_string() const;
};

class CombinedGroundMask: public GroundMask {
public:
  CombinedGroundMask();
  CombinedGroundMask(const std::vector<boost::shared_ptr<GroundMask> >& Ml);
  virtual bool mask(const GroundCoordinate& Gc) const;
  virtual bool region_masked(const GroundCoordinate& Ulc, 
			     const GroundCoordinate& Lrc) const;
  std::vector<boost::shared_ptr<GroundMask> > mask_list;
  %pickle_init(1, self.mask_list)
};

}
%template(Vector_GroundMask) std::vector<boost::shared_ptr<GeoCal::GroundMask> >;
