#ifndef IGC_ARRAY_H
#define IGC_ARRAY_H
#include "igc_collection.h"

namespace GeoCal {
/****************************************************************//**
  This is a IgcCollection that is just an array of independent 
  ImageGroundConnection.
*******************************************************************/

class IgcArray : public virtual IgcCollection,
		 public virtual WithParameterNested {
public:
  IgcArray(const std::vector<boost::shared_ptr<ImageGroundConnection> >& 
	   Igc_list);
  virtual ~IgcArray() {}
  virtual int number_image() const { return (int) igc_list.size(); }
  virtual boost::shared_ptr<ImageGroundConnection> 
  image_ground_connection(int Image_index) const
  { 
    range_check(Image_index, 0, number_image());
    return igc_list[Image_index];
  }
  virtual void print(std::ostream& Os) const;
  virtual boost::shared_ptr<IgcCollection> 
  subset(const std::vector<int>& Index_set) const;

  virtual blitz::Array<double, 1> parameter() const
  { return WithParameterNested::parameter(); }
  virtual void parameter(const blitz::Array<double, 1>& Parm)
  { WithParameterNested::parameter(Parm); }
  virtual ArrayAd<double, 1> parameter_with_derivative() const
  { return WithParameterNested::parameter_with_derivative(); }
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm)
  { WithParameterNested::parameter_with_derivative(Parm);}
  void add_identity_gradient();
  virtual blitz::Array<double, 2> 
  collinearity_residual_jacobian(int Image_index,
				 const GroundCoordinate& Gc,
				 const ImageCoordinate& Ic_actual) const;
private:
  std::vector<boost::shared_ptr<ImageGroundConnection> > igc_list;
  IgcArray() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(IgcArray);
#endif
