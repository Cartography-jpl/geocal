//===========================================================================
//                                                                          
//                              BandTable                                   
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#include "band_table.h"		// Definition of class.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include "hdf5_hl.h" 		// Definition of H5TBmake_table, H5LTfind_dataset
#include <sstream>  		// Definition of std::ostringstream

#define MAX_FIELDNAME_SIZE 1024 // Size of buffer to allocate for each field name.
                                // NOTE: The HDF5 library version 1.6.7 overruns 
                                // an internal buffer when the size of a field
                                // name exceeds 255 characters.
                                // HDF Help Desk has been notified of the problem.

//////////////////////////////////////////////////////////////////////////////
/// Open band table in the given group.  Creates a new table if one doesn't 
/// already exist.
//////////////////////////////////////////////////////////////////////////////

MSPI::Shared::BandTable::BandTable(hid_t Group_id)
  : table_name_("Band Table"),
    group_id_(Group_id)
{
  herr_t status; 		// Status code returned by HDF calls

//---------------------------------------------------------------------------
// Initialize table parameters.
//---------------------------------------------------------------------------

  const hsize_t number_field = 4;
  const char *field_name[number_field] = {"Band Number",
					  "Wavelength",
					  "Solar irradiance at 1 AU",
					  "Grid name"};

  type_size_ = sizeof(BandTableRecord);
  field_offset_[0] = HOFFSET(BandTableRecord, band_number);
  field_offset_[1] = HOFFSET(BandTableRecord, wavelength);
  field_offset_[2] = HOFFSET(BandTableRecord, e0);
  field_offset_[3] = HOFFSET(BandTableRecord, grid_name);

  hid_t string_type = H5Tcopy(H5T_C_S1);
  H5Tset_size( string_type, grid_name_maxlen_);
  field_type_[0] = H5T_NATIVE_INT;
  field_type_[1] = H5T_NATIVE_FLOAT;
  field_type_[2] = H5T_NATIVE_DOUBLE;
  field_type_[3] = string_type;

  BandTableRecord record;
  field_size_[0] = sizeof(record.band_number);
  field_size_[1] = sizeof(record.wavelength);
  field_size_[2] = sizeof(record.e0);
  field_size_[3] = sizeof(record.grid_name);

//---------------------------------------------------------------------------
// If the table does not exist, create a new table.
//---------------------------------------------------------------------------

  
  if (!H5LTfind_dataset(group_id_, table_name_.c_str())) {

    status = H5TBmake_table(table_name_.c_str(),
			    group_id_,
			    table_name_.c_str(),
			    number_field, 
			    0,
			    type_size_, 
			    field_name, 
			    field_offset_, 
			    field_type_, 
			    type_size_, 
			    NULL, 0, 0);
    if (status < 0) {
      std::ostringstream message;
      message << "Trouble with H5TBmake_table, status: "<< status;
      throw MSPI_EXCEPTIONm(message.str());
    }
  } 

//---------------------------------------------------------------------------
// Get the number of records and fields in the table.
//---------------------------------------------------------------------------

  hsize_t number_field_check;
  hsize_t number_record;

  status = H5TBget_table_info(group_id_, table_name_.c_str(), &number_field_check, 
			      &number_record);
  if (status < 0) {
    std::ostringstream message;
    message << "Trouble with H5TBget_table_info, status: "<< status;
    throw MSPI_EXCEPTIONm(message.str());
  }

//---------------------------------------------------------------------------
// Check that the table has the expected number of fields.
//---------------------------------------------------------------------------

  if (number_field_check != number_field) {
    std::ostringstream message;
    message << "Unexpected number of fields: "<< number_field_check
	    << " (expected "<<number_field<<")";
    throw MSPI_EXCEPTIONm(message.str());
  }

//---------------------------------------------------------------------------
// Check that the table has the expected fields and sizes.
//---------------------------------------------------------------------------

  char field_name_check[number_field][MAX_FIELDNAME_SIZE];
  char *field_name_check_ptr[number_field];
  size_t field_size_check[number_field];
  size_t field_offset_check[number_field];
  size_t type_size_check;

  for (hsize_t ifield = 0 ; ifield < number_field; ++ifield) {
    field_name_check_ptr[ifield] = &(field_name_check[ifield][0]);
  }

  status = H5TBget_field_info(group_id_, table_name_.c_str(), 
			      field_name_check_ptr, field_size_check, 
			      field_offset_check,
			      &type_size_check);
  if (status < 0) {
    std::ostringstream message;
    message << "Trouble with H5TBget_table_info, status: "<< status;
    throw MSPI_EXCEPTIONm(message.str());
  }

  for (hsize_t ifield = 0 ; ifield < number_field ; ++ifield) {
    if (strncmp(field_name_check[ifield],field_name[ifield],MAX_FIELDNAME_SIZE)) {
      std::ostringstream message;
      message << "Unexpected field name: "<< field_name_check[ifield] 
	      << " (expected "<<field_name[ifield]<<")";
      throw MSPI_EXCEPTIONm(message.str());
    }
    if (field_size_check[ifield] != field_size_[ifield]) {
      std::ostringstream message;
      message << "Unexpected field size: "<< field_size_check[ifield] 
	      << " (expected "<<field_size_[ifield]<<")";
      throw MSPI_EXCEPTIONm(message.str());
    }
    if (field_offset_check[ifield] != field_offset_[ifield]) {
      std::ostringstream message;
      message << "Unexpected field offset: "<< field_offset_check[ifield] 
	      << " (expected "<<field_offset_[ifield]<<")";
      throw MSPI_EXCEPTIONm(message.str());
    }
  }
  if (type_size_check != type_size_) {
      std::ostringstream message;
      message << "Unexpected type size: "<< type_size_check
	      << " (expected "<< type_size_<<")";
      throw MSPI_EXCEPTIONm(message.str());
  }

//---------------------------------------------------------------------------
// For each record in the table...
//---------------------------------------------------------------------------

  for (hsize_t irecord = 0 ; irecord < number_record ; irecord++) {

//---------------------------------------------------------------------------
// Read this record.
//---------------------------------------------------------------------------

    BandTableRecord record;
    status = H5TBread_records(group_id_, table_name_.c_str(), irecord, 1,
			      type_size_, field_offset_, field_size_, &record);
    if (status < 0) {
      std::ostringstream message;
      message << "Trouble with H5TBget_table_info, status: "<< status;
      throw MSPI_EXCEPTIONm(message.str());
    }

//---------------------------------------------------------------------------
// Check that there are no duplicate band numbers.
//---------------------------------------------------------------------------

    if (band_set_.find(record.band_number) != band_set_.end()) {
      std::ostringstream message;
      message << "Error! More than one record has the same band number: "
	      << record.band_number;
      throw MSPI_EXCEPTIONm(message.str());
    }

//---------------------------------------------------------------------------
// Add this record to table in memory.
//---------------------------------------------------------------------------

    table_[record.band_number] = record;
    band_set_.insert(record.band_number);

//---------------------------------------------------------------------------
// End loop for each record.
//---------------------------------------------------------------------------

  }
}

