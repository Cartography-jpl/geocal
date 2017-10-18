#ifndef RASTER_IMAGE_H
#define RASTER_IMAGE_H
#include "printable.h"
#include "image_coordinate.h"
#include "geocal_exception.h"
#include "map_info.h"
#include "geocal_rpc.h"
#include <boost/multi_array.hpp>
#include <boost/shared_ptr.hpp>
#include <blitz/array.h>
#include <vector>
#include <cmath>

namespace GeoCal {
  class RasterImageTileIterator;

/****************************************************************//**
  This is a general image class. Conceptually, an image is just a two
  dimensional array of integers, although it is not necessarily
  implemented this way. This is an abstract class, containing behavior
  common to all images. It handles keeping track of the size of the
  image, but all other behaviour needs to be defined by subclasses.
 
  It is common for Raster data to be stored on disk as a series of
  tiles. It can be significantly more efficient to access and write
  the data in tiles. To support this, we provide a "preferred" tile
  size given by number_tile_line() and number_tile_sample(). Classes
  using a RasterImage are free to ignore this tiling - data can be
  read and written in any order desired, and the only impact of tiles
  is in the performance.
  
  It is very common to want to step through a file
  "efficiently". There is a macro "RASTER_FOREACH(Img, IND1, IND2)"
  that steps through a given raster image, setting the index variable
  IND1 and IND2 to the line and samples. This steps through by tiles,
  so it avoids reading the same tile multiple times. If you need to
  step through by tiles (rather than looping through all line and
  samples), the class RasterImageTileIterator can be used. 
 
  The default tile size is the entire image, derived classes can
  supply other values if desired.

  A RasterImage may or may not have a Rpc and MapInfo associated with
  it. You can query has_rpc() and has_mapinfo() to find out if it has
  this metadata, and if it does you can access this by rpc() and
  map_info().
*******************************************************************/

class RasterImage : public Printable<RasterImage> {
public:
  virtual ~RasterImage() {}

//-----------------------------------------------------------------------
/// This does a bilinear interpolation of the data for fractional
/// Line and Sample.
//-----------------------------------------------------------------------

  double interpolate(double Line, double Sample) const
  {
    range_check(Line, 0.0, (double) (number_line() - 1));
    range_check(Sample, 0.0, (double) (number_sample() - 1));
    return unchecked_interpolate(Line, Sample);
  }

//-----------------------------------------------------------------------
/// This does a bilinear interpolation of the data for fractional
/// Line and Sample. This does not do range checking.
//-----------------------------------------------------------------------

  double unchecked_interpolate(double Line, double Sample) const
  {
    int i = (int) Line;
    int j = (int) Sample;
    double t1 = unchecked_read_double(i, j);
    double t2 = unchecked_read_double(i, j + 1);
    double t3 = unchecked_read_double(i + 1, j);
    double t4 = unchecked_read_double(i + 1, j + 1);
    double t5 = t1 + (t2 - t1) * (Sample - j);
    double t6 = t3 + (t4 - t3) * (Sample - j);
    return t5 + (t6 - t5) * (Line - i);
  }

//-----------------------------------------------------------------------
/// This calculates the derivative of interpolate with respect to Line
/// and Sample. First entry in results is derivative with respect to
/// Line, the second is with respect to the Sample.
//-----------------------------------------------------------------------

  boost::array<double, 2> interpolate_derivative(double Line, double Sample) 
    const
  {
    int i = (int) Line;
    int j = (int) Sample;
    range_check(i, 0, number_line() - 1);
    range_check(j, 0, number_sample() - 1);
    double t1 = unchecked_read_double(i, j);
    double t2 = unchecked_read_double(i, j + 1);
    double t3 = unchecked_read_double(i + 1, j);
    double t4 = unchecked_read_double(i + 1, j + 1);
    double t5 = t1 + (t2 - t1) * (Sample - j);
    double dt5 = t2 - t1;
    double t6 = t3 + (t4 - t3) * (Sample - j);
    double dt6 = t4 - t3;
    boost::array<double, 2> res;
    res[0] = t6 - t5;
    res[1] = dt5 + (dt6 - dt5) * (Line - i);
    return res;
  }

