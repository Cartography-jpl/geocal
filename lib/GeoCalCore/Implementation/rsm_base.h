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
  RsmBase(const std::string& Image_identifier="",
	  const std::string& Rsm_support_data_edition="fake-1")
    : image_identifier_(Image_identifier),
      rsm_support_data_edition_(Rsm_support_data_edition){}
  virtual ~RsmBase() {}

//-----------------------------------------------------------------------
/// Check for zero crossing in the denominator, i.e., do we have poles
/// in our fit.
//-----------------------------------------------------------------------
  
  virtual bool check_zero_crossing(double Grid_spacing = 0.01) const = 0;
  
  virtual void print(std::ostream& Os) const
  { Os << "RsmBase\n"; }
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
/// Row section number. This only makes sense for
/// RsmRationalPolynomial and RsmGrid, but we want to be able to set
/// this through a general RsmBase. The default returns 1 and throws
/// an error if you try to set it, but derived classes override this.
//-----------------------------------------------------------------------
  virtual int row_section_number() const { return 1; }
  virtual void row_section_number(int UNUSED(V))
  { throw Exception("Not Implemented"); }

//-----------------------------------------------------------------------
/// Col section number. This only makes sense for
/// RsmRationalPolynomial and RsmGrid, but we want to be able to set
/// this through a general RsmBase. The default returns 1 and throws
/// an error if you try to set it, but derived classes override this.
//-----------------------------------------------------------------------
  virtual int col_section_number() const { return 1; }
  virtual void col_section_number(int UNUSED(V))
  { throw Exception("Not Implemented"); }
  
//-----------------------------------------------------------------------
/// Image identification.
//-----------------------------------------------------------------------
  virtual const std::string& image_identifier() const
  { return image_identifier_;}
  virtual void image_identifier(const std::string& V)
  { image_identifier_ = V;}

//-----------------------------------------------------------------------
/// RSM Support Data Edition.
//-----------------------------------------------------------------------
  virtual const std::string& rsm_support_data_edition() const
  { return rsm_support_data_edition_;}
  virtual void rsm_support_data_edition(const std::string& V)
  { rsm_support_data_edition_ = V; }
  
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
/// To support sections, you can pass in a restricted number of
/// line/samples to fit over.
//-----------------------------------------------------------------------

  virtual void fit(const ImageGroundConnection& Igc,
		   const CoordinateConverter& Cconv,
		   double Min_height, double Max_height,
		   int Min_line, int Max_line, int Min_sample,
		   int Max_sample) = 0;

//-----------------------------------------------------------------------
/// Variation of fit that takes data generated by generate_data. This
/// is intended for use with adaptive strategies such as varying the
/// number of sections to meet a certain tolerance. Note that this
/// does *not* call fill_in_ground_domain_vertex (since we don't have
/// the Igc in this call). You should call
/// fill_in_ground_domain_vertex separately once you are happy with
/// the fit.
//-----------------------------------------------------------------------

  virtual void fit_data(const blitz::Array<double, 2>& Data,
			int Min_line, int Max_line, int Min_sample,
			int Max_sample) = 0;

// Doesn't really belong here, but stick here for now
  static blitz::Array<double, 4> generate_data
  (const ImageGroundConnection& Igc,
   const CoordinateConverter& Cconv,
   double Min_x, double Max_x,
   double Min_y, double Max_y,
   double Min_z, double Max_z,
   int Numx, int Numy, int Numz);
  // Note unlike a lot of ranges, the valid line is min_line() <= line
  // <= max_line(). This is just the convention the RSM uses. Same
  // with all the other ranges, they are inclusive on both ends.
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
  std::string base_tre_string() const;
  void base_read_tre_string(std::istream& In);

//-----------------------------------------------------------------------
/// Based on examples, the MSP library seems to prefer that all
/// sections have the same z start and delta. This isn't actually
/// required from the RSM standard, but seems to be one of the those
/// "unstated" requirements. This gives access  for RsmMultiSection to
/// pass the z values to use, rather than computing this in fit.
//-----------------------------------------------------------------------

  virtual void set_z_range(double Z_min, double Z_max)  { } 
protected:
  std::string image_identifier_, rsm_support_data_edition_;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
  template<class Archive>
  void save(Archive & ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(RsmBase);
GEOCAL_CLASS_VERSION(RsmBase, 1);
#endif
