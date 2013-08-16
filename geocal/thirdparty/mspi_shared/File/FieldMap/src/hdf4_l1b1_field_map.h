/*===========================================================================
=                                                                           =
=                            L1B1FieldMap                                   =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                                    Shared

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: L1B1FieldMap

=============================================================================
!END=======================================================================*/

#ifndef HDF4_L1B1_FIELD_MAP_H
#define HDF4_L1B1_FIELD_MAP_H

#include "MSPI-Shared/File/FieldMap/src/field_map.h" // Definition of FieldMap

namespace MSPI {
  namespace Shared {
    namespace HDF4 {
      class L1B1FieldMap;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Provides field map for an HDF-EOS grid file.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::HDF4::L1B1FieldMap : public FieldMap {
public:

  L1B1FieldMap(const std::string& Filename);

protected:

private:

};

#endif