  virtual blitz::Array<double, 2> interpolate(double Line, double Sample, 
      int Number_line, int Number_sample, double Fill_value = 0.0) const;

//-----------------------------------------------------------------------
/// This does a bilinear interpolation of the data for fractional
/// Line and Sample.
//-----------------------------------------------------------------------

  double interpolate(const ImageCoordinate& Ic) const 
  { return interpolate(Ic.line, Ic.sample); }

//-----------------------------------------------------------------------
/// Some RasterImage may have overviews which are lower resolution
/// images. If we do have these, then this will return the lower
/// resolution RasterImage that has at least the given number of
/// samples. If we don't have an overview, this returns a null - which
/// the calling routine needs to be ready to handle.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<RasterImage> overview(int Min_number_sample) const
  { return boost::shared_ptr<RasterImage>(); }

//-----------------------------------------------------------------------
/// Return number of lines in the image.
//-----------------------------------------------------------------------

  virtual int number_line() const = 0;

//-----------------------------------------------------------------------
/// Return number of samples in the image.
//-----------------------------------------------------------------------

  virtual int number_sample() const = 0;

//-----------------------------------------------------------------------
/// Number of lines in the preferred tile size. Default is the entire 
/// image.
//-----------------------------------------------------------------------

  virtual int number_tile_line() const {return number_line();}

//-----------------------------------------------------------------------
/// Number of samples in the preferred tile size. Default is the entire 
/// image.
//-----------------------------------------------------------------------

  virtual int number_tile_sample() const {return number_sample();}

//-----------------------------------------------------------------------
/// Return pixel value at given line and sample.
//-----------------------------------------------------------------------

  int operator()(int Line, int Sample) const
  {
    range_check(Line, 0, number_line());
    range_check(Sample, 0, number_sample());
    return unchecked_read(Line, Sample);
  }

//-----------------------------------------------------------------------
/// Most of the time you should use the range checked operator(). But
/// for some tight loops the range checking may be prohibitive. In
/// those cases you can use this unchecked version.
//-----------------------------------------------------------------------

  virtual int unchecked_read(int Line, int Sample) const = 0;
  virtual double unchecked_read_double(int Line, int Sample) const = 0;

//-----------------------------------------------------------------------
/// As an optimization, we assume when copying that data can be
/// represented as a int. That is true of many images. But we actually
/// need to use double, then we need to know that in the copy command.
/// This function indicates if we need a double or not.
//-----------------------------------------------------------------------

  virtual bool copy_needs_double() const {return false;}
  
//-----------------------------------------------------------------------
/// Alternate name for operator(). Languages that wrap this class such
/// as Ruby don't support operator(), so we give another name for the
/// same function.
//-----------------------------------------------------------------------

  int read(int Line, int Sample) const {return (*this)(Line, Sample);}

  boost::multi_array<int, 2>
    read_array(int Lstart, int Sstart, int Number_line, int Number_sample) 
    const;

//-----------------------------------------------------------------------
/// Return a subset of the image.
//-----------------------------------------------------------------------

  virtual void read_ptr(int Lstart, int Sstart, int Number_line, 
			int Number_sample, int* Res) const = 0;
  blitz::Array<int, 2>
  read(int Lstart, int Sstart, int Number_line, int Number_sample) 
    const;
  blitz::Array<int, 2>
  read_with_pad(int Lstart, int Sstart, int Number_line, int Number_sample,
		int Fill_value = 0) const;
  blitz::Array<int, 2>
  read(const RasterImageTileIterator& Ti) const; 

  virtual blitz::Array<double, 2> 
  read_double(int Lstart, int Sstart, int Number_line, 
	      int Number_sample) const;
  blitz::Array<double, 2>
  read_double_with_pad(int Lstart, int Sstart, int Number_line, 
		       int Number_sample, double Fill_value = 0.0) const;

//-----------------------------------------------------------------------
/// Write the pixel value to the given location.
//-----------------------------------------------------------------------

  virtual void write(int Line, int Sample, int Val)
  {
    range_check(Line, 0, number_line());
    range_check(Sample, 0, number_sample());
    unchecked_write(Line, Sample, Val);
  }
  virtual void write(int Line, int Sample, double Val)
  {
    range_check(Line, 0, number_line());
    range_check(Sample, 0, number_sample());
    unchecked_write(Line, Sample, Val);
  }

//-----------------------------------------------------------------------
/// Most of the time you should use the range checked write. But
/// for some tight loops the range checking may be prohibitive. In
/// those cases you can use this unchecked version.
//-----------------------------------------------------------------------

