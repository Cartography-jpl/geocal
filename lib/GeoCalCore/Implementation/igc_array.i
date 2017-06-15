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
  IgcArray(const std::vector<boost::shared_ptr<ImageGroundConnection> >& 
	   Igc_list);
  virtual boost::shared_ptr<ImageGroundConnection> 
  image_ground_connection(int Image_index) const;
  virtual boost::shared_ptr<IgcCollection> 
  subset(const std::vector<int>& Index_set) const;
  %pickle_serialization()
};
}

