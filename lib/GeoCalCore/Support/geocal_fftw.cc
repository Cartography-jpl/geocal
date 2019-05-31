#include "geocal_fftw.h"
#include "geocal_exception.h"
#include "geocal_config.h"
#ifdef HAVE_FFTW
#include <fftw3.h>
#endif
using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

Fftw2dForward::Fftw2dForward(int Nrow, int Ncol)
{
#ifdef HAVE_FFTW  
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
  plan = (void*) fftw_plan_dft_r2c_2d(Nrow, Ncol, data_in.data(), 
			      reinterpret_cast<fftw_complex*>(data_out.data()),
			      FFTW_MEASURE | FFTW_DESTROY_INPUT);
#else
  throw Exception("Need FFTW to use Fftw2dForward");
#endif
}

//-----------------------------------------------------------------------
/// Run the FFT on the data in data_in, with the results going to
/// data_out. Note that data_in may possibly be destroyed.
//-----------------------------------------------------------------------

void Fftw2dForward::run_fft()
{
#ifdef HAVE_FFTW  
  fftw_execute((fftw_plan) plan);
#else
  throw Exception("Need FFTW to use Fftw2dForward");
#endif
}

//-----------------------------------------------------------------------
/// Destructor
//-----------------------------------------------------------------------

Fftw2dForward::~Fftw2dForward()
{
#ifdef HAVE_FFTW  
  fftw_destroy_plan((fftw_plan) plan);
  fftw_free(data_in.data());
  fftw_free(data_out.data());
#endif  
}

//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

Fftw2dBackward::Fftw2dBackward(int Nrow, int Ncol)
{
#ifdef HAVE_FFTW  
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
  plan = (void*) fftw_plan_dft_c2r_2d(Nrow, Ncol, 
			      reinterpret_cast<fftw_complex*>(data_in.data()), 
			      data_out.data(), 
			      FFTW_MEASURE | FFTW_DESTROY_INPUT);
#else
  throw Exception("Need FFTW to use Fftw2dBackward");
#endif
}

//-----------------------------------------------------------------------
/// Run the FFT on the data in data_in, with the results going to
/// data_out. Note that data_in may possibly be destroyed.
//-----------------------------------------------------------------------

void Fftw2dBackward::run_fft()
{
#ifdef HAVE_FFTW  
  fftw_execute((fftw_plan) plan);
#else
  throw Exception("Need FFTW to use Fftw2dBackward");
#endif
}

//-----------------------------------------------------------------------
/// Destructor
//-----------------------------------------------------------------------

Fftw2dBackward::~Fftw2dBackward()
{
#ifdef HAVE_FFTW  
  fftw_destroy_plan((fftw_plan) plan);
  fftw_free(data_in.data());
  fftw_free(data_out.data());
#endif  
}