  virtual void unchecked_write(int Line, int Sample, int Val) = 0;
  virtual void unchecked_write(int Line, int Sample, double Val) = 0;

//-----------------------------------------------------------------------
/// Write a subset of the data. The default is just to call write for
/// each pixel, but derived classes can implement more efficient
/// versions of this.
//-----------------------------------------------------------------------

  virtual void write_ptr(int Lstart, int Sstart, int Number_line, 
			 int Number_sample, const int* V)
  {
    for(int i = Lstart; i < Lstart + Number_line; ++i)
      for(int j = Sstart; j < Sstart + Number_sample; ++j, ++V)
	write(i, j, *V);
  }
  virtual void write_ptr(int Lstart, int Sstart, int Number_line, 
			 int Number_sample, const double* V)
  {
    for(int i = Lstart; i < Lstart + Number_line; ++i)
      for(int j = Sstart; j < Sstart + Number_sample; ++j, ++V)
	write(i, j, *V);
  }

  virtual void write(int Lstart, int Sstart, const blitz::Array<int, 2>& A);
  virtual void write(int Lstart, int Sstart, const blitz::Array<double, 2>& A);

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { Os << "Raster image " << number_line() << " x " 
       << number_sample() << "\n"; }

/// *********************************************************************
/// Functions available if we have MapInfo data.
/// *********************************************************************

//-----------------------------------------------------------------------
/// Indicate if we have MapInfo. The default is false, but derived
/// classes can override this.
//-----------------------------------------------------------------------

  virtual bool has_map_info() const {return false;}

///-----------------------------------------------------------------------
/// MapInfo for image.
//-----------------------------------------------------------------------

  virtual const MapInfo& map_info() const 
  { throw Exception("Do not have MapInfo for this RasterImage"); }

//-----------------------------------------------------------------------
/// Shortcut to calling mapinfo().ground_coordinate.
//-----------------------------------------------------------------------

  boost::shared_ptr<GroundCoordinate> ground_coordinate
  (const ImageCoordinate& Ic) const
  { return map_info().ground_coordinate(Ic.sample, Ic.line); }

//-----------------------------------------------------------------------
/// Shortcut to calling mapinfo().ground_coordinate.
//-----------------------------------------------------------------------

  boost::shared_ptr<GroundCoordinate> ground_coordinate
  (const ImageCoordinate& Ic, const Dem& D) const
  { return map_info().ground_coordinate(Ic.sample, Ic.line, D); }

//-----------------------------------------------------------------------
/// Shortcut to calling mapinfo().ground_coordinate.
//-----------------------------------------------------------------------

  boost::shared_ptr<GroundCoordinate> ground_coordinate
  (double Line, double Sample) const
  { return map_info().ground_coordinate(Sample, Line); }

//-----------------------------------------------------------------------
/// Shortcut to calling mapinfo().ground_coordinate.
//-----------------------------------------------------------------------

  boost::shared_ptr<GroundCoordinate> ground_coordinate
  (double Line, double Sample, const Dem& D) const
  { return map_info().ground_coordinate(Sample, Line, D); }

//-----------------------------------------------------------------------
/// Shortcut to calling mapinfo().coordinate.
//-----------------------------------------------------------------------

  ImageCoordinate coordinate(const GroundCoordinate& Gc) const
  { ImageCoordinate ic; map_info().coordinate(Gc, ic.sample, ic.line);
    return ic;}
  ImageCoordinate coordinate(const Geodetic& Gc) const
  { ImageCoordinate ic; map_info().coordinate(Gc, ic.sample, ic.line);
    return ic;}
  blitz::Array<double, 2> coordinate(const blitz::Array<double, 1>& Lat,
				     const blitz::Array<double, 1>& Lon) const;
  
  double grid_center_line_resolution() const;
  double grid_center_sample_resolution() const;

/// *********************************************************************
/// Functions available if we have RPC
/// *********************************************************************

//-----------------------------------------------------------------------
/// Indicate if we have Rpc. The default is false, but derived
/// classes can override this.
//-----------------------------------------------------------------------

