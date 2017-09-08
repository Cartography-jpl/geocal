#ifndef RSM_GRID_H
#define RSM_GRID_H
#include "rsm_base.h"

namespace GeoCal {
  class ImageGroundConnection;
  class CoordinateConverter;

/****************************************************************//**
  Use a interpolation grid to map from ground to image. Right now we
  only do a linear interpolation. The RSM documentation suggests doing
  Lagrange interpolation of various orders, we can implement that in the
  future.

  This can contain missing data, either because it is outside of the
  image or because the initial ImageGroundConnection failed for a
  particular point. We record these as NaNs.

  Right now this just carries through, we should probably add some
  kind of a status for image_coordinate.

  We plan on using the same grid for a full ground to image map, or
  for doing a correction.
*******************************************************************/

class RsmGrid : public RsmBase {
public:
  RsmGrid(int N_x, int N_y, int N_z)
    : line_(N_x, N_y, N_z),
      sample_(N_x, N_y, N_z)
  {}
  virtual ~RsmGrid() {}
  virtual void print(std::ostream& Os) const;
  virtual boost::shared_ptr<RsmBase> clone() const
  { return boost::shared_ptr<RsmBase>
      (new RsmGrid(number_x(), number_y(), number_z()));
  }
  virtual ImageCoordinate image_coordinate(double X, double Y, double Z)
    const;
  virtual blitz::Array<double, 2> image_coordinate
  (const blitz::Array<double, 1>& X, const blitz::Array<double, 1>& Y,
   const blitz::Array<double, 1>& Z) const;
  virtual blitz::Array<double, 3> image_coordinate
  (const blitz::Array<double, 2>& X, const blitz::Array<double, 2>& Y,
   const blitz::Array<double, 2>& Z) const;
  virtual blitz::Array<double, 4> image_coordinate
  (const blitz::Array<double, 3>& X, const blitz::Array<double, 3>& Y,
   const blitz::Array<double, 3>& Z) const;

  virtual blitz::Array<double, 2> image_coordinate_jacobian
  (double X, double Y, double Z) const;
  virtual void initial_guess(double Line, double Sample, double Z,
			     double& X_guess, double& Y_guess) const
  { X_guess = x_start() + x_delta() * number_x() / 2.0;
    Y_guess = y_start() + y_delta() * number_y() / 2.0;
  }
  virtual double initial_guess_z(double Line, double Sample) const
  {
    return z_start() + z_delta() * number_z() / 2.0;
  }
  virtual void fit(const ImageGroundConnection& Igc,
		   const CoordinateConverter& Cconv,
		   double Min_height, double Max_height,
		   int Min_line, int Max_line, int Min_sample,
		   int Max_sample,
		   int Nline = 20, int Nsample = 20, int Nheight = 20,
		   bool Skip_masked_point = false,
		   bool Ignore_error = false);
  void fit_corr(const ImageGroundConnection& IGc,
		const CoordinateConverter& Cconv,
		const RsmBase& Rb);

//-----------------------------------------------------------------------
/// Number of X values in grid
//-----------------------------------------------------------------------
  
  int number_x() const { return line_.rows(); }

//-----------------------------------------------------------------------
/// Number of Y values in grid
//-----------------------------------------------------------------------
  
  int number_y() const { return line_.cols(); }

//-----------------------------------------------------------------------
/// Number of Z values in grid
//-----------------------------------------------------------------------
  
  int number_z() const { return line_.depth(); }

//-----------------------------------------------------------------------
/// First X value in grid.
//-----------------------------------------------------------------------

  double x_start() const {return x_start_;}

//-----------------------------------------------------------------------
/// First Y value in grid.
//-----------------------------------------------------------------------

  double y_start() const {return y_start_;}

//-----------------------------------------------------------------------
/// First Z value in grid.
//-----------------------------------------------------------------------

  double z_start() const {return z_start_;}

//-----------------------------------------------------------------------
/// Spacing of X in grid.
//-----------------------------------------------------------------------

  double x_delta() const {return x_delta_;}

//-----------------------------------------------------------------------
/// Spacing of Y in grid.
//-----------------------------------------------------------------------

  double y_delta() const {return y_delta_;}

//-----------------------------------------------------------------------
/// Spacing of Z in grid.
//-----------------------------------------------------------------------

  double z_delta() const {return z_delta_;}

  virtual int min_line() const {return min_line_;}
  virtual int max_line() const {return max_line_;}
  virtual int min_sample() const {return min_sample_;}
  virtual int max_sample() const {return max_sample_;}
  virtual double min_x() const {return x_start();}
  virtual double max_x() const {return x_start()+x_delta()*number_x();}
  virtual double min_y() const {return y_start();}
  virtual double max_y() const {return y_start()+y_delta()*number_y();}
  virtual double min_z() const {return z_start();}
  virtual double max_z() const {return z_start()+z_delta()*number_z();}
private:
  blitz::Array<double, 3> line_, sample_;
  double x_start_, y_start_, z_start_, x_delta_, y_delta_, z_delta_;
  int min_line_,max_line_,min_sample_,max_sample_;
  RsmGrid() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmGrid);
#endif
