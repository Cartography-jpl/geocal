/*===========================================================================
=                                                                           =
=                               L1B1FieldMap                                =
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

#include "hdf5_l1b1_field_map.h" // Definition of class.
#include <HE5_HdfEosDef.h>	 // Definition of HE5_SWinqgrid
#include "MSPI-Shared/Config/src/config_file.h"
				// Definition of ConfigFile
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include "MSPI-Shared/File/Table/src/row_table.h"
				  // Definition of RowTable
#include <sstream>  		// Definition of std::ostringstream
#include <string> 		// Definition of std::string

/////////////////////////////////////////////////////////////////////////////
/// Construct field map using external configuration file.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::HDF5::L1B1FieldMap::L1B1FieldMap(
  const std::string& Filename
)
{
  //-------------------------------------------------------------------------
  // Open file.
  //-------------------------------------------------------------------------

  hid_t hdfeos_fid = HE5_SWopen(Filename.c_str(), H5F_ACC_RDONLY);
  if (hdfeos_fid == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with SWopen, file: " + Filename);
  }

  //-------------------------------------------------------------------------
  // Get grid names and numbers from row table.
  //-------------------------------------------------------------------------

  std::vector<std::string> swath_name;
  std::vector<int> swath_number;

  {
    hid_t hdf_fid;
    hid_t hdfeos_group_id;
    herr_t status = HE5_EHidinfo(hdfeos_fid, &hdf_fid, &hdfeos_group_id);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with EHidinfo, file: " + Filename);
    }

    hid_t row_table_loc_id = H5Gopen(hdfeos_group_id,"ADDITIONAL/FILE_ATTRIBUTES",H5P_DEFAULT);
    if (row_table_loc_id < 0) {
      throw MSPI_EXCEPTIONm("Trouble with H5Gopen, file: " + Filename);
    }

    MSPI::Shared::RowTable row_table(row_table_loc_id);

    std::vector<int> row_numbers = row_table.row_numbers();
    for (std::vector<int>::const_iterator irow = row_numbers.begin() ; 
	 irow != row_numbers.end() ; 
	 ++irow) {
      int row_number = *irow;
      swath_number.push_back(row_number);
      swath_name.push_back(row_table.swath_name(row_number));
    }
  }

  //-------------------------------------------------------------------------
  // For each swath...(begin loop)
  //-------------------------------------------------------------------------

  for (size_t iswath = 0 ; iswath < swath_name.size(); iswath++) {

    //-----------------------------------------------------------------------
    // Attach to swath.
    //-----------------------------------------------------------------------

    hid_t gid = HE5_SWattach(hdfeos_fid, swath_name[iswath].c_str());
    if (gid == FAIL) {
      throw MSPI_EXCEPTIONm("Trouble with SWattach, swath: " + swath_name[iswath]);
    }

    //-----------------------------------------------------------------------
    // Get field names
    //-----------------------------------------------------------------------

    std::vector<std::string> field_name;

    {
      long size;
      long status = HE5_SWnentries(gid,HE5_HDFE_NENTDFLD,&size);
      if (status == -1) {
	throw MSPI_EXCEPTIONm("Trouble with SWnentries, file: " + Filename);
      }
      std::vector<char> buffer(size+1);
      status = HE5_SWinqdatafields(gid,&buffer[0],NULL,NULL);
      if (status == -1) {
	throw MSPI_EXCEPTIONm("Trouble with SWinqdatafields, file: " + Filename);
      }

      std::string field_list(&buffer[0],&buffer[size]);
      std::stringstream ss(field_list);
      std::string item;
      while (std::getline(ss, item, ',')) {
	field_name.push_back(item);
      }
    }

    {
      long size;
      long status = HE5_SWnentries(gid,HE5_HDFE_NENTGFLD,&size);
      if (status == -1) {
	throw MSPI_EXCEPTIONm("Trouble with SWnentries, file: " + Filename);
      }
      std::vector<char> buffer(size+1);
      status = HE5_SWinqgeofields(gid,&buffer[0],NULL,NULL);
      if (status == -1) {
	throw MSPI_EXCEPTIONm("Trouble with SWinqgeofields, file: " + Filename);
      }

      std::string field_list(&buffer[0],&buffer[size]);
      std::stringstream ss(field_list);
      std::string item;
      while (std::getline(ss, item, ',')) {
	field_name.push_back(item);
      }
    }

    //-----------------------------------------------------------------------
    // Initialize swath name, swath number and field names.
    //-----------------------------------------------------------------------
    
    init_grid(swath_number[iswath], swath_name[iswath], field_name);

    //---------------------------------------------------------------------------
    // Detach swath.
    //---------------------------------------------------------------------------

    int status = HE5_SWdetach(gid);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWdetach, swath: " + swath_name[iswath]);
    }

  //-------------------------------------------------------------------------
  // End loop for this swath.
  //-------------------------------------------------------------------------

  }

//---------------------------------------------------------------------------
// Close file.
//---------------------------------------------------------------------------

  if (hdfeos_fid != FAIL) {
    int status = HE5_SWclose(hdfeos_fid);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with SWclose, file: " + Filename);
    }
  }
}
