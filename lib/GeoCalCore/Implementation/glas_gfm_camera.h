#ifndef GLAS_GFM_CAMERA_H
#define GLAS_GFM_CAMERA_H
#include "camera.h"
#include "geocal_exception.h"
#include "geocal_quaternion.h"
#include "geocal_autoderivative_quaternion.h"
#include "geocal_time.h"
#include <vector>
#include <blitz/array.h>

namespace GeoCal {
  // Forward declaration, used internally by GlasGfmCamera
  class GlasGfmCameraModelImp;
/****************************************************************//**
  This is a Camera with some extra metadata the pointing described by
  a field angle map. This is represented by the NITF DES CSSFAB.

  For some of these lower level objects we have directly read the TRE
  or DES (see for example PosCsephb). We haven't currently done this
  with this particular class - the data isn't large or at all slow to
  read so it is easier to just read/write this in python and use this
  lower level class for actually using the camera. We may revisit
  this, but at least for now this is really tied in with the NITF code
  in the python code in geocal_nitf_des.
*******************************************************************/
class GlasGfmCamera : public Camera {
public:
  // Not clear what if any arguments we want to constructor. So for
  // now, just leave most everything off.
  GlasGfmCamera(int Number_line = 1, int Number_sample = 256);
  virtual ~GlasGfmCamera() {}
  void print(std::ostream& Os) const
  { Os << "GlasGfmCamera"; }

//-----------------------------------------------------------------------
/// Number of bands in camera.
//-----------------------------------------------------------------------
  
  virtual int number_band() const  { return 1; }

//-----------------------------------------------------------------------
/// Number of lines in camera for given band.
//-----------------------------------------------------------------------

  virtual int number_line(int Band) const
  { range_check(Band, 0, number_band()); return nline_; }
  void set_number_line(int V)
  { nline_ = V; init_model(); notify_update(); }
  
//-----------------------------------------------------------------------
/// Number of samples in camera for given band.
//-----------------------------------------------------------------------

  virtual int number_sample(int Band) const
  { range_check(Band, 0, number_band()); return nsamp_; }
  void set_number_sample(int V)
  { nsamp_ = V; notify_update(); }

//-----------------------------------------------------------------------
/// Focal length, in meters (so not mm like QuaternionCamera)
//-----------------------------------------------------------------------

  double focal_length() const {return focal_length_.value();}

//-----------------------------------------------------------------------
/// Set focal length, in meters (so not mm like QuaternionCamera)
//-----------------------------------------------------------------------

  void focal_length(double V) { focal_length_ = V; notify_update(); }

//-----------------------------------------------------------------------
/// Focal length, in meters (so not mm like QuaternionCamera)
//-----------------------------------------------------------------------

  const AutoDerivative<double>& focal_length_with_derivative() const 
  {return focal_length_;}


//-----------------------------------------------------------------------
/// Set focal length, in meters (so not mm like QuaternionCamera)
//-----------------------------------------------------------------------

  void focal_length_with_derivative(const AutoDerivative<double>& V) 
  { focal_length_ = V; notify_update(); }

//-----------------------------------------------------------------------
/// Frame to spacecraft quaternion.
//-----------------------------------------------------------------------

  boost::math::quaternion<double> frame_to_sc() const
  {return frame_to_sc_nd_;}

//-----------------------------------------------------------------------
/// Frame to spacecraft quaternion.
//-----------------------------------------------------------------------

  boost::math::quaternion<AutoDerivative<double> > 
  frame_to_sc_with_derivative() const
  {return frame_to_sc_;}

//-----------------------------------------------------------------------
/// Set frame to spacecraft quaternion.
//-----------------------------------------------------------------------

  void frame_to_sc(const boost::math::quaternion<double>& frame_to_sc_q) 
  {
    frame_to_sc_ = to_autoderivative(frame_to_sc_q);
    frame_to_sc_nd_ = frame_to_sc_q;
    notify_update();
  }


//-----------------------------------------------------------------------
/// Set frame to spacecraft quaternion.
//-----------------------------------------------------------------------

