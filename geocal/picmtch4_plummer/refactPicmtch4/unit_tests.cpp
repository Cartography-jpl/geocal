#include "MspiPcorrMatcher.h"
#include "fftw3.h"
#include "picmtch4.h"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE picmtch4_unit_tests
#include <boost/test/unit_test.hpp>
#include <stdio.h>

#include <HE5_HdfEosDef.h>

BOOST_AUTO_TEST_CASE( calcLeastSquaresFit_test)
{
  int nEquations = 3;
  int nEquationsMax = 4;
  double soln[12];

  std::vector<GeoCal::GeoCalCore::ImageCoor> fstImgCoors;
  fstImgCoors.push_back(GeoCal::GeoCalCore::ImageCoor(5,6));
  fstImgCoors.push_back(GeoCal::GeoCalCore::ImageCoor(5,306));
  fstImgCoors.push_back(GeoCal::GeoCalCore::ImageCoor(305,6));
  std::vector<GeoCal::GeoCalCore::ImageCoor> sndImgCoors;
  sndImgCoors.push_back(GeoCal::GeoCalCore::ImageCoor(1,1));
  sndImgCoors.push_back(GeoCal::GeoCalCore::ImageCoor(1,301));
  sndImgCoors.push_back(GeoCal::GeoCalCore::ImageCoor(301,1));

  calcLeastSquaresFit(soln,0,nEquationsMax,fstImgCoors,sndImgCoors);

  BOOST_CHECK_EQUAL(soln[3], 0.0);
  BOOST_CHECK_EQUAL(soln[11], 0.0);
  BOOST_CHECK(soln[2] > -4 && soln[2] < -3.9999999);
  BOOST_CHECK(soln[8] > -5 && soln[8] < -4.9999999);
  BOOST_CHECK(soln[1] < 0 && soln[1] > -1.e-14);
  BOOST_CHECK(soln[6] < 0 && soln[6] > -1.e-14);
}

BOOST_AUTO_TEST_CASE( throwout_OneDiffIndex_test )
{
  int throwcount = 3;
  int nEquationsMax = 6;
  double soln[12];

  std::vector<GeoCal::GeoCalCore::ImageCoor> fstImgCoors;
  std::vector<GeoCal::GeoCalCore::ImageCoor> sndImgCoors;

  int index = 3;
  for (int i = 0; i < nEquationsMax; i++) {
    float val = 3.0;
    if (i == index)
      val = 1.0;
    fstImgCoors.push_back(GeoCal::GeoCalCore::ImageCoor(val,val));
    if (i == index)
      val = 7.0;
    sndImgCoors.push_back(GeoCal::GeoCalCore::ImageCoor(val,val));
  }
  
  throwout(&throwcount,nEquationsMax,fstImgCoors,sndImgCoors);

  BOOST_CHECK_EQUAL(throwcount, 2);
  BOOST_CHECK(fstImgCoors[index].line() > 1.0);
  BOOST_CHECK(fstImgCoors[index].sample() > 1.0);
  BOOST_CHECK(sndImgCoors[index].line() < 7.0);
  BOOST_CHECK(sndImgCoors[index].sample() < 7.0);
}

BOOST_AUTO_TEST_CASE( throwout_twoDiffIndex_test )
{
  int throwcount = 3;
  int nEquationsMax = 8;
  double soln[12];

  std::vector<GeoCal::GeoCalCore::ImageCoor> fstImgCoors;
  std::vector<GeoCal::GeoCalCore::ImageCoor> sndImgCoors;

  int index = 2;
  for (int i = 0; i < nEquationsMax; i++) {
    float val = 3.0;
    if (i == index)
      val = 1.0;
    fstImgCoors.push_back(GeoCal::GeoCalCore::ImageCoor(val,val));
    if (i == index+2)
      val = 7.0;
    sndImgCoors.push_back(GeoCal::GeoCalCore::ImageCoor(val,val));
  }

  throwout(&throwcount,nEquationsMax,fstImgCoors,sndImgCoors);

  BOOST_CHECK_EQUAL(throwcount, 2);
  BOOST_CHECK_EQUAL(fstImgCoors[index].line(), 1.0);
  BOOST_CHECK_EQUAL(fstImgCoors[index].sample(), 1.0);
  BOOST_CHECK(sndImgCoors[index+2].line() < 7.0);
  BOOST_CHECK(sndImgCoors[index+2].sample() < 7.0);
}


