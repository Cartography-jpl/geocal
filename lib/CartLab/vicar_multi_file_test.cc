#include "unit_test_support.h"
#include "vicar_multi_file.h"
#include "vicar_raster_image.h"
#include "vicar_file.h"
#include <stdlib.h>
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(vicar_multi_file, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  // This doesn't actually work yet.
  return;
  if(!VicarFile::vicar_available())
    return;
  try {
    std::string dir = getenv("ELEV_ROOT");
    std::string fname = dir + "/L2_dem_db.int";
    VicarMultiFile f(fname, dir);
    ImageCoordinate ic = f.coordinate(Geodetic(34, -118));
    VicarRasterImage f1(dir + "/n33w118_L2.hlf");
    ImageCoordinate ic1 = f1.coordinate(*f.ground_coordinate(ic));
    BOOST_CHECK_EQUAL(f((int) round(ic.line), (int) round(ic.sample)),
		      f1((int) round(ic1.line), (int) round(ic1.sample)));
    VicarRasterImage f2(dir + "/n34w118_L2.hlf");
    ic.line -= 1;
    ImageCoordinate ic2 = f2.coordinate(*f.ground_coordinate(ic));
    BOOST_CHECK_EQUAL(f((int) round(ic.line), (int) round(ic.sample)),
		      f2((int) round(ic2.line), (int) round(ic2.sample)));
    ic.line += 1;
    ic.sample -= 1;
    VicarRasterImage f3(dir + "/n33w119_L2.hlf");
    ImageCoordinate ic3 = f3.coordinate(*f.ground_coordinate(ic));
    BOOST_CHECK_EQUAL(f((int) round(ic.line), (int) round(ic.sample)),
		      f3((int) round(ic3.line), (int) round(ic3.sample)));
    ic.line -= 1;
    VicarRasterImage f4(dir + "/n34w119_L2.hlf");
    ImageCoordinate ic4 = f4.coordinate(*f.ground_coordinate(ic));
    BOOST_CHECK_EQUAL(f((int) round(ic.line), (int) round(ic.sample)),
		      f4((int) round(ic4.line), (int) round(ic4.sample)));
  } catch(const Exception&) {
    std::cerr << "Skipping VicarMultiFile test, data wasn't found\n";
    // Don't worry if we can't find the data.
  }
}

BOOST_AUTO_TEST_SUITE_END()


