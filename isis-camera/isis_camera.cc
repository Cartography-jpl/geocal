#include <iostream>
#include <isis/LroWideAngleCamera.h>
#include <isis/PushFrameCameraGroundMap.h>
#include <isis/PushFrameCameraDetectorMap.h>
#include <isis/CameraFocalPlaneMap.h>
using namespace Isis;

int main(int Argc, char** Argv)
{
  if(Argc != 5) {
    std::cerr << "Usage: isis_camera <cube file> <band number> <line> <sample>\n";
    exit(1);
  }
  Cube cube(Argv[1]);
  LroWideAngleCamera cam(cube);
  cam.SetBand(atoi(Argv[2]));
  std::cout << "CkFrameId: " << cam.CkFrameId() << "\n"
	    << "CkReferenceId: " << cam.CkReferenceId() << "\n"
	    << "SpkReferenceId: " << cam.SpkReferenceId() << "\n"
	    << "Focal Length: " << cam.FocalLength() << "\n"
	    << "Pixel Pitch: " << cam.PixelPitch() << "\n"
	    << "Exposure Duration: " << cam.exposureDuration() << "\n"
	    << "Instrument ID: " << cam.instrumentId() << "\n"
	    << "naifIKCode(): " << cam.naifIkCode() << "\n";
  //PushFrameCameraDetectorMap* dmap = (PushFrameCameraDetectorMap*) cam.DetectorMap();
  //std::cerr << dmap->FrameletRate() << "\n"
  //	    << dmap->FrameletOffset() << "\n"
  //<< dmap->Framelet() << "\n"
  //	    << dmap->GetBandFirstDetectorLine() << "\n";
  std::cout << "OD_K:" << "\n";
  for(auto i : cam.DistortionMap()->OpticalDistortionCoefficients())
    std::cout << i << "\n";
  double ln = atof(Argv[3]);
  double samp = atof(Argv[4]);
  cam.SetImage(samp,ln);
  double look[3];
  cam.LookDirection(look);
  std::cout << "Look vector [" << look[0] << ", " << look[1] << ", " << look[2] << "]\n";
  AlphaCube acube(cube);
  double psamp = acube.AlphaSample(samp);
  double pline = acube.AlphaLine(ln);
  std::cout << "Alpha cube values: (" << pline << ", " << psamp << ")\n";
  cam.DetectorMap()->SetParent(psamp, pline);
  double dsamp = cam.DetectorMap()->DetectorSample();
  double dln   = cam.DetectorMap()->DetectorLine();
  std::cout << "Detector: (" << dln << ", " << dsamp << ")\n";
  cam.FocalPlaneMap()->SetDetector(dsamp, dln);
  double fx = cam.FocalPlaneMap()->FocalPlaneX();
  double fy = cam.FocalPlaneMap()->FocalPlaneY();
  std::cout << "Focal plane: (" << fx << ", " << fy << ") (compare to fc_to_focal_plane in geocal)\n";
  cam.DistortionMap()->SetFocalPlane(fx, fy);
  double x = cam.DistortionMap()->UndistortedFocalPlaneX();
  double y = cam.DistortionMap()->UndistortedFocalPlaneY();
  double z = cam.DistortionMap()->UndistortedFocalPlaneZ();
  std::cout << "UndistortedFocalPlane [" << x << ", " << y << ", " << z << "] (compare to focal_plane_to_dcs in geocal)\n";
  // Scale to unit vector.
  double k = sqrt(x*x+y*y+z*z);
  std::cout << "Look vector [" << x/k << ", " << y/k << ", " << z/k << "]\n";
}