BOOST_AUTO_TEST_CASE( Match1_Perfect_Predictor )
{
  int nredo = 0;
  int search = 32; 
  int fftsize = 32;

  MspiPcorrMatcher imMatch(search,fftsize,fftsize,nredo);

  HDFEOS5Image Ref("tstimg1.hdf","testImg","I.mask");
  HDFEOS5Image New("tstimg2.hdf","testImg","I.mask");

  GeoCal::GeoCalCore::ImageCoor RefImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(5,6),
     GeoCal::GeoCalCore::ImageCoor(5,306),
     GeoCal::GeoCalCore::ImageCoor(305,6) };
  GeoCal::GeoCalCore::ImageCoor NewImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(1,1),
     GeoCal::GeoCalCore::ImageCoor(1,301),
     GeoCal::GeoCalCore::ImageCoor(301,1) };

  std::vector<GeoCal::GeoCalCore::ImageCoor> Ref_locs;
  Ref_locs.push_back(GeoCal::GeoCalCore::ImageCoor(150,150));

  double New_guess[12];
  initialMatch(New_guess, RefImageTiePoints, NewImageTiePoints, 
	       Ref_locs.size()+3);
  std::vector<GeoCal::GeoCalCore::ImageCoor> Results;
  std::vector<float*> Cov;
  std::vector<bool> Success;
  imMatch.match(Ref,New,Ref_locs,New_guess,Results,Cov,Success);

  BOOST_CHECK_EQUAL(Success[0], true);
  BOOST_CHECK(Cov[0][0] < 0.4806 && Cov[0][0] > 0.4804);
  BOOST_CHECK(Cov[0][1] < 1.e-10 && Cov[0][1] > 0);
  BOOST_CHECK(Cov[0][2] < 1.e-10 && Cov[0][2] > 0);
  BOOST_CHECK_EQUAL(round(Results[0].line()), 146);
  BOOST_CHECK_EQUAL(round(Results[0].sample()), 145);
}

BOOST_AUTO_TEST_CASE( Match2_Predictor_OffBy6PlusPixels )
{
  int nredo = 0;
  int search = 96; 
  int fftsize = 32;

  MspiPcorrMatcher imMatch(search,fftsize,fftsize,false,nredo);

  HDFEOS5Image Ref("tstimg1.hdf","testImg","I.mask");
  HDFEOS5Image New("tstimg2.hdf","testImg","I.mask");

  GeoCal::GeoCalCore::ImageCoor RefImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(1,1),
     GeoCal::GeoCalCore::ImageCoor(1,301),
     GeoCal::GeoCalCore::ImageCoor(301,1) };
  GeoCal::GeoCalCore::ImageCoor NewImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(1,1),
     GeoCal::GeoCalCore::ImageCoor(1,301),
     GeoCal::GeoCalCore::ImageCoor(301,1) };

  std::vector<GeoCal::GeoCalCore::ImageCoor> Ref_locs;
  Ref_locs.push_back(GeoCal::GeoCalCore::ImageCoor(150,150));

  double New_guess[12];
  initialMatch(New_guess, RefImageTiePoints, NewImageTiePoints, 
	       Ref_locs.size()+3);
  std::vector<GeoCal::GeoCalCore::ImageCoor> Results;
  std::vector<float*> Cov;
  std::vector<bool> Success;
  imMatch.match(Ref,New,Ref_locs,New_guess,Results,Cov,Success);

  BOOST_CHECK_EQUAL(Success[0], true);
  BOOST_CHECK(Cov[0][0] < 0.1282 && Cov[0][0] > 0.1281);
  BOOST_CHECK(Cov[0][1] < -3.98 && Cov[0][1] > -3.99);
  BOOST_CHECK(Cov[0][2] < -5.00 && Cov[0][2] > -5.02);
  BOOST_CHECK_EQUAL(round(Results[0].line()), 146);
  BOOST_CHECK_EQUAL(round(Results[0].sample()), 145);
}

