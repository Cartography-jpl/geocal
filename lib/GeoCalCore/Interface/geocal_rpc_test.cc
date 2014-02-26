#include "unit_test_support.h"
#include "geocal_rpc.h"
#include "geodetic.h"
#include "simple_dem.h"
#include "argus_camera.h"
#include "orbit_data_image_ground_connection.h"
#include "memory_raster_image.h"
#include "ecr.h"
#include "srtm_dem.h"
#include "vicar_lite_file.h"
#include "dem_map_info_offset.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(rpc, GlobalFixture)
BOOST_AUTO_TEST_CASE(generate_rpc_test)
{
  boost::shared_ptr<Camera> c(new ArgusCamera(0, 10, 0));
  Time t = Time::time_pgs(100);
  // We directly set up the OrbitData. This allows us to run this test
  // on systems without SPICE or the SDP toolkit, since orb.orbit_data
  // requires conversion from ECI to ECR. This just comes from the
  // commands:
  //   KeplerOrbit orb(t, t + 100.0);
  //   boost::shared_ptr<OrbitData> od = orb.orbit_data(t + 50.0);
  const boost::shared_ptr<CartesianFixed> pos(
	      new Ecr(6340038.96377099, -3153209.32145001, -371293.463374568));
  boost::array<double, 3> vel = {{-7414.2, 1101.67, -7485.97}};
  boost::math::quaternion<double> qt(0.167454,0.528917,-0.440527,-0.705791);
  boost::shared_ptr<OrbitData> od(new QuaternionOrbitData(t + 50.0, pos, 
							  vel, qt));
  boost::shared_ptr<CartesianFixed> t1 = 
    od->reference_surface_intersect_approximate(*c, FrameCoordinate(0,0));
  boost::shared_ptr<CartesianFixed> t2 = 
    od->reference_surface_intersect_approximate(*c, 
	FrameCoordinate(c->number_line(0),c->number_sample(0)));
  boost::shared_ptr<Dem> d(new SimpleDem());
  boost::shared_ptr<RasterImage> img(new MemoryRasterImage(c->number_line(0),
						   c->number_sample(0)));
  OrbitDataImageGroundConnection igc(od, c, d, img);
  Rpc rpc = Rpc::generate_rpc(igc, -100, 100);
  double maxln_diff = 0.0, maxsmp_diff = 0.0;
  for(double lat = std::min(t1->latitude(), t2->latitude()); 
      lat < std::max(t1->latitude(), t2->latitude()); lat += 0.01)
    for(double lon = std::min(t1->longitude(), t2->longitude()); 
	lon < std::max(t1->longitude(), t2->longitude()); lon += 0.01)
      for(double height = -100; height < 100; height += 10) {
	ImageCoordinate ic1 = rpc.image_coordinate(Geodetic(lat, lon, height));
	FrameCoordinate ic2 = od->frame_coordinate(Geodetic(lat, lon, height), 
						   *c);
	double ln_diff = fabs(ic1.line - ic2.line);
	double smp_diff = fabs(ic1.sample - ic2.sample);
	maxln_diff = std::max(ln_diff, maxln_diff);
	maxsmp_diff = std::max(smp_diff, maxsmp_diff);
      }
  BOOST_CHECK(maxln_diff < 1e-3);
  BOOST_CHECK(maxsmp_diff < 1e-3);
}

