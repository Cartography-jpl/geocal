//===========================================================================
//                                                                          
//                               Hdf5Meta                                   
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#ifndef HDF5_META_H
#define HDF5_META_H

#include "meta.h" 		// Definition of Meta
#include <string>		// Definition of std::string
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <sstream> 		// Definition of std::ostringstream
#include <hdf5.h> 		// Definition of hid_t
#include <H5Cpp.h>		// Definition of H5::DataType

namespace MSPI {
  namespace Shared {
    class Hdf5Meta;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Provides read/write methods for HDF5 file metadata.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::Hdf5Meta : public Meta {
public:
  Hdf5Meta();
  Hdf5Meta(const Meta& M);
  void hdf5_export(hid_t Group_id) const;
  static Meta hdf5_import(hid_t Group_id);

private:
  class hdf5_type_visitor : public boost::static_visitor<H5::DataType> {
    // Return HDF5 type object suitable for the given any_type object.
  public:
    H5::DataType operator()(const std::string& P) const { return H5::StrType(H5::PredType::C_S1, P.size()); }
    H5::DataType operator()(const int& P) const { return H5::IntType(H5::PredType::NATIVE_INT); }
    H5::DataType operator()(const unsigned& P) const { return H5::IntType(H5::PredType::NATIVE_UINT); }
    H5::DataType operator()(const bool& P) const { 
      H5::EnumType t(H5::PredType::NATIVE_UINT8);
      char v[2] = {0,1};
      t.insert("false",&v[0]);
      t.insert("true",&v[1]);
      return t;
    }
    H5::DataType operator()(const float& P) const { return H5::FloatType(H5::PredType::NATIVE_FLOAT); }
    H5::DataType operator()(const double& P) const { return H5::FloatType(H5::PredType::NATIVE_DOUBLE); }
  };

