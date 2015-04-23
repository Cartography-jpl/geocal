#ifndef RASTER_IMAGE_TILED_FILE_H
#define RASTER_IMAGE_TILED_FILE_H
#include "tiled_file.h"
#include "raster_image_variable.h"

namespace GeoCal {
/****************************************************************//**
  This is a RasterImage that is implemented as a TiledFile<T, 2>.
*******************************************************************/

class RasterImageTiledFile : public RasterImageVariable {
public:
  typedef TiledFileBase<2>::index index;

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  RasterImageTiledFile(const boost::shared_ptr<TiledFileBase<2> >& D)
  { initialize(D);}

  RasterImageTiledFile(const boost::shared_ptr<TiledFileBase<2> >& D,
		       const MapInfo& M)
  { initialize(D, M);}

  virtual ~RasterImageTiledFile() {}

//-----------------------------------------------------------------------
/// Underlying data.
//-----------------------------------------------------------------------

  const TiledFileBase<2>& tile_file_base() const { return *data_;}

//-----------------------------------------------------------------------
/// Underlying data.
//-----------------------------------------------------------------------

  TiledFileBase<2>& tile_file_base() { return *data_;}

//-----------------------------------------------------------------------
/// Underlying data.
//-----------------------------------------------------------------------

  const boost::shared_ptr<TiledFileBase<2> >& tile_file_base_ptr() const 
  { return data_;}

//-----------------------------------------------------------------------
/// Return pixel value at given line and sample.
//-----------------------------------------------------------------------

  virtual int unchecked_read(int Line, int Sample) const
  { 
    boost::array<index, 2> i = {{Line, Sample}};
    return data_->get_int(i);
  }

//-----------------------------------------------------------------------
/// Return pixel value at given line and sample.
//-----------------------------------------------------------------------

  virtual double unchecked_read_double(int Line, int Sample) const
  { 
    boost::array<index, 2> i = {{Line, Sample}};
    return data_->get_double(i);
  }

//-----------------------------------------------------------------------
/// Number of tiles
//-----------------------------------------------------------------------

  int number_tile() const { return data_->number_tile(); }

//-----------------------------------------------------------------------
/// Return a subset of the image.
//-----------------------------------------------------------------------

  virtual void read_ptr(int Lstart, int Sstart, int Number_line, 
			int Number_sample, int* Res) const
  { 
    boost::array<index, 2> min_index = {{Lstart, Sstart}};
    boost::array<index, 2> max_index = {{Lstart + Number_line, 
					 Sstart + Number_sample}};
    data_->read_int(min_index, max_index, Res);
  }

  virtual blitz::Array<double, 2> 
  read_double(int Lstart, int Sstart, int Number_line, 
	      int Number_sample) const
  {
    boost::array<index, 2> min_index = {{Lstart, Sstart}};
    boost::array<index, 2> max_index = {{Lstart + Number_line, 
					 Sstart + Number_sample}};
    blitz::Array<double, 2> res(Number_line, Number_sample);
    data_->read_double(min_index, max_index, res.data());
    return res;
  }

//-----------------------------------------------------------------------
/// Write the pixel value to the given location.
//-----------------------------------------------------------------------

  virtual void unchecked_write(int Line, int Sample, int Val)
  { 
    boost::array<index, 2> i = {{Line, Sample}};
    data_->write_int(i, Val);
  }

//-----------------------------------------------------------------------
/// Write a subset of the image.
//-----------------------------------------------------------------------

  virtual void write_ptr(int Lstart, int Sstart, int Number_line, 
			int Number_sample, const int* V)
  { 
    boost::array<index, 2> min_index = {{Lstart, Sstart}};
    boost::array<index, 2> max_index = {{Lstart + Number_line, 
					 Sstart + Number_sample}};
    data_->write_int_arr(min_index, max_index, V);
  }

//-----------------------------------------------------------------------
/// Flush data to disk
//-----------------------------------------------------------------------

  virtual void flush() const { data_->flush(); }

//-----------------------------------------------------------------------
/// Number of times we have swapped a tile since reset_number_swap called.
//-----------------------------------------------------------------------

  unsigned int number_swap() const {return data_->number_swap();}

//-----------------------------------------------------------------------
/// Reset number of swap counter to 0.
//-----------------------------------------------------------------------

  void reset_number_swap()  {data_->reset_number_swap();}

protected:
//-----------------------------------------------------------------------
/// Default constructor. Derived classes should call initialize before
/// finishing their constructor.
//-----------------------------------------------------------------------

  RasterImageTiledFile() {}

//-----------------------------------------------------------------------
/// Initialize class.
//-----------------------------------------------------------------------

  void initialize(const boost::shared_ptr<TiledFileBase<2> >& D)
  {
    data_ = D;
    number_line_ = D->size()[0];
    number_sample_ = D->size()[1];
    number_tile_line_ = data_->tile_size()[0];
    number_tile_sample_ = data_->tile_size()[1];
  }
  void initialize(const boost::shared_ptr<TiledFileBase<2> >& D,
		  const MapInfo& M)
  {
    initialize(D);
    map_info_.reset(new MapInfo(M));
  }

  boost::shared_ptr<TiledFileBase<2> > data_; ///< Underlying data.
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
};

}

GEOCAL_EXPORT_KEY(RasterImageTiledFile);
#endif

