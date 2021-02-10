#ifndef VICAR_LITE_FILE_H
#define VICAR_LITE_FILE_H
#include "geocal_exception.h"
#include "map_info.h"
#include "geocal_rpc.h"
#include "rsm.h"
#include "memory_map_array.h"
#include "raster_image_variable.h"
#include "image_ground_connection.h"
#include "dem_map_info.h"
#include "ostream_pad.h"
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

//-----------------------------------------------------------------------
/// Open an existing VICAR file for reading or update.
///
/// The Force_area_pixel forces the file to be treated as
/// "pixel as area" rather than "pixel as point". This is really just
/// meant as a work around for the SRTM data, which incorrectly labels
/// the data as "point" rather than "area". Since this is a 15 meter
/// difference, it matters for many applications. Most users should
/// just ignore this value.
//-----------------------------------------------------------------------

  VicarLiteFile(const std::string& Fname, access_type Access = READ,
		bool Force_area_pixel = false)
  {
    initialize(Fname, Access, Force_area_pixel);
  }
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

//-----------------------------------------------------------------------
/// Non template form of label, useful in some contexts.
//-----------------------------------------------------------------------

  inline std::string label_string(const std::string& F, 
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
/// Test if a label is found in a file, and if so return
/// true. Otherwise return false.
//-----------------------------------------------------------------------

  bool has_label(const std::string& Lbl) const
  { return label_.count(Lbl) != 0; }

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
/// Number of lines of binary data before the start of the image
//-----------------------------------------------------------------------

  int number_line_binary() const {return number_line_binary_; }

//-----------------------------------------------------------------------
/// Number of bytes of binary data before the start of each line
//-----------------------------------------------------------------------

  int number_byte_binary() const {return number_byte_binary_; }
  
//-----------------------------------------------------------------------
/// Type of data in file
//-----------------------------------------------------------------------

  data_type type() const {return type_;}

  bool has_map_info() const;
  bool has_rpc() const;
  bool has_rsm() const;
  bool has_igc_glas_gfm() const;
  MapInfo map_info() const;
  void map_info(const MapInfo& M);
  Rpc rpc() const;
  void rpc(const Rpc& UNUSED(R)) { throw Exception("Not yet implemented"); }
  boost::shared_ptr<Rsm> rsm() const;
  void rsm(const boost::shared_ptr<Rsm>& UNUSED(R)) { throw Exception("Not yet implemented"); }
  boost::shared_ptr<ImageGroundConnection> igc_glas_gfm() const;
  int read_int(int B, int L, int S) const;
  double read_double(int B, int L, int S) const;
  void read_int(int B, int L, int S, int Nb, int Nl, int Ns, int* Res) const;
  void read_double(int B, int L, int S, int Nb, int Nl, int Ns, 
		   double* Res) const;
  void write_int(int B, int L, int S, int V) const;
  void write_double(int B, int L, int S, double V) const;

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { Os << "Vicar Lite File \n" 
       << "  File:          " << file_name() << "\n"
       << "  Number line:   " << number_line() << "\n"
       << "  Number sample: " << number_sample() << "\n"
       << "  Number band:   " << number_band() << "\n";
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
  template<class C, bool Swap_needed> void from_double(const double&, 
						       char* c) const;
protected:
  VicarLiteFile() {}
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
  int number_line_binary_;
  int number_byte_binary_;
  int number_sample_binary_;
  bool swap_needed_;
  data_type type_;
  boost::shared_ptr<std::fstream> f_;
  std::map<std::string, std::string> label_;
  std::string read_label(int& lblsize);
  void process_label(const std::string& label);
  void initialize(const std::string& Fname, access_type Access = READ,
	    bool Force_area_pixel = false);
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
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

template<> inline int VicarLiteFile::to_int<float, false>(const char* c) const
{ return (int) *reinterpret_cast<const float*>(c); }

template<> inline int VicarLiteFile::to_int<double, false>(const char* c) const
{ return (int) *reinterpret_cast<const double*>(c); }

template<> inline double VicarLiteFile::to_double<unsigned char, true>(const char* c) const
{ return to_int<unsigned char, true>(c); }
template<> inline double VicarLiteFile::to_double<unsigned char, false>(const char* c) const
{ return to_int<unsigned char, false>(c); }
template<> inline double VicarLiteFile::to_double<short int, true>(const char* c) const
{ return to_int<short int, true>(c); }
template<> inline double VicarLiteFile::to_double<short int, false>(const char* c) const
{ return to_int<short int, false>(c); }
template<> inline double VicarLiteFile::to_double<int, true>(const char* c) const
{ return to_int<int, true>(c); }
template<> inline double VicarLiteFile::to_double<int, false>(const char* c) const
{ return to_int<int, false>(c); }

template<> inline double VicarLiteFile::to_double<float, false>(const char* c) const
{ return *reinterpret_cast<const float*>(c); }

template<> inline double VicarLiteFile::to_double<double, false>(const char* c) const
{ return *reinterpret_cast<const double*>(c); }

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
template<> inline void 
VicarLiteFile::from_int<float, false>(const int& I, char* c) const
{ *(reinterpret_cast<float *>(c)) = static_cast<float>(I); }
template<> inline void 
VicarLiteFile::from_int<double, false>(const int& I, char* c) const
{ *(reinterpret_cast<double *>(c)) = static_cast<double>(I); }

template<> inline void 
VicarLiteFile::from_double<unsigned char, true>(const double& I, char* c) const
{ from_int<unsigned char, true>(static_cast<int>(I), c); }
template<> inline void 
VicarLiteFile::from_double<unsigned char, false>(const double& I, char* c) const
{ from_int<unsigned char, false>(static_cast<int>(I), c); }
template<> inline void 
VicarLiteFile::from_double<short int, true>(const double& I, char* c) const
{ from_int<short int, true>(static_cast<int>(I), c); }
template<> inline void 
VicarLiteFile::from_double<short int, false>(const double& I, char* c) const
{ from_int<short int, false>(static_cast<int>(I), c); }
template<> inline void 
VicarLiteFile::from_double<int, true>(const double& I, char* c) const
{ from_int<int, true>(static_cast<int>(I), c); }
template<> inline void 
VicarLiteFile::from_double<int, false>(const double& I, char* c) const
{ from_int<int, false>(static_cast<int>(I), c); }
template<> inline void 
VicarLiteFile::from_double<float, false>(const double& I, char* c) const
{ *(reinterpret_cast<float *>(c)) = static_cast<float>(I); }
template<> inline void 
VicarLiteFile::from_double<double, false>(const double& I, char* c) const
{ *(reinterpret_cast<double *>(c)) = static_cast<double>(I); }

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
      return to_int<unsigned char, true>(&(data_raw->data()[B][L][S+number_sample_binary_][0]));
    case VICAR_HALF:
      return to_int<short int, true>(&(data_raw->data()[B][L][S+number_sample_binary_][0]));
    default:
      throw Exception("Unrecognized type");
      }
  else
    switch(type_) {
    case VICAR_BYTE:
      return to_int<unsigned char, false>(&(data_raw->data()[B][L][S+number_sample_binary_][0]));
    case VICAR_HALF:
      return to_int<short int, false>(&(data_raw->data()[B][L][S+number_sample_binary_][0]));
    case VICAR_FULL:
      return to_int<int, false>(&(data_raw->data()[B][L][S+number_sample_binary_][0]));
    case VICAR_FLOAT:
      return to_int<float, false>(&(data_raw->data()[B][L][S+number_sample_binary_][0]));
    case VICAR_DOUBLE:
      return to_int<double, false>(&(data_raw->data()[B][L][S+number_sample_binary_][0]));
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
      return to_double<unsigned char, true>(&(data_raw->data()[B][L][S+number_sample_binary_][0]));
    case VICAR_HALF:
      return to_double<short int, true>(&(data_raw->data()[B][L][S+number_sample_binary_][0]));
    case VICAR_FULL:
      return to_double<int, true>(&(data_raw->data()[B][L][S+number_sample_binary_][0]));
    default:
      throw Exception("Unrecognized type");
      }
  else
    switch(type_) {
    case VICAR_BYTE:
      return to_double<unsigned char, false>(&(data_raw->data()[B][L][S+number_sample_binary_][0]));
    case VICAR_HALF:
      return to_double<short int, false>(&(data_raw->data()[B][L][S+number_sample_binary_][0]));
    case VICAR_FULL:
      return to_double<int, false>(&(data_raw->data()[B][L][S+number_sample_binary_][0]));
    case VICAR_FLOAT:
      return to_double<float, false>(&(data_raw->data()[B][L][S+number_sample_binary_][0]));
    case VICAR_DOUBLE:
      return to_double<double, false>(&(data_raw->data()[B][L][S+number_sample_binary_][0]));
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
	  for(int k = S+number_sample_binary_;
	      k < S + Ns+number_sample_binary_; ++k, ++Res)
	    *Res = to_int<unsigned char, true>(&(data_raw->data()[i][j][k][0]));
      break;
    case VICAR_HALF:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S+number_sample_binary_;
	      k < S + Ns+number_sample_binary_; ++k, ++Res)
	    *Res = to_int<short int, true>(&(data_raw->data()[i][j][k][0]));
      break;
    case VICAR_FULL:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S+number_sample_binary_;
	      k < S + Ns+number_sample_binary_; ++k, ++Res)
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
	  for(int k = S+number_sample_binary_;
	      k < S + Ns+number_sample_binary_; ++k, ++Res)
	    *Res = to_int<unsigned char, false>(&(data_raw->data()[i][j][k][0]));
      break;
    case VICAR_HALF:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S+number_sample_binary_;
	      k < S + Ns+number_sample_binary_; ++k, ++Res)
	    *Res = to_int<short int, false>(&(data_raw->data()[i][j][k][0]));
      break;
    case VICAR_FULL:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S+number_sample_binary_;
	      k < S + Ns+number_sample_binary_; ++k, ++Res)
	    *Res = to_int<int, false>(&(data_raw->data()[i][j][k][0]));
      break;
    case VICAR_FLOAT:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S+number_sample_binary_;
	      k < S + Ns+number_sample_binary_; ++k, ++Res)
	    *Res = to_int<float, false>(&(data_raw->data()[i][j][k][0]));
      break;
    case VICAR_DOUBLE:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S+number_sample_binary_;
	      k < S + Ns+number_sample_binary_; ++k, ++Res)
	    *Res = to_int<double, false>(&(data_raw->data()[i][j][k][0]));
      break;
    default:
      throw Exception("Unrecognized type");
    }
}

