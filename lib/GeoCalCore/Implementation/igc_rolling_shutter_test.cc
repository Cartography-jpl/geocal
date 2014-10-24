#include "unit_test_support.h"
#include "igc_rolling_shutter.h"
#include "quaternion_camera.h"
#include "simple_dem.h"
#include "memory_raster_image.h"
#include "ecr.h"
using namespace GeoCal;
using namespace blitz;
// Don't have a TimeTable like this yet, so we create here.

class RollingShutterConstantSpacingTimeTable : public TimeTable {
public:
  RollingShutterConstantSpacingTimeTable(Time Min_time, Time Max_time, 
			   double Time_space = 40.8e-3)
  : min_t(Min_time),
    tspace(Time_space)
  {
    max_l = (int) round((Max_time - Min_time) / tspace);
  }
  virtual ~RollingShutterConstantSpacingTimeTable() {}
  virtual ImageCoordinate image_coordinate(Time T, const FrameCoordinate& F)
    const
  {
    double line = (T  - min_time()) / tspace;
    return ImageCoordinate(line, F.sample);
  }
  virtual ImageCoordinateWithDerivative 
  image_coordinate_with_derivative(const TimeWithDerivative& T, 
				   const FrameCoordinateWithDerivative& F)
    const
  {
    AutoDerivative<double> line = (T  - min_time()) / tspace;
    return ImageCoordinateWithDerivative(line, F.sample);
  }
  virtual void print(std::ostream& Os) const
  { std::cerr << "RollingShutterConstantSpacingTimeTable\n"; }
  virtual void time(const ImageCoordinate& Ic, Time& T, FrameCoordinate& F)
    const
  {
    range_check(Ic.line, (double) min_line(), (double) max_line() + 1.0);
    T = min_time() + Ic.line * tspace;
    F = FrameCoordinate(Ic.line, Ic.sample);
  }
  virtual void time_with_derivative(const ImageCoordinateWithDerivative& Ic, 
				    TimeWithDerivative& T, 
				    FrameCoordinateWithDerivative& F) const
  {
    range_check(Ic.line.value(), (double) min_line(), 
		(double) max_line() + 1.0);
    T = TimeWithDerivative(min_time()) + Ic.line * tspace;
    F = FrameCoordinateWithDerivative(Ic.line, Ic.sample);
  }
  virtual int min_line() const {return 0;}
  virtual int max_line() const {return max_l;}
  virtual Time min_time() const {return min_t;}
  virtual Time max_time() const {return min_t + tspace * max_l;}
  double time_space() const {return tspace;}
private:
  Time min_t;
  int max_l;
  double tspace;
};

class IgcRollingShutterFixture : public GlobalFixture {
public:
  IgcRollingShutterFixture() 
  {
    tmin = Time::parse_time("2003-01-01T11:11:00Z");
    orb.reset(new KeplerOrbit());
    cam.reset(new QuaternionCamera(quat_rot("zyx", 0.1, 0.2, 0.3),
				   3375, 3648, 1.0 / 2500000, 1.0 / 2500000,
				   1.0, FrameCoordinate(1688.0, 1824.5),
				   QuaternionCamera::LINE_IS_Y));
    dem.reset(new SimpleDem(100));
    boost::shared_ptr<RasterImage> 
      img(new MemoryRasterImage(cam->number_line(0),
				cam->number_sample(0)));
    tspace = 1e-3;
    tt.reset(new RollingShutterConstantSpacingTimeTable(tmin, 
	tmin + cam->number_line(0) * tspace, tspace));
    igc.reset(new IgcRollingShutter(orb, tt, cam, dem, img));
  }
  Time tmin;
  boost::shared_ptr<Orbit> orb;
  boost::shared_ptr<QuaternionCamera> cam;
  boost::shared_ptr<Dem> dem;
  double tspace;
  boost::shared_ptr<TimeTable> tt;
  boost::shared_ptr<IgcRollingShutter> igc;
};

BOOST_FIXTURE_TEST_SUITE(igc_rolling_shutter, IgcRollingShutterFixture)

BOOST_AUTO_TEST_CASE(resolution)
{
  // Check resolution calculation. We just compare to expected results
  // we previously verified by hand.
  
  BOOST_CHECK_CLOSE(igc->resolution_meter(), 6.59975059, 1e-2);
}

BOOST_AUTO_TEST_CASE(footprint_resolution)
{
  // Check resolution calculation. We just compare to expected results
  // we previously verified by hand.
  
  double lres, sres;
  igc->footprint_resolution(igc->number_line() / 2, igc->number_sample() / 2,
			    lres, sres);
  BOOST_CHECK_CLOSE(lres, 0.324327514, 1e-2);
  BOOST_CHECK_CLOSE(sres, 0.314864123, 1e-2);
}

