#ifndef HDF_FILE_H
#define HDF_FILE_H
#include "printable.h"
#include "geocal_exception.h"
#include <boost/shared_ptr.hpp>
#include <blitz/array.h>
#include <H5Cpp.h>
#include <vector>

namespace GeoCal {
/****************************************************************//**
  This class reads a HDF5 file. Note that this is just a thin layer on
  top of the HDF 5 libraries to make the file operations we need to do
  in GeoCal easier. There are many other things that can be done with
  a HDF 5 than what this class exposes.
  
  Right now, we don't support writing. This is just because I don't
  happen to need this. We can add this if we ever want to write files.
*******************************************************************/

class HdfFile : public Printable<HdfFile> {
public:
  enum Mode {READ, CREATE, READ_WRITE};
  HdfFile(const std::string& Fname, Mode M = READ);
  virtual ~HdfFile() {}

//-----------------------------------------------------------------------
/// Check to see if an object (such as a Dataset) is in the file.
//-----------------------------------------------------------------------
  bool has_object(const std::string& Objname) const
  { return is_present(Objname, *h); }

//-----------------------------------------------------------------------
/// Check to see if a attribute is in the file.
//-----------------------------------------------------------------------
  bool has_attribute(const std::string& Aname) const;

  template<class T, int D> blitz::Array<T, D> read_field(
	 const std::string& Dataname) const;
  template<class T, int D> blitz::Array<T, D> read_field(
	 const std::string& Dataname, 
	 const blitz::TinyVector<int,D>& Start, 
	 const blitz::TinyVector<int,D>& Size) const;
  template<class T, int D> blitz::Array<T, D> 
  read_attribute(const std::string& Aname) const;
  template<class T> T read_attribute(const std::string& Aname) const;

//-----------------------------------------------------------------------
/// Close the underlying file. This is automatically done by the
/// destructor, so you only need to call this if you want to force a
/// close (e.g., for a unit test)
//-----------------------------------------------------------------------

  void close() { h->close(); }

//-----------------------------------------------------------------------
/// File name
//-----------------------------------------------------------------------
  const std::string& file_name() const {return fname;}

//-----------------------------------------------------------------------
/// Return true if the given file is an HDF file.
//-----------------------------------------------------------------------

  static bool is_hdf(const std::string& Fname) 
  { 
    try {
      return H5::H5File::isHdf5(Fname); 
    } catch(const H5::FileIException& e) {
      GeoCal::Exception err;
      err << "Error determining if file: "
	  << Fname << " is an HDF file: "
	  << e.getDetailMsg();
      throw err;
    }
  }

