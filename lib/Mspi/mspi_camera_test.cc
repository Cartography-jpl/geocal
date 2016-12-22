#include "unit_test_support.h"
#include "mspi_camera.h"
#include "constant.h"
#include <iostream>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(mspi_camera, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  // Note that this test data *does* not have the backward and forward
  // calculation calculation the same. Doing something like
  // frame_coordinate(sc_look_vector(FC,b),b) gives differences on the
  // order of 2 pixels. This is an artifact of the test data, and do
  // not indicate a problem with the code. But this data matches the
  // unit tests from the original code, so we want to use this to make
  // sure we match.
  MspiCamera cam(test_data_dir() + "mspi_camera_test.config");
  BOOST_CHECK(cam.file_name() == test_data_dir() + "mspi_camera_test.config");
  BOOST_CHECK_CLOSE(cam.epsilon(), 0, 1e-8);
  BOOST_CHECK_CLOSE(cam.psi(), 0, 1e-8);
  BOOST_CHECK_CLOSE(cam.theta(), 0, 1e-8);
  BOOST_CHECK_CLOSE(cam.boresight_angle(), 0, 1e-8);
  BOOST_CHECK_CLOSE(cam.yaw(), 180.0, 1e-8);
  BOOST_CHECK_CLOSE(cam.pitch(), 90.0, 1e-8);
  BOOST_CHECK_CLOSE(cam.roll(), 0, 1e-8);
  BOOST_CHECK_CLOSE(cam.focal_length(), 27.825088, 1e-8);
  BOOST_CHECK_CLOSE(cam.line_pitch(), 0.010, 1e-8);
  BOOST_CHECK_CLOSE(cam.sample_pitch(), 0.010, 1e-8);
  BOOST_CHECK_CLOSE(cam.principal_point(0).sample, 767.5, 1e-8);
  BOOST_CHECK(cam.line_direction() == QuaternionCamera::INCREASE_IS_NEGATIVE);
  BOOST_CHECK(cam.sample_direction() == QuaternionCamera::INCREASE_IS_POSITIVE);
  BOOST_CHECK(cam.number_band() == 7);
  BOOST_CHECK(cam.number_line(0) == 1);
  BOOST_CHECK(cam.number_sample(0) == 1536);
  BOOST_CHECK_CLOSE(cam.parameter()(0), cam.yaw(), 1e-8);
  BOOST_CHECK_CLOSE(cam.parameter()(1), cam.pitch(), 1e-8);
  BOOST_CHECK_CLOSE(cam.parameter()(2), cam.roll(), 1e-8);
  ScLookVector slv(27, -0.3, 0.4);
  const int nband = 7;
  // Results from old code unit test.
  double line_expect[nband] = {4.02840527542328388222e+01,
			       4.82840527542328388222e+01,
			       5.62840527542328388222e+01,
			       3.70840527542328430854e+01,
			       4.50840527542328359800e+01,
			       5.30840527542328430854e+01,
			       -1.07776474074074037901e+01};
  double sample_expect[nband] = {8.00175853880662771189e+02,
				 8.00175853880662771189e+02,
				 8.00175853880662771189e+02,
				 8.00175853880662771189e+02,
				 8.00175853880662771189e+02,
				 8.00175853880662771189e+02,
				 7.98416764444444424953e+02};
  for(int b = 0; b < cam.number_band(); ++b) {
    FrameCoordinate fc = cam.frame_coordinate(slv, b);
    BOOST_CHECK_CLOSE(fc.line, line_expect[b], 1e-8);
    BOOST_CHECK_CLOSE(fc.sample, sample_expect[b], 1e-8);
  }

  // Results from old code unit test.
  double direction_expect[nband][3] = {
    {9.66619417237047029445e-01,
     2.55774135797655954860e-01,
     -1.50497068833303302565e-02},
    {9.66607740287202310547e-01,
     2.55634495509085957199e-01,
     -1.79019865568946813517e-02},
    {9.66588016103036462567e-01,
     2.55492897390171169736e-01,
     -2.07602145743232650854e-02},
    {9.66634991935328646839e-01,
     2.55778256969776296437e-01,
     -1.39382792227167474214e-02},
    {9.66626379606913443965e-01,
     2.55639424967947448408e-01,
     -1.67906715198743762552e-02},
    {9.66609725960589138971e-01,
     2.55498635837488607958e-01,
     -1.96490397622165365032e-02},
    {9.63872717052457916864e-01,
     2.65865218588980323222e-01,
     1.62809970992600361317e-02}
  };

  FrameCoordinate fc(-5.0, 0);
  for(int b = 0; b < cam.number_band(); ++b) {
    ScLookVector slv = cam.sc_look_vector(fc, b);
    for(int i = 0; i < 3; ++i)
      BOOST_CHECK_CLOSE(slv.direction()[i], direction_expect[b][i], 1e-8);
  }

  double direction_expect2[nband][3] = {
    {9.66664123914725004205e-01,
     2.55785965491580502551e-01,
     -1.15763290259476535338e-02},
    {9.66662025013343084723e-01,
     2.55648851951728040000e-01,
     -1.44289255617417611771e-02},
    {9.66651897661589987187e-01,
     2.55509782851415012672e-01,
     -1.72875566367786809296e-02},
    {9.66675967222485499342e-01,
     2.55789099312144663223e-01,
     -1.04647535830482783098e-02},
    {9.66676933910773805714e-01,
     2.55652794842224806438e-01,
     -1.33174297154035146257e-02},
    {9.66669878342800381610e-01,
     2.55514535586047475402e-01,
     -1.61761679319347151385e-02},
    {9.63812579095920662908e-01,
     2.65848630723510781504e-01,
     1.97438071025929848257e-02}
  };

  fc = FrameCoordinate(5.0, 0);
  for(int b = 0; b < cam.number_band(); ++b) {
    ScLookVector slv = cam.sc_look_vector(fc, b);
    for(int i = 0; i < 3; ++i)
      BOOST_CHECK_CLOSE(slv.direction()[i], direction_expect2[b][i], 1e-8);
  }

  double direction_expect3[nband][3] = {
    {9.66664123914725004205e-01,
     -2.55785965491580502551e-01,
     -1.15763290259476535338e-02},
    {9.66662025013343084723e-01,
     -2.55648851951728040000e-01,
     -1.44289255617417611771e-02},
    {9.66651897661589987187e-01,
     -2.55509782851415012672e-01,
     -1.72875566367786809296e-02},
    {9.66675967222485499342e-01,
     -2.55789099312144663223e-01,
     -1.04647535830482783098e-02},
    {9.66676933910773805714e-01,
     -2.55652794842224806438e-01,
     -1.33174297154035146257e-02},
    {9.66669878342800381610e-01,
     -2.55514535586047475402e-01,
     -1.61761679319347151385e-02},
    {9.63812579095920662908e-01,
     -2.65848630723510781504e-01,
     1.97438071025929848257e-02}
  };
  fc = FrameCoordinate(5.0, 1535);
  for(int b = 0; b < cam.number_band(); ++b) {
    ScLookVector slv = cam.sc_look_vector(fc, b);
    for(int i = 0; i < 3; ++i)
      BOOST_CHECK_CLOSE(slv.direction()[i], direction_expect3[b][i], 1e-8);
  }
  double angular_separation_expect[nband] = 
    {-2.87509743311235464389e-03, 
     0.0, 
     2.87505465482229246102e-03,
     -4.02513773725820221683e-03, 
     -1.15003650156956832747e-03, 
     1.72504096818234002250e-03,
     -2.12735828800964844876e-02};

  for(int b = 0; b < cam.number_band(); ++b)
    BOOST_CHECK_CLOSE(cam.angular_separation(1, b), 
		      angular_separation_expect[b], 1e-8);

  double loff_expect[nband] = 
	{3.91114046925584588621e+01,
	 3.93225488022550777600e+01,
	 3.95515012995958485931e+01,
	 3.91114046925584588621e+01,
	 3.93225488022550777600e+01,
	 3.95515012995958485931e+01,
	 0.0};
  double soff_expect[nband] = 
	{3.12289783792453867761e+01,
	 3.16220033730039595810e+01,
	 3.20145509437610442660e+01,
	 3.12289783792453867761e+01,
	 3.16220033730039595810e+01,
	 3.20145509437610442660e+01,
	 0.0};

  FrameCoordinate f(-0.3, 0.1);
  for(int b = 0; b < cam.number_band(); ++b) {
    double loff, soff;
    cam.paraxial_offset(b, f, loff, soff);
    BOOST_CHECK_CLOSE(loff, loff_expect[b], 1e-8);
    BOOST_CHECK_CLOSE(soff, soff_expect[b], 1e-8);
  }
}

