#ifndef VICAR_LITE_FILE_H
#define VICAR_LITE_FILE_H
#include "geocal_exception.h"
#include "map_info.h"
#include "geocal_rpc.h"
#include "memory_map_array.h"
#include "raster_image_variable.h"
#include "dem_map_info.h"
#include "printable.h"
#include <boost/shared_ptr.hpp>
#include <map>
#include <set>
#include <vector>
#include <fstream>

namespace GeoCal {

/****************************************************************//**
  This handles opening and closing a Vicar file. This class doesn't
  actually read and write data, but is used by other classes which do.

  This class is a simple implementation of Vicar. It doesn't use the
  Vicar RTL library, which is the primary attraction of it. We can
  use this to read Vicar files in a context where we don't want to use
  the RTL library, or don't want to be constrained to running in the
  Vicar environment.

  This class does *not* handle new VICAR features such as compression.
*******************************************************************/

class VicarLiteFile : public Printable<VicarLiteFile> {
public:
//-----------------------------------------------------------------------
/// Type of data in file.
//-----------------------------------------------------------------------

  enum data_type {VICAR_BYTE, VICAR_HALF, VICAR_FULL, VICAR_FLOAT,
		  VICAR_DOUBLE};

//-----------------------------------------------------------------------
/// Type of access.
//-----------------------------------------------------------------------

  enum access_type {READ, WRITE, UPDATE};
  VicarLiteFile(const std::string& Fname, access_type Access = READ,
		bool Force_area_pixel = false);
  VicarLiteFile(const std::string& Fname, int Number_line, int Number_sample,
	    const std::string& Type = "BYTE");
  virtual ~VicarLiteFile() {}

//-----------------------------------------------------------------------
/// Access type of file.
//-----------------------------------------------------------------------

  access_type access() const {return access_;}

//-----------------------------------------------------------------------
/// If true, then force the file to be treated as "pixel is area".
/// This is really just meant as a work around for the SRTM data,
/// which incorrectly labels the data as "point" rather than
/// "area". Since this is a 15 meter difference, it matters for many
/// applications. Most users should just ignore this value.
//-----------------------------------------------------------------------

  bool force_area_pixel() const { return force_area_pixel_; }

//-----------------------------------------------------------------------
/// Offset to where the data starts.
//-----------------------------------------------------------------------

  int data_offset() const {return data_offset_;}

//-----------------------------------------------------------------------
/// File name
//-----------------------------------------------------------------------
  
  const std::string& file_name() const { return fname_; }

  static bool is_vicar_file(const std::string& Fname);

//-----------------------------------------------------------------------
/// List of properties.
//-----------------------------------------------------------------------

  const std::set<std::string>& property() const {return prop_set_;}

//-----------------------------------------------------------------------
/// Return value for the given label. Optionally also supply a
/// property for labels that are part of one (e.g., GEOTIFF)
//-----------------------------------------------------------------------
  
  template<class T> T label(const std::string& F, 
			    const std::string& Property = "") const;

//   void label_set(const std::string& F, 
// 		 int Val,
// 		 const std::string& Property = "");
//   void label_set(const std::string& F, 
// 		 double Val,
// 		 const std::string& Property = "");
//   void label_set(const std::string& F, 
// 		 const std::string& Val,
// 		 const std::string& Property = "");

//-----------------------------------------------------------------------
/// Sometimes it is more convenient to work directly with a map 
//-----------------------------------------------------------------------

  const std::map<std::string, std::string>& label_map() const
  {return label_;}

//-----------------------------------------------------------------------
/// Number of bands in file.
//-----------------------------------------------------------------------

  int number_band() const {return number_band_;}

//-----------------------------------------------------------------------
/// Number of lines in file.
//-----------------------------------------------------------------------

  int number_line() const {return number_line_;}

//-----------------------------------------------------------------------
/// Number of samples in file.
//-----------------------------------------------------------------------

  int number_sample() const {return number_sample_;}

//-----------------------------------------------------------------------
/// Type of data in file
//-----------------------------------------------------------------------

  data_type type() const {return type_;}

