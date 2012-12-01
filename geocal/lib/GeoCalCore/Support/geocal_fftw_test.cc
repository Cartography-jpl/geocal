#include "unit_test_support.h"
#include "geocal_fftw.h"

using namespace GeoCal;
using namespace blitz;
BOOST_FIXTURE_TEST_SUITE(geocal_fftw, GlobalFixture)

BOOST_AUTO_TEST_CASE(set_of_point)
{
// Create a simple lattice and then take the FFT of it
  Fftw2dForward fft(256, 256);
  fft.data_in = 0;
  fft.data_in(128, 128) = 1.0;
  fft.data_in(128, 148) = 1.0;
  fft.data_in(128, 158) = 1.0;
  fft.data_in(128 + 10, 128 + 3) = 1.0;
  fft.data_in(128 + 10, 148 + 3) = 1.0;
  fft.data_in(128 + 10, 158 + 3) = 1.0;
  fft.data_in(128 + 2 * 10, 128 + 2 * 3) = 1.0;
  fft.data_in(128 + 2 * 10, 148 + 2 * 3) = 1.0;
  fft.data_in(128 + 2 * 10, 158 + 2 * 3) = 1.0;
  fft.data_in(128 + 3 * 10, 128 + 3 * 3) = 1.0;
  fft.data_in(128 + 3 * 10, 148 + 3 * 3) = 1.0;
  fft.data_in(128 + 3 * 10, 158 + 3 * 3) = 1.0;
  fft.data_in(128 + 4 * 10, 128 + 4 * 3) = 1.0;
  fft.data_in(128 + 4 * 10, 148 + 4 * 3) = 1.0;
  fft.data_in(128 + 4 * 10, 158 + 4 * 3) = 1.0;
  // Keep a copy around, because run_fft may destroy data_in.
  Array<double, 2> data_in_copy(fft.data_in.copy());
  fft.run_fft();
  // Spot check a couple of values
  BOOST_CHECK_CLOSE(real(fft.data_out(0, 0)), 15, 1e-6);
  BOOST_CHECK_CLOSE(imag(fft.data_out(0, 0)), 0, 1e-6);
  BOOST_CHECK_CLOSE(real(fft.data_out(10, 5)), -1.8765187905225584, 1e-6);
  BOOST_CHECK_CLOSE(imag(fft.data_out(10, 5)), 4.3250076491242302, 1e-6);

  // Check that going backwards gives the original data. Note that the
  // FFTW is unnormalized, so going forwards and then backwards gives
  // Nrow * Ncol * original data.
  Fftw2dBackward fftback(256, 256);
  fftback.data_in = fft.data_out;
  fftback.run_fft();
  BOOST_CHECK_MATRIX_CLOSE(fftback.data_out / (256 * 256), data_in_copy);
}

BOOST_AUTO_TEST_SUITE_END()

