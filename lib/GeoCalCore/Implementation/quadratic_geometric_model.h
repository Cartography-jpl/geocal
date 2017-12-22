#ifndef QUADRATIC_GEOMETRIC_MODEL_H
#define QUADRATIC_GEOMETRIC_MODEL_H
#include "geometric_model.h"
#include <blitz/array.h>

namespace GeoCal {
/****************************************************************//**
  This is a GeometricModel that provides a quadratic polynomial
  geometric model.
*******************************************************************/

class QuadraticGeometricModel : public GeometricModel {
public:
  enum FitType {LINEAR = 0, QUADRATIC};
  QuadraticGeometricModel(FitType ft = LINEAR,
			  double Magnify_line = 1.0, 
			  double Magnify_sample = 1.0
			  );
  QuadraticGeometricModel(const boost::shared_ptr<GeometricTiePoints>& Tp,
			  int Min_tp_to_fit,
			  FitType ft = LINEAR,
			  double Magnify_line = 1.0, 
			  double Magnify_sample = 1.0
			  );
  virtual ~QuadraticGeometricModel() {}
  void fit_transformation(const GeometricTiePoints& Tp);
  virtual void notify_update(const GeometricTiePoints& Tp)
  { if(enough_tie_point_to_fit())
      fit_transformation(*tp);
  }
  virtual ImageCoordinate original_image_coordinate
  (const ImageCoordinate& Resampled_ic) const;
  virtual ImageCoordinate resampled_image_coordinate
  (const ImageCoordinate& Original_ic) const;
  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// Transformation, which is the coefficients of the polynomial. We
/// have
///
/// x = trans(0)*px+trans(1)*py+trans(2)+trans(3)*px*px+
///     trans(4)*py*py+trans(5)*px*py
/// y = trans(6)*px+trans(7)*py+trans(8)+trans(9)*px*px+
///     trans(10)*py*py+trans(11)*px*py
//-----------------------------------------------------------------------
  
  const blitz::Array<double, 1>& transformation() const {return trans;}
  blitz::Array<double, 1>& transformation() {return trans;}

//-----------------------------------------------------------------------
/// Inverse of transformation.
//-----------------------------------------------------------------------
  const blitz::Array<double, 1>& inverse_transformation() const 
  {return inv_trans;}
  blitz::Array<double, 1>& inverse_transformation() 
  {return inv_trans;}

//-----------------------------------------------------------------------
/// Magnification factor to apply in line direction.
//-----------------------------------------------------------------------

  double magnify_line() const {return mag_ln;}
  void magnify_line(double v) {mag_ln = v;}

//-----------------------------------------------------------------------
/// Magnification factor to apply sample direction.
//-----------------------------------------------------------------------

  double magnify_sample() const {return mag_smp;}
  void magnify_sample(double v) {mag_smp = v;}

//-----------------------------------------------------------------------
/// Type of fit to do.
//-----------------------------------------------------------------------
  FitType fit_type() const {return ft;}

//-----------------------------------------------------------------------
/// Tiepoints used for Model. This may be null if we aren't actually
/// using tiepoints.
///
/// Adding points to tie_points() (or removing them) will
/// automatically update this transformation, if the number of
/// tie_points() is >= min_number_tie_point_to_fit().
//-----------------------------------------------------------------------
  const boost::shared_ptr<GeometricTiePoints>& tie_points() const
  {return tp;}

//-----------------------------------------------------------------------
/// This is the minimum number of tie_points() before we automatically
/// update our fit.
//-----------------------------------------------------------------------

  int min_number_tie_point_to_fit() const {return min_tp_to_fit_; }

//-----------------------------------------------------------------------
/// True if we 1) have tie_points() and 2) number of tie points >=
/// min_number_tie_point_to_fit().
//-----------------------------------------------------------------------
  bool enough_tie_point_to_fit() const
  { return tp && tp->number_point() >= min_tp_to_fit_; }
private:
  static const int min_tp_for_quadratic;
  boost::shared_ptr<GeometricTiePoints> tp;
  blitz::Array<double, 1> trans;
  blitz::Array<double, 1> inv_trans;
  double mag_ln, mag_smp;
  FitType ft;
  int min_tp_to_fit_;
  void fit_single(const blitz::Array<double, 2>& x,
		  const blitz::Array<double, 2>& y,
		  blitz::Array<double, 1>& tr);
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
  template<class Archive>
  void save(Archive & ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(QuadraticGeometricModel);
#endif

