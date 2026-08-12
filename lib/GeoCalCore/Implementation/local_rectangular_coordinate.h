#ifndef LOCAL_RECTANGULAR_COORDINATE_H
#define LOCAL_RECTANGULAR_COORDINATE_H
#include "ground_coordinate.h"
#include "coordinate_converter.h"
#include "geocal_matrix.h"
#include "array_ad.h"
#include "dem.h"
#include <boost/make_shared.hpp>

namespace GeoCal {
class ImageGroundConnection;
/****************************************************************//**
  This is a support class for LocalRectangularCoordinate.
*******************************************************************/

class LocalRcParameter : public Printable<LocalRcParameter> {
public:
  enum CoordinateCreation {FOLLOW_SAMPLE_LOCAL, FOLLOW_LINE_LOCAL,
    FOLLOW_SAMPLE_FULL, FOLLOW_LINE_FULL};
  LocalRcParameter() {}
  LocalRcParameter(const ImageGroundConnection& Igc, double Height = 0,
		   double Line = -1, double Sample = -1,
		   CoordinateCreation C = FOLLOW_SAMPLE_LOCAL);
  static void check_rc_assumption
  (const ImageGroundConnection& Igc, double Height = 0);
  virtual ~LocalRcParameter() {}
  virtual void print(std::ostream& Os) const
  { Os << "LocalRcParameter\n"; }
  blitz::Array<double, 2> cf_to_rc_arr() const
  { blitz::Array<double, 2> res(3,3);
    for(int i = 0; i < 3; ++i)
      for(int j = 0; j < 3; ++j)
	res(i,j) = cf_to_rc[i][j];
    return res;
  }
  blitz::Array<double, 1> cf_offset_arr() const
  {
    blitz::Array<double, 1> res(3);
    for(int i = 0; i < 3; ++i)
      res(i) = cf_offset[i];
    return res;
  }
  double cf_to_rc[3][3];
  boost::array<double, 3> cf_offset;
  boost::shared_ptr<CartesianFixed> cf_prototype;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a local rectangular coordinate system. This is used by
  Rsm as one of its coordinates choices, this is a rotation and 
  offset of a standard CartesianFixed coordinate system such that the
  origin is the at the center of a ImageGroundConnection projection
  to the surface, oriented to Z in in the look direction, X is mostly
  in the scan direction (orthogonal to Z), and Y completes the right 
  handed coordinate system.
*******************************************************************/

class LocalRectangularCoordinate : public GroundCoordinate {
public:
  LocalRectangularCoordinate() {}
  LocalRectangularCoordinate(const boost::shared_ptr<LocalRcParameter>& P,
			     double X, double Y, double Z)
    :parameter(P)
  {
    position[0] = X;
    position[1] = Y;
    position[2] = Z;
  }
  LocalRectangularCoordinate(const boost::shared_ptr<LocalRcParameter>& P,
			     const GroundCoordinate& Gc);
  virtual ~LocalRectangularCoordinate() { }
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const;
  virtual int naif_code() const { return parameter->cf_prototype->naif_code(); }
  virtual void print(std::ostream& Os) const
  { Os << "LocalRectangularCoordinate (" << position[0] << ", "
       << position[1] << ", "
       << position[2] << ")\n"; }

//-----------------------------------------------------------------------
/// Parameters used to describe the LocalRectangularCoordinate system.
//-----------------------------------------------------------------------
  
  boost::shared_ptr<LocalRcParameter> parameter;

//-----------------------------------------------------------------------
/// Position
//-----------------------------------------------------------------------

  boost::array<double, 3> position;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

class LocalRcConverter : public CoordinateConverter
{
public:
  LocalRcConverter(const boost::shared_ptr<LocalRcParameter>& P)
    : p(P) {}
  virtual ~LocalRcConverter() {}
  virtual boost::shared_ptr<GroundCoordinate>
  convert_from_coordinate(double X, double Y, double Z = 0) const
  { return boost::make_shared<LocalRectangularCoordinate>(p, X, Y, Z); }
  virtual void convert_to_coordinate(const GroundCoordinate& Gc, double& X, 
			       double& Y, double& Z) const
  {
    LocalRectangularCoordinate c(p, Gc);
    X = c.position[0]; Y = c.position[1]; Z = c.position[2];
  }
  ArrayAd<double, 1> convert_to_cf(const AutoDerivative<double>& X,
				   const AutoDerivative<double>& Y,
				   const AutoDerivative<double>& Z) const;
  void convert_from_cf(const ArrayAd<double, 1>& Cf,
		       AutoDerivative<double>& X,
		       AutoDerivative<double>& Y,
		       AutoDerivative<double>& Z) const;
  