  void frame_to_sc_with_derivative
  (const boost::math::quaternion<AutoDerivative<double> >& frame_to_sc_q) 
  { frame_to_sc_ = frame_to_sc_q;
    frame_to_sc_nd_ = value(frame_to_sc_);
    notify_update();
  }

  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;
  virtual FrameCoordinateWithDerivative 
  frame_coordinate_with_derivative(const ScLookVectorWithDerivative& Sl, 
				   int Band) const;

  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  virtual ScLookVectorWithDerivative 
  sc_look_vector_with_derivative(const FrameCoordinateWithDerivative& F, 
				 int Band) const;

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
  void band_type(const std::string& V) { band_type_ = V; notify_update();}

//-----------------------------------------------------------------------
/// Band wavelength. This is in micrometers
//-----------------------------------------------------------------------

  double band_wavelength() const { return band_wavelength_;}
  void band_wavelength(double V) {band_wavelength_ = V; notify_update();}

//-----------------------------------------------------------------------
/// Band index
//-----------------------------------------------------------------------

  const std::vector<int>& band_index() const {return band_index_;}
  void band_index(const std::vector<int>& V) {band_index_ = V; notify_update();}

//-----------------------------------------------------------------------
/// Band-Specific Representation of the nth Band
//-----------------------------------------------------------------------

  const std::vector<std::string>& irepband() const {return irepband_;}
  void irepband(const std::vector<std::string>& V)
  {irepband_ = V; notify_update();}

//-----------------------------------------------------------------------
/// Band-Specific Representation of the nth Band
//-----------------------------------------------------------------------

  const std::vector<std::string>& isubcat() const {return isubcat_;}
  void isubcat(const std::vector<std::string>& V)
  {isubcat_ = V; notify_update();}

//-----------------------------------------------------------------------
/// Focal length time.
//-----------------------------------------------------------------------

  const Time& focal_length_time() const { return focal_length_time_;}
  void focal_length_time(const Time& V)
  {focal_length_time_ = V; notify_update();}

//-----------------------------------------------------------------------
/// Primary mirror offset. In meters. Not sure about the coordinate
/// system, we'll need to track this down if we add support for
/// nonzero values here.
//-----------------------------------------------------------------------

  blitz::Array<double, 1> ppoff() const
  {
    blitz::Array<double, 1> res(3);
    res = 0;
    return res;
  }

  blitz::Array<double, 1> angoff() const;
  void angoff(const blitz::Array<double, 1>& V);

//-----------------------------------------------------------------------
/// Sample Number first. This is applicable for sensor type "S" only.
//-----------------------------------------------------------------------

  double sample_number_first() const {return sample_number_first_; }
  void sample_number_first(double V)
  { sample_number_first_ = V; notify_update();}

//-----------------------------------------------------------------------
/// Delta Sample Pair. This is applicable for sensor type "S" only.
//-----------------------------------------------------------------------

  double delta_sample_pair() const {return delta_sample_pair_; }
  void delta_sample_pair(double V)
  {delta_sample_pair_ = V; notify_update();}

//-----------------------------------------------------------------------
/// Field Alignment. This is n x 4. The columns are start_x, start_y,
/// end_x, end_y. This is applicable for sensor type "S" only.
//-----------------------------------------------------------------------

  const blitz::Array<double, 2>& field_alignment() const
  {return field_alignment_;}
  void field_alignment(const blitz::Array<double, 2>& V)
  { field_alignment_.reference(V.copy()); notify_update();}
  void field_alignment_fit(const Camera& Cam, double Delta_sample);

//-----------------------------------------------------------------------
/// Field angle type. 0 for direct alignment grid, 1 for calibration
/// parameters. Only applicable for sensor type "F".
//-----------------------------------------------------------------------

  int field_angle_type() const { return field_angle_type_; }
  void field_angle_type(int V) 
  {
    field_angle_type_ = V;
    init_model();
    notify_update();
  }

//-----------------------------------------------------------------------
/// Field angle interplation type. 0 for nearest neighbor, 1 for
/// linear. Only applicable for sensor type "F".
//-----------------------------------------------------------------------

  int field_angle_interpolation_type() const { return field_angle_interpolation_type_; }
  void field_angle_interpolation_type(int V)
  { field_angle_interpolation_type_ = V; notify_update(); }
  
//-----------------------------------------------------------------------
/// Starting line for first block in field_alignment_block.
/// Only applicable for sensor type "F" and field_angle_type 0.  
//-----------------------------------------------------------------------

