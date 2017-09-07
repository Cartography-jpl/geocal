#ifndef RSM_BASE_H
#define RSM_BASE_H
#include "image_coordinate.h"
#include <boost/shared_ptr.hpp>

namespace GeoCal {
  class ImageGroundConnection;
  class CoordinateConverter;

/****************************************************************//**
  This is the base for some of the Rsm support classes. This allows
  us to use a RsmRationalPolynomial, RsmMultiSection, as RsmGrid 
  together.
*******************************************************************/

class RsmBase : public Printable<RsmBase> {
public:
  RsmBase() {}
  virtual ~RsmBase() {}
  virtual void print(std::ostream& Os) const
  { Os << "RsmBase"; }
  virtual boost::shared_ptr<RsmBase> clone() const = 0;
  virtual ImageCoordinate image_coordinate(double X, double Y, double Z)
    const = 0;
  virtual blitz::Array<double, 2> image_coordinate
  (const blitz::Array<double, 1>& X, const blitz::Array<double, 1>& Y,
   const blitz::Array<double, 1>& Z) const = 0;
  virtual blitz::Array<double, 3> image_coordinate
  (const blitz::Array<double, 2>& X, const blitz::Array<double, 2>& Y,
   const blitz::Array<double, 2>& Z) const = 0;
  virtual blitz::Array<double, 4> image_coordinate
  (const blitz::Array<double, 3>& X, const blitz::Array<double, 3>& Y,
   const blitz::Array<double, 3>& Z) const = 0;

  virtual blitz::Array<double, 2> image_coordinate_jacobian
  (double X, double Y, double Z) const = 0;
  
//-----------------------------------------------------------------------
/// Initial guess to use when inverting the ground to image
/// function. 
//-----------------------------------------------------------------------
  
  virtual void initial_guess(double Line, double Sample, double Z,
			     double& X_guess, double& Y_guess) const = 0;

//-----------------------------------------------------------------------
/// Initial guess at Z, if we need to find that also.
//-----------------------------------------------------------------------

  virtual double initial_guess_z(double Line, double Sample) const = 0;
  
//-----------------------------------------------------------------------
/// Fit that approximates the calculation done by a ImageGroundConnection.
///
/// This routine always ignores ImageGroundConnectionFailed
/// exceptions, and just skips to the next point. But if we are using
/// python code for the ImageGroundConnection we can't translate
/// errors to ImageGroundConnectionFailed (this is a limitation of
/// SWIG). So you can optionally specify Ignore_error as true, in
/// which case we ignore *all* exceptions and just skip to the next
/// point.
///
/// We normally look at all image points when generating the
/// RsmRationalPolynomial. You can optionally specify
/// Skip_masked_point to skip all image points that are masked.
///
/// To support sections, you can pass in a restricted number of
/// line/samples to fit over.
///
/// The number lines, samples, and heights only applies to a
/// RsmRationalPolynomial. For a grid, we need to calculate the points
/// to fill the grid. But we give a common interface here, the values
/// are just ignored by RsmGrid.
//-----------------------------------------------------------------------

  virtual void fit(const ImageGroundConnection& Igc,
		   const CoordinateConverter& Cconv,
		   double Min_height, double Max_height,
		   int Min_line, int Max_line, int Min_sample,
		   int Max_sample,
		   int Nline = 20, int Nsample = 20, int Nheight = 20,
		   bool Skip_masked_point = false,
		   bool Ignore_error = false) = 0;

// Doesn't really belong here, but stick here for now
  static blitz::Array<double, 4> generate_data
  (const ImageGroundConnection& Igc,
   const CoordinateConverter& Cconv,
   double Min_x, double Max_x,
   double Min_y, double Max_y,
   double Min_z, double Max_z,
   int Numx, int Numy, int Numz);
  virtual int min_line() const = 0;
  virtual int max_line() const = 0;
  virtual int min_sample() const = 0;
  virtual int max_sample() const = 0;
  virtual double min_x() const = 0;
  virtual double max_x() const = 0;
  virtual double min_y() const = 0;
  virtual double max_y() const = 0;
  virtual double min_z() const = 0;
  virtual double max_z() const = 0;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmBase);
#endif
