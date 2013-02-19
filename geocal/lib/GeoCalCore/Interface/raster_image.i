// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "raster_image.h"
#include "igc_map_projected.h"
#include "ipi_map_projected.h"
#include "orbit_map_projected.h"
#include "apply_mask.h"
#include "magnify_bilinear.h"
#include "magnify_replicate.h"
#include "scale_image.h"
#include "smooth_image.h"
#include "worldview2_cloudmask.h"
#include "image_point_display.h"
#include "vicar_raster_image.h"
#include "vicar_multi_file.h"
#include "raster_subsample.h"
#include "raw_raster_image.h"
#include "rpc_image.h"
#include "sub_raster_image.h"
#include "vicar_lite_file.h"
%}

// In geocal_rpc.i
//%geocal_shared_ptr(RasterImage);
%geocal_shared_ptr(RasterImageTileIterator);

// Note, MaterialDetect is not included in the list below. This is
// only compiled if we select afids_b, so we don't always have it
// available. For now, just leave it out so we don't need some
// complicated afids_b logic. We can figure out how to put this in
// if this ever becomes an issue.
%shared_ptr_dynamic_list(GeoCal::RasterImage,
			 GeoCal::MapReprojectedImage,
			 GeoCal::MemoryRasterImage,
			 GeoCal::RasterAveraged,
			 GeoCal::ImagePointDisplay,
			 GeoCal::GdalRasterImage,
			 GeoCal::VicarRasterImage,
			 GeoCal::VicarMultiFile,
			 GeoCal::RasterSubSample,
			 GeoCal::RawRasterImage,
			 GeoCal::RpcImage,
			 GeoCal::SubRasterImage,
			 GeoCal::VicarLiteRasterImage,
			 GeoCal::IgcMapProjected,
			 GeoCal::IpiMapProjected,
			 GeoCal::OrbitMapProjected,
			 GeoCal::ApplyMask,
			 GeoCal::MagnifyBilinear,
			 GeoCal::MagnifyReplicate,
			 GeoCal::ScaleImage,
			 GeoCal::SmoothImage,
			 GeoCal::WorldView2CloudMask,
			 GeoCal::RasterMultifile,
			 GeoCal::RasterImageTiledFile,
			 GeoCal::CalcMapProjected,
			 GeoCal::CalcRaster,
			 GeoCal::RasterImageVariable)
namespace GeoCal {
class RasterImage;

%rename("next") RasterImageTileIterator::operator++;
class RasterImageTileIterator {
public:
  RasterImageTileIterator(const RasterImage& Ri);
  %python_attribute(istart, int)
  %python_attribute(jstart, int)
  %python_attribute(iend, int)
  %python_attribute(jend, int)
  %python_attribute(number_line, int)
  %python_attribute(number_sample, int)
  %python_attribute(end, bool)
  RasterImageTileIterator& operator++();
  std::string print_to_string() const;
};

%nodefaultctor RasterImage;

// Forward declaration.
class Rpc;
class RasterImage {
public:
  double interpolate(double Line, double Sample) const;
  virtual boost::shared_ptr<RasterImage> overview(int Min_number_sample) const;
  // Don't bother with this, since we need to convert from
  //boost::array. Revisit if we actually need this
  //boost::array<double, 2> interpolate_derivative(double Line, double Sample) 
  //  const;
  double interpolate(const ImageCoordinate& Ic) const;
  %python_attribute(number_line, int)
  %python_attribute(number_sample, int)
  %python_attribute(number_tile_line, virtual int)
  %python_attribute(number_tile_sample, virtual int)
  virtual int read(int Line, int Sample) const;

  blitz::Array<int, 2>
  read(int Lstart, int Sstart, int Number_line, int Number_sample) 
    const;
  blitz::Array<double, 2>
  read_double(int Lstart, int Sstart, int Number_line, int Number_sample) 
    const;
  blitz::Array<int, 2>
  read(const RasterImageTileIterator& Ti) const; 
  virtual void write(int Line, int Sample, int Val);
  virtual void write(int Lstart, int Sstart, const blitz::Array<int, 2>& A);
  std::string print_to_string() const;
  %python_attribute(has_map_info, bool)
  %python_attribute(map_info, MapInfo)
  boost::shared_ptr<GroundCoordinate> ground_coordinate
  (const ImageCoordinate& Ic) const;
  boost::shared_ptr<GroundCoordinate> ground_coordinate
  (const ImageCoordinate& Ic, const Dem& D) const;
  boost::shared_ptr<GroundCoordinate> ground_coordinate
  (double Line, double Sample) const;
  boost::shared_ptr<GroundCoordinate> ground_coordinate
  (double Line, double Sample, const Dem& D) const;
  ImageCoordinate coordinate(const GroundCoordinate& Gc) const;
  // SubMapProjectedImage cover(const 
  //      std::vector<boost::shared_ptr<GroundCoordinate> >& Pt,
  //      int boundary = 0) const;

  %python_attribute(grid_center_line_resolution, double)
  %python_attribute(grid_center_sample_resolution, double)
  %python_attribute(has_rpc, bool)
  %python_attribute(rpc, Rpc)
};

void copy(const RasterImage& Img_in, RasterImage& Img_out, bool Diagnostic = false, int Tile_nline = -1, int Tile_nsamp = -1);
void copy_no_fill(const RasterImage& Img_in, RasterImage& Img_out, int Fill_value = 0, bool diagnostic = false);

}

%template(Vector_RasterImage) std::vector<boost::shared_ptr<GeoCal::RasterImage> >;

