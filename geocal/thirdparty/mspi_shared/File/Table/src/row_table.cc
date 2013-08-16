/*===========================================================================
=                                                                           =
=                                  RowTable                                 =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                                    Shared

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: RowTable

!ABSTRACT:

   This contains support routines for the class RowTable 

!PARENT(S):

!REVISION HISTORY:

See ClearCase Log

`=============================================================================
!END=======================================================================*/

#include "row_table.h"		// Definition of class.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include "hdf5_hl.h" 		// Definition of H5TBmake_table, H5LTfind_dataset
#include <sstream>  		// Definition of std::ostringstream

#define MAX_FIELDNAME_SIZE 1024 // Size of buffer to allocate for each field name.
                                // NOTE: The HDF5 library version 1.6.7 overruns 
                                // an internal buffer when the size of a field
                                // name exceeds 255 characters.
                                // HDF Help Desk has been notified of the problem.

//***************************************************************************
// Default constructor
//***************************************************************************

MSPI::Shared::RowTable::RowTable()
  : table_name_("Row Table"),
    loc_id_(-1)
{
}

//***************************************************************************
// Constructor definition.
//***************************************************************************

MSPI::Shared::RowTable::RowTable(hid_t Hdf5_loc_id)
  : table_name_("Row Table"),
    loc_id_(Hdf5_loc_id)
{
  herr_t status; 		// Status code returned by HDF calls
  const hsize_t number_field = 4;

//---------------------------------------------------------------------------
// Initialize table parameters.
//---------------------------------------------------------------------------

  const char *field_name[number_field] = {"Row Number",
			       "Wavelength",
			       "Polarization Angle",
			       "Swath name"};

  type_size_ = sizeof(RowTableRecord);
  field_offset_[0] = HOFFSET(RowTableRecord, row_number);
  field_offset_[1] = HOFFSET(RowTableRecord, wavelength);
  field_offset_[2] = HOFFSET(RowTableRecord, polarization_angle);
  field_offset_[3] = HOFFSET(RowTableRecord, swath_name);

  hid_t string_type = H5Tcopy(H5T_C_S1);
  H5Tset_size( string_type, swath_name_maxlen_);
  field_type_[0] = H5T_NATIVE_INT;
  field_type_[1] = H5T_NATIVE_FLOAT;
  field_type_[2] = H5T_NATIVE_FLOAT;
  field_type_[3] = string_type;

  RowTableRecord record;
  field_size_[0] = sizeof(record.row_number);
  field_size_[1] = sizeof(record.wavelength);
  field_size_[2] = sizeof(record.polarization_angle);
  field_size_[3] = sizeof(record.swath_name);

//---------------------------------------------------------------------------
// If the table does not exist, create a new table.
//---------------------------------------------------------------------------

  if (!H5LTfind_dataset(loc_id_, table_name_.c_str())) {

    status = H5TBmake_table(table_name_.c_str(),
			    loc_id_,
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

  status = H5TBget_table_info(loc_id_, table_name_.c_str(), &number_field_check, 
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

  status = H5TBget_field_info(loc_id_, table_name_.c_str(), 
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

    RowTableRecord record;
    status = H5TBread_records(loc_id_, table_name_.c_str(), irecord, 1,
			      type_size_, field_offset_, field_size_, &record);
    if (status < 0) {
      std::ostringstream message;
      message << "Trouble with H5TBget_table_info, status: "<< status;
      throw MSPI_EXCEPTIONm(message.str());
    }

//---------------------------------------------------------------------------
// Check that there are no duplicate row numbers.
//---------------------------------------------------------------------------

    if (row_set_.find(record.row_number) != row_set_.end()) {
      std::ostringstream message;
      message << "Error! More than one record has the same row number: "
	      << record.row_number;
      throw MSPI_EXCEPTIONm(message.str());
    }

//---------------------------------------------------------------------------
// Add this record to table in memory.
//---------------------------------------------------------------------------

    table_[record.row_number] = record;
    row_set_.insert(record.row_number);

//---------------------------------------------------------------------------
// End loop for each record.
//---------------------------------------------------------------------------

  }
}

//***************************************************************************
// Destructor definition.
//***************************************************************************

MSPI::Shared::RowTable::~RowTable()
{
}


//***************************************************************************
// Public method.
//***************************************************************************

void 
MSPI::Shared::RowTable::add(
  int Row_number, 
  float Wavelength, 
  float Polarization_angle, 
  const std::string& Swath_name
)
{
  herr_t status; 		// Status code returned by HDF calls

//---------------------------------------------------------------------------
// Check that this row number is not already in the table.
//---------------------------------------------------------------------------

  std::set<int>::iterator h = row_set_.find(Row_number);
  if (h != row_set_.end()) {
    std::ostringstream message;
    message << "Row number already in table: "<<Row_number;
    throw MSPI_EXCEPTIONm(message.str());
  }

//---------------------------------------------------------------------------
// Add new record to table in file.
//---------------------------------------------------------------------------

  RowTableRecord record;
  record.row_number = Row_number;
  record.wavelength = Wavelength;
  record.polarization_angle = Polarization_angle;
  strncpy(record.swath_name, Swath_name.c_str(), swath_name_maxlen_);

  status = H5TBappend_records(loc_id_, table_name_.c_str(), 
			      1, type_size_, field_offset_,
			      field_size_, &record);
  if (status < 0) {
    std::ostringstream message;
    message << "Trouble with H5TBappend_records, row number: "<<Row_number;
    throw MSPI_EXCEPTIONm(message.str());
  }
  
//---------------------------------------------------------------------------
// Add new record to table in memory.
//---------------------------------------------------------------------------

  row_set_.insert(Row_number);
  table_[Row_number] = record;
}