  bool has_map_info() const;
  bool has_rpc() const;
  MapInfo map_info() const;
  void map_info(const MapInfo& M);
  Rpc rpc() const;
  void rpc(const Rpc& R) { throw Exception("Not yet implemented"); }
  int read_int(int B, int L, int S) const;
  double read_double(int B, int L, int S) const;
  void read_int(int B, int L, int S, int Nb, int Nl, int Ns, int* Res) const;
  void write_int(int B, int L, int S, int V) const;

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { Os << "Vicar Lite File \n" 
       << "  File:          " << file_name() << "\n"
       << "  Number line:   " << number_line() << "\n"
       << "  Number sample: " << number_sample() << "\n";
  }

//-----------------------------------------------------------------------
/// Indicate if the file is compressed. If it is, we can't actually
/// read the data but we can still read all of the labels.
//-----------------------------------------------------------------------

  bool is_compressed() const { return is_compressed_; }

protected:
//-----------------------------------------------------------------------
/// Data in vicar file, as a memory map. This is the raw data, so it
/// has not been byte swapped if needed. Regardless of the type of
/// file, this is always in the order band, line, number,
/// byte_in_pixel (so we use boost:multi_array ability to rearrange
/// the order).
//-----------------------------------------------------------------------

  boost::shared_ptr<MemoryMapArray<char, 4> > 
       data_raw;

  template<class C, bool Swap_needed> int to_int(const char* c) const;
  template<class C, bool Swap_needed> void from_int(const int&, 
						    char* c) const;

//-----------------------------------------------------------------------
/// Convert raw bytes to a double, swapping if needed.
//-----------------------------------------------------------------------

