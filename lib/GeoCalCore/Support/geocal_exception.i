// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geocal_exception.h"
%}

%base_import(generic_object)

%init {
  GeoCal::no_gsl_abort();
}

%geocal_shared_ptr(GeoCal::Exception)
%geocal_shared_ptr(GeoCal::ConvergenceFailure)
%geocal_shared_ptr(GeoCal::NoCoverage)
%geocal_shared_ptr(GeoCal::MetadataMissing)
%geocal_shared_ptr(GeoCal::VicarException)

namespace GeoCal {
  class Exception : public GenericObject {
  public:
    Exception(const std::string& W);
    const char* what();
    std::string print_to_string() const;
  };

  class ConvergenceFailure : public Exception {
  public:
    ConvergenceFailure(const std::string& W);
  };

  class NoCoverage : public Exception {
  public:
    NoCoverage(const std::string& W = "");
  };

  class MetadataMissing : public Exception {
  public:
    MetadataMissing(const std::string& W);
  };

class VicarException : public Exception {
public:
  VicarException(int status, const std::string& W = "");
};

void no_gsl_abort();
}

// List of things "import *" will include
%python_export("no_gsl_abort","Exception","ConvergenceFailure","NoCoverage","MetadataMissing","VicarException")
