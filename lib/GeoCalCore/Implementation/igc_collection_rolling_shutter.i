// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "igc_collection_rolling_shutter.h"
%}
%base_import(igc_collection)
%import "image_ground_connection.i"
%import "orbit.i"
%import "camera.i"
%import "dem.i"
%import "time_table.i"
%import "raster_image.i"
%geocal_shared_ptr(GeoCal::IgcCollectionRollingShutter);
namespace GeoCal {
class IgcCollectionRollingShutter: public IgcCollection {
public:
  IgcCollectionRollingShutter
  (const std::vector<boost::shared_ptr<RasterImage> >& Img_list, 
   const std::vector<boost::shared_ptr<TimeTable> >& Tt_list,
   const std::vector<std::string>& Title_list,
    const boost::shared_ptr<Orbit>& Orb,
   const boost::shared_ptr<Camera>& Cam,
   const boost::shared_ptr<Dem>& D);
  IgcCollectionRollingShutter
  (const boost::shared_ptr<Orbit>& Orb,
   const boost::shared_ptr<Camera>& Cam,
   const boost::shared_ptr<Dem>& D);
  void add_image(const boost::shared_ptr<RasterImage>& Img,
		 const boost::shared_ptr<TimeTable>& Tt,
		 const std::string& Title);
  %python_attribute_with_set(orbit, boost::shared_ptr<Orbit>);
  %python_attribute_with_set(camera, boost::shared_ptr<Camera>);
  virtual boost::shared_ptr<ImageGroundConnection> 
  image_ground_connection(int Image_index) const;
  virtual boost::shared_ptr<IgcCollection> 
  subset(const std::vector<int>& Index_set) const;
  %pickle_serialization()
};
}

// List of things "import *" will include
%python_export("IgcCollectionRollingShutter")
