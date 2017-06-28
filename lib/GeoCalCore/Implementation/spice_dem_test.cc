#include "unit_test_support.h"
#include "spice_dem.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(spice_dem, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  if(!SpiceHelper::spice_available()) {
    BOOST_WARN_MESSAGE(false, "Not configured to use SPICE library, so skipping Spice tests.");
    return;
  }
  if(!boost::filesystem::exists("/data/smyth/DawnGpuWork/SPC170222/SHAPE_SPC170222_1024.bds")) {
    BOOST_WARN_MESSAGE(false, "Don't have CERES BDS file available, so skipping test");
    return;
  }    
  std::vector<std::string> klist;
  klist.push_back("/data/smyth/DawnGpuWork/SPC170222/SHAPE_SPC170222_1024.bds");
  klist.push_back("/data/smyth/DawnGpuWork/SPC170222/dawn_ceres_SPC170222.tpc");
  klist.push_back("/data/smyth/DawnGpuWork/SPC170222/traj_ceres18c.bsp");
  SpiceDem dem(2000001, Time::parse_time("2016-01-01T00:00:00Z"), klist);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!SpiceHelper::spice_available()) {
    BOOST_WARN_MESSAGE(false, "Not configured to use SPICE library, so skipping Spice tests.");
    return;
  }
  if(!have_serialize_supported())
    return;
  if(!boost::filesystem::exists("/data/smyth/DawnGpuWork/SPC170222/SHAPE_SPC170222_1024.bds")) {
    BOOST_WARN_MESSAGE(false, "Don't have CERES BDS file available, so skipping test");
    return;
  }    
  std::vector<std::string> klist;
  klist.push_back("/data/smyth/DawnGpuWork/SPC170222/SHAPE_SPC170222_1024.bds");
  klist.push_back("/data/smyth/DawnGpuWork/SPC170222/dawn_ceres_SPC170222.tpc");
  klist.push_back("/data/smyth/DawnGpuWork/SPC170222/traj_ceres18c.bsp");
  boost::shared_ptr<SpiceDem> dem
    (new SpiceDem(2000001, Time::parse_time("2016-01-01T00:00:00Z"), klist));
  std::string d = serialize_write_string(dem);
  if(true)
    std::cerr << d;
  boost::shared_ptr<SpiceDem> demr = 
    serialize_read_string<SpiceDem>(d);
  BOOST_CHECK_EQUAL(demr->naif_id(), dem->naif_id());
  BOOST_CHECK_EQUAL(demr->kernel_list().kernel_list()[0], klist[0]);
}

BOOST_AUTO_TEST_SUITE_END()

