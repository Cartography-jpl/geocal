//===========================================================================
//                                                                          
//                              PolygonTable                                   
//                                                                          
//===========================================================================
//
//             Copyright 2013, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#ifndef HDF4_POLYGON_TABLE_H
#define HDF4_POLYGON_TABLE_H

#include "hdf.h"		  // Definition of int32
#include <vector>		// Definition of std::vector

namespace MSPI {
  namespace Shared {
    namespace HDF4 {
      void polygon_table_write(int32 Fid,
			       const std::vector<double>& Latitude,
			       const std::vector<double>& Longitude);
      
      void polygon_table_read(int32 Fid,
			      std::vector<double>& Latitude,
			      std::vector<double>& Longitude);
      
    }
  }
}

#endif
