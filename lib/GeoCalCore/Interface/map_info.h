#ifndef MAP_INFO_H
#define MAP_INFO_H
#include "ground_coordinate.h"
#include "coordinate_converter.h"
#include "dem.h"
#include "printable.h"
#include <boost/shared_ptr.hpp>
#include <blitz/array.h>
#include <vector>

namespace GeoCal {
/****************************************************************//**
  This contains information describing a map.

  This class is used to treat map projection information the same,
  regardless of the specific map projection used. However, the pixels
  of the map are required to be equally spaced in the X and Y
  coordinates of the underlying map projection.

  Note that the various coordinates (e.g., lrc_x()) are for the
  bounding box, not for the center of the lrc or ulc pixel.

  The parameters used in MapInfo are the affine parameters, see for
  example GeoTiff. Note though that by convention we use the middle of
  the ulc pixel is at (0,0), and the upper left corner of the pixel is
  at (-0.5, -0.5). The parameters on the other hand have the center of
  the ulc pixel at (0.5, 0.5) and the upper left corner of the pixel
  at (0, 0). This class handles this difference, but we mention this
  in this comment so you can understand where the various "0.5" terms
  come in.

  We have a field indicating if the pixels are points or area. Note
  that we follow the GDAL convention - the value of the field doesn't
  change the geotiff parameters or any of the other fields. (-0.5,
  -0.5) is the ulc of the area, regardless of if we are point or area.
  But the value of this tells how to interpret the pixels in the
  RasterImage that this MapInfo is attached to - either as points or
  averages over area.
*******************************************************************/

class MapInfo : public Printable<MapInfo> {
public:
//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  MapInfo() : is_point_(false), param(6) {}
  MapInfo(const MapInfo& Mi)
    : conv_(Mi.conv_), is_point_(Mi.is_point_),
      number_x_pixel_(Mi.number_x_pixel_),
      number_y_pixel_(Mi.number_y_pixel_), param(Mi.param.copy()) {}

  MapInfo(const boost::shared_ptr<CoordinateConverter>& Conv, double Ulc_x, 
	  double Ulc_y, double Lrc_x, double Lrc_y, int Number_x_pixel, 
	  int Number_y_pixel, bool Is_point=false);
  MapInfo(const boost::shared_ptr<CoordinateConverter>& Conv, 
	  const blitz::Array<double, 1>& Param,
	  int Number_x_pixel, 
	  int Number_y_pixel, bool Is_point=false);

//-----------------------------------------------------------------------
/// Coordinate converter used by map projection.
//-----------------------------------------------------------------------

  const CoordinateConverter& coordinate_converter() const 
  {return *conv_; }

//-----------------------------------------------------------------------
/// Pointer to Coordinate converter used by map projection.
//-----------------------------------------------------------------------

  const boost::shared_ptr<CoordinateConverter>& 
  coordinate_converter_ptr() const 
  {return conv_; }


  void coordinate(const GroundCoordinate& Gc, double& Pixel_x_index, 
		  double& Pixel_y_index) const;
  void coordinate(const Geodetic& Gc, double& Pixel_x_index, 
		  double& Pixel_y_index) const;
  boost::shared_ptr<GroundCoordinate> ground_coordinate(double Pixel_x_index, 
  	           double Pixel_y_index, const Dem& D) const;
  boost::shared_ptr<GroundCoordinate> 
  ground_coordinate(double Pixel_x_index, double Pixel_y_index) const;

  MapInfo cover(const std::vector<boost::shared_ptr<GroundCoordinate> >& Pt,
		int boundary = 0) const;

  MapInfo intersection(const MapInfo& Mi) const;
  MapInfo map_union(const MapInfo& Mi) const;

//-----------------------------------------------------------------------
/// True if we should interpret pixel as a point. Note that this
/// doesn't change the coordinate calculation at all, (-0.5,-0.5) is
/// still the coordinate of the area covered by the pixel.
//-----------------------------------------------------------------------

  bool is_point() const { return is_point_;}
  
//-----------------------------------------------------------------------
/// Convert pixel index to pixel coordinate.
//-----------------------------------------------------------------------

  void index_to_coordinate(double Pixel_x_index, double Pixel_y_index,
	   double& Pixel_x_coordinate, double& Pixel_y_coordinate) const
  {
    Pixel_x_coordinate = param(0) + (Pixel_x_index + 0.5) * param(1) + 
      (Pixel_y_index + 0.5) * param(2);
    Pixel_y_coordinate = param(3) + (Pixel_x_index + 0.5) * param(4) + 
      (Pixel_y_index + 0.5) * param(5);
  }

  void index_to_coordinate(const blitz::Array<double, 1>& Pixel_x_index,
			   const blitz::Array<double, 1>& Pixel_y_index,
			   blitz::Array<double, 1>& Pixel_x_coordinate,
			   blitz::Array<double, 1>& Pixel_y_coordinate) const
  {
    Pixel_x_coordinate.resize(Pixel_x_index.shape());
    Pixel_y_coordinate.resize(Pixel_x_index.shape());
    Pixel_x_coordinate = param(0) + (Pixel_x_index + 0.5) * param(1) + 
      (Pixel_y_index + 0.5) * param(2);
    Pixel_y_coordinate = param(3) + (Pixel_x_index + 0.5) * param(4) + 
      (Pixel_y_index + 0.5) * param(5);
  }
  void index_to_coordinate(const blitz::Array<double, 2>& Pixel_x_index,
			   const blitz::Array<double, 2>& Pixel_y_index,
			   blitz::Array<double, 2>& Pixel_x_coordinate,
			   blitz::Array<double, 2>& Pixel_y_coordinate) const
  {
    Pixel_x_coordinate.resize(Pixel_x_index.shape());
    Pixel_y_coordinate.resize(Pixel_x_index.shape());
    Pixel_x_coordinate = param(0) + (Pixel_x_index + 0.5) * param(1) + 
      (Pixel_y_index + 0.5) * param(2);
    Pixel_y_coordinate = param(3) + (Pixel_x_index + 0.5) * param(4) + 
      (Pixel_y_index + 0.5) * param(5);
  }

//-----------------------------------------------------------------------
/// Convert pixel coordinate to pixel index.
//-----------------------------------------------------------------------