  template<class C, bool Swap_needed> double to_double(const char* c) const
  { return to_int<C, Swap_needed>(c);}

private:
  access_type access_;
  bool is_compressed_;
  std::set<std::string> prop_set_; ///<List of properties.
  int data_offset_;
  std::string fname_;
  bool force_area_pixel_;
  int number_line_;
  int number_sample_;
  int number_band_;
  bool swap_needed_;
  data_type type_;
  boost::shared_ptr<std::fstream> f_;
  std::map<std::string, std::string> label_;
  std::string read_label(int& lblsize);
  void process_label(const std::string& label);
};

//-----------------------------------------------------------------------
/// Return value for the given label.
//-----------------------------------------------------------------------
  
template<> inline int VicarLiteFile::label<int>(const std::string& K, 
					 const std::string& Property) const
{
  std::string t = (Property == "" ? K : Property + " " + K);
  std::map<std::string, std::string>::const_iterator i = label_.find(t);
  if(i ==label_.end())
    throw Exception("Label " + K + " is not found in file " + fname_);
  return atoi((*i).second.c_str());
}

//-----------------------------------------------------------------------
/// Return value for the given label.
//-----------------------------------------------------------------------
  
template<> inline std::string 
VicarLiteFile::label<std::string>(const std::string& K, 
				  const std::string& Property) const
{
  std::string t = (Property == "" ? K : Property + " " + K);
  std::map<std::string, std::string>::const_iterator i = label_.find(t);
  if(i ==label_.end())
    throw Exception("Label " + K + " is not found in file " + fname_);
  return (*i).second;
}

//-----------------------------------------------------------------------
/// Return value for the given label.
//-----------------------------------------------------------------------
  
template<> inline double 
VicarLiteFile::label<double>(const std::string& K, 
			     const std::string& Property) const
{
  std::string t = (Property == "" ? K : Property + " " + K);
  std::map<std::string, std::string>::const_iterator i = label_.find(t);
  if(i ==label_.end())
    throw Exception("Label " + K + " is not found in file " + fname_);
  return atof((*i).second.c_str());
}

//-----------------------------------------------------------------------
/// Convert to int.
//-----------------------------------------------------------------------

template<> inline int VicarLiteFile::to_int<unsigned char, true>(const char* c) const
{ return *reinterpret_cast<const unsigned char*>(c); }
template<> inline int VicarLiteFile::to_int<unsigned char, false>(const char* c) const
{ return *reinterpret_cast<const unsigned char*>(c); }
template<> inline int 
VicarLiteFile::to_int<short int, true>(const char* c) const
{ int res = 0;
  char* cr = reinterpret_cast<char*>(&res);
  cr[0] = c[1];
  cr[1] = c[0];
  return res; }
template<> inline int 
VicarLiteFile::to_int<short int, false>(const char* c) const
{ return *(reinterpret_cast<const short int*>(c)); }
template<> inline int 
VicarLiteFile::to_int<int, false>(const char* c) const
{ return *(reinterpret_cast<const int*>(c)); }
template<> inline int 
VicarLiteFile::to_int<int, true>(const char* c) const
{ int res = 0;
  char* cr = reinterpret_cast<char*>(&res);
  cr[0] = c[3];
  cr[1] = c[2];
  cr[2] = c[1];
  cr[3] = c[0];
  return res; 
}

//-----------------------------------------------------------------------
/// Convert from int.
//-----------------------------------------------------------------------

template<> inline void 
VicarLiteFile::from_int<unsigned char, true>(const int& I, char* c) const
{ *c = static_cast<unsigned char>(I); }
template<> inline void 
VicarLiteFile::from_int<unsigned char, false>(const int& I, char* c) const
{ *c = static_cast<unsigned char>(I); }
template<> inline void 
VicarLiteFile::from_int<short int, true>(const int& I, char* c) const
{ 
  const char* cr = reinterpret_cast<const char*>(&I);
  c[0] = cr[1];
  c[1] = cr[0];
}
template<> inline void 
VicarLiteFile::from_int<short int, false>(const int& I, char* c) const
{ *(reinterpret_cast<short int *>(c)) = static_cast<short int>(I); }
template<> inline void 
VicarLiteFile::from_int<int, true>(const int& I, char* c) const
{ 
  const char* cr = reinterpret_cast<const char*>(&I);
  c[0] = cr[3];
  c[1] = cr[2];
  c[2] = cr[1];
  c[3] = cr[0];
}
template<> inline void 
VicarLiteFile::from_int<int, false>(const int& I, char* c) const
{ *(reinterpret_cast<int *>(c)) = static_cast<int>(I); }

//-----------------------------------------------------------------------
/// Read data as an int.
//-----------------------------------------------------------------------

inline int VicarLiteFile::read_int(int B, int L, int S) const 
{
  if(is_compressed_)
    throw Exception("Can't use VicarLiteFile to read a compressed file");
  if(swap_needed_)
    switch(type_) {
    case VICAR_BYTE:
      return to_int<unsigned char, true>(&(data_raw->data()[B][L][S][0]));
    case VICAR_HALF:
      return to_int<short int, true>(&(data_raw->data()[B][L][S][0]));
    case VICAR_FULL:
      return to_int<int, true>(&(data_raw->data()[B][L][S][0]));
    default:
      throw Exception("Unrecognized type");
      }
  else
    switch(type_) {
    case VICAR_BYTE:
      return to_int<unsigned char, false>(&(data_raw->data()[B][L][S][0]));
    case VICAR_HALF:
      return to_int<short int, false>(&(data_raw->data()[B][L][S][0]));
    case VICAR_FULL:
      return to_int<int, false>(&(data_raw->data()[B][L][S][0]));
    default:
      throw Exception("Unrecognized type");
    }
}	  

//-----------------------------------------------------------------------
/// Read data as an double.
//-----------------------------------------------------------------------

inline double VicarLiteFile::read_double(int B, int L, int S) const 
{
  if(is_compressed_)
    throw Exception("Can't use VicarLiteFile to read a compressed file");
  if(swap_needed_)
    switch(type_) {
    case VICAR_BYTE:
      return to_double<unsigned char, true>(&(data_raw->data()[B][L][S][0]));
    case VICAR_HALF:
      return to_double<short int, true>(&(data_raw->data()[B][L][S][0]));
    case VICAR_FULL:
      return to_double<int, true>(&(data_raw->data()[B][L][S][0]));
    default:
      throw Exception("Unrecognized type");
      }
  else
    switch(type_) {
    case VICAR_BYTE:
      return to_double<unsigned char, false>(&(data_raw->data()[B][L][S][0]));
    case VICAR_HALF:
      return to_double<short int, false>(&(data_raw->data()[B][L][S][0]));
    case VICAR_FULL:
      return to_double<int, false>(&(data_raw->data()[B][L][S][0]));
    default:
      throw Exception("Unrecognized type");
    }
}	  

//-----------------------------------------------------------------------
/// Read data as an int.
//-----------------------------------------------------------------------

inline void VicarLiteFile::read_int(int B, int L, int S, int Nb, int Nl, 
			     int Ns, int* Res) const
{
  if(is_compressed_)
    throw Exception("Can't use VicarLiteFile to read a compressed file");
  if(swap_needed_)
    switch(type_) {
    case VICAR_BYTE:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S; k < S + Ns; ++k, ++Res)
	    *Res = to_int<unsigned char, true>(&(data_raw->data()[i][j][k][0]));
      break;
    case VICAR_HALF:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S; k < S + Ns; ++k, ++Res)
	    *Res = to_int<short int, true>(&(data_raw->data()[i][j][k][0]));
      break;
    case VICAR_FULL:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S; k < S + Ns; ++k, ++Res)
	    *Res = to_int<int, true>(&(data_raw->data()[i][j][k][0]));
      break;
    default:
      throw Exception("Unrecognized type");
      }
  else
    switch(type_) {
    case VICAR_BYTE:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S; k < S + Ns; ++k, ++Res)
	    *Res = to_int<unsigned char, false>(&(data_raw->data()[i][j][k][0]));
      break;
    case VICAR_HALF:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S; k < S + Ns; ++k, ++Res)
	    *Res = to_int<short int, false>(&(data_raw->data()[i][j][k][0]));
      break;
    case VICAR_FULL:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S; k < S + Ns; ++k, ++Res)
	    *Res = to_int<int, false>(&(data_raw->data()[i][j][k][0]));
      break;
    default:
      throw Exception("Unrecognized type");
    }
}

