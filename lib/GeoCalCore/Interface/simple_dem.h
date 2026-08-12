#ifndef SIMPLE_DEM_H
#define SIMPLE_DEM_H
#include "dem.h"
#include "geodetic.h"

namespace GeoCal {
/****************************************************************//**
  This is a simple implementation of a Dem, intended primarily for use
  during testing. It find the height of a given point above the reference
  ellipsoid + fixed height, in the given coordinates.
*******************************************************************/

template<class G> class SimpleDemT : public Dem {
public:
//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  SimpleDemT(double H = 0) : h_(H) {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~SimpleDemT() {}

//-----------------------------------------------------------------------
/// Return height of surface above/below the reference surface (e.g.,
/// WGS-84 for the earth). Positive means above, negative below. This is 
/// in meters.
//-----------------------------------------------------------------------

  virtual double height_reference_surface(const GroundCoordinate& UNUSED(Gp))
    const {return h_;}

//-----------------------------------------------------------------------
/// Return distance to surface directly above/below the given point.
/// Distance is in meters. Positive means Gp is above the surface, 
/// negative means below.
//-----------------------------------------------------------------------

  virtual double distance_to_surface(const GroundCoordinate& Gp) 
    const
  {   return Gp.height_reference_surface() - h_; }

  virtual void height_range(const GroundCoordinate& Ulc,
			    const GroundCoordinate& Lrc,
			    double& Min_h, double& Max_h,
			    double H_pad = 10.0) const
  {
    Min_h = h_ - H_pad;
    Max_h = h_ + H_pad;
  }
  virtual boost::shared_ptr<GroundCoordinate> 
    surface_point(const GroundCoordinate& Gp) const
  {
    G g(Gp);
    return boost::shared_ptr<GroundCoordinate>(
		       new G(g.latitude(), g.longitude(), h_));
  }

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { Os << "Simple Dem, height " << h_ << "m \n"; }

//-----------------------------------------------------------------------
/// Return height value used by this object.
//-----------------------------------------------------------------------

  double h() const {return h_;}

//-----------------------------------------------------------------------
/// Set height value used by this object.
//-----------------------------------------------------------------------

  void h(double Hnew) {h_ = Hnew;}
private:
  double h_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

//-----------------------------------------------------------------------
/// SimpleDem for WGS84. Note that we should probably call this 
/// "SimpleDemWgs84", and the template SimpleDem. But historically we 
/// only had SimpleDem based on WGS84, and there is a lot of code that uses
/// this name. So we settle on leaving this in place.
//-----------------------------------------------------------------------
  typedef SimpleDemT<Geodetic> SimpleDem;
}

GEOCAL_EXPORT_KEY(SimpleDem);
#endif
