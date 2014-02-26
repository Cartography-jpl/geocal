// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "geocal_rpc.h"
#include "image_ground_connection.h"
%}
%base_import(generic_object)
%import "dem.i"
%import "ground_coordinate.i"
%import "image_coordinate.i"
%import "image_ground_connection.i"
%import "geodetic.i"
%import "raster_image.i"
%import "raster_image_multi_band.i"
%geocal_shared_ptr(GeoCal::Rpc);

%pythoncode {
def _new_rpc(cls, version, is_rpc_a, error_bias, error_random, height_offset, 
             height_scale, latitude_offset,
             latitude_scale, longitude_offset, longitude_scale,
             line_offset, line_scale, sample_offset, sample_scale,
             line_denominator, line_numerator, sample_denominator,
             sample_numerator, fit_line_numerator, fit_sample_numerator):
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    rpc = Rpc()
    rpc.rpc_type = Rpc.RPC_A if(is_rpc_a) else Rpc.RPC_B
    rpc.error_bias = error_bias
    rpc.error_random = error_random				
    rpc.height_offset = height_offset
    rpc.height_scale = height_scale
    rpc.latitude_offset = latitude_offset
    rpc.latitude_scale = latitude_scale
    rpc.longitude_offset = longitude_offset
    rpc.longitude_scale = longitude_scale
    rpc.line_offset = line_offset
    rpc.line_scale = line_scale
    rpc.sample_offset = sample_offset
    rpc.sample_scale = sample_scale
    rpc.line_denominator = line_denominator
    rpc.line_numerator = line_numerator
    rpc.sample_denominator = sample_denominator
    rpc.sample_numerator = sample_numerator
    rpc.fit_line_numerator = fit_line_numerator
    rpc.fit_sample_numerator = fit_sample_numerator
    return rpc
}
namespace GeoCal {
  class RasterImage;
  class ImageGroundConnection;
  class Rpc : public GenericObject {
  public:
%pythoncode {
@classmethod
def pickle_format_version(cls):
  return 1

def __reduce__(self):
  return _new_rpc, (self.__class__, 1, self.rpc_type == Rpc.RPC_A,
		    self.error_bias,
                    self.error_random,
		    self.height_offset,
		    self.height_scale,
		    self.latitude_offset,
		    self.latitude_scale,
		    self.longitude_offset,
		    self.longitude_scale,
		    self.line_offset,
		    self.line_scale,
		    self.sample_offset,
		    self.sample_scale,
		    list(self.line_denominator),
		    list(self.line_numerator),
		    list(self.sample_denominator),
		    list(self.sample_numerator),
		    list(self.fit_line_numerator),
		    list(self.fit_sample_numerator))
}
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
    Geodetic ground_coordinate(const ImageCoordinate& Ic, const Dem& D) const;
    Geodetic ground_coordinate(const ImageCoordinate& Ic, double Height) const;
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
  };
}
