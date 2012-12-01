#include "geocal_fftw.h"
using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

Fftw2dForward::Fftw2dForward(int Nrow, int Ncol)
{
  // We use fftw_malloc because it makes sure we have any alignment
  // needed to get the best FFT performance.
  data_in.reference(Array<double, 2>
		    ((double*) fftw_malloc(sizeof(double) * Nrow * Ncol),
		     shape(Nrow, Ncol), neverDeleteData));
  data_out.reference(Array<std::complex<double>, 2>
                    ((std::complex<double>*)
		     fftw_malloc(sizeof(std::complex<double>) * Nrow * Ncol),
		     shape(Nrow, Ncol), neverDeleteData));
  // There is a long discussion in the FFTW documentation that says
  // fftw_complex and std::complex<double> are bitwise
  // compatible. They are both implemented as a pair of doubles, one
  // after the other.
  plan = fftw_plan_dft_r2c_2d(Nrow, Ncol, data_in.data(), 
			      reinterpret_cast<fftw_complex*>(data_out.data()),
			      FFTW_MEASURE | FFTW_DESTROY_INPUT);
}

//-----------------------------------------------------------------------
/// Destructor
//-----------------------------------------------------------------------

Fftw2dForward::~Fftw2dForward()
{
  fftw_destroy_plan(plan);
  fftw_free(data_in.data());
  fftw_free(data_out.data());
}

//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

Fftw2dBackward::Fftw2dBackward(int Nrow, int Ncol)
{
  // We use fftw_malloc because it makes sure we have any alignment
  // needed to get the best FFT performance.
  data_out.reference(Array<double, 2>
		    ((double*) fftw_malloc(sizeof(double) * Nrow * Ncol),
		     shape(Nrow, Ncol), neverDeleteData));
  data_in.reference(Array<std::complex<double>, 2>
                    ((std::complex<double>*)
		     fftw_malloc(sizeof(std::complex<double>) * Nrow * Ncol),
		     shape(Nrow, Ncol), neverDeleteData));
  // There is a long discussion in the FFTW documentation that says
  // fftw_complex and std::complex<double> are bitwise
  // compatible. They are both implemented as a pair of doubles, one
  // after the other.
  plan = fftw_plan_dft_c2r_2d(Nrow, Ncol, 
			      reinterpret_cast<fftw_complex*>(data_in.data()), 
			      data_out.data(), 
			      FFTW_MEASURE | FFTW_DESTROY_INPUT);
}

//-----------------------------------------------------------------------
/// Destructor
//-----------------------------------------------------------------------

Fftw2dBackward::~Fftw2dBackward()
{
  fftw_destroy_plan(plan);
  fftw_free(data_in.data());
  fftw_free(data_out.data());
}

