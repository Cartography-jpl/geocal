// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%{
#include "geocal_exception.h"
%}

namespace GeoCal {
  class Exception {
  public:
    Exception(const std::string& W);
    const char* what();
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

  
}

