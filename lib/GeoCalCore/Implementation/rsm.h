#ifndef RSM_H
#define RSM_H
#include "rsm_base.h"
#include "rsm_id.h"
#include "rsm_direct_covariance.h"
#include "rsm_indirect_covariance.h"
#include "rsm_adjustable_parameter.h"
#include "coordinate_converter.h"
#include <deque>
#include <boost/array.hpp>

namespace GeoCal {
  class ImageGroundConnection; // Forward declaration.
/****************************************************************//**
  This class is a Replacement Sensor Model (see Dolloff, J.T.,
  M.M. Iiyama, and C.R. Taylor, 2008. The Replacement Sensor Model
  (RSM): Overview, Status, and Performance Summary, ASPRS 2008 Annual
  Conference, April 28 - May 2, 2008)

  This is the top level class which provides access to mapping from 
  GroundCoordinate to ImageCoordinate and vice versa.

  Note that the same ambiguity mentioned in the Rpc comments exist
  here, it isn't clear the exact definition of row/sample in the
  standard. For the Rpc, I'm *fairly* sure that it matches our
  ImageCoordinate convention - (0,0) is the center of the upper left
  corner pixel.
*******************************************************************/

class Rsm : public WithParameterNested, public Printable<Rsm> {
public:
  Rsm(const boost::shared_ptr<RsmId>& Rsm_id)
    : rid(Rsm_id) {}
  Rsm(const boost::shared_ptr<RsmBase>& Rp,
      const boost::shared_ptr<CoordinateConverter>& Cconv);
  virtual ~Rsm() {}

  boost::shared_ptr<GroundCoordinate> ground_coordinate(const ImageCoordinate& Ic, const Dem& D) const;
  boost::shared_ptr<GroundCoordinate> ground_intersection(const ImageCoordinate& Ic, const Dem& D, double Z_accuracy=1e-2) const;
  boost::shared_ptr<GroundCoordinate> polish_intersection(const ImageCoordinate& Ic, const Dem& D, const GroundCoordinate& Surfp, double Z_accuracy=1e-2) const;
  boost::shared_ptr<GroundCoordinate> ground_coordinate_z(const ImageCoordinate& Ic, double Z) const;
  boost::shared_ptr<GroundCoordinate> ground_coordinate_approx_height(const ImageCoordinate& Ic, double H) const;
  void image_coordinate(const GroundCoordinate& Gc, ImageCoordinate& Res,
			bool& In_valid_range) const;
  void image_coordinate(double X, double Y, double Z, ImageCoordinate& Res,
			bool& In_valid_range) const;
  blitz::Array<double, 2> image_coordinate_jacobian
  (double X, double Y, double Z) const;
  blitz::Array<double, 2> 
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const;
  void fit(const ImageGroundConnection& Igc, double Min_height,
	   double Max_height);
  void fit_data(const blitz::Array<double, 2>& Data,
		int Nline, int Nsamp) const;
  void fill_in_ground_domain_vertex(const ImageGroundConnection& Igc,
				    double Min_height, double Max_height);
  void fill_in_ground_domain_vertex();
  blitz::Array<double, 2> mapping_matrix(const ImageGroundConnection& Igc,
					 double Min_height, double Max_height,
					 int Nline_fit = 20,
					 int Nsample_fit = 20,
					 int Nheight_fit = 20,
					 bool Ignore_igc_error_in_fit = false);
  void print(std::ostream& Os) const;

