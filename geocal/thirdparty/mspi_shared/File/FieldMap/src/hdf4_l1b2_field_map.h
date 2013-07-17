/*===========================================================================
=                                                                           =
=                            L1B2FieldMap                                   =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                                    Shared

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: L1B2FieldMap

=============================================================================
!END=======================================================================*/

#ifndef HDF4_L1B2_FIELD_MAP_H
#define HDF4_L1B2_FIELD_MAP_H

#include <hdf.h> 		// Definition of int32
#include <HdfEosDef.h>				     // Definition of GDgetfillvalue
#include "MSPI-Shared/File/FieldMap/src/field_map.h" // Definition of FieldMap

namespace MSPI {
  namespace Shared {
    namespace HDF4 {
      class L1B2FieldMap;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Provides field map for an HDF-EOS grid file.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::HDF4::L1B2FieldMap : public FieldMap {
public:

  L1B2FieldMap(const std::string& Filename);

protected:

private:
  template <class T>
  T get_fill_value(int32 Gid, const std::string& Field_name);

};

/////////////////////////////////////////////////////////////////////////////
/// Retrieve fill value for the given field.
/////////////////////////////////////////////////////////////////////////////

template <class T>
inline T 
MSPI::Shared::HDF4::L1B2FieldMap::get_fill_value(
  int32 Gid, 
  const std::string& Field_name
)
{
  T fill;
  intn status = GDgetfillvalue(Gid, (char *)Field_name.c_str(), &fill);
  if (status == -1) {
    throw MSPI_EXCEPTIONm("Trouble with GDgetfill");
  }
  return fill;
}

#endif