BOOST_AUTO_TEST_CASE(line_direction_reversed_test)
{
  // Note that this test data *does* not have the backward and forward
  // calculation calculation the same. Doing something like
  // frame_coordinate(sc_look_vector(FC,b),b) gives differences on the
  // order of 2 pixels. This is an artifact of the test data, and does
  // not indicate a problem with the code. But this data matches the
  // unit tests from the original code, so we want to use this to make
  // sure we match.

  // This is like mspi_camera_test.config, but line direction is reversed.

  MspiCamera cam(test_data_dir() + "mspi_camera_test.config2");
  const int nband = 7;

  double direction_expect[nband][3] = {
    {9.66664123914725226250e-01,
     2.55785965491580558062e-01,
     -1.15763290259476552685e-02},
    {9.66662025013343084723e-01,
     2.55648851951728040000e-01,
     -1.44289255617417611771e-02},
    {9.66651897661589987187e-01,
     2.55509782851415012672e-01,
     -1.72875566367786809296e-02},
    {9.66675967222485499342e-01,
     2.55789099312144663223e-01,
     -1.04647535830482783098e-02},
    {9.66676933910773805714e-01,
     2.55652794842224806438e-01,
     -1.33174297154035146257e-02},
    {9.66669878342800381610e-01,
     2.55514535586047475402e-01,
     -1.61761679319347151385e-02},
    {9.63812579095920662908e-01,
     2.65848630723510781504e-01,
     1.97438071025929848257e-02}
  };

  FrameCoordinate fc(-5.0, 0);
  for(int b = 0; b < cam.number_band(); ++b) {
    ScLookVector slv = cam.sc_look_vector(fc, b);
    for(int i = 0; i < 3; ++i)
      BOOST_CHECK_CLOSE(slv.direction()[i], direction_expect[b][i], 1e-8);
  }
}

