#ifndef RSM_H
#define RSM_H
#include "rsm_base.h"
#include "rsm_id.h"
#include "rsm_direct_covariance.h"
#include "rsm_indirect_covariance.h"
#include "rsm_adjustable_parameter.h"
#include "coordinate_converter.h"
#include "image_ground_connection.h"

namespace GeoCal {
/****************************************************************//**
  This class is a Replacement Sensor Model (see Dolloff, J.T.,
  M.M. Iiyama, and C.R. Taylor, 2008. The Replacement Sensor Model
  (RSM): Overview, Status, and Performance Summary, ASPRS 2008 Annual
  Conference, April 28 - May 2, 2008)

  This is the top level class which provides access to mapping from 
  GroundCoordinate to ImageCoordinate and vice versa.

  Note that this class will likely be changed/updated in the future,
  this is mostly a placeholder for now.
*******************************************************************/

class Rsm : public WithParameterNested, public Printable<Rsm> {
public:
  Rsm(const boost::shared_ptr<RsmId>& Rsm_id)
    : rid(Rsm_id) {}
  Rsm(const boost::shared_ptr<RsmBase>& Rp,
      const boost::shared_ptr<CoordinateConverter>& Cconv);
  virtual ~Rsm() {}

  boost::shared_ptr<GroundCoordinate> ground_coordinate(const ImageCoordinate& Ic, const Dem& D) const;
  boost::shared_ptr<GroundCoordinate> ground_coordinate(const ImageCoordinate& Ic, double Z) const;
  boost::shared_ptr<GroundCoordinate> ground_coordinate_approx_height(const ImageCoordinate& Ic, double H) const;
  ImageCoordinate image_coordinate(const GroundCoordinate& Gc) const;
  ImageCoordinate image_coordinate(double X, double Y, double Z) const;
  blitz::Array<double, 2> image_coordinate_jacobian
  (double X, double Y, double Z) const;

//-----------------------------------------------------------------------
/// Return the Jacobian of the image coordinates with respect to the
/// parameters (what we have is RsmAdjustableParameter object)
//-----------------------------------------------------------------------

  blitz::Array<double, 2> 
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const
  {// For now, no parameters.
    return blitz::Array<double, 2>(2, 0);
  }

  void fit(const ImageGroundConnection& Igc, double Min_height,
	   double Max_height);
  void fill_in_ground_domain_vertex(double Min_height, double Max_height);
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
  const boost::shared_ptr<RsmBase>& rsm_base() const {return rp;}
  void rsm_base(const boost::shared_ptr<RsmBase>& V) { rp = V;}
  const boost::shared_ptr<CoordinateConverter>& coordinate_converter() const
  { return rid->coordinate_converter(); }
  void coordinate_converter(const boost::shared_ptr<CoordinateConverter>& V) 
  { rid->coordinate_converter(V); }
  void compare_igc(const ImageGroundConnection& Igc, int Number_line_spacing,
		   int Number_sample_spacing, double Height,
		   blitz::Array<double, 2>& True_line,
		   blitz::Array<double, 2>& True_sample,
		   blitz::Array<double, 2>& Calc_line,
		   blitz::Array<double, 2>& Calc_sample)
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
  virtual const std::string& rsm_suport_data_edition() const
  { return rid->rsm_suport_data_edition();}
  virtual void rsm_suport_data_edition(const std::string& V)
  {
    if(rp)
      rp->rsm_suport_data_edition(V);
    if(rid)
      rid->rsm_suport_data_edition(V);
    if(rdcov)
      rdcov->rsm_suport_data_edition(V);
    if(ricov)
      ricov->rsm_suport_data_edition(V);
    if(rparm)
      rparm->rsm_suport_data_edition(V);
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