////////////////////////////////////////////////////////////////////////////
/// Add record.
////////////////////////////////////////////////////////////////////////////

void 
MSPI::Shared::BandTable::add(
  int Band_number, 
  float Wavelength, 
  double E0,
  const std::string& Grid_name
)
{
  herr_t status; 		// Status code returned by HDF calls

//---------------------------------------------------------------------------
// Check that this band number is not already in the table.
//---------------------------------------------------------------------------

  std::set<int>::iterator h = band_set_.find(Band_number);
  if (h != band_set_.end()) {
    std::ostringstream message;
    message << "Band number already in table: "<<Band_number;
    throw MSPI_EXCEPTIONm(message.str());
  }

//---------------------------------------------------------------------------
// Add new record to table in file.
//---------------------------------------------------------------------------

  BandTableRecord record;
  record.band_number = Band_number;
  record.wavelength = Wavelength;
  record.e0 = E0;
  strncpy(record.grid_name, Grid_name.c_str(), grid_name_maxlen_);

  status = H5TBappend_records(group_id_, table_name_.c_str(), 
			      1, type_size_, field_offset_,
			      field_size_, &record);
  if (status < 0) {
    std::ostringstream message;
    message << "Trouble with H5TBappend_records, band number: "<<Band_number;
    throw MSPI_EXCEPTIONm(message.str());
  }
  
//---------------------------------------------------------------------------
// Add new record to table in memory.
//---------------------------------------------------------------------------

  band_set_.insert(Band_number);
  table_[Band_number] = record;
}