//-----------------------------------------------------------------------
/// Read data as a double.
//-----------------------------------------------------------------------

inline void VicarLiteFile::read_double(int B, int L, int S, int Nb, int Nl, 
				       int Ns, double* Res) const
{
  if(is_compressed_)
    throw Exception("Can't use VicarLiteFile to read a compressed file");
  if(swap_needed_)
    switch(type_) {
    case VICAR_BYTE:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S+number_sample_binary_;
	      k < S + Ns+number_sample_binary_; ++k, ++Res)
	    *Res = to_double<unsigned char, true>(&(data_raw->data()[i][j][k][0]));
      break;
    case VICAR_HALF:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S+number_sample_binary_;
	      k < S + Ns+number_sample_binary_; ++k, ++Res)
	    *Res = to_double<short int, true>(&(data_raw->data()[i][j][k][0]));
      break;
    case VICAR_FULL:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S+number_sample_binary_;
	      k < S + Ns+number_sample_binary_; ++k, ++Res)
	    *Res = to_double<int, true>(&(data_raw->data()[i][j][k][0]));
      break;
    default:
      throw Exception("Unrecognized type");
      }
  else
    switch(type_) {
    case VICAR_BYTE:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S+number_sample_binary_;
	      k < S + Ns+number_sample_binary_; ++k, ++Res)
	    *Res = to_double<unsigned char, false>(&(data_raw->data()[i][j][k][0]));
      break;
    case VICAR_HALF:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S+number_sample_binary_;
	      k < S + Ns+number_sample_binary_; ++k, ++Res)
	    *Res = to_double<short int, false>(&(data_raw->data()[i][j][k][0]));
      break;
    case VICAR_FULL:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S+number_sample_binary_;
	      k < S + Ns+number_sample_binary_; ++k, ++Res)
	    *Res = to_double<int, false>(&(data_raw->data()[i][j][k][0]));
      break;
    case VICAR_FLOAT:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S+number_sample_binary_;
	      k < S + Ns+number_sample_binary_; ++k, ++Res)
	    *Res = to_double<float, false>(&(data_raw->data()[i][j][k][0]));
      break;
    case VICAR_DOUBLE:
      for(int i = B; i < B + Nb; ++i)
	for(int j = L; j < L + Nl; ++j)
	  for(int k = S+number_sample_binary_;
	      k < S + Ns+number_sample_binary_; ++k, ++Res)
	    *Res = to_double<double, false>(&(data_raw->data()[i][j][k][0]));
      break;
    default:
      throw Exception("Unrecognized type");
    }
}

