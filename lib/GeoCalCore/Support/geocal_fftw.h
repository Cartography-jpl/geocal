#ifndef GEOCAL_FFTW_H
#define GEOCAL_FFTW_H
#include "printable.h"
#include <blitz/array.h>
#include <boost/utility.hpp>
#include <complex>

namespace GeoCal {

/****************************************************************//**
  This is a light wrapper around the FFTW library. This maintains the
  lifetime of plans. This is for a 2D real transform, going forward to
  a complex results.
*******************************************************************/

class Fftw2dForward : public Printable<Fftw2dForward>,
		      public boost::noncopyable {
public:
  Fftw2dForward(int Nrow, int Ncol);
  virtual ~Fftw2dForward();

//-----------------------------------------------------------------------
/// Input data. This array should not be resized, or the underlying
/// FFTW library may core dump.
//-----------------------------------------------------------------------

  blitz::Array<double, 2> data_in;

//-----------------------------------------------------------------------
/// Output data. This array should not be resize, or the underlying
/// FFTW library may core dump.
//-----------------------------------------------------------------------

  blitz::Array<std::complex<double>, 2> data_out;
  void run_fft();

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------
  virtual void print(std::ostream& Os)
  { 
    Os << "Fftw2dForward " << data_in.rows() << " x " 
       << data_in.cols();
  }
private:
  // Use void *, just so we don't depend on having fftw available in
  // header file.
  //fftw_plan plan;
  void* plan;
};

/****************************************************************//**
  This is a light wrapper around the FFTW library. This maintains the
  lifetime of plans. This is for a 2D real transform, going backward from
  a complex value to real results.

  Note that the FFTW library calculates an unnormalized
  transform. This means that a forward calculation followed by a
  backward calculation will return a results Nrow * Ncol * original
  data. 
*******************************************************************/

class Fftw2dBackward : public Printable<Fftw2dBackward>,
		       public boost::noncopyable {
public:
  Fftw2dBackward(int Nrow, int Ncol);
  virtual ~Fftw2dBackward();

//-----------------------------------------------------------------------
/// Input data. This array should not be resized, or the underlying
/// FFTW library may core dump.
//-----------------------------------------------------------------------

  blitz::Array<std::complex<double>, 2> data_in;

//-----------------------------------------------------------------------
/// Output data. This array should not be resize, or the underlying
/// FFTW library may core dump.
//-----------------------------------------------------------------------

  blitz::Array<double, 2> data_out;
  void run_fft();

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------
  virtual void print(std::ostream& Os)
  { 
    Os << "Fftw2dBackward " << data_in.rows() << " x " 
       << data_in.cols();
  }
private:
  // Use void *, just so we don't depend on having fftw available in
  // header file.
  //fftw_plan plan;
  void* plan;
};
}
#endif
