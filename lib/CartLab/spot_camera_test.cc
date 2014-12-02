#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include "geocal_serialize_common.h"
#include "unit_test_support.h"
#include "spot_camera.h"
using namespace GeoCal;

BOOST_CLASS_EXPORT(GeoCal::Camera);
BOOST_CLASS_EXPORT(GeoCal::SpotCamera);

BOOST_FIXTURE_TEST_SUITE(spot_camera, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  std::vector<double> psi_x(3), psi_y(3);
  psi_x[0] = -0.0095544910301;
  psi_x[1] = -0.0095545017004;
  psi_x[2] = -0.0095545133715;
  psi_y[0] = 0.067955961852;
  psi_y[1] = 0.067967964881;
  psi_y[2] = 0.067979966909;  
  SpotCamera c(psi_x, psi_y);
  FrameCoordinate f1(1, 1.5);
  ScLookVector sl = c.sc_look_vector(f1, 0);
  FrameCoordinate f2 = c.frame_coordinate(sl, 0);
  BOOST_CHECK_EQUAL(c.number_band(), 1);
  BOOST_CHECK_EQUAL(c.number_line(0), 1);
  BOOST_CHECK_EQUAL(c.number_sample(0), 3);
  BOOST_CHECK_CLOSE(f2.line, f1.line, 1e-4);
  BOOST_CHECK_CLOSE(f2.sample, f1.sample, 1e-4);
  f1.line = -1;
  f1.sample = -1;
  sl = c.sc_look_vector(f1,0);
  f2 = c.frame_coordinate(sl, 0);
  BOOST_CHECK_CLOSE(f2.line, f1.line, 1e-4);
  BOOST_CHECK_CLOSE(f2.sample, f1.sample, 1e-4);
  f1.sample = 4;
  sl = c.sc_look_vector(f1,0);
  f2 = c.frame_coordinate(sl, 0);
  BOOST_CHECK_CLOSE(f2.line, f1.line, 1e-4);
  BOOST_CHECK_CLOSE(f2.sample, f1.sample, 1e-4);

  sl = c.sc_look_vector(FrameCoordinate(1, 1.5), 0);
  FrameCoordinateWithDerivative f1d(AutoDerivative<double>(1, 0, 2), 
				    AutoDerivative<double>(1.5, 1, 2));
  ScLookVectorWithDerivative sld = c.sc_look_vector_with_derivative(f1d, 0);
  FrameCoordinateWithDerivative f2d = 
    c.frame_coordinate_with_derivative(sld, 0);
  BOOST_CHECK_CLOSE(sld.look_vector[0].value(), sl.look_vector[0], 1e-4);
  BOOST_CHECK_CLOSE(sld.look_vector[1].value(), sl.look_vector[1], 1e-4);
  BOOST_CHECK_CLOSE(sld.look_vector[2].value(), sl.look_vector[2], 1e-4);
  BOOST_CHECK_CLOSE(f2d.line.value(), f1d.line.value(), 1e-4);
  BOOST_CHECK_MATRIX_CLOSE(f2d.line.gradient(), f1d.line.gradient());
  BOOST_CHECK_CLOSE(f2d.sample.value(), f1d.sample.value(), 1e-4);
  BOOST_CHECK_MATRIX_CLOSE(f2d.sample.gradient(), f1d.sample.gradient());
  double eps = 0.01;
  ScLookVector slv2 = c.sc_look_vector(FrameCoordinate(1 + eps, 1.5), 0);
  BOOST_CHECK_CLOSE(sld.look_vector[0].gradient()(0), 
		    (slv2.look_vector[0] - sl.look_vector[0]) / eps, 1e-4);
  BOOST_CHECK_CLOSE(sld.look_vector[1].gradient()(0), 
   		    (slv2.look_vector[1] - sl.look_vector[1]) / eps, 1e-4);
  BOOST_CHECK_CLOSE(sld.look_vector[2].gradient()(0), 
		    (slv2.look_vector[2] - sl.look_vector[2]) / eps, 1e-4);
  slv2 = c.sc_look_vector(FrameCoordinate(1, 1.5 + eps), 0);
  BOOST_CHECK_CLOSE(sld.look_vector[0].gradient()(1), 
		    (slv2.look_vector[0] - sl.look_vector[0]) / eps, 1e-4);
  BOOST_CHECK_CLOSE(sld.look_vector[1].gradient()(1), 
		    (slv2.look_vector[1] - sl.look_vector[1]) / eps, 1e-4);
  BOOST_CHECK_CLOSE(sld.look_vector[2].gradient()(1), 
		    (slv2.look_vector[2] - sl.look_vector[2]) / eps, 1e-4);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  std::ostringstream os;
  boost::archive::xml_oarchive oa(os);

  std::vector<double> psi_x(3), psi_y(3);
  psi_x[0] = -0.0095544910301;
  psi_x[1] = -0.0095545017004;
  psi_x[2] = -0.0095545133715;
  psi_y[0] = 0.067955961852;
  psi_y[1] = 0.067967964881;
  psi_y[2] = 0.067979966909;  
  boost::shared_ptr<Camera> cam(new SpotCamera(psi_x, psi_y));
  FrameCoordinate f1(1, 1.5);
  ScLookVector sl = cam->sc_look_vector(f1, 0);
  oa << BOOST_SERIALIZATION_NVP(cam);
  if(false)
    std::cerr << os.str();
  
  std::istringstream is(os.str());
  boost::archive::xml_iarchive ia(is);
  boost::shared_ptr<Camera> camr;
  ia >> BOOST_SERIALIZATION_NVP(camr);

  BOOST_CHECK_EQUAL(camr->number_band(), 1);
  BOOST_CHECK_EQUAL(camr->number_line(0), 1);
  BOOST_CHECK_EQUAL(camr->number_sample(0), 3);
  FrameCoordinate f2 = camr->frame_coordinate(sl, 0);
  BOOST_CHECK_CLOSE(f2.line, f1.line, 1e-4);
  BOOST_CHECK_CLOSE(f2.sample, f1.sample, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
