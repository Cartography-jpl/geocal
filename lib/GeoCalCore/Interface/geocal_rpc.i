// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geocal_rpc.h"
#include "image_ground_connection.h"
%}
%base_import(generic_object)
%import "dem.i"
%import "look_vector.i"
%import "ground_coordinate.i"
%import "image_coordinate.i"
%import "image_ground_connection.i"
%import "ground_coordinate.i"
%import "raster_image.i"
%import "raster_image_multi_band.i"
%import "coordinate_converter.i"
%geocal_shared_ptr(GeoCal::Rpc);

namespace GeoCal {
  class RasterImage;
  class ImageGroundConnection;

  %copyctor Rpc;

  class Rpc : public GenericObject {
  public:
    std::string print_to_string() const;
    enum RpcType {RPC_A, RPC_B};
    RpcType rpc_type;
    double error_bias;
    double error_random;
    double height_offset;
    double height_scale;
    double latitude_offset;
    double latitude_scale;
    double longitude_offset;
    double longitude_scale;
    double line_offset;
    double line_scale;
    double sample_offset;
    double sample_scale;
    %python_attribute_boost_array(line_denominator, double, 20);
    %python_attribute_boost_array(line_numerator, double, 20);
    %python_attribute_boost_array(sample_denominator, double, 20);
    %python_attribute_boost_array(sample_numerator, double, 20);
    %python_attribute_boost_array(fit_line_numerator, bool, 20);
    %python_attribute_boost_array(fit_sample_numerator, bool, 20);
    boost::shared_ptr<CoordinateConverter> coordinate_converter;
    double resolution_meter(const Dem& D) const;
    void fit(const std::vector<boost::shared_ptr<GroundCoordinate> >& Gc,
	   const std::vector<ImageCoordinate>& Ic, 
	   const std::vector<double>& Line_sigma,
	   const std::vector<double>& Sample_sigma,
	   std::set<int> Blunder,
	   double Blunder_threshold = 3,
	   double Chisq_threshold = 0.8,
	   bool Blunder_detect = true);
    void fit_all(const std::vector<double>& Line,
	       const std::vector<double>& Sample,
	       const std::vector<double>& Latitude,
	       const std::vector<double>& Longitude,
	       const std::vector<double>& Height);
    static Rpc generate_rpc(const ImageGroundConnection& Igc,
			    double Min_height, double Max_height,
			    int Nlat = 20, int Nlon = 20, int Nheight = 20,
			    bool Skip_masked_point = false,
			    bool Ignore_error = false
			    );
    boost::shared_ptr<GroundCoordinate> ground_coordinate(const ImageCoordinate& Ic, const Dem& D) const;
    boost::shared_ptr<GroundCoordinate> ground_coordinate(const ImageCoordinate& Ic, double Height) const;
    ImageCoordinate image_coordinate(const GroundCoordinate& Gc) const;
    ImageCoordinate image_coordinate(double Latitude, double Longitude, 
	 			     double Height_ellipsoid) const;
    blitz::Array<double, 2> image_coordinate_jac_parm(const GroundCoordinate& Gc)
    const;
    blitz::Array<double, 2> 
    image_coordinate_jac_parm(double Latitude, double Longitude, 
			      double Height_ellipsoid) const;
    blitz::Array<double, 2> image_coordinate_jac(double Latitude, 
					      double Longitude, 
					      double Height_ellipsoid) const;
    void rpc_project(RasterImage& Res, const RasterImage& Img, 
		      const Dem& D, double Line_scale = 1, 
		      double Sample_scale = 1) const;
    Rpc rpc_type_a() const;
    Rpc rpc_type_b() const;
    %pickle_serialization()
  };
}