BOOST_AUTO_TEST_CASE( Match3_OffByScale )
{
  int nredo = 0;
  int search = 96; 
  int fftsize = 32;
  
  MspiPcorrMatcher imMatch(search,fftsize,fftsize,false,nredo);

  HDFEOS5Image Ref("tstimg1.hdf","testImg","I.mask");
  HDFEOS5Image New("tstimg2.hdf","testImg","I.mask");

  GeoCal::GeoCalCore::ImageCoor RefImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(1,2),
     GeoCal::GeoCalCore::ImageCoor(1,310),
     GeoCal::GeoCalCore::ImageCoor(309,2) };
  GeoCal::GeoCalCore::ImageCoor NewImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(1,1),
     GeoCal::GeoCalCore::ImageCoor(1,301),
     GeoCal::GeoCalCore::ImageCoor(301,1) };
  
  std::vector<GeoCal::GeoCalCore::ImageCoor> Ref_locs;
  Ref_locs.push_back(GeoCal::GeoCalCore::ImageCoor(150,150));

  double New_guess[12];
  initialMatch(New_guess, RefImageTiePoints, NewImageTiePoints, 
	       Ref_locs.size()+3);
  std::vector<GeoCal::GeoCalCore::ImageCoor> Results;
  std::vector<float*> Cov;
  std::vector<bool> Success;
  imMatch.match(Ref,New,Ref_locs,New_guess,Results,Cov,Success);

  BOOST_CHECK_EQUAL(Success[0], true);
  BOOST_CHECK(Cov[0][0] < 0.108 && Cov[0][0] > 0.107);
  BOOST_CHECK(Cov[0][1] < -0.028 && Cov[0][1] > -0.029);
  BOOST_CHECK(Cov[0][2] < 0.020 && Cov[0][2] > 0.019);
  BOOST_CHECK_EQUAL(round(Results[0].line()), 146);
  BOOST_CHECK_EQUAL(round(Results[0].sample()), 145);
}

BOOST_AUTO_TEST_CASE( Match4_PerfectPredictor_NewPoint )
{ 
  int nredo = 0;
  int search = 32; 
  int fftsize = 32;

  MspiPcorrMatcher imMatch(search,fftsize,fftsize,false,nredo);

  HDFEOS5Image Ref("tstimg1.hdf","testImg","I.mask");
  HDFEOS5Image New("tstimg2.hdf","testImg","I.mask");

  GeoCal::GeoCalCore::ImageCoor RefImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(16,16),
     GeoCal::GeoCalCore::ImageCoor(286,16),
     GeoCal::GeoCalCore::ImageCoor(16,286) };
  GeoCal::GeoCalCore::ImageCoor NewImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(12,11),
     GeoCal::GeoCalCore::ImageCoor(282,11),
     GeoCal::GeoCalCore::ImageCoor(12,281) };

  std::vector<GeoCal::GeoCalCore::ImageCoor> Ref_locs;
  Ref_locs.push_back(GeoCal::GeoCalCore::ImageCoor(211,226));

  double New_guess[12];
  initialMatch(New_guess, RefImageTiePoints, NewImageTiePoints, 
	       Ref_locs.size()+3);
  std::vector<GeoCal::GeoCalCore::ImageCoor> Results;
  std::vector<float*> Cov;
  std::vector<bool> Success;
  imMatch.match(Ref,New,Ref_locs,New_guess,Results,Cov,Success);

  BOOST_CHECK_EQUAL(Success[0], true);
  BOOST_CHECK(Cov[0][0] < 0.4814 && Cov[0][0] > 0.4805);
  BOOST_CHECK(Cov[0][1] < 0 && Cov[0][1] > -1.e-10);
  BOOST_CHECK(Cov[0][2] < 1.e-14 && Cov[0][2] >= 0);
  BOOST_CHECK_EQUAL(round(Results[0].line()), 207);
  BOOST_CHECK_EQUAL(round(Results[0].sample()), 221);
}