inline std::string VicarLiteFile::label_string(const std::string& F, 
				  const std::string& Property) const
{ return label<std::string>(F, Property); }

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
      from_int<unsigned char, true>(V, &(data_raw->data()[B][L][S+number_sample_binary_][0]));
      break;
    case VICAR_HALF:
      from_int<short int, true>(V, &(data_raw->data()[B][L][S+number_sample_binary_][0]));
      break;
    case VICAR_FULL:
      from_int<int, true>(V, &(data_raw->data()[B][L][S+number_sample_binary_][0]));
      break;
    default:
      throw Exception("Unrecognized type");
    }
  else
    switch(type_) {
    case VICAR_BYTE:
      from_int<unsigned char, false>(V, &(data_raw->data()[B][L][S+number_sample_binary_][0]));
      break;
    case VICAR_HALF:
      from_int<short int, false>(V, &(data_raw->data()[B][L][S+number_sample_binary_][0]));
      break;
    case VICAR_FULL:
      from_int<int, false>(V, &(data_raw->data()[B][L][S+number_sample_binary_][0]));
      break;
    case VICAR_FLOAT:
      from_int<float, false>(V, &(data_raw->data()[B][L][S+number_sample_binary_][0]));
      break;
    case VICAR_DOUBLE:
      from_int<double, false>(V, &(data_raw->data()[B][L][S+number_sample_binary_][0]));
      break;
    default:
      throw Exception("Unrecognized type");
    }
}