BOOST_AUTO_TEST_CASE(airmisr_roundtrip_test)
{
  // Unlike the test data in mspi_camera_test.config, this is a real
  // camera model and we expect the round trip to work.
  MspiCamera cam(test_data_dir() + "AIRMSPI_CONFIG_CAMERA_MODEL_0003.config");
  for(int b = 0; b < cam.number_band(); ++b)
    for(double ln = -1.0; ln < 1.05; ln += 0.5) {
      for(int smp = 0; smp < cam.number_sample(b); smp += 10) {
	FrameCoordinate fc(ln, smp);
	FrameCoordinate fc2 = cam.frame_coordinate(cam.sc_look_vector(fc, b), b);
	BOOST_CHECK(fabs(fc.line - fc2.line) < 0.1);
	BOOST_CHECK(fabs(fc.sample - fc2.sample) < 0.1);
      }
    }
}

BOOST_AUTO_TEST_CASE(gndmisr_roundtrip_test)
{
  // Unlike the test data in mspi_camera_test.config, this is a real
  // camera model and we expect the round trip to work.
  MspiCamera cam(test_data_dir() + "GndMSPI_CONFIG_CAMERA_MODEL_V004.config");
  for(int b = 0; b < cam.number_band(); ++b)
    for(double ln = -1.0; ln < 1.05; ln += 0.5) {
      for(int smp = 0; smp < cam.number_sample(b); smp += 10) {
	FrameCoordinate fc(ln, smp);
	FrameCoordinate fc2 = cam.frame_coordinate(cam.sc_look_vector(fc, b), b);
	BOOST_CHECK(fabs(fc.line - fc2.line) < 0.1);
	BOOST_CHECK(fabs(fc.sample - fc2.sample) < 0.1);
      }
    }
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<Camera> cam(new MspiCamera(test_data_dir() + "AIRMSPI_CONFIG_CAMERA_MODEL_0003.config"));
  std::string d = serialize_write_string(cam);
  if(false)
    std::cerr << d;
  boost::shared_ptr<Camera> camr = serialize_read_string<Camera>(d);
  BOOST_CHECK_EQUAL(cam->number_band(), camr->number_band());
  for(int b = 0; b < cam->number_band(); ++b) {
    BOOST_CHECK_EQUAL(cam->number_sample(b), camr->number_sample(b));
    for(double ln = -1.0; ln < 1.05; ln += 0.5) {
      for(int smp = 0; smp < cam->number_sample(b); smp += 10) {
	FrameCoordinate fc(ln, smp);
	FrameCoordinate fc2 = 
	  camr->frame_coordinate(cam->sc_look_vector(fc, b), b);
	BOOST_CHECK(fabs(fc.line - fc2.line) < 0.1);
	BOOST_CHECK(fabs(fc.sample - fc2.sample) < 0.1);
	FrameCoordinate fc3 = 
	  cam->frame_coordinate(camr->sc_look_vector(fc, b), b);
	BOOST_CHECK(fabs(fc.line - fc3.line) < 0.1);
	BOOST_CHECK(fabs(fc.sample - fc3.sample) < 0.1);
      }
    }
  }
}
BOOST_AUTO_TEST_CASE(jac_test)
{
  // Compare finite difference jacobian to what we calculate using
  // AutoDerivative.
  MspiCamera cam(test_data_dir() + "AIRMSPI_CONFIG_CAMERA_MODEL_0003.config");
  // Print out parameter, useful if we are diagnosing and issue.
  if(0)
    for(int i = 0; i < cam.parameter().rows(); ++i)
      std::cerr << i << ": " << cam.parameter()(i)
		<< " " << cam.parameter_name()[i] << "\n";
  cam.add_identity_gradient();
  FrameCoordinate fc(-0.9, 100);
  ScLookVector slv = cam.sc_look_vector(fc, 0);
  // Camera doesn't exactly calculate reverse (just "pretty close").
  // Jacobian is sensitive to the difference, so we will use the
  // calculated inverse in the finite difference.
  FrameCoordinate fc_calc = cam.frame_coordinate(slv, 0);
  ScLookVectorWithDerivative slvwd = 
    cam.sc_look_vector_with_derivative(fc, 0);
  FrameCoordinateWithDerivative fcwd = 
    cam.frame_coordinate_with_derivative(slv, 0);
  const int parm_size = 3;
  blitz::Array<double, 1> eps(parm_size);
  eps = 1e-5,1e-5,1e-5;
  blitz::Array<double, 2> jac_fd(3, parm_size);
  blitz::Array<double, 2> jac_calc(3, parm_size);
  jac_calc(0, blitz::Range::all()) = slvwd.look_vector[0].gradient();
  jac_calc(1, blitz::Range::all()) = slvwd.look_vector[1].gradient();
  jac_calc(2, blitz::Range::all()) = slvwd.look_vector[2].gradient();
  blitz::Array<double, 2> jac2_fd(2, parm_size);
  blitz::Array<double, 2> jac2_calc(2, parm_size);
  jac2_calc(0, blitz::Range::all()) = fcwd.line.gradient();
  jac2_calc(1, blitz::Range::all()) = fcwd.sample.gradient();
  blitz::Array<double, 1> p0 = cam.parameter();
  for(int i = 0; i < parm_size; ++i) {
    blitz::Array<double, 1> p(p0.copy());
    p(i) += eps(i);
    cam.parameter(p);
    ScLookVector slvp = cam.sc_look_vector(fc, 0);
    for(int j = 0; j < 3; ++j)
      jac_fd(j, i) = (slvp.look_vector[j] - slv.look_vector[j]) / eps(i);
    FrameCoordinate fcp = cam.frame_coordinate(slv, 0);
    jac2_fd(0, i) = (fcp.line - fc_calc.line) / eps(i);
    jac2_fd(1, i) = (fcp.sample - fc_calc.sample) / eps(i);
  }
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac_calc, jac_fd, 1e-3);
  BOOST_CHECK_MATRIX_CLOSE_TOL(jac2_calc, jac2_fd, 1e-2);
}

BOOST_AUTO_TEST_SUITE_END()
