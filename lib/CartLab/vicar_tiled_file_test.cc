#include "unit_test_support.h"
#include "vicar_tiled_file.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(vicar_tiled_file, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  if(!VicarFile::vicar_available())
    return;
  std::string fname = test_data_dir() + "vicar.img";
  VicarTiledFile<VicarByte> 
    f(boost::shared_ptr<VicarFile>(new VicarFile(fname)));
  BOOST_CHECK_EQUAL(f.size()[0], 10);
  BOOST_CHECK_EQUAL(f.size()[1], 10);
  BOOST_CHECK_EQUAL(f.tile_size()[0], 10);
  BOOST_CHECK_EQUAL(f.tile_size()[0], 10);
  
  for(int i = 0; i < 10; ++i)
    for(int j = 0; j < 10; ++j)
      BOOST_CHECK_EQUAL(f(i, j), (VicarByte) (i + j));

// Writing doesn't work yet.

//   {				// This will close the file by calling
// 				// the destructor
//     VicarTiledFile<VicarByte> f3(1, 10, 20);
//     VicarByte valb = 0;
//     for(int i = 0; i < f3.size()[0]; ++i)
//       for(int j = 0; j < f3.size()[1]; ++j, ++valb)
// 	f3(i, j) = valb;
//     VicarTiledFile<VicarHalf> f4(fname4, 4, 5);
//     VicarHalf val = 0;
//     for(int i = 0; i < f4.size()[0]; ++i)
//       for(int j = 0; j < f4.size()[1]; ++j, ++val)
// 	f4(i, j) = val;
//   }
//   VicarTiledFile<VicarByte> f3(fname3);
//   BOOST_CHECK_EQUAL(f3.size()[0], 10);
//   BOOST_CHECK_EQUAL(f3.size()[1], 20);
//   VicarHalf valb = 0;
//   for(int i = 0; i < f3.size()[0]; ++i)
//     for(int j = 0; j < f3.size()[1]; ++j, ++valb)
//       BOOST_CHECK_EQUAL(f3(i, j), valb);
//   VicarTiledFile<VicarHalf> f4(fname4);
//   BOOST_CHECK_EQUAL(f4.size()[0], 4);
//   BOOST_CHECK_EQUAL(f4.size()[1], 5);
//   VicarHalf val = 0;
//   for(int i = 0; i < f4.size()[0]; ++i)
//     for(int j = 0; j < f4.size()[1]; ++j, ++val)
//       BOOST_CHECK_EQUAL(f4(i, j), val);
}

BOOST_AUTO_TEST_SUITE_END()

