#ifndef RSM_RATIONAL_POLYNOMIAL_H
#define RSM_RATIONAL_POLYNOMIAL_H
#include "rsm_polynomial.h"
#include "rsm_base.h"
#include "image_coordinate.h"

namespace GeoCal {
  class Rpc;
  class ImageGroundConnection;
  class CoordinateConverter;
  
/****************************************************************//**
  This is a RsmRationalPolynomial.
*******************************************************************/

class RsmRationalPolynomial : public RsmBase {
public:
  RsmRationalPolynomial(int Np_x, int Np_y, int Np_z, int Dp_x, int Dp_y,
			int Dp_z, int N_max_order = -1, int D_max_order = -1);
  RsmRationalPolynomial(const RsmRationalPolynomial& Rp);
  virtual ~RsmRationalPolynomial() {}
  virtual void print(std::ostream& Os) const;
  virtual boost::shared_ptr<RsmBase> clone() const
  { return boost::shared_ptr<RsmBase>(new RsmRationalPolynomial(*this)); }
  virtual ImageCoordinate image_coordinate(double X, double Y, double Z) const;
  virtual blitz::Array<double, 2> image_coordinate
  (const blitz::Array<double, 1>& X, const blitz::Array<double, 1>& Y,
   const blitz::Array<double, 1>& Z) const;
  virtual blitz::Array<double, 3> image_coordinate
  (const blitz::Array<double, 2>& X,
   const blitz::Array<double, 2>& Y, const blitz::Array<double, 2>& Z) const;
  virtual blitz::Array<double, 4> image_coordinate
  (const blitz::Array<double, 3>& X,
   const blitz::Array<double, 3>& Y, const blitz::Array<double, 3>& Z) const;
  void set_rpc_coeff(const Rpc& R);
  void fit_offset_and_scale(double Min_line, double Max_line, double Min_sample,
			    double Max_sample, double Min_x, double Max_x,
			    double Min_y, double Max_y, double Min_z,
			    double Max_z);
  void fit_data(const std::vector<double>& Line,
		const std::vector<double>& Sample,
		const std::vector<double>& X,
		const std::vector<double>& Y,
		const std::vector<double>& Z);
  virtual void fit(const ImageGroundConnection& Igc,
	   const CoordinateConverter& Cconv,
	   double Min_height, double Max_height,
	   int Min_line, int Max_line, int Min_sample,
	   int Max_sample,
	   int Nline = 20, int Nsample = 20, int Nheight = 20,
	   bool Skip_masked_point = false,
	   bool Ignore_error = false);

//-----------------------------------------------------------------------
/// Initial guess to use when inverting the ground to image
/// function. Not sure if this belongs here or not, but for now we'll
/// try having this here.  Currently our initial guess is just the
/// middle of our X,Y range, but we can do something more intelligent
/// in the future if needed (e.g., a linear approximation to the
/// rational polynomial, or something like that.
//-----------------------------------------------------------------------
  
  virtual void initial_guess(double Line, double Sample, double Z,
		     double& X_guess, double& Y_guess) const
  { X_guess = x_offset_; Y_guess = y_offset_; }

//-----------------------------------------------------------------------
/// Initial guess at Z, if we need to find that also. This is just the
/// middle Z value.
//-----------------------------------------------------------------------

  virtual double initial_guess_z(double Line, double Sample) const
  { return z_offset_;}
  virtual blitz::Array<double, 2> image_coordinate_jacobian
  (double X, double Y, double Z) const;
  double line_offset() const {return line_offset_;}
  double line_scale() const {return line_scale_;}
  double sample_offset() const {return sample_offset_;}
  double sample_scale() const {return sample_scale_;}
  double x_offset() const {return x_offset_;}
  double x_scale() const {return x_scale_;}
  double y_offset() const {return y_offset_;}
  double y_scale() const {return y_scale_;}
  double z_offset() const {return z_offset_;}
  double z_scale() const {return z_scale_;}
  const RsmPolynomial& line_numerator() const {return line_num_;}
  const RsmPolynomial& line_denominator() const {return line_den_;}
  const RsmPolynomial& sample_numerator() const {return sample_num_;}
  const RsmPolynomial& sample_denominator() const {return sample_den_;}
private:
  double line_offset_, line_scale_, sample_offset_, sample_scale_,
    x_offset_, x_scale_, y_offset_, y_scale_, z_offset_, z_scale_;
  RsmPolynomial line_num_, line_den_, sample_num_, sample_den_;
  RsmRationalPolynomial() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmRationalPolynomial);
#endif


