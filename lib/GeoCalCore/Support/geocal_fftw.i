// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "geocal_common.i"

%{
#include "geocal_fftw.h"
%}
%base_import(generic_object)
%geocal_shared_ptr(GeoCal::Fftw2dForward);
%geocal_shared_ptr(GeoCal::Fftw2dBackward);

namespace GeoCal {
class Fftw2dForward : public GenericObject {
public:
  Fftw2dForward(int Nrow, int Ncol);
  blitz::Array<double, 2> data_in;
  blitz::Array<std::complex<double>, 2> data_out;
  void run_fft();
  std::string print_to_string() const;
};
class Fftw2dBackward : public GenericObject {
public:
  Fftw2dBackward(int Nrow, int Ncol);
  blitz::Array<std::complex<double>, 2> data_in;
  blitz::Array<double, 2> data_out;
  void run_fft();
  std::string print_to_string() const;
};
}

// List of things "import *" will include
%python_export("Fftw2dForward","Fftw2dBackward")