  const blitz::Array<double, 1>& first_line_block() const
  { return first_line_block_; }
  void first_line_block(const blitz::Array<double, 1>& V)
  { first_line_block_.reference(V.copy()); notify_update(); }

//-----------------------------------------------------------------------
/// Starting sample for first block in field_alignment_block.
/// Only applicable for sensor type "F" and field_angle_type 0.  
//-----------------------------------------------------------------------

  const blitz::Array<double, 1>& first_sample_block() const
  { return first_sample_block_; }
  void first_sample_block(const blitz::Array<double, 1>& V)
  { first_sample_block_.reference(V.copy()); notify_update(); }

//-----------------------------------------------------------------------
/// Delta line for blocks in field_alignment_block.
/// Only applicable for sensor type "F" and field_angle_type 0.  
//-----------------------------------------------------------------------

  const blitz::Array<double, 1>& delta_line_block() const
  { return delta_line_block_; }
  void delta_line_block(const blitz::Array<double, 1>& V)
  { delta_line_block_.reference(V.copy()); notify_update(); }

//-----------------------------------------------------------------------
/// Delta sample for blocks in field_alignment_block.
/// Only applicable for sensor type "F" and field_angle_type 0.  
//-----------------------------------------------------------------------

  const blitz::Array<double, 1>& delta_sample_block() const
  { return delta_sample_block_; }
  void delta_sample_block(const blitz::Array<double, 1>& V)
  { delta_sample_block_.reference(V.copy()); notify_update(); }

//-----------------------------------------------------------------------
/// field_alignment_block.
/// Only applicable for sensor type "F" and field_angle_type 0.  
//-----------------------------------------------------------------------

  const blitz::Array<double, 5>& field_alignment_block(int i) const
  {
    range_check(i, 0, (int) first_line_block_.size());
    // This should always be true since we size field_alignment_block_
    // to the maximum number of field angle datasets. But check
    // anyways, in case something weird has happened.
    range_check(i, 0, (int) field_alignment_block_.size());
    return field_alignment_block_[i];
  }
  void field_alignment_block(int i, const blitz::Array<double, 5>& V)
  {
    range_check(i, 0, (int) first_line_block_.size());
    // This should always be true since we size field_alignment_block_
    // to the maximum number of field angle datasets. But check
    // anyways, in case something weird has happened.
    range_check(i, 0, (int) field_alignment_block_.size());
    if(V.extent(2) != 2 || V.extent(3) != 2 || V.extent(4) != 2)
      throw Exception("Field alignment data should be n x m x 2 x 2 x 2");
    field_alignment_block_[i].reference(V.copy());
    notify_update();
  }

  void field_alignment_block(const Camera& Cam, double Delta_line,
			     double Delta_sample);
    
//-----------------------------------------------------------------------
/// The UUID for the DES that contains this object, if any. This is an
/// empty string we don't have a actual DES we are part of.
//-----------------------------------------------------------------------

  std::string id() const { return id_;}
  void id(const std::string& V) { id_ = V; notify_update();}
private:
  boost::shared_ptr<GlasGfmCameraModelImp> model_;
  void init_model();
  AutoDerivative<double> focal_length_;	
				// Focal length, in mm.
  int nline_;			// Number of lines in camera.
  int nsamp_;			// Number of samples in camera.
  // ** Important, see note below about frame_to_sc_nd_. You can
  // use the member function frame_to_sc(val) to set both at the same
  // time if you like ***
  boost::math::quaternion<AutoDerivative<double> > frame_to_sc_;
  // Turns out that converting frame_to_sc_ to a version without
  // derivatives is a bit of a bottle neck in some calculations (e.g.,
  // Ipi). So we keep a copy of value(frame_to_sc_) so we don't need
  // to calculate it multiple times.
  boost::math::quaternion<double> frame_to_sc_nd_;
  std::string id_;
  std::string band_type_;
  double band_wavelength_;
  std::vector<int> band_index_;
  std::vector<std::string> irepband_, isubcat_;
  Time focal_length_time_;
  double sample_number_first_, delta_sample_pair_;
  blitz::Array<double, 2> field_alignment_;
  int field_angle_type_, field_angle_interpolation_type_;
  blitz::Array<double, 1> first_line_block_, first_sample_block_,
	  delta_line_block_, delta_sample_block_;
  std::vector<blitz::Array<double, 5> > field_alignment_block_;
  virtual void notify_update()
  {
    notify_update_do(*this);
  }
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(GlasGfmCamera);
#endif
