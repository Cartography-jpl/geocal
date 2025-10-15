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
  Msp();
  void msp_register_plugin(const std::string& Plugin_name);
  std::vector<std::string> msp_plugin_list();
  std::vector<std::string> msp_model_list(const std::string& Plugin);
  void msp_print_plugin_list();
};

