// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%{
#include "map_info.h"
%}
%geocal_shared_ptr(GeoCal::MapInfo);

namespace GeoCal {
class MapInfo {
public:
  MapInfo() {}
  MapInfo(const boost::shared_ptr<CoordinateConverter>& Conv, double Ulc_x, 
	  double Ulc_y, double Lrc_x, double Lrc_y, int Number_x_pixel, 
	  int Number_y_pixel);
  MapInfo(const boost::shared_ptr<CoordinateConverter>& Conv, 
	  const blitz::Array<double, 1>& Param,
	  int Number_x_pixel, 
	  int Number_y_pixel);
  %python_attribute2(coordinate_converter, coordinate_converter_ptr,
		     boost::shared_ptr<CoordinateConverter>)
  void coordinate(const GroundCoordinate& Gc, double& OUTPUT, 
		  double& OUTPUT) const;
  MapInfo cover(const std::vector<boost::shared_ptr<GroundCoordinate> >& Pt,
		int boundary = 0) const;
  boost::shared_ptr<GroundCoordinate> ground_coordinate(double Pixel_x_index, 
   	           double Pixel_y_index, const Dem& D) const;
  boost::shared_ptr<GroundCoordinate> 
  ground_coordinate(double Pixel_x_index, double Pixel_y_index) const;
  MapInfo intersection(const MapInfo& Mi) const;
  MapInfo map_union(const MapInfo& Mi) const;
  void index_to_coordinate(double Pixel_x_index, double Pixel_y_index,
      double& Pixel_x_coordinate, double& Pixel_y_coordinate) const;
  void coordinate_to_index(double Pixel_x_coordinate, double Pixel_y_coordinate,
			   double& Pixel_x_index, double& Pixel_y_index) const;
  %python_attribute(lrc_x, double)
  %python_attribute(lrc_y, double)
  %python_attribute(number_x_pixel, int)
  %python_attribute(number_y_pixel, int)
  %python_attribute(resolution_meter, double)
  MapInfo subset(int x_index, int y_index, int nx_pixel, int ny_pixel) const;
  MapInfo scale(double Number_x_per_pixel, double Number_y_per_pixel) 
    const;
  %python_attribute(ulc_x, double)
  %python_attribute(ulc_y, double)
  %python_attribute(transform, blitz::Array<double, 1>)
  std::string print_to_string() const;
  %pickle_init(1, self.coordinate_converter, self.transform,
	       self.number_x_pixel, self.number_y_pixel)
};

}
