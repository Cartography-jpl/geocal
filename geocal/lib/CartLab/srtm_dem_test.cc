#include "unit_test_support.h"
#include "srtm_dem.h"
#include "vicar_lite_file.h"
#include "vicar_raster_image.h"
#include <stdlib.h>
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(srtm_dem, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{

  if(!VicarFile::vicar_available())
    return;
  try {
      SrtmDem d;
      // Compared with google earth, using a datum undulation of -33.3435
      BOOST_CHECK_CLOSE(d.height_reference_surface(Geodetic(34.2,-118.03)),
			891.656, 1e-4);
      // Check for proper handling of pixel/point and overrides
      char *t = getenv("ELEV_ROOT");
      std::string fname = std::string(t) + "/n35w117_L2.hlf";
      // Treat as point (incorrect metadata)
      VicarLiteRasterImage f1(fname);
      BOOST_CHECK_CLOSE(distance(*f1.ground_coordinate(0.0,0.0), 
				 Geodetic(36, -117)), 0, 1e-4);
      std::cerr << f1.coordinate(Geodetic(36, -117)) << "\n";
      // Override
      VicarLiteRasterImage f2(fname, VicarLiteFile::READ, 0, -1, -1,
			      true);
      BOOST_CHECK_CLOSE(distance(*f2.ground_coordinate(-0.5,-0.5), 
				 Geodetic(36, -117)), 0, 1e-4);
      std::cerr << f2.coordinate(Geodetic(36, -117)) << "\n";
      // Repeat for VicarRasterImage
      VicarRasterImage f3(fname);
      BOOST_CHECK_CLOSE(distance(*f3.ground_coordinate(0.0,0.0), 
				 Geodetic(36, -117)), 0, 1e-4);
      std::cerr << f3.coordinate(Geodetic(36, -117)) << "\n";
      VicarRasterImage f4(fname, VicarFile::READ, 100, 4, true);
      BOOST_CHECK_CLOSE(distance(*f4.ground_coordinate(-0.5,-0.5), 
				 Geodetic(36, -117)), 0, 1e-4);
      std::cerr << f4.coordinate(Geodetic(36, -117)) << "\n";
  } catch(const Exception&) {
    BOOST_WARN_MESSAGE(false, "Skipping SrtmDem test, data wasn't found");
    // Don't worry if we can't find the data.
  }
}

BOOST_AUTO_TEST_SUITE_END()


