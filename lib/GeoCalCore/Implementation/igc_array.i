// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "igc_array.h"
%}
%base_import(igc_collection)
%import "image_ground_connection.i"
%geocal_shared_ptr(GeoCal::IgcArray);
namespace GeoCal {
class IgcArray: public IgcCollection, public WithParameterNested {
public:
  // Note that passing in a python array often has lifetime
  // issues. You should either work directly with a
  // Vector_ImageGroundConnection, or set this up as empty an use
  // add_igc to populate this.
  IgcArray(const std::vector<boost::shared_ptr<ImageGroundConnection> >& 
	   Igc_list, bool Assume_igc_independent=true);
  virtual boost::shared_ptr<ImageGroundConnection> 
  image_ground_connection(int Image_index) const;
  virtual boost::shared_ptr<IgcCollection> 
  subset(const std::vector<int>& Index_set) const;
  void add_igc(const boost::shared_ptr<ImageGroundConnection>& Igc);
  %python_attribute_with_set(assume_igc_independent, bool);
  %pickle_serialization()
};
}

// List of things "import *" will include
%python_export("IgcArray")
