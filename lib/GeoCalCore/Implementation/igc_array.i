// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "igc_array.h"
%}
%base_import(igc_collection)
%import "image_ground_connection.i"
%geocal_shared_ptr(GeoCal::IgcArray);
namespace GeoCal {
class IgcArray: public IgcCollection {
public:
  IgcArray(const std::vector<boost::shared_ptr<ImageGroundConnection> >& 
	   Igc_list);
  virtual boost::shared_ptr<ImageGroundConnection> 
  image_ground_connection(int Image_index) const;
  virtual boost::shared_ptr<IgcCollection> 
  subset(const std::vector<int>& Index_set) const;
  %pickle_serialization()
%pythoncode {
def __init__(self, igc_list):
    # We can probably eventually fix the automatic swig conversion to
    # vector, but right now this doesn not work correctly. So we just
    # replace init with a version that checks for this
    if(isinstance(igc_list, geocal_swig.Vector_ImageGroundConnection)):
        t = igc_list
    else:
        t = geocal_swig.Vector_ImageGroundConnection()
        for igc in igc_list:
            t.push_back(igc)
    _igc_array.IgcArray_swiginit(self,_igc_array.new_IgcArray(t))
  
}
};
}

