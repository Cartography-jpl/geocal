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

/////////////////////////////////////////////////////////////////////////////
/// Read polygon table from HDF file.
/////////////////////////////////////////////////////////////////////////////

void 
MSPI::Shared::HDF4::polygon_table_read(
  int32 Fid,
  std::vector<double>& Latitude,
  std::vector<double>& Longitude
)
{
  //-------------------------------------------------------------------------
  // Initialize table parameters.
  //-------------------------------------------------------------------------

  const std::string table_name = "GeoPolygon";
  const int number_field = 2;
  const char *field_name[number_field] = {"Latitude",
					  "Longitude"};

  std::ostringstream list;
  for (int i = 0 ; i < number_field ; i++) {
    list << field_name[i];
    if (i < number_field-1) {
      list << ",";
    }
  }
  std::string field_name_list = list.str();

  //-------------------------------------------------------------------------
  // Start vdata API
  //-------------------------------------------------------------------------
  
  intn status = Vstart(Fid);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with Vstart");
  }

  //-------------------------------------------------------------------------
  // Attach to vdata
  //-------------------------------------------------------------------------

  int32 vdata_ref;
  vdata_ref = VSfind(Fid, table_name.c_str());
  if (vdata_ref == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with VSfind");
  }

  int32 vdata_id = VSattach(Fid, vdata_ref, "r");
  if (vdata_id == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with VSattach(readonly)");
  }

  //-------------------------------------------------------------------------
  // Get the number of records.
  //-------------------------------------------------------------------------

  int32 number_record = VSelts(vdata_id);
  if (number_record == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with VSelts");
  }

  //-------------------------------------------------------------------------
  // Read records
  //-------------------------------------------------------------------------

  std::vector<uint8> buffer(number_record * sizeof(double) * 2);
    
  int32 pos = VSseek(vdata_id, 0);
  if (pos != 0) {
    std::ostringstream message;
    message << "Trouble with VSseek(), pos="<< pos<<"\n";
    throw MSPI_EXCEPTIONm(message.str());
  }
  
  status = VSsetfields(vdata_id, field_name_list.c_str());
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with VSsetfields");
  }

  int32 count = VSread(vdata_id, &buffer[0], number_record, FULL_INTERLACE);
  if (count != number_record) {
    throw MSPI_EXCEPTIONm("Trouble with VSread");
  }

  VOIDP record_ptrs[2];
  Latitude.resize(number_record);
  Longitude.resize(number_record);
  record_ptrs[0] = &(Latitude[0]);
  record_ptrs[1] = &(Longitude[0]);
  
  status = VSfpack(vdata_id, _HDF_VSUNPACK, NULL, &buffer[0], buffer.size(), number_record, NULL,
		   record_ptrs);
  if (status) {
    HEprint(stderr,0);
    throw MSPI_EXCEPTIONm("Trouble with VSfpack(unpack)");
  }

  //-------------------------------------------------------------------------
  // Detach from vdata.
  //-------------------------------------------------------------------------

  VSdetach(vdata_id);

  //-------------------------------------------------------------------------
  // End vdata API
  //-------------------------------------------------------------------------

  Vend(Fid);
}


/////////////////////////////////////////////////////////////////////////////
/// Write polygon table to HDF file.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::HDF4::polygon_table_write(
  int32 Fid,
  const std::vector<double>& Latitude,
  const std::vector<double>& Longitude
)
{
  intn status; 		// Status code returned by HDF calls

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
  const int number_field = 2;
  const char *field_name[number_field] = {"Latitude",
					  "Longitude"};

  std::ostringstream list;
  for (int i = 0 ; i < number_field ; i++) {
    list << field_name[i];
    if (i < number_field-1) {
      list << ",";
    }
  }
  std::string field_name_list = list.str();

  int32 field_type[2];
  field_type[0] = DFNT_FLOAT64;
  field_type[1] = DFNT_FLOAT64;

  int32 field_order[number_field] = {1, 1};

  //-------------------------------------------------------------------------
  // Start vdata API
  //-------------------------------------------------------------------------
  
  status = Vstart(Fid);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with Vstart");
  }

  //-------------------------------------------------------------------------
  // Create table
  //-------------------------------------------------------------------------

  int32 vdata_id = VSattach(Fid, -1, "w");
  if (vdata_id == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with VSattach");
  }

  for (int i = 0 ; i < number_field ; i++) {
    status = VSfdefine(vdata_id, field_name[i], 
		       field_type[i], field_order[i]);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with VSfdefine");
    }
  }

  status = VSsetname(vdata_id, table_name.c_str());
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with VSsetname(read)");
  }

  for (int i = 0 ; i < number_field ; i++) {
    status = VSfdefine(vdata_id, field_name[i], 
		       field_type[i], field_order[i]);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with VSfdefine");
    }
  }

  status = VSsetfields(vdata_id, field_name_list.c_str());
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with VSsetfields");
  }

  //-------------------------------------------------------------------------
  // Write records
  //-------------------------------------------------------------------------

  std::vector<uint8> buffer(number_record * sizeof(double) * 2);

  VOIDP record_ptrs[2];
  record_ptrs[0] = (VOIDP)&(Latitude[0]);
  record_ptrs[1] = (VOIDP)&(Longitude[0]);
  
  status = VSfpack(vdata_id, _HDF_VSPACK, NULL, &buffer[0], buffer.size(), number_record, NULL, 
		   record_ptrs);
  if (status) {
    HEprint(stderr,0);
    throw MSPI_EXCEPTIONm("Trouble with VSfpack(pack)");
  }

  int32 count = VSwrite(vdata_id, &buffer[0], number_record, FULL_INTERLACE);
  if ((size_t)count != number_record) {
    throw MSPI_EXCEPTIONm("Trouble with VSwrite");
  }

  //-------------------------------------------------------------------------
  // Detach from vdata.
  //-------------------------------------------------------------------------

  VSdetach(vdata_id);

  //-------------------------------------------------------------------------
  // End vdata API
  //-------------------------------------------------------------------------

  Vend(Fid);

}
