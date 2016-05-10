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
};

/****************************************************************//**
  Often GeometricModels are created by fitting a set of points tieing 
  the image together. This is just complicated enough to need a class
  to maintain it. This is little more than a structure.
*******************************************************************/
class GeometricTiePoints : public Printable<GeometricTiePoints> {
public:
  GeometricTiePoints() {}

  virtual ~GeometricTiePoints() {} 
//-----------------------------------------------------------------------
/// Add a point
//-----------------------------------------------------------------------
  void add_point(const ImageCoordinate& Resampled_ic,
		 const ImageCoordinate& Original_ic)
  { itie.push_back(Original_ic); otie.push_back(Resampled_ic); }

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
};

}
#endif

