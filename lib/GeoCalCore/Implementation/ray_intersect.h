#ifndef RAY_INTERSECT_H
#define RAY_INTERSECT_H
#include "image_ground_connection.h"

namespace GeoCal {
/****************************************************************//**
  This finds the point closest intersection of two look vector. Note
  that in generate two look vectors don't intersect, we find the point
  closest to the two look vectors.
*******************************************************************/
class RayIntersect : public Printable<ImageGroundConnection> {
public:
  RayIntersect(const boost::shared_ptr<ImageGroundConnection> Igc1,
	       const boost::shared_ptr<ImageGroundConnection> Igc2)
    : igc1(Igc1), igc2(Igc2) { }

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { Os << "RayIntersect"; }

  static void two_look_vector_intersect(const CartesianFixed& P1,
				       const CartesianFixedLookVector& L1,
				       const CartesianFixed& P2,
				       const CartesianFixedLookVector& L2,
				       boost::shared_ptr<CartesianFixed>& P, 
				       double& Dist);

//-----------------------------------------------------------------------
/// Return intersection of look vector corresponding to two image
/// coordinates. This is the point closest to both look vectors, along
/// with the distance of that point from each look vector.
//-----------------------------------------------------------------------

  void two_ray_intersect(const ImageCoordinate& Ic1,
			const ImageCoordinate& Ic2,
			boost::shared_ptr<CartesianFixed>& P, 
			double& Dist)
  { 
    boost::shared_ptr<CartesianFixed> p1, p2;
    CartesianFixedLookVector l1, l2;
    igc1->cf_look_vector(Ic1, l1, p1);
    igc2->cf_look_vector(Ic2, l2, p2);
    return two_look_vector_intersect(*p1, l1, *p2, l2, P, Dist);
  }

  boost::shared_ptr<ImageGroundConnection> image_ground_connection1()
    const { return igc1;}
  boost::shared_ptr<ImageGroundConnection> image_ground_connection2()
    const { return igc2;}
private:
  boost::shared_ptr<ImageGroundConnection> igc1, igc2;
};
}
#endif
