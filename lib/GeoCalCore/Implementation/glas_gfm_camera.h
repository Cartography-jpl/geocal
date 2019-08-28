#ifndef GLAS_GFM_CAMERA_H
#define GLAS_GFM_CAMERA_H
#include "quaternion_camera.h"
#include "geocal_time.h"

namespace GeoCal {
/****************************************************************//**
  This is a QuaternionCamera with some extra metadata the pointing
  described by a field angle map. This is represented by the NITF
  DES CSSFAB.

  We may want to make this a different class than QuaternionCamera,
  there  are a number of differences here.

  For some of these lower level objects we have directly read the TRE
  or DES (see for example PosCsephb). We haven't currently done this
  with this particular class - the data isn't large or at all slow to
  read so it is easier to just read/write this in python and use this
  lower level class for actually using the camera. We may revisit
  this, but at least for now this is really tied in with the NITF code
  in the python code in geocal_nitf_des.
*******************************************************************/
class GlasGfmCamera : public QuaternionCamera {
public:
  // Not clear what if any arguments we want to constructor. So for
  // now, just leave off.
  GlasGfmCamera();
  virtual ~GlasGfmCamera() {}
  void print(std::ostream& Os) const
  { Os << "GlasGfmCamera"; }

//-----------------------------------------------------------------------
/// Sensor type, "S" or "F"
//-----------------------------------------------------------------------
  
  std::string sensor_type() const
  {
    return number_line(0) == 1 ? "S" : "F";
  }

//-----------------------------------------------------------------------
/// Band type. "M" for for Panchromatic, R for Red, G for Green,
/// "B" for Blue, "N" for NIR, blank
//-----------------------------------------------------------------------

  std::string band_type() const { return band_type_;}
  void band_type(const std::string& V) { band_type_ = V; }

//-----------------------------------------------------------------------
/// Band wavelength. This is in micrometers
//-----------------------------------------------------------------------
  double band_wavelength() const { return band_wavelength_;}
  void band_wavelength(double V) {band_wavelength_ = V;}

//-----------------------------------------------------------------------
/// Band index
//-----------------------------------------------------------------------
  const std::vector<int>& band_index() const {return band_index_;}
  void band_index(const std::vector<int>& V) {band_index_ = V;}

//-----------------------------------------------------------------------
/// Band-Specific Representation of the nth Band
//-----------------------------------------------------------------------
  const std::vector<std::string>& irepband() const {return irepband_;}
  void irepband(const std::vector<std::string>& V) {irepband_ = V;}

//-----------------------------------------------------------------------
/// Band-Specific Representation of the nth Band
//-----------------------------------------------------------------------
  const std::vector<std::string>& isubcat() const {return isubcat_;}
  void isubcat(const std::vector<std::string>& V) {isubcat_ = V;}

//-----------------------------------------------------------------------
/// Focal length time.
//-----------------------------------------------------------------------
  const Time& focal_length_time() const { return focal_length_time_;}
  void focal_length_time(const Time& V) {focal_length_time_ = V; }

//-----------------------------------------------------------------------
/// Primary mirror offset.
//-----------------------------------------------------------------------

  blitz::Array<double, 1> ppoff() const
  {
    blitz::Array<double, 1> res(3);
    res = 0;
    return res;
  }

//-----------------------------------------------------------------------
/// Angular sensor frame offset
//-----------------------------------------------------------------------

  blitz::Array<double, 1> angoff() const
  {
    blitz::Array<double, 1> res(3);
    res = 0;
    return res;
  }

//-----------------------------------------------------------------------
/// Sample Number first
//-----------------------------------------------------------------------
  double sample_number_first() const {return sample_number_first_; }
  void sample_number_first(double V) {sample_number_first_ = V;}

//-----------------------------------------------------------------------
/// Delta Sample Pair
//-----------------------------------------------------------------------
  double delta_sample_pair() const {return delta_sample_pair_; }
  void delta_sample_pair(double V) {delta_sample_pair_ = V; }

//-----------------------------------------------------------------------
/// Field Alignment. This is n x 4. The columns are start_x, start_y,
/// end_x, end_y.
//-----------------------------------------------------------------------
  const blitz::Array<double, 2>& field_alignment() const
  {return field_alignment_;}
  void field_alignment(const blitz::Array<double, 2>& V)
  { field_alignment_ = V;}
  
//-----------------------------------------------------------------------
/// The UUID for the DES that contains this object, if any. This is an
/// empty string we don't have a actual DES we are part of.
//-----------------------------------------------------------------------

  std::string id() const { return id_;}
  void id(const std::string& V) { id_ = V;}
private:
  std::string id_;
  std::string band_type_;
  double band_wavelength_;
  std::vector<int> band_index_;
  std::vector<std::string> irepband_, isubcat_;
  Time focal_length_time_;
  double sample_number_first_, delta_sample_pair_;
  blitz::Array<double, 2> field_alignment_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(GlasGfmCamera);
#endif
