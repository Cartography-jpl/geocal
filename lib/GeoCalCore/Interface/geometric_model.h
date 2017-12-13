#ifndef GEOMETRIC_MODEL_H
#define GEOMETRIC_MODEL_H
#include "printable.h"
#include "image_coordinate.h"
#include <vector>
#include <blitz/array.h>

namespace GeoCal {
/****************************************************************//**
  This supplies a geometric model that can be used to deform an image,
  e.g., resample an image to match to geometry of another reference
  image. 
*******************************************************************/
class GeometricModel : public Printable<GeometricModel> {
public:
  virtual ~GeometricModel() {}

//-----------------------------------------------------------------------
/// Map image coordinates to an underlying set of image
/// coordinates. This takes the image coordinates of the resampled
/// image (magnified, rotated, whatever) and returns the corresponding 
/// coordinate in the original image.
//-----------------------------------------------------------------------

  virtual ImageCoordinate original_image_coordinate
  (const ImageCoordinate& Resampled_ic) const = 0;

//-----------------------------------------------------------------------
/// This is the inversion of original_image_coordinate, taking the
/// original image coordinates and returning the resampled image
/// coordinates. 
//-----------------------------------------------------------------------

  virtual ImageCoordinate resampled_image_coordinate
  (const ImageCoordinate& Original_ic) const = 0;

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { Os << "Geometric model"; }
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  Often GeometricModels are created by fitting a set of points tieing 
  the image together. This is just complicated enough to need a class
  to maintain it. This is little more than a structure.
*******************************************************************/
class GeometricTiePoints : public Printable<GeometricTiePoints> {
public:
  GeometricTiePoints() : replace_point_(-1) {}

  virtual ~GeometricTiePoints() {} 
  void add_point(const ImageCoordinate& Resampled_ic,
		 const ImageCoordinate& Original_ic);

//-----------------------------------------------------------------------
/// To get started we may have a first set of approximate points added
/// to the GeometricTiePoints, which it can be useful to replace
/// them. For example, picmtch5 starts with 3 points, but then
/// replaces these 3 points with the first 3 image matches.
///  
/// To support this, you can call "start_replacing". Each subsequent
/// call to add_point then replaces one of the existing points rather
/// than adding a new set to the end.
//-----------------------------------------------------------------------

  void start_replacing() { replace_point_ = 0; }
  
//-----------------------------------------------------------------------
/// Remove the point at the given index.
//-----------------------------------------------------------------------

  void remove_point(int Index)
  {
    range_check(Index, 0, (int) otie.size());
    itie.erase(itie.begin() + Index);
    otie.erase(otie.begin() + Index);
  }

//-----------------------------------------------------------------------
/// Number of points.
//-----------------------------------------------------------------------

  int number_point() const { return (int) otie.size(); }
  
//-----------------------------------------------------------------------
/// Return the resampled_ic as 2 columns, first is line second is sample;
//-----------------------------------------------------------------------
  blitz::Array<double, 2> x() const
  { blitz::Array<double, 2> res((int) otie.size(), 2);
    for(int i = 0; i < res.rows(); ++i) {
      res(i, 0) = otie[i].line;
      res(i, 1) = otie[i].sample;
    }
    return res;
  }

//-----------------------------------------------------------------------
/// Return the resampled_ic as 2 columns, first is line second is sample;
//-----------------------------------------------------------------------
  blitz::Array<double, 2> y() const
  { blitz::Array<double, 2> res((int) itie.size(), 2);
    for(int i = 0; i < res.rows(); ++i) {
      res(i, 0) = itie[i].line;
      res(i, 1) = itie[i].sample;
    }
    return res;
  }
  virtual void print(std::ostream& Os) const 
  { Os << "GeometricTiePoints"; }
private:
  std::vector<ImageCoordinate> itie, otie;
  int replace_point_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(GeometricModel);
GEOCAL_EXPORT_KEY(GeometricTiePoints);
#endif

