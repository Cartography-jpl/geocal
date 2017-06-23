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
	   Igc_list, bool Assume_igc_independent = true);
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
//-----------------------------------------------------------------------
/// Return assumption about ImageGroundConnection being
/// independent. If they are, then we can get a optimization in the
/// jacobian calculation to speed it up. If they aren't (e.g., they
/// share a common Orbit that has been added to the
/// WithParameterNested), that is fine. We just take longer to do the
/// calculation. But we need to know this to avoid making an incorrect
/// optimization.
//-----------------------------------------------------------------------
  bool assume_igc_independent() const {return assume_igc_independent_;}
  void assume_igc_independent(bool v) {assume_igc_independent_ = v;}
private:
  std::vector<boost::shared_ptr<ImageGroundConnection> > igc_list;
  bool assume_igc_independent_;
  IgcArray() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
  template<class Archive>
  void save(Archive & ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(IgcArray);
GEOCAL_CLASS_VERSION(IgcArray, 1);
#endif