  static any_type hdf5_any(const H5::DataType& Type);
  static void hdf5_export(hid_t Group_id, const std::string& Name,
			  const any_type& Value);
  static any_type hdf5_import(hid_t Group_id, const std::string& Name);
  static bool bool_H5Tequal(hid_t dtype_id1, hid_t dtype_id2);
  static bool bool_H5Aexists(hid_t obj_id, const char *attr_name);
  static std::string hdf5_type_debug(hid_t Type_id);
};

/////////////////////////////////////////////////////////////////////////////
/// Default constructor
/////////////////////////////////////////////////////////////////////////////

inline
MSPI::Shared::Hdf5Meta::Hdf5Meta()
{
}

/////////////////////////////////////////////////////////////////////////////
/// Constructor
/////////////////////////////////////////////////////////////////////////////

inline
MSPI::Shared::Hdf5Meta::Hdf5Meta(const Meta& M)
  : Meta(M)
{
}

/////////////////////////////////////////////////////////////////////////////
/// Return diagnostic message regarding the given type.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::Hdf5Meta::hdf5_type_debug(hid_t Type_id)
{
  const int number_type = 18;
  hid_t types[number_type] = {H5T_NATIVE_CHAR,
			      H5T_NATIVE_SHORT,
			      H5T_NATIVE_INT,
			      H5T_NATIVE_LONG,
			      H5T_NATIVE_LLONG,
			      H5T_NATIVE_UCHAR,
			      H5T_NATIVE_USHORT,
			      H5T_NATIVE_UINT,
			      H5T_NATIVE_ULONG,
			      H5T_NATIVE_ULLONG,
			      H5T_NATIVE_FLOAT,
			      H5T_NATIVE_DOUBLE,
			      H5T_NATIVE_LDOUBLE,
			      H5T_NATIVE_B8,
			      H5T_NATIVE_B16,
			      H5T_NATIVE_B32,
			      H5T_NATIVE_B64,
			      H5T_C_S1};
  const char *names[number_type] = {"H5T_NATIVE_CHAR",
				    "H5T_NATIVE_SHORT",
				    "H5T_NATIVE_INT",
				    "H5T_NATIVE_LONG",
				    "H5T_NATIVE_LLONG",
				    "H5T_NATIVE_UCHAR",
				    "H5T_NATIVE_USHORT",
				    "H5T_NATIVE_UINT",
				    "H5T_NATIVE_ULONG",
				    "H5T_NATIVE_ULLONG",
				    "H5T_NATIVE_FLOAT",
				    "H5T_NATIVE_DOUBLE",
				    "H5T_NATIVE_LDOUBLE",
				    "H5T_NATIVE_B8",
				    "H5T_NATIVE_B16",
				    "H5T_NATIVE_B32",
				    "H5T_NATIVE_B64",
                                    "H5T_C_S1"};

  std::ostringstream message;
  message << "Type_id("<< Type_id << ")={";
  for (int i = 0 ; i < number_type ; i++) {
    if (bool_H5Tequal(Type_id, types[i])) {
      message << names[i] << " ";
    }
  }
  message << "}";

  message << " class=" << H5Tget_class(Type_id) << " ";

  int number_member;

  switch (H5Tget_class(Type_id)) {
  case H5T_INTEGER:
    message << "[Integer ByteOrder=";
    switch (H5Tget_order(Type_id)) {
    case H5T_ORDER_LE: message << "LE"; break;
    case H5T_ORDER_BE: message << "BE"; break;
    default: message << "?";
    }
    message << " Sign=";
    switch (H5Tget_sign(Type_id)) {
    case H5T_SGN_NONE: message << "false"; break;
    case H5T_SGN_2: message << "true"; break;
    default: message << "?";
    }
    message << " Size= ";
    message << H5Tget_size(Type_id) << "]";
    break;
  case H5T_ENUM:
    number_member = H5Tget_nmembers(Type_id);
    message << "[Enum ";
    if (number_member < 0) {
      throw MSPI_EXCEPTIONm("Trouble with H5Tget_nmembers");
    }
    for (int i = 0 ; i < number_member ; i++) {
      char value;
      herr_t status = H5Tget_member_value(Type_id, i, &value);
      if (status < 0) {
	throw MSPI_EXCEPTIONm("Trouble with H5Tget_member_value");
      }
      std::string name(256,0);
      H5Tenum_nameof(Type_id, &value, &name[0], name.size());
      message << name << ":" << int(value) << " ";
    }
    message << "]";
    break;
  default:
    message << "[other type]";
  }
    
  return message.str();
}

/////////////////////////////////////////////////////////////////////////////
/// Wrapper for H5Tequal that returns a proper boolean.
/////////////////////////////////////////////////////////////////////////////

inline bool 
MSPI::Shared::Hdf5Meta::bool_H5Tequal(hid_t dtype_id1, hid_t dtype_id2)
{
  htri_t state = H5Tequal(dtype_id1, dtype_id2);
  if (state < 0) {
    throw MSPI_EXCEPTIONm("Trouble with H5Tequal");
  }
  return (state != 0);
}

/////////////////////////////////////////////////////////////////////////////
/// Wrapper for H5Aexists that returns a proper boolean.
/////////////////////////////////////////////////////////////////////////////

inline bool 
MSPI::Shared::Hdf5Meta::bool_H5Aexists(hid_t obj_id, const char *attr_name)
{
  htri_t state = H5Aexists(obj_id, attr_name);
  if (state < 0) {
    throw MSPI_EXCEPTIONm("Trouble with H5Aexists");
  }
  return (state != 0);
}

/////////////////////////////////////////////////////////////////////////////
/// Return any_type object suitable for holding an HDF5 attribute
/// of the given type.
/////////////////////////////////////////////////////////////////////////////

inline MSPI::Shared::Meta::any_type 
MSPI::Shared::Hdf5Meta::hdf5_any(
  const H5::DataType& Type
)
{
  size_t size_bytes = H5Tget_size(Type.getId());
  if (size_bytes < 0) {
    throw MSPI_EXCEPTIONm("Trouble with H5Tget_size");
  }

  hid_t class_id = H5Tget_class(Type.getId());
  if (class_id < 0) {
    throw MSPI_EXCEPTIONm("Trouble with H5Tget_class");
  }
  
  if (class_id == H5T_STRING) {
    return any_type(std::string(size_bytes,0));
  } else if (bool_H5Tequal(Type.getId(), H5T_NATIVE_INT) and
	     size_bytes == sizeof(int)) {
    return any_type(int(0));
  } else if (bool_H5Tequal(Type.getId(), H5T_NATIVE_UINT) and
	     size_bytes == sizeof(unsigned)) {
    return any_type(unsigned(0));
  } else if (class_id == H5T_ENUM and size_bytes == sizeof(bool)) {
    return any_type(bool(false));
  } else if (bool_H5Tequal(Type.getId(), H5T_NATIVE_FLOAT) and
	     size_bytes == sizeof(float)) {
    return any_type(float(0.0));
  } else if (bool_H5Tequal(Type.getId(), H5T_NATIVE_DOUBLE) and
	     size_bytes == sizeof(double)) {
    return any_type(double(0.0));
  }

  std::ostringstream msg;
  msg << "Unsupported class, type  or size for metadata: type_id=" 
      << Type.getId() << ", size=" << size_bytes << ", class=" << class_id 
      << ", " << hdf5_type_debug(Type.getId());
  throw MSPI_EXCEPTIONm(msg.str());
}

#endif
