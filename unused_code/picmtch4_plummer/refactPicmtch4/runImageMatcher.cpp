#include "MspiPcorrMatcher.h"
#include "AffineWarp.h"
#include "ImageVis.h"
#include <fstream>

int main(int argc, char** argv)
{
  int searchWindow = 512;
  int fftSize = 256;
  MspiPcorrMatcher imMatch(searchWindow,fftSize);
  std::string refFileName = "/data/svn_workspace/plummer/mspi/MSPI-Ground/refactPicmtch4/hdf/AIRMSPI_L1B2_20120106_210223_SanFrancisco_05_V006-57-terrain.hdf";
  //std::string newFileName = "/data/svn_workspace/plummer/mspi/MSPI-Ground/refactPicmtch4/hdf/AIRMSPI_L1B2_20120106_210223_SanFrancisco_05_V006-57-terrain.hdf";
  std::string newFileName = "/data/svn_workspace/plummer/mspi/MSPI-Ground/refactPicmtch4/hdf/AIRMSPI_L1B2_20120106_210223_SanFrancisco_07_V006-57-terrain.hdf";

  std::string band = "660nm_band";
  HDFEOS5Image Ref(refFileName,band,"I");
  HDFEOS5Image New(newFileName,band,"I");

  // These are arbitrary currently
  GeoCal::GeoCalCore::ImageCoor RefImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(1830,389),
     GeoCal::GeoCalCore::ImageCoor(1113,1059),
     GeoCal::GeoCalCore::ImageCoor(643,488) };
  GeoCal::GeoCalCore::ImageCoor NewImageTiePoints[3] = 
    {GeoCal::GeoCalCore::ImageCoor(1830,389),
     GeoCal::GeoCalCore::ImageCoor(1113,1059),
     GeoCal::GeoCalCore::ImageCoor(643,488) };
  
  printf("Ref Image Tie Points:\n");
  for (int i = 0; i < 3; i++)
    printf("%i: (%g, %g)\t",i+1,RefImageTiePoints[i].line(),RefImageTiePoints[i].sample());
  printf("\nNew Image Tie Points:\n");
  for (int i = 0; i < 3; i++)
    printf("%i: (%g, %g)\t",i+1,NewImageTiePoints[i].line(),NewImageTiePoints[i].sample());
  printf("\n\n");
  std::vector<GeoCal::GeoCalCore::ImageCoor> Ref_locs;

  // fit grid
  int gridSections = 9;
  makeGrid(Ref,gridSections,Ref_locs);
  
  //get initial guess transformation
  double New_guess[12];
  initialMatch(New_guess, RefImageTiePoints, NewImageTiePoints, 
	       Ref_locs.size()+3);

  printf("New Guess:\n");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++)
      printf("%g ",New_guess[(i*3)+j]);
    printf("\n");
  }
  printf("\n");

  // perform matching
  std::vector<GeoCal::GeoCalCore::ImageCoor> Results;
  std::vector<float*> Cov;
  std::vector<bool> Success;
  imMatch.match(Ref,New,Ref_locs,New_guess,Results,Cov,Success);

  printf("\n\n#   Ref_loc     Result       Corr\n");
  for (int i = 0; i < Results.size(); i++) {
    printf("%i (%g, %g)  ",i+1,Ref_locs[i].line(),Ref_locs[i].sample());
    if (Success[i])
      printf("(%g, %g)  %g\n",round(Results[i].line()),round(Results[i].sample()),Cov[i][0]);
    else
      printf("****FAILED****\n");
   }

  // get successful matches
  std::vector<GeoCal::GeoCalCore::ImageCoor> goodRefLocs, goodResults;
  for (int i = 0; i < Ref_locs.size();i++) {
    if(Success[i]) {
      goodRefLocs.push_back(Ref_locs[i]);
      goodResults.push_back(Results[i]);
    }
  }
  
  // warp the image
  AffineWarp imWarp;
  cv::Mat transform = imWarp.getTransformation(goodRefLocs,goodResults);
  cv::Mat warped = imWarp.warp(New,transform);
  
  //display the warping
  ImageVis imVis;
  cv::Mat refMat = rastToMat(Ref);
  cv::Mat newMat = rastToMat(New);
  imVis.displayCombinedWarped(refMat,newMat,warped);

  HDFEOS5Image hdfWarped(warped.rows,warped.cols);

  matToRast(warped,hdfWarped);
  return 0;
}