BOOST_AUTO_TEST_CASE(basic_test)
{
  Rpc rpc;
  rpc.rpc_type = Rpc::RPC_B;
  rpc.line_offset = 2881;
  rpc.line_scale = 2921;
  rpc.sample_offset = 13763;
  rpc.sample_scale = 14238;
  rpc.latitude_offset = 35.8606;
  rpc.latitude_scale = 0.0209;
  rpc.longitude_offset = 44.9534;
  rpc.longitude_scale = 0.1239;
  rpc.height_offset = 1017;
  rpc.height_scale = 634;
  boost::array<double, 20> t1 = 
    {{ 0.0132748, -0.14751, -1.13465, -0.0138959, 0.0020018,
      6.35242e-05, 0.000115861, -0.00286551, -0.00778726, 
      3.88308e-06, -1.26487e-06, 7.881069999999999e-06, 
      3.65929e-05, 2.32154e-06, -2.25421e-05, -2.08933e-05, 
       1.8091e-05, 3.6393e-07, -9.39815e-07, -4.31269e-08 }};
  rpc.line_numerator = t1;
  boost::array<double, 20> t2 = 
    {{ 1, 0.00380256, 0.00643151, 0.00031479,
      1.84958e-05, -1.98243e-06, -1.16422e-06,
      -1.92637e-05, 7.224010000000001e-05, -1.61779e-05,
      4.95049e-06, 1.26794e-06, 0.000190771, -1.57429e-07,
      2.46815e-05, 0.00049166, -5.41022e-07, 3.209e-07,
       1.81401e-05, 1.43592e-07}};
  rpc.line_denominator = t2;
  boost::array<double, 20> t3 = 
    {{ -0.0104025, 0.96885, -0.000487887, -0.0325142,
      -0.000710444, 0.000217572, -6.549690000000001e-05,
      0.0107384, -5.19453e-06, -1.76761e-05, -1.21058e-06,
      0.000106017, 5.41226e-06, -3.8968e-06, 1.03307e-05,
      5.84016e-05, 3.80777e-08, 9.01765e-06, 1.65988e-06,
       -1.19374e-07}};
  rpc.sample_numerator = t3;
  boost::array<double, 20> t4 = 
    {{1, -0.000319831, 0.000681092, -0.000549762,
     -2.67977e-06, -6.19388e-06, 2.67975e-08, 4.76371e-06,
     -1.47985e-05, -4.23457e-06, 1.44009e-08, -1.07213e-06,
     1.1059e-07, 4.10217e-08, -1.69482e-07, 1.08104e-06,
      1e-9, -2.33038e-07, 1.86413e-08, -1.35637e-08}};
  rpc.sample_denominator = t4;
    
  BOOST_CHECK(rpc.print_to_string() != "");
				// String too long to check, so
				// just make sure call returns
				// something.

  ImageCoordinate ic_expect(5729.22, 27561.36);
  Geodetic g(35.8399968, 45.0770183, 1017);
  BOOST_CHECK_EQUAL(rpc.image_coordinate(g), ic_expect);
  blitz::Array<double, 2> ic_jac = 
    rpc.image_coordinate_jac(g.latitude(),
			     g.longitude(),
			     g.height_reference_surface());
  ImageCoordinate ic2;
  double eps = 1e-4;
  ic2 = rpc.image_coordinate(g.latitude() + eps, g.longitude(),
			     g.height_reference_surface());
  BOOST_CHECK_CLOSE(ic_jac(0, 0), (ic2.line - ic_expect.line) / eps, 1.0);
  BOOST_CHECK_CLOSE(ic_jac(1, 0), (ic2.sample - ic_expect.sample) / eps, 1.5);
  ic2 = rpc.image_coordinate(g.latitude(), g.longitude() + eps,
			     g.height_reference_surface());
  BOOST_CHECK_CLOSE(ic_jac(0, 1), (ic2.line - ic_expect.line) / eps, 1.5);
  BOOST_CHECK_CLOSE(ic_jac(1, 1), (ic2.sample - ic_expect.sample) / eps, 1.0);
  ic2 = rpc.image_coordinate(g.latitude(), g.longitude(),
			     g.height_reference_surface() + 10);
  BOOST_CHECK_CLOSE(ic_jac(0, 2), (ic2.line - ic_expect.line) / 10, 1.0);
  BOOST_CHECK_CLOSE(ic_jac(1, 2), (ic2.sample - ic_expect.sample) / 10, 1.0);
  blitz::Array<double, 1> lat(1), lon(1), height(1);
  lat = 35.8399968; lon = 45.0770183; height = 1017;
  blitz::Array<double, 2> r = rpc.image_coordinate(lat, lon, height);
  BOOST_CHECK_CLOSE(r(0, 0), ic_expect.line, 1e-4);
  BOOST_CHECK_CLOSE(r(1, 0), ic_expect.sample, 1e-4);
  Geodetic gcalc = rpc.ground_coordinate(ic_expect, 1017);
  BOOST_CHECK(distance(gcalc, g) < 0.1);
  SimpleDem dem(1017);
  Geodetic gcalc2 = rpc.ground_coordinate(ic_expect, dem);
  BOOST_CHECK(distance(gcalc2, g) < 0.1);

//-----------------------------------------------------------------------
// Test fit function.
//-----------------------------------------------------------------------

  std::vector<boost::shared_ptr<GroundCoordinate> > gc;
  std::vector<ImageCoordinate> ic;
  std::vector<double> line_sigma;
  std::vector<double> sample_sigma;
  for(double lat = 35.8399968; lat < 35.8819; 
      lat += (35.8819 - 35.8399968) / 20)
    for(double lon = 44.8264539; lon < 45.0770183;
	lon += (45.0770183 - 44.8264539) / 20) {
      double height = 1017 + (lon - 45.0770183) * 1000;
      gc.push_back(boost::shared_ptr<GroundCoordinate>
		   (new Geodetic(lat, lon, height)));
      ic.push_back(rpc.image_coordinate(*gc.back()));
      line_sigma.push_back(0.1);
      sample_sigma.push_back(0.1);
    }
  Rpc rpc2(rpc);
  for(int i = 0; i < 3; ++i) {
    rpc2.line_numerator[i] = 0;
    rpc2.fit_line_numerator[i] = true;
    rpc2.sample_numerator[i] = 0;
    rpc2.fit_sample_numerator[i] = true;
  }
  std::set<int> blunder;
  rpc2.fit(gc, ic, line_sigma, sample_sigma, blunder);
  for(int i = 0; i < 3; ++i) {
    BOOST_CHECK_CLOSE(rpc2.line_numerator[i], rpc.line_numerator[i], 0.1);
    BOOST_CHECK_CLOSE(rpc2.sample_numerator[i], rpc.sample_numerator[i], 0.1);
  }
  eps = 1e-4;
  blitz::Array<double, 2> jac = rpc2.image_coordinate_jac_parm(g);
  ImageCoordinate ic0 = rpc2.image_coordinate(g);
  for(int i = 0; i < 3; ++i) {
    rpc2.line_numerator[i] += eps;
    rpc2.sample_numerator[i] += eps;
    ImageCoordinate ic2 = rpc2.image_coordinate(g);
    BOOST_CHECK_CLOSE((ic2.line - ic0.line) / eps, jac(0, i), 1e-2);
    BOOST_CHECK_CLOSE((ic2.sample - ic0.sample) / eps, jac(1, 3 + i), 1e-2);
    rpc2.line_numerator[i] -= eps;
    rpc2.sample_numerator[i] -= eps;
  }

//-----------------------------------------------------------------------
// Test fit_all function.
//-----------------------------------------------------------------------

  std::vector<double> line, sample, latitude, longitude, h2;
  for(double lat = 35.8399968; lat < 35.8819; 
      lat += (35.8819 - 35.8399968) / 20)
    for(double lon = 44.8264539; lon < 45.0770183;
	lon += (45.0770183 - 44.8264539) / 20) {
      for(double h = 0; h < 2000; h += 100) {
	latitude.push_back(lat);
	longitude.push_back(lon);
	h2.push_back(h);
	ImageCoordinate ic = rpc.image_coordinate(Geodetic(lat, lon, h));
	line.push_back(ic.line);
	sample.push_back(ic.sample);
      }
    }
  Rpc rpc3(rpc);
  for(int i = 0; i < 20; ++i) {
    rpc3.line_numerator[i] = 0;
    rpc3.sample_numerator[i] = 0;
    if(i > 1) {
      rpc3.line_denominator[i] = 0;
      rpc3.sample_denominator[i] = 0;
    }
  }
  rpc3.fit_all(line, sample, latitude, longitude, h2);
  for(int i = 0; i < 20; ++i) {
    BOOST_CHECK_CLOSE(rpc3.line_numerator[i], rpc.line_numerator[i], 0.20);
    BOOST_CHECK_CLOSE(rpc3.sample_numerator[i], rpc.sample_numerator[i], 0.20);
    BOOST_CHECK_CLOSE(rpc3.line_denominator[i], rpc.line_denominator[i], 0.20);
    BOOST_CHECK_CLOSE(rpc3.sample_denominator[i], rpc.sample_denominator[i], 0.20);
  }

//-----------------------------------------------------------------------
/// Test A type.
//-----------------------------------------------------------------------

  Rpc rpc4 = rpc.rpc_type_a();
  r = rpc4.image_coordinate(lat, lon, height);
  BOOST_CHECK_CLOSE(r(0, 0), ic_expect.line, 1e-4);
  BOOST_CHECK_CLOSE(r(1, 0), ic_expect.sample, 1e-4);
  Rpc rpc5 = rpc4.rpc_type_b();
  for(int i = 0; i < 20; ++i) {
    BOOST_CHECK_CLOSE(rpc5.line_numerator[i], rpc.line_numerator[i], 1e-4);
    BOOST_CHECK_CLOSE(rpc5.sample_numerator[i], rpc.sample_numerator[i], 1e-4);
    BOOST_CHECK_CLOSE(rpc5.line_denominator[i], rpc.line_denominator[i], 1e-4);
    BOOST_CHECK_CLOSE(rpc5.sample_denominator[i], rpc.sample_denominator[i], 1e-4);
  }
}