  void coordinate_to_index(double Pixel_x_coordinate, double Pixel_y_coordinate,
			   double& Pixel_x_index, double& Pixel_y_index) const
  {
    Pixel_x_index = (param(5) * (Pixel_x_coordinate - param(0)) - 
		     param(2) * (Pixel_y_coordinate - param(3))) / 
      (param(5) * param(1) - param(2) * param(4)) - 0.5;
    Pixel_y_index = (param(4) * (Pixel_x_coordinate - param(0)) - 
		     param(1) * (Pixel_y_coordinate - param(3))) / 
      (param(4) * param(2) - param(1) * param(5)) - 0.5;
  }
  void coordinate_to_index(const blitz::Array<double,1>& Pixel_x_coordinate,
			   const blitz::Array<double,1>& Pixel_y_coordinate,
			   blitz::Array<double,1>& Pixel_x_index,
			   blitz::Array<double,1>& Pixel_y_index) const
  {
    Pixel_x_index.resize(Pixel_x_coordinate.shape());
    Pixel_y_index.resize(Pixel_y_coordinate.shape());
    Pixel_x_index = (param(5) * (Pixel_x_coordinate - param(0)) - 
		     param(2) * (Pixel_y_coordinate - param(3))) / 
      (param(5) * param(1) - param(2) * param(4)) - 0.5;
    Pixel_y_index = (param(4) * (Pixel_x_coordinate - param(0)) - 
		     param(1) * (Pixel_y_coordinate - param(3))) / 
      (param(4) * param(2) - param(1) * param(5)) - 0.5;
  }
  void coordinate_to_index(const blitz::Array<double,2>& Pixel_x_coordinate,
			   const blitz::Array<double,2>& Pixel_y_coordinate,
			   blitz::Array<double,2>& Pixel_x_index,
			   blitz::Array<double,2>& Pixel_y_index) const
  {
    Pixel_x_index.resize(Pixel_x_coordinate.shape());
    Pixel_y_index.resize(Pixel_y_coordinate.shape());
    Pixel_x_index = (param(5) * (Pixel_x_coordinate - param(0)) - 
		     param(2) * (Pixel_y_coordinate - param(3))) / 
      (param(5) * param(1) - param(2) * param(4)) - 0.5;
    Pixel_y_index = (param(4) * (Pixel_x_coordinate - param(0)) - 
		     param(1) * (Pixel_y_coordinate - param(3))) / 
      (param(4) * param(2) - param(1) * param(5)) - 0.5;
  }

//-----------------------------------------------------------------------
/// X of lower right corner. Note that this is for the outer edge of
/// the pixel, not the center (i.e., for the bounding box).
//-----------------------------------------------------------------------

  double lrc_x() const 
  {
    return param(0) + number_x_pixel() * param(1) +
      number_y_pixel() * param(2);
  }

//-----------------------------------------------------------------------
/// Y of lower right corner. Note that this is for the outer edge of
/// the pixel, not the center (i.e., for the bounding box).
//-----------------------------------------------------------------------

  double lrc_y() const
  {
    return param(3) + number_x_pixel() * param(4) +
      number_y_pixel() * param(5);
  }

//-----------------------------------------------------------------------
/// Number of pixels in X direction.
//-----------------------------------------------------------------------

  int number_x_pixel() const {return number_x_pixel_; }

//-----------------------------------------------------------------------
/// Number of pixels in Y direction.
//-----------------------------------------------------------------------

  int number_y_pixel() const {return number_y_pixel_; }

  double resolution_meter() const;

  MapInfo subset(int x_index, int y_index, int nx_pixel, int ny_pixel) const;
  MapInfo subset(double x_index, double y_index, 
		 int nx_pixel, int ny_pixel) const;
  MapInfo scale(double Number_x_per_pixel, double Number_y_per_pixel) 
    const;

//-----------------------------------------------------------------------
/// X of upper left corner. Note that this is for the outer edge of
/// the pixel, not the center (i.e., for the bounding box).
//-----------------------------------------------------------------------

  double ulc_x() const 
  {
    return param(0);
  }

//-----------------------------------------------------------------------
/// Y of upper left corner. Note that this is for the outer edge of
/// the pixel, not the center (i.e., for the bounding box).
//-----------------------------------------------------------------------

  double ulc_y() const 
  {
    return param(3);
  }

//-----------------------------------------------------------------------
/// This is the affine geometric transformation used by the
/// MapInfo. You don't normally use this directly, but it is useful
/// when writing out file metadata, e.g., with Gdal SetGeoTransform.
//-----------------------------------------------------------------------

  blitz::Array<double, 1> transform() const {return param; }
  
  void print(std::ostream& Os) const;
private:
  boost::shared_ptr<CoordinateConverter> conv_; ///< Coordinate
						///converter for map
						///projection.
  bool is_point_;		///< True if pixel should be
                                /// interpreted as a point
  int number_x_pixel_;          ///< Number of X pixels in image
  int number_y_pixel_;		///< Number of Y pixels in image
  blitz::Array<double, 1> param; ///< Affine parameters.
  void latitude_range_correct();
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}
GEOCAL_EXPORT_KEY(MapInfo);
GEOCAL_CLASS_VERSION(MapInfo, 1);
#endif
