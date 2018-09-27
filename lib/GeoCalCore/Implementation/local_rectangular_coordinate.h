#ifndef LOCAL_RECTANGULAR_COORDINATE_H
#define LOCAL_RECTANGULAR_COORDINATE_H
#include "ground_coordinate.h"
#include "coordinate_converter.h"
#include "geocal_matrix.h"
#include "array_ad.h"
#include <boost/make_shared.hpp>

namespace GeoCal {
class ImageGroundConnection;
/****************************************************************//**
  This is a support class for LocalRectangularCoordinate.
*******************************************************************/

class LocalRcParameter : public Printable<LocalRcParameter> {
public:
  LocalRcParameter() {}
  LocalRcParameter(const ImageGroundConnection& Igc, double Height = 0);
  static void check_rc_assumption
  (const ImageGroundConnection& Igc, double Height = 0);
  virtual ~LocalRcParameter() {}
  virtual void print(std::ostream& Os) const
  { Os << "LocalRcParameter"; }
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
       << position[2] << ")"; }

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
  { Os << "LocalRcConverter"; }
  virtual int naif_code() const { return p->cf_prototype->naif_code(); }
  const boost::shared_ptr<LocalRcParameter>& parameter() const { return p; }
private:
  LocalRcConverter() {}
  boost::shared_ptr<LocalRcParameter> p;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
  
}
GEOCAL_EXPORT_KEY(LocalRcParameter);
GEOCAL_EXPORT_KEY(LocalRectangularCoordinate);
GEOCAL_EXPORT_KEY(LocalRcConverter);
#endif