//-----------------------------------------------------------------------
/// Write data as an int.
//-----------------------------------------------------------------------

inline void VicarLiteFile::write_int(int B, int L, int S, int V) const 
{
  if(is_compressed_)
    throw Exception("Can't use VicarLiteFile to write a compressed file");
  if(swap_needed_)
    switch(type_) {
    case VICAR_BYTE:
      from_int<unsigned char, true>(V, &(data_raw->data()[B][L][S][0]));
      break;
    case VICAR_HALF:
      from_int<short int, true>(V, &(data_raw->data()[B][L][S][0]));
      break;
    case VICAR_FULL:
      from_int<int, true>(V, &(data_raw->data()[B][L][S][0]));
      break;
    default:
      throw Exception("Unrecognized type");
    }
  else
    switch(type_) {
    case VICAR_BYTE:
      from_int<unsigned char, false>(V, &(data_raw->data()[B][L][S][0]));
      break;
    case VICAR_HALF:
      from_int<short int, false>(V, &(data_raw->data()[B][L][S][0]));
      break;
    case VICAR_FULL:
      from_int<int, false>(V, &(data_raw->data()[B][L][S][0]));
      break;
    default:
      throw Exception("Unrecognized type");
    }
}

/****************************************************************//**
  This uses VicarLiteFile to implement a RasterImage.

  We use a VicarLiteFile to implement a RasterImage.
*******************************************************************/

class VicarLiteRasterImage : public RasterImageVariable {
public:
  typedef VicarLiteFile::access_type access_type;

//-----------------------------------------------------------------------
/// Constructor.
///
/// The Force_area_pixel forces the file to be treated as
/// "pixel as area" rather than "pixel as point". This is really just
/// meant as a work around for the SRTM data, which incorrectly labels
/// the data as "point" rather than "area". Since this is a 15 meter
/// difference, it matters for many applications. Most users should
/// just ignore this value.
//-----------------------------------------------------------------------