  virtual bool has_rpc() const {return false;}

///-----------------------------------------------------------------------
/// Rpc for image.
//-----------------------------------------------------------------------

  virtual Rpc rpc() const 
  { throw Exception("Do not have RPC for this RasterImage"); }

///-----------------------------------------------------------------------
/// Pointer version of rpc(). This makes python happier.
///-----------------------------------------------------------------------

  boost::shared_ptr<Rpc> rpc_ptr() const
  { return boost::shared_ptr<Rpc>(new Rpc(rpc())); }

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a array of Raster Image. This isn't much more than a 
  std::vector, but we do have added support in python of being able to
  serialize this.
*******************************************************************/

class ArrayRasterImage : public Printable<ArrayRasterImage> {
public:
//-----------------------------------------------------------------------
/// Create an empty array.
//-----------------------------------------------------------------------
  
  ArrayRasterImage() {}
  virtual ~ArrayRasterImage() {}
  
//-----------------------------------------------------------------------
/// Add to the array.
//-----------------------------------------------------------------------
  void push_back(const boost::shared_ptr<RasterImage>& Img)
  { img_.push_back(Img); }

//-----------------------------------------------------------------------
/// Return size
//-----------------------------------------------------------------------
  int size() const {return int(img_.size()); }

//-----------------------------------------------------------------------
/// Return value at given index
//-----------------------------------------------------------------------
  const boost::shared_ptr<RasterImage>& operator()(int I) const
  { range_check(I, 0, size()); return img_[I];}

  boost::shared_ptr<RasterImage>& operator()(int I)
  { range_check(I, 0, size()); return img_[I];}

  void print(std::ostream& Os) const
  {
    Os << "ArrayRasterImage of " << size() << " images";
  }
private:
  std::vector<boost::shared_ptr<RasterImage> > img_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};  

void copy(const RasterImage& Img_in, RasterImage& Img_out, bool diagnostic = false, int Tile_nline = -1, int Tile_nsamp = -1);
void copy_no_fill(const RasterImage& Img_in, RasterImage& Img_out, int Fill_value = 0, bool diagnostic = false);
void copy_only_to_fill(const RasterImage& Img_in, RasterImage& Img_out, int Fill_value = 0, bool diagnostic = false);

/// Loop through all data, going by tiles.

#define RASTER_FOREACH(IMG, IND1, IND2) \
for(int i1 = 0; i1 < (IMG).number_line(); i1 += (IMG).number_tile_line()) \
  for(int j1 = 0; j1 < (IMG).number_sample(); j1 += (IMG).number_tile_sample()) \
    for(int IND1 = i1; IND1 < i1 + (IMG).number_tile_line() && \
        IND1 < (IMG).number_line(); ++IND1) \
      for(int IND2 = j1; IND2 < j1 + (IMG).number_tile_sample() && \
          IND2 < (IMG).number_sample(); ++IND2)

/****************************************************************//**
  It can be useful to step through a Raster Image in tiles. This is
  a helper class for RasterImage to do this.
*******************************************************************/

class RasterImageTileIterator : public Printable<RasterImageTileIterator>
{
public:
  RasterImageTileIterator(const RasterImage& Ri);
  RasterImageTileIterator(const RasterImage& Ri, int Number_tile_line, 
			  int Number_tile_sample);
  int istart() const { return istart_;}
  int jstart() const { return jstart_;}
  int iend() const {return iend_;}
  int jend() const {return jend_;}
  int number_line() const {return iend_ - istart_; }
  int number_sample() const {return jend_ - jstart_; }
  bool end() const
  { return istart() >= ri.number_line(); }
  RasterImageTileIterator& operator++();
  void print(std::ostream& Os) const
  { 
    Os << "RasterImageTileIterator (" << istart() << ", " << jstart() 
       << ") - (" << iend() << ", " << jend() << ")";
  }
private:
  const RasterImage& ri;
  int istart_, jstart_, iend_, jend_, ntl, nts;
  bool end_;
};

}

GEOCAL_EXPORT_KEY(ArrayRasterImage);
GEOCAL_EXPORT_KEY(RasterImage);
#endif