BOOST_AUTO_TEST_CASE(ground_position)
{
  // Check ground and spacecraft position by directly calculating
  ImageCoordinate ic(100, 200);
  boost::shared_ptr<OrbitData> od = orb->orbit_data(tmin + 100 * tspace);
  boost::shared_ptr<GroundCoordinate> gc = igc->ground_coordinate(ic);
  boost::shared_ptr<GroundCoordinate> gcexpect =
    od->surface_intersect(*cam, FrameCoordinate(100, 200), *dem, 1, 0, 100);
  BOOST_CHECK(GeoCal::distance(*gc, *gcexpect) < 1e-2);
}

BOOST_AUTO_TEST_CASE(image_coordinate)
{

  // Check we get back to the original image_coordinate. Note that ==
  // is already done with a tolerance for ImageCoordinate
  ImageCoordinate ic(100, 200);
  BOOST_CHECK(igc->image_coordinate(*igc->ground_coordinate(ic)) == ic);
}

BOOST_AUTO_TEST_CASE(cf_look_vector)
{
  // Check look vector and spacecraft position by directly calculating
  ImageCoordinate ic(100, 200);
  boost::shared_ptr<OrbitData> od = orb->orbit_data(tmin + 100 * tspace);
  BOOST_CHECK(GeoCal::distance(*igc->cf_look_vector_pos(ic), *od->position_cf())
	      < 1e-2);
  CartesianFixedLookVector cl_expect = od->cf_look_vector
    (cam->sc_look_vector(FrameCoordinate(100,200), 0));
  BOOST_CHECK_CLOSE(cl_expect.look_vector[0], 
		    igc->cf_look_vector_lv(ic)->look_vector[0], 1e-2);
  BOOST_CHECK_CLOSE(cl_expect.look_vector[1],
		    igc->cf_look_vector_lv(ic)->look_vector[1], 1e-2);
  BOOST_CHECK_CLOSE(cl_expect.look_vector[2],
		    igc->cf_look_vector_lv(ic)->look_vector[2], 1e-2);
}

BOOST_AUTO_TEST_CASE(cf_look_vector_arr)
{
  // Test cf_look_vector_lv by comparing a point of the array with a
  // direct calculation.
  int lstart = 10;
  int nline = 4;
  int sstart = 20;
  int nsamp = 5;
  int nsub_line = 3;
  int nsub_samp = 4;
  int integration_step = 0;
  int nintegration_step = 2;
  int line_check = 12;
  int samp_check = 23;
  int subline_check = 2;
  int subsamp_check = 2;
  Array<double, 7> cf_lv = igc->cf_look_vector_arr(lstart, sstart, nline, 
			  nsamp, nsub_line, nsub_samp, nintegration_step);
  boost::shared_ptr<OrbitData> od = orb->orbit_data(tmin + line_check * tspace);
  Array<double, 2> cf_lv_check = 
    cf_lv(line_check-lstart, samp_check-sstart, 
	  subline_check,subsamp_check,integration_step,
	  Range::all(), Range::all());
  Ecr pexpect(cf_lv_check(0,0), cf_lv_check(0,1), cf_lv_check(0,2));
  BOOST_CHECK(distance(*od->position_cf(), pexpect) < 1e-2);
  CartesianFixedLookVector cl_expect = od->cf_look_vector
    (cam->sc_look_vector(FrameCoordinate
			 (line_check + subline_check/((double) nsub_line), 
			  samp_check + subsamp_check/((double) nsub_samp)), 0));
  BOOST_CHECK_CLOSE(cl_expect.look_vector[0], cf_lv_check(1,0), 1e-2);
  BOOST_CHECK_CLOSE(cl_expect.look_vector[1], cf_lv_check(1,1), 1e-2);
  BOOST_CHECK_CLOSE(cl_expect.look_vector[2], cf_lv_check(1,2), 1e-2);
}

BOOST_AUTO_TEST_CASE(jacobian)
{
  // Note we only check the camera jacobian. However this is enough to
  // make sure the propagation goes through correctly, the use of
  // orbit parameter jacobians is handled exactly the same way by the
  // code. 
  ArrayAd<double, 1> p2(cam->parameter());
  p2.resize_number_variable(p2.rows());
  for(int i = 0; i < p2.rows(); ++i)
    p2.jacobian()(i,i) = 1.0;
  cam->parameter_with_derivative(p2);
  blitz::Array<double, 1> eps(8);
  eps = 0.00001, 0.00001, 0.00001, 1e-9, 1e-9, 0.001, 0.1, 0.1;
  Array<double, 2> jac_fd(2, 8);
  Array<double, 1> p0(cam->parameter());
  boost::shared_ptr<GroundCoordinate> gc = 
    igc->ground_coordinate(ImageCoordinate(100,200));
  ImageCoordinate ic0 = igc->image_coordinate(*gc);
  for(int i = 0; i < eps.rows(); ++i) {
    Array<double, 1> p = p0.copy();
    p(i) += eps(i);
    cam->parameter(p);
    ImageCoordinate ic = igc->image_coordinate(*gc);
    jac_fd(0,i) = (ic.line - ic0.line) / eps(i);
    jac_fd(1,i) = (ic.sample - ic0.sample) / eps(i);
  }
  std::cerr << jac_fd << "\n";
}

BOOST_AUTO_TEST_SUITE_END()

