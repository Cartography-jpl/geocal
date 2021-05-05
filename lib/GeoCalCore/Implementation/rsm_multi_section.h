#ifndef RSM_MULTI_SECTION_H
#define RSM_MULTI_SECTION_H
#include "rsm_base.h"
#include "rsm_low_order_polynomial.h"

namespace GeoCal {
/****************************************************************//**
  This is the base for some of the Rsm support classes. This allows
  us to use a RsmRationalPolynomial, RsmMultiSection, as RsmGrid 
  together.
*******************************************************************/

class RsmMultiSection : public RsmBase {
public:
  RsmMultiSection(int Nline, int Nsamp, int Nrow_section, int Ncol_section,
		  const RsmBase& Rsm_prototype, int Border=5,
		  const std::string& Image_identifier="",
		  const std::string& Rsm_support_data_edition="fake-1");
  virtual ~RsmMultiSection() {}
  virtual void print(std::ostream& Os) const;
  virtual boost::shared_ptr<RsmBase> clone() const
  { return boost::shared_ptr<RsmBase>
      (new RsmMultiSection(int(nline_sec * sec.rows()),
			   int(nsamp_sec * sec.cols()),
			   sec.rows(), sec.cols(), *sec(0,0), border_));
  }
  virtual ImageCoordinate image_coordinate(double X, double Y, double Z)
    const
  { return section_xyz(X,Y,Z)->image_coordinate(X,Y,Z);}
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
  (double X, double Y, double Z) const 
  { return section_xyz(X,Y,Z)->image_coordinate_jacobian(X,Y,Z);}
  
//-----------------------------------------------------------------------
/// Initial guess to use when inverting the ground to image
/// function. 
//-----------------------------------------------------------------------
  
  virtual void initial_guess(double Line, double Sample, double Z,
			     double& X_guess, double& Y_guess) const
  { section_ls(Line,Sample)->initial_guess(Line, Sample, Z, X_guess,
					   Y_guess);} 

//-----------------------------------------------------------------------
/// Initial guess at Z, if we need to find that also.
//-----------------------------------------------------------------------

  virtual double initial_guess_z(double Line, double Sample) const
  { return section_ls(Line,Sample)->initial_guess_z(Line, Sample); }
  
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
/// The number lines, samples, heights, and second_pass only applies to a
/// RsmRationalPolynomial. For a grid, we need to calculate the points
/// to fill the grid. But we give a common interface here, the values
/// are just ignored by RsmGrid.
//-----------------------------------------------------------------------

  virtual void fit(const ImageGroundConnection& Igc,
		   const CoordinateConverter& Cconv,
		   double Min_height, double Max_height,
		   int Min_line, int Max_line, int Min_sample,
		   int Max_sample);
  virtual int min_line() const {return lp.min_line();}
  virtual int max_line() const {return lp.max_line();}
  virtual int min_sample() const {return lp.min_sample();}
  virtual int max_sample() const {return lp.max_sample();}
  virtual double min_x() const;
  virtual double max_x() const;
  virtual double min_y() const;
  virtual double max_y() const;
  virtual double min_z() const;
  virtual double max_z() const;
  std::string tre_string() const;
  static boost::shared_ptr<RsmMultiSection>
  read_tre_string(const std::string& Tre_in);

  double number_line_per_section() const { return nline_sec;}
  double number_sample_per_section() const { return nsamp_sec;}
  int number_row_section() const { return sec.rows(); }
  int number_col_section() const { return sec.cols(); }
//-----------------------------------------------------------------------
/// Access a single section. This isn't something you normally want to
/// do, but is needed by the python Rsm TRE code because the
/// individual sections are separate TREs.
//-----------------------------------------------------------------------
  boost::shared_ptr<RsmBase> section(int i, int j) const
  { range_check(i, 0, sec.rows()); range_check(j, 0, sec.cols());
    return sec(i, j);
  }
  void section(int i, int j, const boost::shared_ptr<RsmBase>& V)
  { range_check(i, 0, sec.rows()); range_check(j, 0, sec.cols());
    sec(i, j) = V;
  }

  virtual const std::string& image_identifier() const
  { return image_identifier_;}
  virtual void image_identifier(const std::string& V)
  {
    image_identifier_ = V;
    for(int i = 0; i < sec.rows(); ++i)
      for(int j = 0; j < sec.cols(); ++j)
	sec(i, j)->image_identifier(V);
  }
  virtual const std::string& rsm_suport_data_edition() const
  { return rsm_suport_data_edition_;}
  virtual void rsm_suport_data_edition(const std::string& V)
  {
    rsm_suport_data_edition_ = V;
    for(int i = 0; i < sec.rows(); ++i)
      for(int j = 0; j < sec.cols(); ++j)
	sec(i, j)->rsm_suport_data_edition(V);
  }
  const boost::shared_ptr<RsmBase>&
  section_ls(double Line, double Sample) const;
  const boost::shared_ptr<RsmBase>&
  section_xyz(double X, double Y, double Z) const;
protected:
  RsmMultiSection() {}
private:
  int border_;
  double nline_sec, nsamp_sec;
  blitz::Array<boost::shared_ptr<RsmBase>, 2> sec;
  RsmLowOrderPolynomial lp;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmMultiSection);
#endif
