// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%module msp_wrap

%include "std_string.i"
%include "std_vector.i"

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
  const std::string& file_name() const { return fname_;}
  int image_index() const { return image_index_;}
  std::string family() const { return model->getFamily(); }
  std::string version() const { return model->getVersion().version(); }
  std::string model_name() const { return model->getModelName(); }
  std::string pedigree() const { return model->getPedigree(); }
  std::string image_identifer() const { return model->getImageIdentifier(); }
  std::string sensor_identifer() const { return model->getSensorIdentifier(); }
  std::string platform_identifer() const { return model->getPlatformIdentifier(); }
  std::string collection_identifer() const { return model->getCollectionIdentifier(); }
  std::string trajectory_identifer() const { return model->getTrajectoryIdentifier(); }
  std::string sensor_type() const { return model->getSensorType(); }
  std::string sensor_mode() const { return model->getSensorMode(); }
  std::string reference_date_time() const { return model->getReferenceDateAndTime(); }
};

