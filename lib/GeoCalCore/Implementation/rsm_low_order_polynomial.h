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

  RsmLowOrderPolynomial(int Nline_fit=10, int Nsample_fit=10,
			int Nheight_fit=10,
			bool Ignore_igc_error_in_fit = false)
    : nline_fit_(Nline_fit),
      nsample_fit_(Nsample_fit),
      nheight_fit_(Nheight_fit),
      ignore_igc_error_in_fit_(Ignore_igc_error_in_fit) {}
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
	   int Max_sample);
  blitz::Array<double, 1> parameter_line() const
  { return blitz::Array<double, 1>(const_cast<double*>(pline), blitz::shape(10), blitz::neverDeleteData); }
  blitz::Array<double, 1> parameter_sample() const
  { return blitz::Array<double, 1>(const_cast<double*>(psamp), blitz::shape(10), blitz::neverDeleteData); }
  void print(std::ostream& Os) const
  { Os << "RsmLowOrderPolynomial\n"; }
  int min_line() const {return min_line_;}
  void min_line(int V) { min_line_ = V;}
  int max_line() const {return max_line_;}
  void max_line(int V) { max_line_ = V;}
  int min_sample() const {return min_sample_;}
  void min_sample(int V) { min_sample_ = V;}
  int max_sample() const {return max_sample_;}
  void max_sample(int V) { max_sample_ = V;}
  
//-----------------------------------------------------------------------
/// Number of lines in the grid we fit for.
//-----------------------------------------------------------------------

  int number_line_fit() const {return nline_fit_;}

//-----------------------------------------------------------------------
/// Number of lines in the grid we fit for.
//-----------------------------------------------------------------------

  void number_line_fit(int V) { nline_fit_ = V;}

//-----------------------------------------------------------------------
/// Number of samples in the grid we fit for.
//-----------------------------------------------------------------------

  int number_sample_fit() const {return nsample_fit_;}

//-----------------------------------------------------------------------
/// Number of samples in the grid we fit for.
//-----------------------------------------------------------------------

  void number_sample_fit(int V) { nsample_fit_ = V;}
  
//-----------------------------------------------------------------------
/// Number of heights in the grid we fit for.
//-----------------------------------------------------------------------

  int number_height_fit() const {return nheight_fit_;}

//-----------------------------------------------------------------------
/// Number of heights in the grid we fit for.
//-----------------------------------------------------------------------

  void number_height_fit(int V) { nheight_fit_ = V;}

//-----------------------------------------------------------------------
/// If true, ignore igc errors in fit.
//-----------------------------------------------------------------------
  
  bool ignore_igc_error_in_fit() const { return ignore_igc_error_in_fit_;}

//-----------------------------------------------------------------------
/// If true, ignore igc errors in fit.
//-----------------------------------------------------------------------
  
  void ignore_igc_error_in_fit(bool V) { ignore_igc_error_in_fit_ = V;}
  std::string tre_string() const;
  void read_tre_string(std::istream& In);
private:
  double pline[10], psamp[10];
  int min_line_, max_line_, min_sample_, max_sample_;
  int nline_fit_, nsample_fit_, nheight_fit_;
  bool ignore_igc_error_in_fit_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmLowOrderPolynomial);
#endif