BOOST_AUTO_TEST_CASE( Match5_MagnifyCase )
{
  int nredo = 0;
  int search = 32; 
  int fftsize = 32;
  bool ffthalf = false;
  int minsearch = 32;
  int autofit = 0;
  bool magshrk = false; 
  double rmagtae[2] = {7,7};
  
  MspiPcorrMatcher imMatch(search,fftsize,minsearch,ffthalf,nredo,autofit,magshrk,rmagtae);

  HDFEOS5Image Ref("tstimg1.hdf","testImg","I.mask");
  HDFEOS5Image New("tstimg2.hdf","testImg","I.mask");

  GeoCal::GeoCalCore::ImageCoor RefImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(5,6),
     GeoCal::GeoCalCore::ImageCoor(5,306),
     GeoCal::GeoCalCore::ImageCoor(305,6) };
  GeoCal::GeoCalCore::ImageCoor NewImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(1,1),
     GeoCal::GeoCalCore::ImageCoor(1,301),
     GeoCal::GeoCalCore::ImageCoor(301,1) };

  std::vector<GeoCal::GeoCalCore::ImageCoor> Ref_locs;
  Ref_locs.push_back(GeoCal::GeoCalCore::ImageCoor(150,150));

  double New_guess[12];
  initialMatch(New_guess, RefImageTiePoints, NewImageTiePoints, 
	       Ref_locs.size()+3);
  std::vector<GeoCal::GeoCalCore::ImageCoor> Results;
  std::vector<float*> Cov;
  std::vector<bool> Success;
  imMatch.match(Ref,New,Ref_locs,New_guess,Results,Cov,Success);

  BOOST_CHECK_EQUAL(Success[0], true);
  BOOST_CHECK(Cov[0][0] < 0.4814 && Cov[0][0] > 0.4805);
  BOOST_CHECK(Cov[0][1] < 1.e-10 && Cov[0][1] > 0);
  BOOST_CHECK(Cov[0][2] < 1.e-10 && Cov[0][2] > 0);
  BOOST_CHECK_EQUAL(round(Results[0].line()), 146);
  BOOST_CHECK_EQUAL(round(Results[0].sample()), 145);
}

BOOST_AUTO_TEST_CASE( Match6_64x64FFT )
{
  int nredo = 0;
  int search = 64; 
  int fftsize = 64;
  int minsrch = 64;

  MspiPcorrMatcher imMatch(search,fftsize,minsrch,false,nredo);

  HDFEOS5Image Ref("tstimg1.hdf","testImg","I.mask");
  HDFEOS5Image New("tstimg2.hdf","testImg","I.mask");

  GeoCal::GeoCalCore::ImageCoor RefImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(5,6),
     GeoCal::GeoCalCore::ImageCoor(5,306),
     GeoCal::GeoCalCore::ImageCoor(305,6) };
  GeoCal::GeoCalCore::ImageCoor NewImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(1,1),
     GeoCal::GeoCalCore::ImageCoor(1,301),
     GeoCal::GeoCalCore::ImageCoor(302,1) };

  std::vector<GeoCal::GeoCalCore::ImageCoor> Ref_locs;
  Ref_locs.push_back(GeoCal::GeoCalCore::ImageCoor(150,150));

  double New_guess[12];
  initialMatch(New_guess, RefImageTiePoints, NewImageTiePoints, 
	       Ref_locs.size()+3);
  std::vector<GeoCal::GeoCalCore::ImageCoor> Results;
  std::vector<float*> Cov;
  std::vector<bool> Success;
  imMatch.match(Ref,New,Ref_locs,New_guess,Results,Cov,Success);

  BOOST_CHECK_EQUAL(Success[0], true);
  BOOST_CHECK(Cov[0][0] < 0.3153 && Cov[0][0] > 0.3152);
  BOOST_CHECK(Cov[0][1] < -0.504 && Cov[0][1] > -0.505);
  BOOST_CHECK(Cov[0][2] < -5.14e-3 && Cov[0][2] > -5.15e-3);
  BOOST_CHECK_EQUAL(round(Results[0].line()), 146);
  BOOST_CHECK_EQUAL(round(Results[0].sample()), 145);
}

BOOST_AUTO_TEST_CASE( Match7_DiffTiePoints )
{
  int nredo = 0;
  int search = 96; 
  int fftsize = 64;
  int minsrch = 96;

  MspiPcorrMatcher imMatch(search,fftsize,minsrch,false,nredo);

  HDFEOS5Image Ref("tstimg1.hdf","testImg","I.mask");
  HDFEOS5Image New("tstimg2.hdf","testImg","I.mask");

  GeoCal::GeoCalCore::ImageCoor RefImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(1,1),
     GeoCal::GeoCalCore::ImageCoor(300,1),
     GeoCal::GeoCalCore::ImageCoor(1,310) };
  GeoCal::GeoCalCore::ImageCoor NewImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(-3,-4),
     GeoCal::GeoCalCore::ImageCoor(296,-4),
     GeoCal::GeoCalCore::ImageCoor(-3,305) };

  std::vector<GeoCal::GeoCalCore::ImageCoor> Ref_locs;
  Ref_locs.push_back(GeoCal::GeoCalCore::ImageCoor(150,150));

  double New_guess[12];
  initialMatch(New_guess, RefImageTiePoints, NewImageTiePoints, 
	       Ref_locs.size()+3);
  std::vector<GeoCal::GeoCalCore::ImageCoor> Results;
  std::vector<float*> Cov;
  std::vector<bool> Success;
  imMatch.match(Ref,New,Ref_locs,New_guess,Results,Cov,Success);
  
  BOOST_CHECK_EQUAL(Success[0], true);
  BOOST_CHECK(Cov[0][0] < 0.2862 && Cov[0][0] > 0.286);
  BOOST_CHECK(Cov[0][1] < -9.04e-3 && Cov[0][1] > -9.05e-3);
  BOOST_CHECK(Cov[0][2] < -2.07e-3 && Cov[0][2] > -2.08e-3);
  BOOST_CHECK_EQUAL(round(Results[0].line()), 146);
  BOOST_CHECK_EQUAL(round(Results[0].sample()), 145);
}

