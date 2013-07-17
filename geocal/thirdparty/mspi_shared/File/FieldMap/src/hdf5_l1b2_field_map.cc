/*===========================================================================
=                                                                           =
=                               L1B2FieldMap                                =
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

#include "hdf5_l1b2_field_map.h" // Definition of class.
#include <HE5_HdfEosDef.h>	 // Definition of HE5_GDinqgrid
#include "MSPI-Shared/Config/src/config_file.h"
				// Definition of ConfigFile
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include "MSPI-Shared/File/Table/src/band_table.h"
				  // Definition of BandTable
#include <sstream>  		// Definition of std::ostringstream
#include <string> 		// Definition of std::string

/////////////////////////////////////////////////////////////////////////////
/// Construct field map using external configuration file.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::HDF5::L1B2FieldMap::L1B2FieldMap(
  const std::string& Filename
)
{

  //-------------------------------------------------------------------------
  // Open file.
  //-------------------------------------------------------------------------

  hid_t hdfeos_fid = HE5_GDopen(Filename.c_str(), H5F_ACC_RDONLY);
  if (hdfeos_fid == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with GDopen, file: " + Filename);
  }

  //-------------------------------------------------------------------------
  // Get grid names and numbers from band table.
  //-------------------------------------------------------------------------

  std::vector<std::string> grid_name;
  std::vector<int> grid_number;

  {
    hid_t hdf_fid;
    hid_t hdfeos_group_id;
    herr_t status = HE5_EHidinfo(hdfeos_fid, &hdf_fid, &hdfeos_group_id);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with EHidinfo, file: " + Filename);
    }

    hid_t band_table_loc_id = H5Gopen(hdfeos_group_id,"ADDITIONAL/FILE_ATTRIBUTES",H5P_DEFAULT);
    if (band_table_loc_id < 0) {
      throw MSPI_EXCEPTIONm("Trouble with H5Gopen, file: " + Filename);
    }

    MSPI::Shared::BandTable band_table(band_table_loc_id);

    std::vector<int> band_numbers = band_table.band_numbers();
    for (std::vector<int>::const_iterator iband = band_numbers.begin() ; 
	 iband != band_numbers.end() ; 
	 ++iband) {
      int band_number = *iband;
      grid_number.push_back(band_number);
      grid_name.push_back(band_table.grid_name(band_number));
    }
  }

  //-------------------------------------------------------------------------
  // For each grid...(begin loop)
  //-------------------------------------------------------------------------

  for (size_t igrid = 0 ; igrid < grid_name.size(); igrid++) {

    //-----------------------------------------------------------------------
    // Attach to grid.
    //-----------------------------------------------------------------------

    hid_t gid = HE5_GDattach(hdfeos_fid, grid_name[igrid].c_str());
    if (gid == FAIL) {
      throw MSPI_EXCEPTIONm("Trouble with GDattach, grid: " + grid_name[igrid]);
    }

    //-----------------------------------------------------------------------
    // Get field names
    // Get HDF-EOS5 datatype identifiers.
    //-----------------------------------------------------------------------

    std::vector<std::string> field_name;
    std::vector<hid_t> hdfeos_datatype;  // These are *not* HDF5 datatype identifiers
    int number_field;

    {
      long size;
      {
	long status = HE5_GDnentries(gid,HE5_HDFE_NENTDFLD,&size);
	if (status == -1) {
	  throw MSPI_EXCEPTIONm("Trouble with GDnentries, file: " + Filename);
	}
      }
      std::vector<char> buffer(size+1);
      number_field = HE5_GDinqfields(gid,&buffer[0],NULL,NULL);
      if (number_field == -1) {
	throw MSPI_EXCEPTIONm("Trouble with GDinqfields, file: " + Filename);
      }

      std::string field_list(&buffer[0],&buffer[size]);
      std::stringstream ss(field_list);
      std::string item;
      while (std::getline(ss, item, ',')) {
	field_name.push_back(item);
      }

      hdfeos_datatype.resize(number_field);
      {
	herr_t status = HE5_GDinqfields(gid,&buffer[0],NULL,&hdfeos_datatype[0]);
	if (status == -1) {
	  throw MSPI_EXCEPTIONm("Trouble with GDinqfields, file: " + Filename);
	}
      }
    }

    //-----------------------------------------------------------------------
    // Get fill values
    //-----------------------------------------------------------------------
      
    std::vector<std::string> types(number_field);
    std::vector<boost::any> fill_values(number_field);

    for (int ifield = 0 ; ifield < number_field ; ifield++) {
      const std::string& field = field_name[ifield];
      const hid_t& datatype = hdfeos_datatype[ifield];

      if (datatype == HE5T_NATIVE_INT) {
	types[ifield] = "int"; 
	fill_values[ifield] = get_fill_value<int>(gid, field.c_str());
      } else if (datatype == HE5T_NATIVE_FLOAT) {
	types[ifield] = "float"; 
	fill_values[ifield] = get_fill_value<float>(gid, field.c_str());
      } else if (datatype == HE5T_NATIVE_DOUBLE) {
	types[ifield] = "double"; 
	fill_values[ifield] = get_fill_value<double>(gid, field.c_str());
      } else {
	throw MSPI_EXCEPTIONm("Unsupported datatype for field: " + 
			      field_name[ifield] + ", file: " + Filename);
      }
    }

    //-----------------------------------------------------------------------
    // Initialize grid name, grid number and field names.
    //-----------------------------------------------------------------------
    
    init_grid(grid_number[igrid], grid_name[igrid], field_name, types, fill_values);

    //---------------------------------------------------------------------------
    // Detach grid.
    //---------------------------------------------------------------------------

    int status = HE5_GDdetach(gid);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with GDdetach, grid: " + grid_name[igrid]);
    }

  //-------------------------------------------------------------------------
  // End loop for this grid.
  //-------------------------------------------------------------------------

  }

//---------------------------------------------------------------------------
// Close file.
//---------------------------------------------------------------------------

  if (hdfeos_fid != FAIL) {
    int status = HE5_GDclose(hdfeos_fid);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with GDclose, file: " + Filename);
    }
  }
}
