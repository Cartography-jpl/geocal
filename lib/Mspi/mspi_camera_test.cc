#include "mspi_camera.h"
#include "unit_test_support.h"
#include "constant.h"
#include <iostream>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(mspi_camera, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  MspiCamera cam(test_data_dir() + "mspi_camera_test.config");
  BOOST_CHECK(cam.file_name() == test_data_dir() + "mspi_camera_test.config");
  BOOST_CHECK_CLOSE(cam.epsilon(), 0, 1e-8);
  BOOST_CHECK_CLOSE(cam.psi(), 0, 1e-8);
  BOOST_CHECK_CLOSE(cam.theta(), 0, 1e-8);
  BOOST_CHECK_CLOSE(cam.boresight_angle(), 0, 1e-8);
  BOOST_CHECK_CLOSE(cam.yaw(), 180.0 * Constant::deg_to_rad, 1e-8);
  BOOST_CHECK_CLOSE(cam.pitch(), 90.0 * Constant::deg_to_rad, 1e-8);
  BOOST_CHECK_CLOSE(cam.roll(), 0, 1e-8);
  BOOST_CHECK_CLOSE(cam.focal_length(), 27.825088, 1e-8);
  BOOST_CHECK_CLOSE(cam.dx(), 0.010, 1e-8);
  BOOST_CHECK_CLOSE(cam.ypitch(), 0.010, 1e-8);
  BOOST_CHECK_CLOSE(cam.dy(), 0.016, 1e-8);
  BOOST_CHECK_CLOSE(cam.s_origin(), 767.5, 1e-8);
  BOOST_CHECK(cam.line_direction() == 1);
  BOOST_CHECK(cam.pixel_order() == 1);
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
    ScLookVector slv2 = cam.sc_look_vector(fc, b);
    BOOST_CHECK_CLOSE(fc.line, line_expect[b], 1e-8);
    BOOST_CHECK_CLOSE(fc.sample, sample_expect[b], 1e-8);
    // Because the paraxial inversion is only approximately a full
    // inversion, we don't get back exactly what we put in. Not sure
    // how much of a problem this might be, but it is what we have.
    BOOST_CHECK_CLOSE(slv2.direction()[0], slv.direction()[0], 1.5);
    BOOST_CHECK_CLOSE(slv2.direction()[1], slv.direction()[1], 1.5);
    BOOST_CHECK_CLOSE(slv2.direction()[2], slv.direction()[2], 1.5);
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
    // FrameCoordinate fc2 = cam.frame_coordinate(slv, b);
    // BOOST_CHECK(fabs(fc2.line - fc.line) < 0.01);
    // BOOST_CHECK(fabs(fc2.sample - fc.sample) < 0.01);
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
    // FrameCoordinate fc2 = cam.frame_coordinate(slv, b);
    // BOOST_CHECK(fabs(fc2.line - fc.line) < 0.01);
    // BOOST_CHECK(fabs(fc2.sample - fc.sample) < 0.01);
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
    // FrameCoordinate fc2 = cam.frame_coordinate(slv, b);
    // BOOST_CHECK(fabs(fc2.line - fc.line) < 0.01);
    // BOOST_CHECK(fabs(fc2.sample - fc.sample) < 0.01);
  }


  // std::cerr << cam.frame_coordinate(cam.sc_look_vector(FrameCoordinate(0,0), 0), 0) << "\n";
}

BOOST_AUTO_TEST_CASE(line_direction_reversed_test)
{
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

BOOST_AUTO_TEST_SUITE_END()