  const boost::shared_ptr<RsmId>& rsm_id() const {return rid;}
  const boost::shared_ptr<RsmDirectCovariance>& rsm_direct_covariance() const
  {return rdcov;}
  void rsm_direct_covariance(const boost::shared_ptr<RsmDirectCovariance>& V)
  {rdcov = V;}
  const boost::shared_ptr<RsmIndirectCovariance>&
  rsm_indirect_covariance() const {return ricov;}
  void rsm_indirect_covariance
  (const boost::shared_ptr<RsmIndirectCovariance>& V) {ricov = V;}
  const boost::shared_ptr<RsmAdjustableParameter>&
  rsm_adjustable_parameter() const {return rparm;}
  void rsm_adjustable_parameter
  (const boost::shared_ptr<RsmAdjustableParameter>&V)
  {
    clear_object();
    rparm = V;
    if(rparm)
      add_object(rparm);
  }
  blitz::Array<double, 2> generate_data(const ImageGroundConnection& Igc,
	double Min_height, double Max_height,
        int Nline_fit=20, int Nsample_fit=20, int Nheight_fit=5,
	int Nsecond_pass_fit=20, 					
        int Min_line=0, int Max_line=-1, int Min_sample=0,
	int Max_sample=-1, bool Ignore_igc_error = true) const;
  const boost::shared_ptr<RsmBase>& rsm_base() const {return rp;}
  void rsm_base(const boost::shared_ptr<RsmBase>& V) { rp = V;}
  const boost::shared_ptr<CoordinateConverter>& coordinate_converter() const
  { return rid->coordinate_converter(); }
  void coordinate_converter(const boost::shared_ptr<CoordinateConverter>& V) 
  { rid->coordinate_converter(V); }

//-----------------------------------------------------------------------
/// Check for zero crossing in the denominator, i.e., do we have poles
/// in our fit.
//-----------------------------------------------------------------------

  bool check_zero_crossing(double Grid_spacing = 0.01) const
  {
    return rp->check_zero_crossing(Grid_spacing);
  }

  blitz::Array<double, 2> compare_data(const blitz::Array<double, 2>& Data)
    const;
  blitz::Array<double, 1> compare_data_dist(const blitz::Array<double, 2>& Data)
    const;
  
  void compare_igc(const ImageGroundConnection& Igc, int Number_line_spacing,
		   int Number_sample_spacing, double Height,
		   blitz::Array<double, 2>& True_line,
		   blitz::Array<double, 2>& True_sample,
		   blitz::Array<double, 2>& Calc_line,
		   blitz::Array<double, 2>& Calc_sample,
		   blitz::Array<double, 2>& Distance_true_vs_calc)
    const;
  virtual const std::string& image_identifier() const
  { return rid->image_identifier();}
  virtual void image_identifier(const std::string& V)
  {
    if(rp)
      rp->image_identifier(V);
    if(rid)
      rid->image_identifier(V);
    if(rdcov)
      rdcov->image_identifier(V);
    if(ricov)
      ricov->image_identifier(V);
    if(rparm)
      rparm->image_identifier(V);
  }
  virtual const std::string& rsm_support_data_edition() const
  { return rid->rsm_support_data_edition();}
  virtual void rsm_support_data_edition(const std::string& V)
  {
    if(rp)
      rp->rsm_support_data_edition(V);
    if(rid)
      rid->rsm_support_data_edition(V);
    if(rdcov)
      rdcov->rsm_support_data_edition(V);
    if(ricov)
      ricov->rsm_support_data_edition(V);
    if(rparm)
      rparm->rsm_support_data_edition(V);
  }

//-----------------------------------------------------------------------
/// Return the NAIF code for the planet/body we are working with.
//-----------------------------------------------------------------------
  
  int naif_code() const { return rid->naif_code(); }

//-----------------------------------------------------------------------
/// Set the NAIF code for the planet/body we are working with.
///
/// Note that the NITF TRE structure does not have a place to store
/// the NAIF code, it implicitly assumes earth. So when we read a TRE,
/// even for something like Mars, we have the NAIF code set to
/// earth. We need to update this with other metadata
/// (e.g. TARGET_NAME in PDS label).
///
/// This is not a problem for boost serialization (which keeps the
/// NAIF code), just for NITF TRE.
//-----------------------------------------------------------------------
  
  void naif_code(int Naif_code)
  {
    rid->naif_code(Naif_code);
    if(rparm)
      rparm->naif_code(Naif_code);
  }
  
protected:
  Rsm() {}
private:
  boost::shared_ptr<RsmBase> rp;
  boost::shared_ptr<RsmId> rid;
  boost::shared_ptr<RsmDirectCovariance> rdcov;
  boost::shared_ptr<RsmIndirectCovariance> ricov;
  boost::shared_ptr<RsmAdjustableParameter> rparm;
  // *Important* This is not actually used anymore. We keep this as a
  // scratch variable for reading the old format of the data. Instead,
  // the coordinate converter is stored in RsmId.
  boost::shared_ptr<CoordinateConverter> cconv_not_used;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
  template<class Archive>
  void save(Archive & ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(Rsm);
GEOCAL_CLASS_VERSION(Rsm, 1);
#endif