inline void VicarLiteFile::write_double(int B, int L, int S, double V) const 
{
  if(is_compressed_)
    throw Exception("Can't use VicarLiteFile to write a compressed file");
  if(swap_needed_)
    switch(type_) {
    case VICAR_BYTE:
      from_double<unsigned char, true>(V, &(data_raw->data()[B][L][S+number_sample_binary_][0]));
      break;
    case VICAR_HALF:
      from_double<short int, true>(V, &(data_raw->data()[B][L][S+number_sample_binary_][0]));
      break;
    case VICAR_FULL:
      from_double<int, true>(V, &(data_raw->data()[B][L][S+number_sample_binary_][0]));
      break;
    default:
      throw Exception("Unrecognized type");
    }
  else
    switch(type_) {
    case VICAR_BYTE:
      from_double<unsigned char, false>(V, &(data_raw->data()[B][L][S+number_sample_binary_][0]));
      break;
    case VICAR_HALF:
      from_double<short int, false>(V, &(data_raw->data()[B][L][S+number_sample_binary_][0]));
      break;
    case VICAR_FULL:
      from_double<int, false>(V, &(data_raw->data()[B][L][S+number_sample_binary_][0]));
      break;
    case VICAR_FLOAT:
      from_double<float, false>(V, &(data_raw->data()[B][L][S+number_sample_binary_][0]));
      break;
    case VICAR_DOUBLE:
      from_double<double, false>(V, &(data_raw->data()[B][L][S+number_sample_binary_][0]));
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

  VicarLiteRasterImage(const std::string& Fname, int Band_id = 1,
		       access_type Access = VicarLiteFile::READ,
		       int Number_tile_line = -1,
		       int Number_tile_sample = -1,
		       bool Force_area_pixel = false)
    : band_(Band_id - 1), 
      f_(new VicarLiteFile(Fname, Access, Force_area_pixel)),
      force_map_info_(false)
  {
    number_tile_line_ = Number_tile_line;
    number_tile_sample_ = Number_tile_sample;
    initialize();
  }

//-----------------------------------------------------------------------
/// Constructor. We force the given map info to apply to the image.
/// This is a workaround for pickling mapinfo that VICAR doesn't
/// support yet.
//-----------------------------------------------------------------------

  VicarLiteRasterImage(const std::string& Fname, 
		       const MapInfo& Mi,
		       int Band_id = 1,
		       access_type Access = VicarLiteFile::READ,
		       int Number_tile_line = -1,
		       int Number_tile_sample = -1,
		       bool Force_area_pixel = false)
    : band_(Band_id - 1), 
      f_(new VicarLiteFile(Fname, Access, Force_area_pixel)),
      force_map_info_(true)
  {
    range_check(band_, 0, f_->number_band());
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
    if(f_->has_rsm())
      rsm_ = f_->rsm();
    map_info_.reset(new MapInfo(Mi));
  }
  virtual ~VicarLiteRasterImage() {}

  bool has_igc_glas_gfm() const { return f_->has_igc_glas_gfm(); }
  boost::shared_ptr<ImageGroundConnection> igc_glas_gfm() const
  { return f_->igc_glas_gfm(); }
  
  virtual bool copy_needs_double() const
  { return (file().type() == VicarLiteFile::VICAR_FLOAT ||
	    file().type() == VicarLiteFile::VICAR_DOUBLE); }

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
/// Marker to force a map_info in python pickle. This is a work around
/// for map info that VICAR doesn't support writing.
//-----------------------------------------------------------------------
  
  bool force_map_info() const { return force_map_info_;}

//-----------------------------------------------------------------------
/// Return pixel value at given line and sample.
//-----------------------------------------------------------------------

  virtual int unchecked_read(int Line, int Sample) const
  { return f_->read_int(band_, Line, Sample); }
  virtual double unchecked_read_double(int Line, int Sample) const
  { return f_->read_double(band_, Line, Sample); }
  
//-----------------------------------------------------------------------
/// Return a subset of the image.
//-----------------------------------------------------------------------

  virtual void read_ptr(int Lstart, int Sstart, int Number_line, 
			int Number_sample, int* Res) const
  { f_->read_int(band_, Lstart, Sstart, 1, Number_line, Number_sample, Res); }


  virtual blitz::Array<double, 2> 
  read_double(int Lstart, int Sstart, int Number_line, 
	      int Number_sample) const
  { 
    blitz::Array<double, 2> res(Number_line, Number_sample);
    f_->read_double(band_, Lstart, Sstart, 1, Number_line, Number_sample, 
		       res.data());
    return res;
  }

//-----------------------------------------------------------------------
/// Write the pixel value to the given location.
//-----------------------------------------------------------------------

  virtual void unchecked_write(int Line, int Sample, int Val)
  { f_->write_int(band_, Line, Sample, Val); }
  virtual void unchecked_write(int Line, int Sample, double Val)
  { f_->write_double(band_, Line, Sample, Val); }

//-----------------------------------------------------------------------
/// Indicate if the file is compressed. If it is, we can't actually
/// read the data but we can still read all of the labels.
//-----------------------------------------------------------------------

  bool is_compressed() const { return f_->is_compressed(); }

//-----------------------------------------------------------------------
/// Return band number. This is 1 based (like GDAL).
//-----------------------------------------------------------------------
  int band_id() const { return band_ + 1; }

  virtual void print(std::ostream& Os) const 
  {
    OstreamPad opad(Os, "    ");
    Os << "VicarLiteRasterImage:\n"
       << "  File:          " << f_->file_name() << "\n"
       << "  Band:          " << band_id() << "\n"
       << "  Number line:   " << number_line() << "\n"
       << "  Number sample: " << number_sample() << "\n";
    Os << "  Map Info:      ";
    if(has_map_info()) {
      Os << "\n";
      opad << map_info();
      opad.strict_sync();
    } else
      Os << "None\n";
    Os << "  RPC:           ";
    if(has_rpc()) {
      Os << "\n";
      opad << rpc();
      opad.strict_sync();
    } else
      Os << "None\n";
    Os << "  RSM:           ";
    if(has_rsm()) {
      Os << "\n";
      opad << *rsm();
      opad.strict_sync();
    } else
      Os << "None\n";
    Os << "  GLAS/GFM:      ";
    if(has_igc_glas_gfm()) {
      Os << "\n";
      opad << *igc_glas_gfm();
      opad.strict_sync();
    } else
      Os << "None\n";
  }
protected:
  VicarLiteRasterImage() {}
private:
  void initialize() 
  {
    range_check(band_, 0, f_->number_band());
    number_line_ = f_->number_line();
    number_sample_ = f_->number_sample();
    if(number_tile_line_ < 0)
      number_tile_line_ = number_line_;
    if(number_tile_sample_ < 0)
      number_tile_sample_ = number_sample_;
    if(f_->has_rpc())
      rpc_.reset(new Rpc(f_->rpc()));
    if(f_->has_rsm())
      rsm_ = f_->rsm();
    if(f_->has_map_info())
      map_info_.reset(new MapInfo(f_->map_info()));
  }

  int band_;
  boost::shared_ptr<VicarLiteFile> f_;
  bool force_map_info_;
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
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
///
/// Probably should have the Naif_code stored as metadata in the file,
/// but for now we just explicitly pass this in.
//-----------------------------------------------------------------------

  VicarLiteDem(const std::string& Fname, 
	       bool Outside_dem_is_error = false,
	       const boost::shared_ptr<Datum>& D = 
	       boost::shared_ptr<Datum>(new NoDatum()),
	       int Band_id = 1)
    : band_(Band_id - 1), f_(new VicarLiteFile(Fname))
  {
    initialize(D, f_->map_info(), Outside_dem_is_error);
    range_check(band_, 0, f_->number_band());
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
  { 
    OstreamPad opad(Os, "    ");
    Os << "Vicar Lite Dem:\n"
       << "  File: " << f_->file_name() << "\n"
       << "  Band: " << band_ + 1 << "\n"
       << "  Map info:\n";
    opad << map_info();
    opad.strict_sync();
    Os << "  Datum:\n";
    opad << datum();
    opad.strict_sync();
    Os << "  Outside Dem is error: " << outside_dem_is_error() << "\n";
  }

//-----------------------------------------------------------------------
/// Return band number
//-----------------------------------------------------------------------
  int band() const { return band_  + 1; }
protected:
  VicarLiteDem() {}
private:
  int band_;
  boost::shared_ptr<VicarLiteFile> f_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& Ar, const unsigned int version);
};

}  

GEOCAL_EXPORT_KEY(VicarLiteFile)
GEOCAL_EXPORT_KEY(VicarLiteDem)
GEOCAL_EXPORT_KEY(VicarLiteRasterImage)
#endif
