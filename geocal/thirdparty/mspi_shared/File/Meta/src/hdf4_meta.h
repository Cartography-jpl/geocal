//===========================================================================
//                                                                          
//                               Hdf4Meta                                   
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#ifndef HDF4_META_H
#define HDF4_META_H

#include "meta.h" 		// Definition of Meta
#include <string>		// Definition of std::string
#include <map>			// Definition of std::map
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <sstream> 		// Definition of std::ostringstream
#include <hdf.h> 		// Definition of int32

namespace MSPI {
  namespace Shared {
    class Hdf4Meta;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Provides read/write methods for HDF4 file metadata.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::Hdf4Meta : public Meta {
public:
  Hdf4Meta();
  Hdf4Meta(const Meta& M);
  void hdf4_export(int32 Group_id) const;
  static Meta hdf4_import(int32 Group_id);

private:
  struct hdf4_type {
    hdf4_type(int32 Id, int32 Count = 1) : id(Id), count(Count) {}
    int32 id;
    int32 count;
  };

  class hdf4_type_visitor : public boost::static_visitor<hdf4_type> {
    // Return HDF4 type object suitable for the given any_type object.
  public:
    hdf4_type operator()(const std::string& P) const { return hdf4_type(DFNT_CHAR8, P.size()); }
    hdf4_type operator()(const int& P) const { return hdf4_type(DFNT_INT32); }
    hdf4_type operator()(const unsigned& P) const { return hdf4_type(DFNT_UINT32); }
    hdf4_type operator()(const bool& P) const { return hdf4_type(DFNT_UINT8); }
    hdf4_type operator()(const float& P) const { return hdf4_type(DFNT_FLOAT32); }
    hdf4_type operator()(const double& P) const { return hdf4_type(DFNT_FLOAT64); }
  };

  static any_type hdf4_any(const hdf4_type& Type);
  static void hdf4_export(int32 Group_id, const std::string& Name,
			  const any_type& Value);
  static any_type hdf4_import(int32 Group_id, const std::string& Name);
};

/////////////////////////////////////////////////////////////////////////////
/// Default constructor
/////////////////////////////////////////////////////////////////////////////

inline
MSPI::Shared::Hdf4Meta::Hdf4Meta()
{
}

/////////////////////////////////////////////////////////////////////////////
/// Constructor
/////////////////////////////////////////////////////////////////////////////

inline
MSPI::Shared::Hdf4Meta::Hdf4Meta(const Meta& M)
  : Meta(M)
{
}

/////////////////////////////////////////////////////////////////////////////
/// Return any_type object suitable for holding an HDF4 attribute
/// of the given type.
/////////////////////////////////////////////////////////////////////////////

inline MSPI::Shared::Meta::any_type 
MSPI::Shared::Hdf4Meta::hdf4_any(
  const hdf4_type& Type
)
{
  size_t size_bytes = DFKNTsize(Type.id);
  if (size_bytes < 0) {
    throw MSPI_EXCEPTIONm("DFKNTsize");
  }

  if (Type.count < 1) {
    throw MSPI_EXCEPTIONm("Type.count < 1");
  }

  size_bytes *= Type.count;

  if (Type.id == DFNT_CHAR8) {
    return any_type(std::string(size_bytes,0));
  } else if (Type.id == DFNT_INT32 and size_bytes == sizeof(int)) {
    return any_type(int(0));
  } else if (Type.id == DFNT_UINT32 and size_bytes == sizeof(unsigned)) {
    return any_type(unsigned(0));
  } else if (Type.id == DFNT_UINT8 and size_bytes == sizeof(bool)) {
    return any_type(bool(false));
  } else if (Type.id == DFNT_FLOAT32 and size_bytes == sizeof(float)) {
    return any_type(float(0.0));
  } else if (Type.id == DFNT_FLOAT64 and size_bytes == sizeof(double)) {
    return any_type(double(0.0));
  }

  std::ostringstream msg;
  msg << "Unsupported HDF4 type or size : type_id=" << Type.id 
      << ", size=" << size_bytes;
  throw MSPI_EXCEPTIONm(msg.str());
}

#endif