  virtual boost::shared_ptr<GroundCoordinate> create
  (const GroundCoordinate& Gc) const
  { return boost::make_shared<LocalRectangularCoordinate>(p, Gc); }
  virtual void convert_to_coordinate(const Geodetic& Gc, double& X, 
			       double& Y, double& Z) const
  { 
    LocalRectangularCoordinate c(p, Gc);
    X = c.position[0]; Y = c.position[1]; Z = c.position[2];
  }
  virtual bool is_same(const CoordinateConverter& Conv) const;
  virtual void print(std::ostream& Os) const
  { Os << "LocalRcConverter\n"; }
  virtual int naif_code() const { return p->cf_prototype->naif_code(); }
  const boost::shared_ptr<LocalRcParameter>& parameter() const { return p; }
private:
  LocalRcConverter() {}
  boost::shared_ptr<LocalRcParameter> p;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a Dem that is at a constant Z in a LocalRcConverter 
  coordinate system.
*******************************************************************/
  
class LocalZDem: public Dem {
public:
//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------
  LocalZDem(const boost::shared_ptr<LocalRcConverter>& Cconv,
	    int Z)
    : z_(Z), cconv(Cconv)
  {
  }

  virtual ~LocalZDem() { }

//-----------------------------------------------------------------------
/// Return height of surface above/below the reference surface (e.g.,
/// WGS-84 for the earth). Positive means above, negative below. This is 
/// in meters.
//-----------------------------------------------------------------------

  virtual double height_reference_surface(const GroundCoordinate& Gp)
    const
  {
    return surface_point(Gp)->height_reference_surface();
  }

//-----------------------------------------------------------------------
/// Return distance to surface directly above/below the given point.
/// Distance is in meters. Positive means Gp is above the surface, 
/// negative means below.
//-----------------------------------------------------------------------

  virtual double distance_to_surface(const GroundCoordinate& Gp) 
    const
  {
    double px,py,pz;
    cconv->convert_to_coordinate(Gp, px, py, pz);
    return pz - z_;
  }
  virtual boost::shared_ptr<GroundCoordinate> 
    surface_point(const GroundCoordinate& Gp) const
  {
    double px,py,pz;
    cconv->convert_to_coordinate(Gp, px, py, pz);
    return cconv->convert_from_coordinate(px, py, z_);
  }
  
  virtual void height_range(const GroundCoordinate& Ulc,
			      const GroundCoordinate& Lrc,
			      double& Min_h, double& Max_h,
			      double H_pad = 10.0) const
  {
    // Not real easy for us to implement. We could probably come up
    // with something if this becomes an issue, but we don't use this
    // DEM a lot. For now, just treat this as an error. We can revisit
    // this if this becomes an issue
    throw Exception("Not implemented");
  }
  
//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { Os << "Local RC Z Dem, Z " << z_ << "m \n"; }

//-----------------------------------------------------------------------
/// Return z value used by this object.
//-----------------------------------------------------------------------

  double z() const {return z_;}

//-----------------------------------------------------------------------
/// Return LocalRcConverter
//-----------------------------------------------------------------------

  const boost::shared_ptr<LocalRcConverter>& coordinate_converter() const
  { return cconv; }
private:
  double z_;
  boost::shared_ptr<LocalRcConverter>  cconv;
  LocalZDem() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}
GEOCAL_EXPORT_KEY(LocalRcParameter);
GEOCAL_EXPORT_KEY(LocalRectangularCoordinate);
GEOCAL_EXPORT_KEY(LocalRcConverter);
GEOCAL_EXPORT_KEY(LocalZDem);
#endif