BOOST_AUTO_TEST_CASE( Match8_NewTiePoints64x64FFT )
{
  int nredo = 0;
  int search = 64; 
  int fftsize = 64;
  int minsrch = 64;
 
  MspiPcorrMatcher imMatch(search,fftsize,minsrch,false,nredo);

  HDFEOS5Image Ref("tstimg1.hdf","testImg","I.mask");
  HDFEOS5Image New("tstimg2.hdf","testImg","I.mask");

  GeoCal::GeoCalCore::ImageCoor RefImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(1,1),
     GeoCal::GeoCalCore::ImageCoor(300,1),
     GeoCal::GeoCalCore::ImageCoor(1,310) };
  GeoCal::GeoCalCore::ImageCoor NewImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(-3,-4),
     GeoCal::GeoCalCore::ImageCoor(296,-4),
     GeoCal::GeoCalCore::ImageCoor(-3,305) };

  std::vector<GeoCal::GeoCalCore::ImageCoor> Ref_locs;
  Ref_locs.push_back(GeoCal::GeoCalCore::ImageCoor(211,226));

  double New_guess[12];
  initialMatch(New_guess, RefImageTiePoints, NewImageTiePoints, 
	       Ref_locs.size()+3);

  std::vector<GeoCal::GeoCalCore::ImageCoor> Results;
  std::vector<float*> Cov;
  std::vector<bool> Success;
  imMatch.match(Ref,New,Ref_locs,New_guess,Results,Cov,Success);

  BOOST_CHECK_EQUAL(Success[0], true);
  BOOST_CHECK(Cov[0][0] < 0.4786 && Cov[0][0] > 0.4785);
  BOOST_CHECK(Cov[0][1] < 0 && Cov[0][1] > -1.e-10);
  BOOST_CHECK(Cov[0][2] < 1.e-10 && Cov[0][2] > 0);
  BOOST_CHECK_EQUAL(round(Results[0].line()), 207);
  BOOST_CHECK_EQUAL(round(Results[0].sample()), 221);
}

BOOST_AUTO_TEST_CASE( Match9_RotatedImage )
{
  int nredo = 0;
  int search = 64; 
  int fftsize = 64;
  int minsrch = 64;
  MspiPcorrMatcher imMatch(search,fftsize,minsrch,false,nredo);

  HDFEOS5Image Ref("tstimg1.hdf","testImg","I.mask");
  HDFEOS5Image New("tstimg2_rotate.hdf","testImg","I.mask");

  GeoCal::GeoCalCore::ImageCoor RefImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(1,1),
     GeoCal::GeoCalCore::ImageCoor(300,1),
     GeoCal::GeoCalCore::ImageCoor(1,310) };
  GeoCal::GeoCalCore::ImageCoor NewImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(309,299),
     GeoCal::GeoCalCore::ImageCoor(309,0),
     GeoCal::GeoCalCore::ImageCoor(0,299) };

  std::vector<GeoCal::GeoCalCore::ImageCoor> Ref_locs;
  Ref_locs.push_back(GeoCal::GeoCalCore::ImageCoor(211,226));

  double New_guess[12];
  initialMatch(New_guess, RefImageTiePoints, NewImageTiePoints, 
	       Ref_locs.size()+3);
  std::vector<GeoCal::GeoCalCore::ImageCoor> Results;
  std::vector<float*> Cov;
  std::vector<bool> Success;
  imMatch.match(Ref,New,Ref_locs,New_guess,Results,Cov,Success);

  BOOST_CHECK_EQUAL(Success[0], true);
  BOOST_CHECK(Cov[0][0] < 0.4786 && Cov[0][0] > 0.4785);
  BOOST_CHECK(Cov[0][1] < 0 && Cov[0][1] > -1.e-13);
  BOOST_CHECK(Cov[0][2] < 1.e-7 && Cov[0][2] >= 0);
  BOOST_CHECK_EQUAL(round(Results[0].line()), 84);
  BOOST_CHECK_EQUAL(round(Results[0].sample()), 89);
}