BOOST_AUTO_TEST_CASE(inverse_fails)
{
  return;    // Don't normally run this, it depends on data we don't
	     // normally have

  // This was an inverse test case that failed to calculate
  // ground_coordinate. Extract this out so we can look at this in
  // more detail and see what it is going on here.

  Rpc rpc;
  rpc.rpc_type = Rpc::RPC_B;
  rpc.line_offset = 23669;
  rpc.line_scale = 23670;
  rpc.sample_offset = 17589;
  rpc.sample_scale = 17590;
  rpc.longitude_offset = -117.887;; 
  rpc.longitude_scale = 0.1149;
  rpc.latitude_offset = 34.1027;
  rpc.latitude_scale = 0.1458;
  rpc.height_offset = 705;
  rpc.height_scale = 656;
  boost::array<double, 20> t1 = 
    {{ -0.00838895, 0.0922011, -1.11169, -0.0195271, 0.000288563,
       4.28151e-05, -0.000172025, -0.000233032, 0.00767244,
       -3.24008e-06, -2.31889e-07, -1.52355e-06, -2.10979e-05,
       -2.0696e-06, 2.20435e-05, 4.27367e-05, 2.49598e-05, 2.81582e-07,
       2.75618e-06, 4.41884e-07}};
  rpc.line_numerator = t1;
  boost::array<double, 20> t2 = 
    {{ 1, 0.00111794, -0.000852869, -0.000359327, -2.47099e-05,
       8.88349e-07, 4.48492e-06, -9.51672e-06, 7.1117e-05,
       -2.2577e-05, -6.84602e-07, -9.23611e-08, -1.9514e-05,
       -4.87803e-08, 1.52217e-06, 7.75716e-05, -6.56811e-08,
       3.98379e-08, 4.15723e-06, 1.45924e-08}};
  rpc.line_denominator = t2;
  boost::array<double, 20> t3 = 
    {{ 0.00983618, 1.02525, -0.0031443, 0.0187995, -0.00309236,
       0.000475726, -0.000251547, -0.00792802, 2.17018e-05,
       4.9027e-06, 1.53072e-06, 2.79616e-05, -8.35535e-05,
       -1.97936e-05, 8.50012e-05, -2.63949e-05, -2.02596e-07,
       -7.59024e-06, -1.04969e-06, -3.69081e-07}};
  rpc.sample_numerator = t3;
  boost::array<double, 20> t4 = 
    {{1, -0.00195409, 0.00320623, -0.000673322, -5.13971e-05,
      4.2353e-06, 3.27826e-06, 6.80956e-06, 6.86083e-05,
      -1.99026e-05, -2.03299e-07, -4.49819e-08, -9.30909e-07,
      -1.02235e-07, 1.24731e-06, 2.11734e-07, -1.23425e-07,
      -5.28526e-08, 3.22005e-07, 2.41005e-08}};
  rpc.sample_denominator = t4;
  SrtmDem dem;
  SimpleDem dem2;
  ImageCoordinate ic(12300.07480763955, 11274.740393180751);
  Geodetic g2 = rpc.ground_coordinate(ic, dem2);
  Geodetic g = rpc.ground_coordinate(ic, dem);
}

BOOST_AUTO_TEST_CASE(inverse_fails2)
{
  return;    // Don't normally run this, since it depends on data that
             // we normally don't have.

  // Another inverse that fails. This depends on test data that we
  // don't normally have available, so this will not normally be run.
  // But have this in place so we can investigate what is going on
  // here.

  VicarLiteRasterImage f("/home/smyth/Local/ShivaEndToEnd/syria1/pre_pan.img");
  Rpc rpc = f.rpc();
  boost::shared_ptr<DemMapInfo> dem1(new VicarLiteDem("/raid10/sba_gold/syria1/syria1_dem.hlf"));
  DemMapInfoOffset dem(dem1, 20.5801);
  ImageCoordinate ic(8862, 14383);
  Geodetic g = rpc.ground_coordinate(ic, dem);
  ImageCoordinate ic2 = rpc.image_coordinate(g);
  BOOST_CHECK_CLOSE(ic.line, ic2.line, 1e-4);
  BOOST_CHECK_CLOSE(ic.sample, ic2.sample, 1e-4);
}
BOOST_AUTO_TEST_SUITE_END()
