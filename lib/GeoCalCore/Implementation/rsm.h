#ifndef RSM_H
#define RSM_H
#include "rsm_base.h"
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

class Rsm : public Printable<Rsm> {
public:
  Rsm(const boost::shared_ptr<RsmBase>& Rp,
      const boost::shared_ptr<CoordinateConverter>& Cconv);
  virtual ~Rsm() {}

  boost::shared_ptr<GroundCoordinate> ground_coordinate(const ImageCoordinate& Ic, const Dem& D) const;
  boost::shared_ptr<GroundCoordinate> ground_coordinate(const ImageCoordinate& Ic, double Z) const;
  ImageCoordinate image_coordinate(const GroundCoordinate& Gc) const;
  ImageCoordinate image_coordinate(double X, double Y, double Z) const;
  blitz::Array<double, 2> image_coordinate_jacobian
  (double X, double Y, double Z) const;

  void fit(const ImageGroundConnection& Igc, double Min_height,
	   double Max_height,
	   int Nline = 20, int Nsample = 20, int Nheight = 20,
	   bool Skip_masked_point = false,
	   bool Ignore_error = false);
  void print(std::ostream& Os) const;
private:
  boost::shared_ptr<RsmBase> rp;
  boost::shared_ptr<CoordinateConverter> cconv;

  Rsm() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(Rsm);
#endif

