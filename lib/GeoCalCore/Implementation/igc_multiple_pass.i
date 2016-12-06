// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "igc_multiple_pass.h"
%}
%base_import(igc_collection)
%import "image_ground_connection.i"
%geocal_shared_ptr(GeoCal::IgcMultiplePass);
namespace GeoCal {
class IgcMultiplePass: public IgcCollection, public WithParameterNested {
public:
  IgcMultiplePass();
  IgcMultiplePass(const std::vector<boost::shared_ptr<IgcCollection> >& Igc_list);  
  void add_igc(const boost::shared_ptr<IgcCollection> Igccol);
  virtual boost::shared_ptr<ImageGroundConnection> 
  image_ground_connection(int Image_index) const;
  virtual boost::shared_ptr<IgcCollection> 
  subset(const std::vector<int>& Index_set) const;
  %pickle_serialization();
};
}

