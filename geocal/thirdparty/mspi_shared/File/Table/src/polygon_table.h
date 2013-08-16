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

#ifndef HDF5_POLYGON_TABLE_H
#define HDF5_POLYGON_TABLE_H

#include "hdf5.h"		  // Definition of hid_t
#include <vector>		// Definition of std::vector

namespace MSPI {
  namespace Shared {
    void polygon_table_write(hid_t Group_id,
			     const std::vector<double>& Latitude,
			     const std::vector<double>& Longitude);
    
    void polygon_table_read(hid_t Group_id,
			    std::vector<double>& Latitude,
			    std::vector<double>& Longitude);
    
  }
}

#endif
