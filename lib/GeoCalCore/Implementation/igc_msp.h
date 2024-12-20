#ifndef IGC_MSP_H
#define IGC_MSP_H
#include "image_ground_connection.h"
#include "simple_dem.h"

namespace GeoCal {
bool have_msp_supported();
  
/****************************************************************//**
  Exception thrown if MSP isn't available.
*******************************************************************/

class MspNotAvailableException : public Exception {
public:
//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

  MspNotAvailableException() : Exception("Geocal library wasn't configured to use MSP library during installation, so can't call MSP functions.")
  { }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~MspNotAvailableException() throw() {}
};

/****************************************************************//**
  This is an ImageGroundConnection that uses the MSP (Mensuration
  Service Program) library to implement an ImageGroundConnection. This
  uses a set of plugins for the CSM (Community Sensor Model, see 
  https://github.com/sminster/csm) to support various sensor models.
  In particular, this supports various NITF formats such as RPC, RSM,
  SENSRB, GLAS/GFM.

  Note this goes from the CSM library to our ImageGroundConnection
  interface. We may well write a plugin to go the other way at some
  point, so software that uses the CSM library can use GeoCal
  ImageGroundConnection objects.
*******************************************************************/

class IgcMsp: public virtual ImageGroundConnectionCopy {
public:
  IgcMsp(const std::string& Fname,
     const boost::shared_ptr<Dem>& D = boost::shared_ptr<Dem>(new SimpleDem()),
	 int Image_index = 0);
  IgcMsp(const std::string& Fname, const boost::shared_ptr<Dem>& D,
	 int Image_index,
	 const std::string& Plugin_name, const std::string& Model_name);
  virtual ~IgcMsp() {}
  blitz::Array<double, 2> covariance() const;
  blitz::Array<double, 2> joint_covariance(const IgcMsp& igc2) const;
  static void msp_print_plugin_list();
  static std::vector<std::string> msp_plugin_list();
  static void msp_register_plugin(const std::string& Plugin_name);
  static void msp_init();
  static std::vector<std::string> msp_model_list(const std::string& Plugin);
  static std::vector<std::string> image_ids(const std::string& Fname);
  std::string family() const;
  std::string version() const;
  std::string pedigree() const;
  std::string model_name() const;
  std::string image_identifer() const;
  std::string sensor_identifer() const;
  std::string platform_identifer() const;
  std::string collection_identifer() const;
  std::string trajectory_identifer() const;
  std::string sensor_type() const;
  std::string sensor_mode() const;
  std::string reference_date_time() const;
  std::string file_name() const;
  int image_index() const;
  virtual void print(std::ostream& Os) const;
  blitz::Array<double, 1> sensor_velocity(const ImageCoordinate& Ic) const;
  std::string generate_rsm_tre(const std::string& Report = "",
			       const std::string& Rsm_config = "") const;
  void ground_coordinate_with_cov(const ImageCoordinate& Ic,
				  const blitz::Array<double, 2>& Ic_cov,
				  double H,
				  double H_var,
				  boost::shared_ptr<GroundCoordinate>& Gp,
				  blitz::Array<double, 2>& Gp_cov) const;
  void ce90_le90(const ImageCoordinate& Ic, double H,
		 double& Ce90, double& Le90) const;
private:
  static void* lib_ptr;
  IgcMsp() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(IgcMsp);
#endif