  // Map native type to type of blitz array
  template<class T> H5::PredType pred_arr() const;
  // Map type of blitz array to type of data in file.
  template<class T> H5::PredType pred_data() const;
  void print(std::ostream& Os) const;
private:
  boost::shared_ptr<H5::H5File> h;
  unsigned int flag;
  std::string fname;
  H5::Attribute open_attribute(const std::string& Aname) const;
  bool is_group(const std::string& Objname) const;
  bool is_present(const std::string& Objname, 
		  const H5::H5File& Parent) const;
  HdfFile() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
  template<class Archive>
  void save(Archive & ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive & ar, const unsigned int version);
};

template<> inline H5::PredType HdfFile::pred_arr<int>() const 
{return H5::PredType::NATIVE_INT;}
template<> inline H5::PredType HdfFile::pred_arr<double>() const 
{return H5::PredType::NATIVE_DOUBLE;}
template<> inline H5::PredType HdfFile::pred_data<int>() const 
{return H5::PredType::NATIVE_INT32;}
template<> inline H5::PredType HdfFile::pred_data<double>() const
{return H5::PredType::NATIVE_DOUBLE;}

//-----------------------------------------------------------------------
/// Read the given attribute attached to a group or dataset.
//-----------------------------------------------------------------------

template<class T, int D> inline blitz::Array<T, D> 
HdfFile::read_attribute(const std::string& Aname) const
{
  try {
  using namespace H5;
  Attribute a = open_attribute(Aname);
  DataSpace ds = a.getSpace();
  if(ds.getSimpleExtentNdims() != D) {
    Exception e;
    e << "Attribute " << Aname << " does not have the expected rank of " 
      << D;
    throw e;
  }
  hsize_t dims[D];
  ds.getSimpleExtentDims(dims, NULL);
  blitz::TinyVector<int,D> dims2;
  for(int i = 0; i < D; ++i)
    dims2(i) = (int) dims[i];
  blitz::Array<T, D> res(dims2);
  a.read(pred_arr<T>(), res.dataFirst());
  return res;
  } catch(const H5::Exception& e) {
    Exception en;
    en << "While reading attribute " << Aname
       << " for the file '" << fname
       << "' a HDF 5 Exception thrown:\n"
       << "  " << e.getDetailMsg();
    throw en;
  }
}

//-----------------------------------------------------------------------
/// Read the given attribute attached to a group or dataset.
//-----------------------------------------------------------------------

template<class T> inline T 
HdfFile::read_attribute(const std::string& Aname) const
{
  return read_attribute<T, 1>(Aname)(0);
}

//-----------------------------------------------------------------------
/// Read the given attribute attached to a group or dataset.
//-----------------------------------------------------------------------

template<> inline std::string HdfFile::read_attribute<std::string>(const 
	    std::string& Aname) const
{
  try {
  using namespace H5;
  Attribute a = open_attribute(Aname);
  std::string res;
  a.read(a.getStrType(),res);
  // Fixed length strings may have trailing spaces. Truncate them.
  size_t t = res.find_last_not_of(" ");
  if(t != std::string::npos)
    res.erase(t + 1);
  // For some reason, HDF sometimes reads past the end. Probably a
  // bug, but we can work around this.
  if(res.size() > a.getInMemDataSize())
    res = res.substr(0, a.getInMemDataSize());
  return res;
  } catch(const H5::Exception& e) {
    Exception en;
    en << "While reading attribute " << Aname
       << " for the file '" << fname
       << "' a HDF 5 Exception thrown:\n"
       << "  " << e.getDetailMsg();
    throw en;
  }
}

//-----------------------------------------------------------------------
/// Read the given attribute attached to a group or dataset.
//-----------------------------------------------------------------------

template<> inline std::vector<std::string> 
HdfFile::read_attribute<std::vector<std::string> >(const 
						   std::string& Aname) const
{
  try {
  using namespace H5;
  Attribute a = open_attribute(Aname);
  DataSpace ds = a.getSpace();
  if(ds.getSimpleExtentNdims() != 1) {
    Exception e;
    e << "Attribute " << Aname << " does not have the expected rank of 1";
    throw e;
  }
  hsize_t dims[1];
  ds.getSimpleExtentDims(dims, NULL);
  int nelem = (int) dims[0];
  std::vector<char*> ptr(nelem);
  a.read(a.getStrType(),&(*ptr.begin()));
  std::vector<std::string> res(nelem);
  for(int i = 0; i < nelem; ++i) {
    res[i] = std::string(ptr[i]);
    // Fixed length strings may have trailing spaces. Truncate them.
    size_t t = res[i].find_last_not_of(" ");
    if(t != std::string::npos)
      res[i].erase(t + 1);
  }
  return res;
  } catch(const H5::Exception& e) {
    Exception en;
    en << "While reading attribute " << Aname
       << " for the file '" << fname
       << "' a HDF 5 Exception thrown:\n"
       << "  " << e.getDetailMsg();
    throw en;
  }
}

//-----------------------------------------------------------------------
// Somewhat annoyingly, C++ doesn't allow function partial
// specialization. Introduce a class as a roundabout way to do this,
// since we can do partial template specialization of a class.
//-----------------------------------------------------------------------
// Don't have Doxygen document this class.
/// @cond
template<class T, int D> class HdfFilePartialSpecialHelper {
public:

blitz::Array<T, D> read_field(const HdfFile& hf,
			      const H5::H5File& h, 
			      const std::string& Dataname) const
{
  try {
  using namespace H5;
  DataSet d = h.openDataSet(Dataname);
  DataSpace ds = d.getSpace();
  if(ds.getSimpleExtentNdims() != D) {
    Exception e;
    e << "Dataset " << Dataname << " does not have the expected rank of " 
      << D;
    throw e;
  }
  hsize_t dims[D];
  ds.getSimpleExtentDims(dims, NULL);
  blitz::TinyVector<int,D> dims2;
  for(int i = 0; i < D; ++i)
    dims2(i) = (int) dims[i];
  blitz::Array<T, D> res(dims2);
  d.read(res.dataFirst(), hf.pred_arr<T>());
  return res;
  } catch(const H5::Exception& e) {
    Exception en;
    en << "While reading field " << Dataname
       << " for the file '" << hf.file_name()
       << "' a HDF 5 Exception thrown:\n"
       << "  " << e.getDetailMsg();
    throw en;
  }
}

blitz::Array<T, D> read_field(const HdfFile& hf,
			      const H5::H5File& h, 
			      const std::string& Dataname,
			      const blitz::TinyVector<hsize_t,D>& Start, 
			      const blitz::TinyVector<hsize_t,D>& Size) const
{
  try {
  using namespace H5;
  DataSet d = h.openDataSet(Dataname);
  DataSpace ds = d.getSpace();
  if(ds.getSimpleExtentNdims() != D) {
    Exception e;
    e << "Dataset " << Dataname << " does not have the expected rank of " 
      << D;
    throw e;
  }
  DataSpace ms(D, &Size[0]);
  ds.selectHyperslab(H5S_SELECT_SET, &Size[0], &Start[0]);
  blitz::Array<T, D> res(Size);
  d.read(res.dataFirst(), hf.pred_arr<T>(), ms, ds);
  return res;
  } catch(const H5::Exception& e) {
    Exception en;
    en << "While reading field " << Dataname
       << " for the file '" << hf.file_name()
       << "' a HDF 5 Exception thrown:\n"
       << "  " << e.getDetailMsg();
    throw en;
  }
}

};

template<int D> class HdfFilePartialSpecialHelper<std::string, D> {
public:
blitz::Array<std::string, D> read_field(const HdfFile& hf,
					const H5::H5File& h, 
					const std::string& Dataname) const
{
  try {
  using namespace H5;
  DataSet d = h.openDataSet(Dataname);
  DataSpace ds = d.getSpace();
  if(ds.getSimpleExtentNdims() != D) {
    Exception e;
    e << "Dataset " << Dataname << " does not have the expected rank of " 
      << D;
    throw e;
  }
  hsize_t dims[D];
  ds.getSimpleExtentDims(dims, NULL);
  blitz::TinyVector<int,D> dims2;
  for(int i = 0; i < D; ++i)
    dims2(i) = (int) dims[i];

  DataType dt = d.getDataType();
  blitz::Array<std::string, D> result_data(dims2);
  if(dt.isVariableStr()) {
    blitz::Array<const char*, D> read_data(result_data.shape());
    StrType st(PredType::C_S1, H5T_VARIABLE);
    d.read(read_data.dataFirst(), st);
    
    typename blitz::Array<const char*, D>::const_iterator i1;
    typename blitz::Array<std::string, D>::iterator i2 = result_data.begin();
    for(i1 = read_data.begin(); i1 != read_data.end(); ++i1, ++i2)
      *i2 = std::string(*i1);
  } else {
    int flat_size = 1;
    for(int i = 0; i < D; i++)
      flat_size *= dims2[i];
    flat_size *= dt.getSize();
    blitz::Array<char, 1> read_data(flat_size);
    d.read(read_data.dataFirst(), d.getStrType());

    char* i1 = read_data.dataFirst();
    typename blitz::Array<std::string, D>::iterator i2;
    for(i2 = result_data.begin(); i2 != result_data.end(); 
	++i2, i1 += dt.getSize()) {
      // By our fairly odd convention, this string may contain a
      // trailing '\0'. So we strip that off if we find it.
      std::string s(i1, i1 + dt.getSize());
      *i2 = std::string(s.c_str());
    }
  }
  return result_data;
  } catch(const H5::Exception& e) {
    Exception en;
    en << "While reading field " << Dataname
       << " for the file '" << hf.file_name()
       << "' a HDF 5 Exception thrown:\n"
       << "  " << e.getDetailMsg();
    throw en;
  }
}

blitz::Array<std::string, D> read_field(const HdfFile& hf,
			const H5::H5File& h, 
			const std::string& Dataname,
		        const blitz::TinyVector<hsize_t,D>& Start, 
 		        const blitz::TinyVector<hsize_t,D>& Size) const
{
  try {
  using namespace H5;
  DataSet d = h.openDataSet(Dataname);
  DataSpace ds = d.getSpace();
  if(ds.getSimpleExtentNdims() != D) {
    Exception e;
    e << "Dataset " << Dataname << " does not have the expected rank of " 
      << D;
    throw e;
  }
  DataSpace ms(D, &Size[0]);
  ds.selectHyperslab(H5S_SELECT_SET, &Size[0], &Start[0]);
  DataType dt = d.getDataType();
  blitz::Array<std::string, D> result_data(Size);
  if(dt.isVariableStr()) {
    blitz::Array<const char*, D> read_data(result_data.shape());
    StrType st(PredType::C_S1, H5T_VARIABLE);
    d.read(read_data.dataFirst(), st, ms, ds);
    
    typename blitz::Array<const char*, D>::const_iterator i1;
    typename blitz::Array<std::string, D>::iterator i2 = result_data.begin();
    for(i1 = read_data.begin(); i1 != read_data.end(); ++i1, ++i2)
      *i2 = std::string(*i1);
  } else {
    int flat_size = 1;
    for(int i = 0; i < D; i++)
      flat_size *= Size(i);
    flat_size *= dt.getSize();
    blitz::Array<char, 1> read_data(flat_size);
    d.read(read_data.dataFirst(), d.getStrType(), ms, ds);

    char* i1 = read_data.dataFirst();
    typename blitz::Array<std::string, D>::iterator i2;
    for(i2 = result_data.begin(); i2 != result_data.end(); 
	++i2, i1 += dt.getSize()) {
      // By our fairly odd convention, this string may contain a
      // trailing '\0'. So we strip that off if we find it.
      std::string s(i1, i1 + dt.getSize());
      *i2 = std::string(s.c_str());
    }
  }
  return result_data;
  } catch(const H5::Exception& e) {
    Exception en;
    en << "While reading field " << Dataname
       << " for the file '" << hf.file_name()
       << "' a HDF 5 Exception thrown:\n"
       << "  " << e.getDetailMsg();
    throw en;
  }
}

};

/// @endcond
  
//-----------------------------------------------------------------------
/// Read a given field.
//-----------------------------------------------------------------------

template<class T, int D> inline blitz::Array<T, D> 
HdfFile::read_field(const std::string& Dataname) const
{
  HdfFilePartialSpecialHelper<T,D> helper;
  return helper.read_field(*this, *h, Dataname);
}

//-----------------------------------------------------------------------
/// Read a given field. This reads a subset of the data, given by
/// Start and Size.
//-----------------------------------------------------------------------

template<class T, int D> blitz::Array<T, D> 
HdfFile::read_field(
	 const std::string& Dataname, 
	 const blitz::TinyVector<int,D>& Start, 
	 const blitz::TinyVector<int,D>& Size) const
{
  HdfFilePartialSpecialHelper<T,D> helper;
  blitz::TinyVector<hsize_t, D> start2, size2;
  start2 = Start;
  size2 = Size;
  return helper.read_field(*this, *h, Dataname, start2, size2);
}
}

GEOCAL_EXPORT_KEY(HdfFile);
#endif
