#ifndef RSM_LOW_ORDER_POLYNOMIAL_H
#define RSM_LOW_ORDER_POLYNOMIAL_H
#include "printable.h"
#include "image_coordinate.h"

namespace GeoCal {
  class ImageGroundConnection;
  class CoordinateConverter;

/****************************************************************//**
  This is the low order polynomial used to determine approximate 
  row/column (line/sample, the RSM documentation calls this Row/Column)
*******************************************************************/

class RsmLowOrderPolynomial : public Printable<RsmLowOrderPolynomial> {
public:
//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  RsmLowOrderPolynomial() {}
  virtual ~RsmLowOrderPolynomial() {}

//-----------------------------------------------------------------------
/// Return approximate image coordinates for given X, Y, and Z.
//-----------------------------------------------------------------------
  
  ImageCoordinate image_coordinate(double X, double Y, double Z) const
  {
    ImageCoordinate ic;
    ic.line =  pline[0] +
      X * (pline[1] + pline[4] * X + pline[5] * Y + pline[6] * Z) +
      Y * (pline[2] + pline[7] * Y + pline[8] * Z) +
      Z * (pline[3] + pline[9] * Z);
    ic.sample =  psamp[0] +
      X * (psamp[1] + psamp[4] * X + psamp[5] * Y + psamp[6] * Z) +
      Y * (psamp[2] + psamp[7] * Y + psamp[8] * Z) +
      Z * (psamp[3] + psamp[9] * Z);
    return ic;
  }
  blitz::Array<double, 2> image_coordinate(const blitz::Array<double, 1>& X,
     const blitz::Array<double, 1>& Y, const blitz::Array<double, 1>& Z) const;
  blitz::Array<double, 3> image_coordinate(const blitz::Array<double, 2>& X,
     const blitz::Array<double, 2>& Y, const blitz::Array<double, 2>& Z) const;
  blitz::Array<double, 4> image_coordinate(const blitz::Array<double, 3>& X,
     const blitz::Array<double, 3>& Y, const blitz::Array<double, 3>& Z) const;
  void fit_data(const std::vector<double>& Line,
		const std::vector<double>& Sample,
		const std::vector<double>& X,
		const std::vector<double>& Y,
		const std::vector<double>& Z);
  void fit(const ImageGroundConnection& Igc,
	   const CoordinateConverter& Cconv,
	   double Min_height, double Max_height,
	   int Min_line, int Max_line, int Min_sample,
	   int Max_sample,
	   int Nline = 10, int Nsample = 10, int Nheight = 10,
	   bool Skip_masked_point = false,
	   bool Ignore_error = false);
  blitz::Array<double, 1> parameter_line() const
  { return blitz::Array<double, 1>(const_cast<double*>(pline), blitz::shape(10), blitz::neverDeleteData); }
  blitz::Array<double, 1> parameter_sample() const
  { return blitz::Array<double, 1>(const_cast<double*>(psamp), blitz::shape(10), blitz::neverDeleteData); }
  void print(std::ostream& Os) const
  { Os << "RsmLowOrderPolynomial"; }
  int min_line() const {return min_line_;}
  int max_line() const {return max_line_;}
  int min_sample() const {return min_sample_;}
  int max_sample() const {return max_sample_;}
private:
  double pline[10], psamp[10];
  int min_line_, max_line_, min_sample_, max_sample_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmLowOrderPolynomial);
#endif