  VicarLiteRasterImage(const std::string& Fname, 
		       access_type Access = VicarLiteFile::READ,
		       int Band = 0, int Number_tile_line = -1,
		       int Number_tile_sample = -1,
		       bool Force_area_pixel = false)
    : band_(Band), f_(new VicarLiteFile(Fname, Access, Force_area_pixel))
  {
    number_line_ = f_->number_line();
    number_sample_ = f_->number_sample();
    number_tile_line_ = Number_tile_line;
    number_tile_sample_ = Number_tile_sample;
    if(number_tile_line_ < 0)
      number_tile_line_ = number_line_;
    if(number_tile_sample_ < 0)
      number_tile_sample_ = number_sample_;
    if(f_->has_rpc())
      rpc_.reset(new Rpc(f_->rpc()));
    if(f_->has_map_info())
      map_info_.reset(new MapInfo(f_->map_info()));
  }
  virtual ~VicarLiteRasterImage() {}

//-----------------------------------------------------------------------
/// Underlying file.
//-----------------------------------------------------------------------

  const VicarLiteFile& file() const { return *f_; }

//-----------------------------------------------------------------------
/// Underlying file.
//-----------------------------------------------------------------------

  VicarLiteFile& file() { return *f_; }

  boost::shared_ptr<VicarLiteFile> file_ptr() const { return f_;}

//-----------------------------------------------------------------------
/// Return pixel value at given line and sample.
//-----------------------------------------------------------------------

  virtual int unchecked_read(int Line, int Sample) const
  { return f_->read_int(band_, Line, Sample); }
  
//-----------------------------------------------------------------------
/// Return a subset of the image.
//-----------------------------------------------------------------------

  virtual void read_ptr(int Lstart, int Sstart, int Number_line, 
			int Number_sample, int* Res) const
  { f_->read_int(band_, Lstart, Sstart, 1, Number_line, Number_sample, Res); }

//-----------------------------------------------------------------------
/// Write the pixel value to the given location.
//-----------------------------------------------------------------------

  virtual void unchecked_write(int Line, int Sample, int Val)
  { f_->write_int(band_, Line, Sample, Val); }

//-----------------------------------------------------------------------
/// Indicate if the file is compressed. If it is, we can't actually
/// read the data but we can still read all of the labels.
//-----------------------------------------------------------------------

  bool is_compressed() const { return f_->is_compressed(); }

//-----------------------------------------------------------------------
/// Return band number
//-----------------------------------------------------------------------
  int band() const { return band_; }
private:
  int band_;
  boost::shared_ptr<VicarLiteFile> f_;
};

/****************************************************************//**
  This uses VicarLiteFile to implement a Dem

  We use VicarLiteFile to implement a Dem
*******************************************************************/

class VicarLiteDem : public DemMapInfo {
public:
//-----------------------------------------------------------------------
/// Constructor. If Outside_dem_is_error is true, then calls for Dem
/// data outside of the range of the Dem cause an exception to be
/// thrown, otherwise we just return a height of 0.
//-----------------------------------------------------------------------

  VicarLiteDem(const std::string& Fname, 
	       bool Outside_dem_is_error = false,
	       const boost::shared_ptr<Datum>& D = 
	       boost::shared_ptr<Datum>(new NoDatum()),
	       int Band = 0)
    : band_(Band), f_(new VicarLiteFile(Fname))
  {
    initialize(D, f_->map_info(), Outside_dem_is_error);
  }
  virtual ~VicarLiteDem() {}

//-----------------------------------------------------------------------
/// Underlying file.
//-----------------------------------------------------------------------

  const VicarLiteFile& file() const { return *f_; }

//-----------------------------------------------------------------------
/// Underlying file.
//-----------------------------------------------------------------------

  VicarLiteFile& file() { return *f_; }

  boost::shared_ptr<VicarLiteFile> file_ptr() const { return f_;}

//-----------------------------------------------------------------------
/// Return height in meters relative to datum().
//-----------------------------------------------------------------------

  virtual double elevation(int Y_index, int X_index) const
  { return f_->read_double(band_, Y_index, X_index);}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { Os << "Vicar Lite Dem:\n"
       << "  File: " << f_->file_name() << "\n"
       << "  Band: " << band_ << "\n"
       << "  Map info:\n"
       << map_info()
       << "  Datum:\n"
       << datum()
       << "  Outside Dem is error: " << outside_dem_is_error() << "\n";
  }

//-----------------------------------------------------------------------
/// Return band number
//-----------------------------------------------------------------------
  int band() const { return band_; }
private:
  int band_;
  boost::shared_ptr<VicarLiteFile> f_;
};


}  
#endif
