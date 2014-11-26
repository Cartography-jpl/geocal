#ifndef IMAGE_COORDINATE_H
#define IMAGE_COORDINATE_H
#include "printable.h"
#include "auto_derivative.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include <iostream>		// Definition of ostream.
namespace GeoCal {
  class VicarImageCoordinate;

/****************************************************************//**
    This describes an image coordinate.

    We also define the print operator <<, and equality operator ==.
    Two image coordinates are considers equal if they are within 0.01
    in the line and sample.

    The convention used throughout GeoCal is that an integer value of
    the ImageCoordinate line or sample refers to the center of a
    pixel. This means the upper left corner of the bounding box of an
    image is at (-0.5, -0.5).
*******************************************************************/

class ImageCoordinate : public Printable<ImageCoordinate> {
public:
//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

  ImageCoordinate(double Line, double Sample) : line(Line), sample(Sample) {}

//-----------------------------------------------------------------------
/// Convert from VicarImageCoordinate.
//-----------------------------------------------------------------------

  ImageCoordinate(const VicarImageCoordinate& Vic);

//-----------------------------------------------------------------------
/// Default constructor, line and sample aren't initialized.
//-----------------------------------------------------------------------

  ImageCoordinate() {}

  virtual ~ImageCoordinate() {}

//-----------------------------------------------------------------------
///  Line number of image coordinate.
//-----------------------------------------------------------------------

  double line;

//-----------------------------------------------------------------------
/// Sample number of image coordinate.
//-----------------------------------------------------------------------

  double sample;
  void print(std::ostream& Os) const;
private:
  friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(GenericObject);
    ar & BOOST_SERIALIZATION_NVP(line) & BOOST_SERIALIZATION_NVP(sample);
  }
};

bool operator==(const ImageCoordinate& C1, const ImageCoordinate& C2);

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// Test for the inequality between two ImageCoordinates. This is done
/// with a tolerance, if the line and sample are with 0.01 of each
/// other than we consider the ImageCoordinates as equal.
//-----------------------------------------------------------------------

inline bool operator!=(const ImageCoordinate& C1, const ImageCoordinate& C2)
{ return !(C1 ==C2);}

/****************************************************************//**
    VICAR programs use a different convention than our
    ImageCoordinate. Instead of the upper left hand corner of the
    bounding box being (-0.5,-0.5), VICAR uses a 1-based convention
    where the upper left corner is (0.5, 0.5). This is just a
    difference of 1-pixel. To avoid any confusion, we define a
    VicarImageCoordinate class, and allow conversions to and from
    ImageCoordinate. 
*******************************************************************/

class VicarImageCoordinate : public Printable<VicarImageCoordinate> {
public:
//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

  VicarImageCoordinate(double Line, double Sample) : line(Line), sample(Sample) {}

//-----------------------------------------------------------------------
/// Convert from ImageCoordinate.
//-----------------------------------------------------------------------

  VicarImageCoordinate(const ImageCoordinate& Ic);

//-----------------------------------------------------------------------
/// Default constructor, line and sample aren't initialized.
//-----------------------------------------------------------------------

  VicarImageCoordinate() {}

  virtual ~VicarImageCoordinate() {}
//-----------------------------------------------------------------------
///  Line number of image coordinate.
//-----------------------------------------------------------------------

  double line;

//-----------------------------------------------------------------------
/// Sample number of image coordinate.
//-----------------------------------------------------------------------

  double sample;
  void print(std::ostream& Os) const;
private:
};

bool operator==(const VicarImageCoordinate& C1, const VicarImageCoordinate& C2);

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// Test for the inequality between two VicarImageCoordinates. This is done
/// with a tolerance, if the line and sample are with 0.01 of each
/// other than we consider the VicarImageCoordinates as equal.
//-----------------------------------------------------------------------

inline bool operator!=(const VicarImageCoordinate& C1, const VicarImageCoordinate& C2)
{ return !(C1 ==C2);}


inline ImageCoordinate::ImageCoordinate(const VicarImageCoordinate& Vic)
  : line(Vic.line - 1), sample(Vic.sample - 1) {}

inline VicarImageCoordinate::VicarImageCoordinate(const ImageCoordinate& Ic)
  : line(Ic.line + 1), sample(Ic.sample + 1) {}

/****************************************************************//**
    This describes an image coordinate, including the gradient of the 
    line and sample.
*******************************************************************/

class ImageCoordinateWithDerivative : public Printable<ImageCoordinateWithDerivative> {
public:
//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

  ImageCoordinateWithDerivative(const AutoDerivative<double>& Line, 
				const AutoDerivative<double>& Sample) 
  : line(Line), sample(Sample) {}

//-----------------------------------------------------------------------
/// Default constructor, line and sample aren't initialized.
//-----------------------------------------------------------------------

  ImageCoordinateWithDerivative() {}

  virtual ~ImageCoordinateWithDerivative() {}

//-----------------------------------------------------------------------
///  Line number of image coordinate.
//-----------------------------------------------------------------------

  AutoDerivative<double> line;

//-----------------------------------------------------------------------
/// Sample number of image coordinate.
//-----------------------------------------------------------------------

  AutoDerivative<double> sample;
  void print(std::ostream& Os) const;
private:
};

}
#endif
