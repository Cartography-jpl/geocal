//===========================================================================
//                                                                          
//                              polygon_table
//                                                                          
//===========================================================================
//
//             Copyright 2013, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#include "polygon_table.h"		// Definition of class.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <sstream>  		// Definition of std::ostringstream
#include "hdf5_hl.h" 		// Definition of H5TBmake_table, H5LTfind_dataset

/////////////////////////////////////////////////////////////////////////////
// Structure to contain a record.
/////////////////////////////////////////////////////////////////////////////

namespace {
  struct RecordType {
    double latitude;
    double longitude;
  };
}

/////////////////////////////////////////////////////////////////////////////
/// Read polygon table from HDF file.
/////////////////////////////////////////////////////////////////////////////

void 
MSPI::Shared::polygon_table_read(
  hid_t Group_id,
  std::vector<double>& Latitude,
  std::vector<double>& Longitude
)
{
  //-------------------------------------------------------------------------
  // Initialize table parameters.
  //-------------------------------------------------------------------------

  const std::string table_name = "GeoPolygon";
  const hsize_t number_field = 2;

  size_t type_size = sizeof(RecordType);

  size_t field_offset[number_field];
  field_offset[0] = HOFFSET(RecordType, latitude);
  field_offset[1] = HOFFSET(RecordType, longitude);

  RecordType record;
  size_t field_size[number_field];
  field_size[0] = sizeof(record.latitude);
  field_size[1] = sizeof(record.longitude);

  //-------------------------------------------------------------------------
  // Get the number of records and fields in the table.
  //-------------------------------------------------------------------------

  hsize_t number_field_check;
  hsize_t number_record;

  herr_t status = H5TBget_table_info(Group_id, table_name.c_str(), &number_field_check, 
				     &number_record);
  if (status < 0) {
    std::ostringstream message;
    message << "Trouble with H5TBget_table_info, status: "<< status;
    throw MSPI_EXCEPTIONm(message.str());
  }

  //-------------------------------------------------------------------------
  // Check that the table has the expected number of fields.
  //-------------------------------------------------------------------------

  if (number_field_check != number_field) {
    std::ostringstream message;
    message << "Unexpected number of fields: "<< number_field_check
	    << " (expected "<<number_field<<")";
    throw MSPI_EXCEPTIONm(message.str());
  }

  //-------------------------------------------------------------------------
  // Read records
  //-------------------------------------------------------------------------

  std::vector<RecordType> records(number_record);
  status = H5TBread_records(Group_id, table_name.c_str(), 0, number_record,
			    type_size, field_offset, field_size, &(records[0]));
  if (status < 0) {
    std::ostringstream message;
    message << "Trouble with H5TBget_table_info, status: "<< status;
    throw MSPI_EXCEPTIONm(message.str());
  }

  Latitude.resize(number_record);
  Longitude.resize(number_record);
  for (size_t i = 0; i < number_record ; i++) {
    Latitude[i] = records[i].latitude;
    Longitude[i] = records[i].longitude;
  }
}


/////////////////////////////////////////////////////////////////////////////
/// Write polygon table to HDF file.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::polygon_table_write(
  hid_t Group_id,
  const std::vector<double>& Latitude,
  const std::vector<double>& Longitude
)
{
  herr_t status; 		// Status code returned by HDF calls

  //-------------------------------------------------------------------------
  // Get number of records.
  //-------------------------------------------------------------------------

  size_t number_record = Latitude.size();

  //-------------------------------------------------------------------------
  // Argument check: number_record < 1
  //-------------------------------------------------------------------------

  if (number_record < 1) {
    throw MSPI_EXCEPTIONm("number_record < 1");
  }

  //-------------------------------------------------------------------------
  // Argument check: Longitude.size() != number_record
  //-------------------------------------------------------------------------
  
  if (Longitude.size() != number_record) {
    throw MSPI_EXCEPTIONm("Longitude.size() != number_record");
  }

  //-------------------------------------------------------------------------
  // Initialize table parameters.
  //-------------------------------------------------------------------------

  const std::string table_name = "GeoPolygon";
  const hsize_t number_field = 2;
  const char *field_name[number_field] = {"Latitude",
					  "Longitude"};

  size_t type_size = sizeof(RecordType);

  size_t field_offset[number_field];
  field_offset[0] = HOFFSET(RecordType, latitude);
  field_offset[1] = HOFFSET(RecordType, longitude);

  hid_t field_type[number_field];
  field_type[0] = H5T_NATIVE_DOUBLE;
  field_type[1] = H5T_NATIVE_DOUBLE;

  RecordType record;
  size_t field_size[number_field];
  field_size[0] = sizeof(record.latitude);
  field_size[1] = sizeof(record.longitude);

  //-------------------------------------------------------------------------
  // Create table
  //-------------------------------------------------------------------------

  status = H5TBmake_table(table_name.c_str(),
			  Group_id,
			  table_name.c_str(),
			  number_field, 
			  0,
			  type_size, 
			  field_name, 
			  field_offset, 
			  field_type, 
			  type_size, 
			  NULL, 0, 0);
  if (status < 0) {
    std::ostringstream message;
    message << "Trouble with H5TBmake_table, status: "<< status;
    throw MSPI_EXCEPTIONm(message.str());
  }

  //-------------------------------------------------------------------------
  // Write records
  //-------------------------------------------------------------------------

  std::vector<RecordType> records(number_record);
  for (size_t i = 0 ; i < number_record; i++) {
    records[i].latitude = Latitude[i];
    records[i].longitude = Longitude[i];
  }
  
  status = H5TBappend_records(Group_id, table_name.c_str(), 
			      number_record, type_size, field_offset,
			      field_size, &(records[0]));
  if (status < 0) {
    throw MSPI_EXCEPTIONm("Trouble with H5TBappend_records");
  }
}