BOOST_AUTO_TEST_CASE( Match10_RotatedImageWithRedo )
{
  int nredo = 1;
  int search = 64; 
  int fftsize = 64;
  int minsrch = 64;
  MspiPcorrMatcher imMatch(search,fftsize,minsrch,false,nredo);

  HDFEOS5Image Ref("tstimg1.hdf","testImg","I.mask");
  HDFEOS5Image New("tstimg2_rotate.hdf","testImg","I.mask");

  GeoCal::GeoCalCore::ImageCoor RefImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(1,1),
     GeoCal::GeoCalCore::ImageCoor(300,1),
     GeoCal::GeoCalCore::ImageCoor(1,310) };
  GeoCal::GeoCalCore::ImageCoor NewImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(309,299),
     GeoCal::GeoCalCore::ImageCoor(309,0),
     GeoCal::GeoCalCore::ImageCoor(0,299) };

  std::vector<GeoCal::GeoCalCore::ImageCoor> Ref_locs;
  Ref_locs.push_back(GeoCal::GeoCalCore::ImageCoor(140,130));

  double New_guess[12];
  initialMatch(New_guess, RefImageTiePoints, NewImageTiePoints, 
	       Ref_locs.size()+3);
  std::vector<GeoCal::GeoCalCore::ImageCoor> Results;
  std::vector<float*> Cov;
  std::vector<bool> Success;
  imMatch.match(Ref,New,Ref_locs,New_guess,Results,Cov,Success);
 
  BOOST_CHECK_EQUAL(Success[0], true);
  BOOST_CHECK(Cov[0][0] < 0.4786 && Cov[0][0] > 0.4785);
  BOOST_CHECK(Cov[0][1] < 0 && Cov[0][1] > -1.e-13);
  BOOST_CHECK(Cov[0][2] < 1.e-7 && Cov[0][2] >= 0);
  BOOST_CHECK_EQUAL(round(Results[0].line()), 180);
  BOOST_CHECK_EQUAL(round(Results[0].sample()), 160);
}

