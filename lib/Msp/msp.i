// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%module msp_wrap

%include "std_string.i"
%include "std_vector.i"
#define SWIG_MODULE_ALREADY_DONE
%include "swig_array.i"

%template(StringVector) std::vector<std::string>;

// Map std::string to and from the native string type
%naturalvar std::string;

%include "exception.i"

%exception {
  try {
    $action
  } catch (const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}

%{
#include "msp.h"  
%}

class Msp {
public:
  Msp(const std::string& Fname, int Image_index = 0,
      const std::string& Plugin_name = "", const std::string& Model_name = "");
  static void msp_register_plugin(const std::string& Plugin_name);
  static void msp_print_plugin_list();
  static std::vector<std::string> msp_plugin_list();
  static std::vector<std::string> msp_model_list(const std::string& Plugin);
  static std::vector<std::string> image_ids(const std::string& Fname);
  const std::string& file_name() const;
  int image_index() const;
  std::string family() const;
  std::string version() const;
  std::string model_name() const;
  std::string pedigree() const;
  std::string image_identifer() const;
  std::string sensor_identifer() const;
  std::string platform_identifer() const;
  std::string collection_identifer() const;
  std::string trajectory_identifer() const;
  std::string sensor_type() const;
  std::string sensor_mode() const;
  std::string reference_date_time() const;
  blitz::Array<double, 1> sensor_velocity(double Line, double Sample);
  blitz::Array<double, 1> image_coordinate(const blitz::Array<double, 1>& Gc_ecr) const;
  blitz::Array<double, 2> cf_look_vector(double Line, double Sample) const;
  blitz::Array<double, 1>
  ground_coordinate_approx_height(double Line, double Sample, double H) const;
  blitz::Array<int, 1> image_size() const;
  std::string pixel_time_base(double Line, double Sample) const;
  double pixel_time_offset(double Line, double Sample) const;
};

%pythoncode {
  __all__.append("Msp")
}
