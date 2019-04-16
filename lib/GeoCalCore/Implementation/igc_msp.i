// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "igc_msp.h"
%}
%base_import(image_ground_connection)
%import "raster_image.i"
%geocal_shared_ptr(GeoCal::IgcMsp);
namespace GeoCal {
bool have_msp_supported();
  
class IgcMsp : public ImageGroundConnectionCopy {
public:
  IgcMsp(const std::string& Fname);
  IgcMsp(const std::string& Fname, const boost::shared_ptr<Dem>& Dem);
  IgcMsp(const std::string& Fname, const boost::shared_ptr<Dem>& Dem,
	 const std::string& Plugin_name, const std::string& Model_name);
  static void msp_print_plugin_list();
  static void msp_register_plugin(const std::string& Plugin_name);
  static std::vector<std::string> msp_plugin_list();
  static std::vector<std::string> msp_model_list(const std::string& Plugin);
  %pickle_serialization;
};

}

// List of things "import *" will include
%python_export("IgcMsp", "have_msp_supported")