BOOST_AUTO_TEST_CASE( Match11_FourEdgeCases_For_SubImages )
{
  int nredo = 0;
  int search = 64; 
  int fftsize = 64;
  bool ffthalf = true;
  int minsrch = 32;
  MspiPcorrMatcher imMatch(nredo,search,fftsize,minsrch,ffthalf,nredo);

  HDFEOS5Image Ref("tstimg3.hdf","testImg","I.mask");
  HDFEOS5Image New("tstimg4.hdf","testImg","I.mask");

  GeoCal::GeoCalCore::ImageCoor RefImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(5,6),
     GeoCal::GeoCalCore::ImageCoor(5,306),
     GeoCal::GeoCalCore::ImageCoor(305,6) };
  GeoCal::GeoCalCore::ImageCoor NewImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(1,1),
     GeoCal::GeoCalCore::ImageCoor(1,301),
     GeoCal::GeoCalCore::ImageCoor(301,1) };

  std::vector<GeoCal::GeoCalCore::ImageCoor> Ref_locs;
  Ref_locs.push_back(GeoCal::GeoCalCore::ImageCoor(240,238));
  Ref_locs.push_back(GeoCal::GeoCalCore::ImageCoor(30,30));
  Ref_locs.push_back(GeoCal::GeoCalCore::ImageCoor(270,270));
  Ref_locs.push_back(GeoCal::GeoCalCore::ImageCoor(20,20));
  Ref_locs.push_back(GeoCal::GeoCalCore::ImageCoor(100,100));

  double New_guess[12];
  initialMatch(New_guess, RefImageTiePoints, NewImageTiePoints, 
	       Ref_locs.size()+3);
  std::vector<GeoCal::GeoCalCore::ImageCoor> Results;
  std::vector<float*> Cov;
  std::vector<bool> Success;

  imMatch.match(Ref,New,Ref_locs,New_guess,Results,Cov,Success);
  BOOST_CHECK_EQUAL(Success[0], true);
  BOOST_CHECK(Cov[0][0] < 0.4786 && Cov[0][0] > 0.4785);
  BOOST_CHECK(Cov[0][1] < 1.e-10 && Cov[0][1] > 0);
  BOOST_CHECK(Cov[0][2] < 1.e-10 && Cov[0][2] > 0);
  BOOST_CHECK_EQUAL(round(Results[0].line()), 236);
  BOOST_CHECK_EQUAL(round(Results[0].sample()), 233);
 
  BOOST_CHECK_EQUAL(Success[1], true);
  BOOST_CHECK(Cov[1][0] < 0.4806 && Cov[1][0] > 0.4804);
  BOOST_CHECK(Cov[1][1] < 1.e-7 && Cov[1][1] >= 0);
  BOOST_CHECK(Cov[1][2] < 1.e-10 && Cov[0][2] > 0);
  BOOST_CHECK_EQUAL(round(Results[1].line()), 26);
  BOOST_CHECK_EQUAL(round(Results[1].sample()), 25);

  BOOST_CHECK_EQUAL(Success[2], true);
  BOOST_CHECK(Cov[2][0] < 0.4806 && Cov[2][0] > 0.4804);
  BOOST_CHECK(Cov[2][1] < 1.e-10 && Cov[2][1] > 0);
  BOOST_CHECK(Cov[2][2] < 1.e-10 && Cov[2][2] > 0);
  BOOST_CHECK_EQUAL(round(Results[2].line()), 266);
  BOOST_CHECK_EQUAL(round(Results[2].sample()), 265);

  BOOST_CHECK_EQUAL(Success[3], false);
  BOOST_CHECK(Cov[3][0] < -9998.9 && Cov[3][0] > -9999.1);
  BOOST_CHECK(Cov[3][1] < -9998.9 && Cov[3][1] > -9999.1);
  BOOST_CHECK(Cov[3][2] < -9998.9 && Cov[3][2] > -9999.1);
  BOOST_CHECK_EQUAL(round(Results[3].line()), -9999.);
  BOOST_CHECK_EQUAL(round(Results[3].sample()), -9999.);

  BOOST_CHECK_EQUAL(Success[4], true);
  BOOST_CHECK(Cov[4][0] < 0.4786 && Cov[4][0] > 0.4785);
  BOOST_CHECK(Cov[4][1] < 1.e-10 && Cov[4][1] > 0);
  BOOST_CHECK(Cov[4][2] < 1.e-7 && Cov[4][2] >= 0);
  BOOST_CHECK_EQUAL(round(Results[4].line()), 96);
  BOOST_CHECK_EQUAL(round(Results[4].sample()), 95);
}

