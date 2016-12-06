// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "ray_intersect.h"
%}
%base_import(generic_object)
%import "image_ground_connection.i"
%import "ground_coordinate.i"
%geocal_shared_ptr(GeoCal::RayIntersect);
namespace GeoCal {
class RayIntersect : public GenericObject {
public:
  RayIntersect(const boost::shared_ptr<ImageGroundConnection> Igc1,
	       const boost::shared_ptr<ImageGroundConnection> Igc2);
  static void two_look_vector_intersect(const CartesianFixed& P1,
				       const CartesianFixedLookVector& L1,
				       const CartesianFixed& P2,
				       const CartesianFixedLookVector& L2,
				       boost::shared_ptr<CartesianFixed>& 
				       OUTPUT, double& OUTPUT);
  void two_ray_intersect(const ImageCoordinate& Ic1,
			const ImageCoordinate& Ic2,
			boost::shared_ptr<CartesianFixed>& OUTPUT, 
			double& OUTPUT);
  %python_attribute(image_ground_connection1, 
		    boost::shared_ptr<ImageGroundConnection>)
  %python_attribute(image_ground_connection2, 
		    boost::shared_ptr<ImageGroundConnection>)
  %pickle_init(1, self.image_ground_connection1, self.image_ground_connection2)
};
}

