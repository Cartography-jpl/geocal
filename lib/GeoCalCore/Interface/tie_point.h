#ifndef TIE_POINT_H
#define TIE_POINT_H
#include "image_coordinate.h"
#include "ground_coordinate.h"
#include "geocal_exception.h"
#include <blitz/array.h>

namespace GeoCal {
/****************************************************************//**
    This is a tiepoint. This is little more than a structure.
*******************************************************************/

class TiePoint : public Printable<TiePoint> {
public:
//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  TiePoint(int Number_image)
    : id_(1),
      is_gcp_(false),
      ic_(Number_image),
      line_sigma_(Number_image),
      sample_sigma_(Number_image)
  {
    line_sigma_ = 0.1;
    sample_sigma_ = 0.1;
  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~TiePoint() {}

//-----------------------------------------------------------------------
/// ID of tiepoint
//-----------------------------------------------------------------------

  int id() const { return id_;}
  void id(int V) { id_ = V; }

//-----------------------------------------------------------------------
/// True if TiePoint is a ground control point.
//-----------------------------------------------------------------------

  bool is_gcp() const { return is_gcp_;}
  void is_gcp(bool V) { is_gcp_ = V; }
  
//-----------------------------------------------------------------------
// Ground location.
//-----------------------------------------------------------------------

  boost::shared_ptr<GroundCoordinate> ground_location() const
  { return ground_location_; }
  void ground_location(const boost::shared_ptr<GroundCoordinate>& Gc)
  { ground_location_ = Gc; }

//-----------------------------------------------------------------------
/// Number of images covered by the tie point.
//-----------------------------------------------------------------------

  int number_image() const { return (int) ic_.size(); }

//-----------------------------------------------------------------------
// Number of image coordinate that we have values for.
//-----------------------------------------------------------------------

  int number_image_location() const
  {
    int res = 0;
    BOOST_FOREACH(const boost::shared_ptr<ImageCoordinate>& i, ic_)
      if(i)
	++res;
    return res;
  }

//-----------------------------------------------------------------------  
/// Image coordinate
//-----------------------------------------------------------------------

  boost::shared_ptr<ImageCoordinate> image_coordinate(int Image_index) const
  { range_check(Image_index, 0, number_image());
    return ic_[Image_index];
  }
  void image_coordinate(int Image_index,
			const boost::shared_ptr<ImageCoordinate>& Ic,
			double Line_sigma = 0.1, double Sample_sigma = 0.1)
  {
    range_check(Image_index, 0, number_image());
    ic_[Image_index] = Ic;
    line_sigma_(Image_index) = Line_sigma;
    sample_sigma_(Image_index) = Sample_sigma;
  }

//-----------------------------------------------------------------------
/// Line sigma.
//-----------------------------------------------------------------------  

  double line_sigma(int Image_index) const
  {
    range_check(Image_index, 0, number_image());
    return line_sigma_(Image_index);
  }

//-----------------------------------------------------------------------
/// Sample sigma.
//-----------------------------------------------------------------------  

  double sample_sigma(int Image_index) const
  {
    range_check(Image_index, 0, number_image());
    return sample_sigma_(Image_index);
  }
  
  virtual void print(std::ostream& Os) const;
private:
  int id_;
  bool is_gcp_;
  boost::shared_ptr<GroundCoordinate> ground_location_;
  std::vector<boost::shared_ptr<ImageCoordinate> > ic_;
  blitz::Array<double, 1> line_sigma_;
  blitz::Array<double, 1> sample_sigma_;
  TiePoint() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(TiePoint);
#endif
