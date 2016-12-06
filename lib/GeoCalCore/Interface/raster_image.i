// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "geocal_common.i"

%{
#include "raster_image.h"
%}
%base_import(generic_object)
%import "image_coordinate.i"
%import "map_info.i"
%import "ground_coordinate.i"
%geocal_shared_ptr(GeoCal::RasterImage);
%geocal_shared_ptr(GeoCal::ArrayRasterImage);
%geocal_shared_ptr(GeoCal::RasterImageTileIterator);

namespace GeoCal {
class RasterImage;

%rename("next") RasterImageTileIterator::operator++;
class RasterImageTileIterator: public GenericObject {
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
class RasterImage : public GenericObject {
public:
  double interpolate(double Line, double Sample) const;
  boost::shared_ptr<RasterImage> overview(int Min_number_sample) const;
  // Don't bother with this, since we need to convert from
  //boost::array. Revisit if we actually need this
  //boost::array<double, 2> interpolate_derivative(double Line, double Sample) 
  //  const;
  double interpolate(const ImageCoordinate& Ic) const;
  %python_attribute(number_line, int)
  %python_attribute(number_sample, int)
  %python_attribute(number_tile_line, int)
  %python_attribute(number_tile_sample, int)
  int read(int Line, int Sample) const;
  double unchecked_read_double(int Line, int Sample) const;

  blitz::Array<int, 2>
  read(int Lstart, int Sstart, int Number_line, int Number_sample) 
    const;
  blitz::Array<int, 2>
  read_with_pad(int Lstart, int Sstart, int Number_line, int Number_sample,
		int Fill_value = 0) const;
  blitz::Array<double, 2>
  read_double(int Lstart, int Sstart, int Number_line, int Number_sample) 
    const;
  blitz::Array<double, 2>
  read_double_with_pad(int Lstart, int Sstart, int Number_line, 
		       int Number_sample, double Fill_value = 0.0) const;
  blitz::Array<int, 2>
  read(const RasterImageTileIterator& Ti) const; 
  void write(int Line, int Sample, int Val);
  void write(int Lstart, int Sstart, const blitz::Array<int, 2>& A);
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
  blitz::Array<double, 2> coordinate(const blitz::Array<double, 1>& Lat,
				     const blitz::Array<double, 1>& Lon) const;
  %python_attribute(grid_center_line_resolution, double)
  %python_attribute(grid_center_sample_resolution, double)
  %python_attribute(has_rpc, bool)
  %python_attribute2(rpc, rpc_ptr, boost::shared_ptr<Rpc>)
};

class ArrayRasterImage: public GenericObject {
public:
  ArrayRasterImage();
  %rename(append) push_back;
  void push_back(const boost::shared_ptr<RasterImage>& Img);
  int size() const;
  %extend {
    boost::shared_ptr<GeoCal::RasterImage> get(int i) const
    { return (*$self)(i); }
    void set(int i, const boost::shared_ptr<GeoCal::RasterImage>& V) 
    { (*$self)(i) = V; }
  }
%pythoncode %{
def __getitem__(self, index):
  return self.get(index)

def __setitem__(self, index, val):
  self.set(index, val)

def __len__(self):
  return self.size()
%}
  std::string print_to_string() const;
  %pickle_serialization();
};

// Move this to raster_image_multi_band.i so SWIG can do the
// handling of overloaded function correctly
//void copy(const RasterImage& Img_in, RasterImage& Img_out, bool Diagnostic = false, int Tile_nline = -1, int Tile_nsamp = -1);
void copy_no_fill(const RasterImage& Img_in, RasterImage& Img_out, int Fill_value = 0, bool diagnostic = false);

}

%template(Vector_RasterImage) std::vector<boost::shared_ptr<GeoCal::RasterImage> >;