BOOST_AUTO_TEST_CASE( Match12_AutoThresh_Case )
{
  int nredo = 0;
  int search = 64; 
  int fftsize = 32;
  bool ffthalf = false;
  int autofit = 12;
  int minsrch = 32;

  MspiPcorrMatcher imMatch(search,fftsize,minsrch,ffthalf,nredo,autofit);

  HDFEOS5Image Ref("tstimg3.hdf","testImg","I.mask");
  HDFEOS5Image New("tstimg4.hdf","testImg","I.mask");

  GeoCal::GeoCalCore::ImageCoor RefImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(5,6),
     GeoCal::GeoCalCore::ImageCoor(5,306),
     GeoCal::GeoCalCore::ImageCoor(305,6) };
  GeoCal::GeoCalCore::ImageCoor NewImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(1,1),
     GeoCal::GeoCalCore::ImageCoor(1,301),
     GeoCal::GeoCalCore::ImageCoor(301,1) };

  std::vector<GeoCal::GeoCalCore::ImageCoor> Ref_locs;

  int ptMin = 145;
  int ptMax = 155;
  double xIncrement = (ptMax-ptMin)/7.;
  int yIncrement = 2;
  int cnt = 0;
  double xCurrent = ptMin;
  int yCurrent = ptMin;

  for (int j = 0; j < 48; j++) {
    Ref_locs.push_back( GeoCal::GeoCalCore::ImageCoor(xCurrent,yCurrent+(cnt*yIncrement)));

    cnt++;
    if (cnt == 6) {
      xCurrent += xIncrement;
      cnt = 0;
    }
  }

  double New_guess[12];
  initialMatch(New_guess, RefImageTiePoints, NewImageTiePoints, 
	       Ref_locs.size()+3);
  std::vector<GeoCal::GeoCalCore::ImageCoor> Results;
  std::vector<float*> Cov;
  std::vector<bool> Success;
  imMatch.match(Ref,New,Ref_locs,New_guess,Results,Cov,Success);


  std::vector<bool> GT_Success;

  GT_Success.insert(GT_Success.begin(),Ref_locs.size()-1,true);
  GT_Success.push_back(false);
  BOOST_CHECK_EQUAL_COLLECTIONS(Success.begin(),Success.end(),
				GT_Success.begin(),GT_Success.end());

  int idx = 0;
  double cov_low = 0.1796;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 141);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 140);

  idx = 1;
  cov_low = 0.1699;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 141);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 142);

  idx = 2;
  cov_low = 0.1794;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 141);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 144);

  idx = 3;
  cov_low = 0.1786;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 141);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 146);

  idx = 4;
  cov_low = 0.1806;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 141);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 148);

  idx = 5;
  cov_low = 0.1860;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 141);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 150);
 
  idx = 6;
  cov_low = 0.1757;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 142);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 140);

  idx = 7;
  cov_low = 0.1688;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 142);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 142);


  idx = 8;
  cov_low = 0.1752;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 142);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 144);

  idx = 9;
  cov_low = 0.1727;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 142);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 146);

  idx = 10;
  cov_low = 0.1797;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 142);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 148);

  idx = 11;
  cov_low = 0.1842;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 142);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 150);

  idx = 12;
  cov_low = 0.1685;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 144);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 140);

  idx = 13;
  cov_low = 0.1711;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 144);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 142);

  idx = 14;
  cov_low = 0.1701;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 144);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 144);

  idx = 15;
  cov_low = 0.1740;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 144);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 146);

  idx = 16;
  cov_low = 0.1789;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 144);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 148);

  idx = 17;
  cov_low = 0.1868;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 144);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 150);

  idx = 18;
  cov_low = 0.1610;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 145);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 140);

  idx = 19;
  cov_low = 0.1642;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 145);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 142);

  idx = 20;
  cov_low = 0.1740;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 145);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 144);

  idx = 21;
  cov_low = 0.1793;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 145);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 146);

  idx = 22;
  cov_low = 0.1827;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 145);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 148);

  idx = 23;
  cov_low = 0.1938;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 145);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 150);

  idx = 24;
  cov_low = 0.1693;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 147);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 140);

  idx = 25;
  cov_low = 0.1751;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 147);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 142);

  idx = 26;
  cov_low = 0.1749;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 147);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 144);

  idx = 27;
  cov_low = 0.1791;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 147);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 146);

  idx = 28;
  cov_low = 0.1823;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 147);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 148);

  idx = 29;
  cov_low = 0.1867;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 147);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 150);

  idx = 30;
  cov_low = 0.1903;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 148);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 140);

  idx = 31;
  cov_low = 0.1840;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 148);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 142);

  idx = 32;
  cov_low = 0.1884;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 148);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 144);

  idx = 33;
  cov_low = 0.1990;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 148);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 146);

  idx = 34;
  cov_low = 0.2197;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 148);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 148);

  idx = 35;
  cov_low = 0.2423;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 148);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 150);

  idx = 36;
  cov_low = 0.2529;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 150);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 140);

  idx = 37;
  cov_low = 0.2794;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 150);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 142);

  idx = 38;
  cov_low = 0.2863;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 150);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 144);

  idx = 39;
  cov_low = 0.2999;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 150);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 146);

  idx = 40;
  cov_low = 0.3328;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 150);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 148);

  idx = 41;
  cov_low = 0.3235;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 150);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 150);

  idx = 42;
  cov_low = 0.3247;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 151);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 140);

  idx = 43;
  cov_low = 0.3193;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 151);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 142);

  idx = 44;
  cov_low = 0.3316;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 151);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 144);

  idx = 45;
  cov_low = 0.3360;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 151);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 146);

  idx = 46;
  cov_low = 0.3260;
  BOOST_CHECK(Cov[idx][0] < cov_low+0.001 && Cov[idx][0] > cov_low);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), 151);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), 148);

  idx = 47;
  BOOST_CHECK(Cov[idx][0] < -9998.9 && Cov[idx][0] > -9999.1);
  BOOST_CHECK_EQUAL(round(Results[idx].line()), -9999);
  BOOST_CHECK_EQUAL(round(Results[idx].sample()), -9999);
}

// EOF
