#ifndef RSM_RP_PLUS_GRID_H
#define RSM_RP_PLUS_GRID_H
#include "rsm_grid.h"
#include "rsm_rational_polynomial.h"

namespace GeoCal {
  class ImageGroundConnection;
  class CoordinateConverter;

/****************************************************************//**
  This uses a RsmRationalPolynomial plus a RsmGrid for correcting it.
*******************************************************************/

class RsmRpPlusGrid : public RsmBase {
public:
  RsmRpPlusGrid(const boost::shared_ptr<RsmBase>& Rp,
		const boost::shared_ptr<RsmGrid>& Rgrid)
    : rp(Rp),
      rgrid(Rgrid)
  {}
  virtual ~RsmRpPlusGrid() {}
  virtual void print(std::ostream& Os) const;
  virtual boost::shared_ptr<RsmBase> clone() const
  { return boost::shared_ptr<RsmBase>
      (new RsmRpPlusGrid(rp->clone(), boost::dynamic_pointer_cast<RsmGrid>(rgrid->clone())));
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
  { rp->initial_guess(Line, Sample, Z, X_guess, Y_guess); }
  virtual double initial_guess_z(double Line, double Sample) const
  { return rp->initial_guess_z(Line, Sample); }
  virtual void fit(const ImageGroundConnection& Igc,
		   const CoordinateConverter& Cconv,
		   double Min_height, double Max_height,
		   int Min_line, int Max_line, int Min_sample,
		   int Max_sample);
  virtual void fit_data(const blitz::Array<double, 2>& Data,
			int Min_line, int Max_line, int Min_sample,
			int Max_sample)
  { throw Exception("Not implemented"); }

//-----------------------------------------------------------------------
/// RsmRationalPolynomial we are using
//-----------------------------------------------------------------------

  const boost::shared_ptr<RsmBase>& rational_polynomial() const
  { return rp;}

//-----------------------------------------------------------------------
/// RsmGrid we are using to correction rational_polynomial.
//-----------------------------------------------------------------------

  const boost::shared_ptr<RsmGrid>& correction_grid() const
  { return rgrid;}

  virtual const std::string& image_identifier() const
  { return image_identifier_;}
  virtual void image_identifier(const std::string& V)
  { image_identifier_ = V;
    rp->image_identifier(V);
    rgrid->image_identifier(V);
  }
  virtual const std::string& rsm_support_data_edition() const
  { return rsm_support_data_edition_;}
  virtual void rsm_support_data_edition(const std::string& V)
  {
    rsm_support_data_edition_ = V;
    rp->rsm_support_data_edition(V);
    rgrid->rsm_support_data_edition(V);
  }
  virtual bool check_zero_crossing(double Grid_spacing = 0.01) const
  {
    return rp->check_zero_crossing(Grid_spacing) ||
      rgrid->check_zero_crossing(Grid_spacing);
  }
  virtual int min_line() const {return rp->min_line();}
  virtual int max_line() const {return rp->max_line();}
  virtual int min_sample() const {return rp->min_sample(); }
  virtual int max_sample() const {return rp->max_sample(); }
  virtual double min_x() const {return rp->min_x(); }
  virtual double max_x() const {return rp->max_x(); }
  virtual double min_y() const {return rp->min_y(); }
  virtual double max_y() const {return rp->max_y(); }
  virtual double min_z() const {return rp->min_z(); }
  virtual double max_z() const {return rp->max_z(); }
protected:
  RsmRpPlusGrid() {}
private:
  boost::shared_ptr<RsmBase> rp;
  boost::shared_ptr<RsmGrid> rgrid;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmRpPlusGrid);
#endif
