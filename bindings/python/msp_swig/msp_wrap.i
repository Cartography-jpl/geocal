// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%module msp_wrap

%include "std_string.i"

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
  void msp_print_plugin_list();
};

