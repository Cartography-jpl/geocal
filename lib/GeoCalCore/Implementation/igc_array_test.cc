#include "unit_test_support.h"
#include "igc_array.h"
#include "rpc_image_ground_connection.h"
#include "vicar_lite_file.h"
using namespace GeoCal;

class IgcArrayFixture : public GlobalFixture {
public:
  IgcArrayFixture() 
  {
    boost::shared_ptr<Dem> dem
      (new VicarLiteDem(stereo_test_data_dir() + "nevada_elv_aoi.img"));
    boost::shared_ptr<RasterImage> img1
      (new VicarLiteRasterImage(stereo_test_data_dir() + "10MAY21-1.img"));
    boost::shared_ptr<RasterImage> img2
      (new VicarLiteRasterImage(stereo_test_data_dir() + "10MAY21-2.img"));
    boost::shared_ptr<RasterImage> img3
      (new VicarLiteRasterImage(stereo_test_data_dir() + "10MAY21-3.img"));
    boost::shared_ptr<RpcImageGroundConnection> igc1
      (new RpcImageGroundConnection(img1->rpc(), dem, img1, "Image 1"));
    igc1->rpc().fit_line_numerator[0] = true;
    igc1->rpc().fit_line_numerator[1] = true;
    igc1->rpc().fit_sample_numerator[0] = true;
    igc1->rpc().fit_sample_numerator[1] = true;
    boost::shared_ptr<RpcImageGroundConnection> igc2
      (new RpcImageGroundConnection(img2->rpc(), dem, img2, "Image 2"));
    igc2->rpc().fit_line_numerator[0] = true;
    igc2->rpc().fit_line_numerator[1] = true;
    igc2->rpc().fit_sample_numerator[0] = true;
    igc2->rpc().fit_sample_numerator[1] = true;
    boost::shared_ptr<RpcImageGroundConnection> igc3
      (new RpcImageGroundConnection(img3->rpc(), dem, img3, "Image 3"));
    igc3->rpc().fit_line_numerator[0] = true;
    igc3->rpc().fit_line_numerator[1] = true;
    igc3->rpc().fit_sample_numerator[0] = true;
    igc3->rpc().fit_sample_numerator[1] = true;
    std::vector<boost::shared_ptr<ImageGroundConnection> > igcv;
    igcv.push_back(igc1);
    igcv.push_back(igc2);
    igcv.push_back(igc3);
    igccol.reset(new IgcArray(igcv));
  }
  virtual ~IgcArrayFixture() {}
  boost::shared_ptr<IgcArray> igccol;
};

BOOST_FIXTURE_TEST_SUITE(igc_array, IgcArrayFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  BOOST_CHECK_EQUAL(igccol->parameter_subset().rows(), 12);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  std::string d = serialize_write_string(igccol);
  if(false)
    std::cerr << d;
  boost::shared_ptr<IgcArray> igccolr = 
    serialize_read_string<IgcArray>(d);
  ImageCoordinate ic(100, 200);
  BOOST_CHECK(igccolr->image_coordinate(0, *igccol->ground_coordinate(0, ic)) == ic);
  BOOST_CHECK_MATRIX_CLOSE(igccol->parameter(), igccolr->parameter());
}

BOOST_AUTO_TEST_SUITE_END()

