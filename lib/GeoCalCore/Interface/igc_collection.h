#ifndef IGC_COLLECTION_H
#define IGC_COLLECTION_H
#include "image_ground_connection.h"

namespace GeoCal {
/****************************************************************//**
  This is a collection of ImageGroundConnection. 
*******************************************************************/

class IgcCollection : public Printable<IgcCollection>,
		      public virtual WithParameter {
public:
  virtual ~IgcCollection() {}
  virtual int number_image() const { return 0; }

//-----------------------------------------------------------------------
/// Return collinearity residual for given Image_index (see
/// ImageGroundConnection for details on this.
//-----------------------------------------------------------------------

  virtual blitz::Array<double, 1> 
  collinearity_residual(int Image_index,
			const GroundCoordinate& Gc,
			const ImageCoordinate& Ic_actual) const
  {
    return image_ground_connection(Image_index)->collinearity_residual(Gc, Ic_actual);
  }

//-----------------------------------------------------------------------
/// Return jacobian of collinearity_residual. The parameters for this
/// class should already have AutoDerivative extended for the jacobian
/// (e.g., call add_identity_gradient()).
/// 
/// We add the derivative wrt the CartesianFixed coordinates of the
/// Ground_coor (X, Y, Z in that order), at the end of the
/// Jacobian. So the total Jacobian is 2 x (number parameter + 3).
//-----------------------------------------------------------------------

  virtual blitz::Array<double, 2> 
  collinearity_residual_jacobian(int Image_index,
  		        const GroundCoordinate& Gc,
			const ImageCoordinate& Ic_actual) const
  {
    return image_ground_connection(Image_index)->collinearity_residual_jacobian(Gc, Ic_actual);
  }

//-----------------------------------------------------------------------
/// Return ground coordinate that goes with a particular image
/// coordinate. 
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate(int Image_index, const ImageCoordinate& Ic) const
  { return ground_coordinate_dem(Image_index, Ic, *dem(Image_index)); }

//-----------------------------------------------------------------------
/// Return ground coordinate that goes with a particular image
/// coordinate. This version supplies a Dem to use.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(int Image_index, const ImageCoordinate& Ic,
			const Dem& D) const
  { return image_ground_connection(Image_index)->ground_coordinate_dem(Ic, D); }

//-----------------------------------------------------------------------
/// Dem used by ground_coordinate
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<Dem> dem(int Image_index) const
  { return image_ground_connection(Image_index)->dem_ptr(); }

//-----------------------------------------------------------------------
/// Return image coordinate that goes with a particular
/// GroundCoordinate.
///
/// For some types of ImageGroundConnection, we might not be able to
/// calculate image_coordinate for all values (e.g., Ipi might fail).
/// In those cases, we will throw a ImageGroundConnectionFailed
/// exception. This means that nothing is wrong, other than that we
/// can't calculate the image_coordinate. Callers can catch this
/// exception if they have some way of handling no image coordinate
/// data. 
//-----------------------------------------------------------------------

  virtual ImageCoordinate image_coordinate(int Image_index,
					   const GroundCoordinate& Gc) 
    const
  { return image_ground_connection(Image_index)->image_coordinate(Gc); }

//-----------------------------------------------------------------------
/// Return the Jacobian of the image coordinates with respect to the
/// parameters.
//-----------------------------------------------------------------------

  virtual blitz::Array<double, 2> 
  image_coordinate_jac_parm(int Image_index, const CartesianFixed& Gc) 
    const
  { return image_ground_connection(Image_index)->image_coordinate_jac_parm(Gc); }


virtual blitz::Array<double, 2> 
image_coordinate_jac_parm_fd(int Image_index, const CartesianFixed& Gc,
			     const blitz::Array<double, 1> Pstep) const;

//-----------------------------------------------------------------------
/// Return the Jacobian of the image coordinates with respect to the
/// X, Y, and Z components of the CartesianFixed ground location. 
//-----------------------------------------------------------------------

  virtual blitz::Array<double, 2> 
  image_coordinate_jac_cf(int Image_index, const CartesianFixed& Gc) 
    const
  { return image_ground_connection(Image_index)->image_coordinate_jac_cf(Gc); }

//-----------------------------------------------------------------------
/// Return the Jacobian of the image coordinates with respect to the
/// X, Y, and Z components of the CartesianFixed ground location.
/// Calculated as a finite difference with the given step size.
//-----------------------------------------------------------------------

  virtual blitz::Array<double, 2> 
  image_coordinate_jac_cf_fd(int Image_index, const CartesianFixed& Gc,
			     double Step_size) 
    const
  { return image_ground_connection(Image_index)->image_coordinate_jac_cf_fd(Gc, Step_size); }

//-----------------------------------------------------------------------
/// Title that we can use to describe the image. This can be any
/// string that is useful as a label.
//-----------------------------------------------------------------------

  virtual std::string title(int Image_index) const 
  { return image_ground_connection(Image_index)->title(); }

//-----------------------------------------------------------------------
/// Underlying image (if present)
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<RasterImage> image(int Image_index) const
  { return image_ground_connection(Image_index)->image(); }

//-----------------------------------------------------------------------
/// Image ground connection for given image index.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<ImageGroundConnection> 
  image_ground_connection(int Image_index) const = 0;

//-----------------------------------------------------------------------
/// Return IgcCollection for a subset of the data.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<IgcCollection> 
  subset(const std::vector<int>& Index_set) const = 0;

//-----------------------------------------------------------------------
/// Default is that ImageGroundConnection ignores AutoDerivative
/// information in the parameters. Derived classes can override this
/// if they support this. This should mostly be an internal matter,
/// the derived class can use the AutoDerivative in its implementation
/// of image_coordinate_jac_parm, but it can also do this calculation
/// in another manner if desired.
//-----------------------------------------------------------------------

  virtual ArrayAd<double, 1> parameter_with_derivative() const
  { return ArrayAd<double, 1>(parameter()); }
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& P) 
  { parameter(P.value()); }

//-----------------------------------------------------------------------
/// Default is no parameters.
//-----------------------------------------------------------------------
  virtual blitz::Array<double, 1> parameter() const
  { return blitz::Array<double, 1>(); }
  virtual void parameter(const blitz::Array<double, 1>& P)
  { 
    if(P.rows() != 0)
      throw Exception("No parameters supported");
  }

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { Os << "IgcCollection"; }

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(IgcCollection);

#endif
