/*===========================================================================
=                                                                           =
=                            ConfiguredFieldMap                             =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                                    Shared

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: ConfiguredFieldMap

!ABSTRACT:

!DEFINITIONS:

!DERIVED FROM:

!DESCRIPTION:

!PUBLIC MEMBERS:

!PROTECTED MEMBERS:

!PRIVATE MEMBERS:

!ASSOCIATED FUNCTIONS:

!KNOWN BUGS AND LIMITATIONS:

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#ifndef CONFIGURED_FIELD_MAP_H
#define CONFIGURED_FIELD_MAP_H

#include "MSPI-Shared/File/FieldMap/src/field_map.h" // Definition of FieldMap

namespace MSPI {
  namespace Shared {
    class ConfiguredFieldMap;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Contains a mapping from row numbers to field names for MSPI L0 
///   Standard Product.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::ConfiguredFieldMap : public FieldMap {
public:

  ConfiguredFieldMap(const std::string& Filename);
  const std::string& granule_id() const;

protected:

private:
  std::string granule_id_;

};

/////////////////////////////////////////////////////////////////////////////
/// Return granule id.
/////////////////////////////////////////////////////////////////////////////

inline const std::string&
MSPI::Shared::ConfiguredFieldMap::granule_id() const
{
  return granule_id_;
}

#endif
