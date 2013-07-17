//===========================================================================
//                                                                          
//                              BandTable                                   
//                                                                          
//===========================================================================
//
//             Copyright 2013, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#include "band_table.h"		// Definition of class.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <sstream>  		// Definition of std::ostringstream

/////////////////////////////////////////////////////////////////////////////
/// Default constructor
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::HDF4::BandTable::BandTable()
  : fid_(FAIL),
    table_name_("Band Table")
{
}

/////////////////////////////////////////////////////////////////////////////
/// Open band table in the given file.  Creates a new table if one doesn't 
/// already exist.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::HDF4::BandTable::BandTable(int32 Fid)
  : fid_(Fid),
    table_name_("Band Table")
{
  intn status; 		// Status code returned by HDF calls

//---------------------------------------------------------------------------
// Initialize table parameters.
//---------------------------------------------------------------------------

  const int number_field = 4;
  const char *field_name[number_field] = {"Band Number",
					  "Wavelength",
					  "Solar irradiance at 1 AU",
					  "Grid name"};

  std::ostringstream list;
  for (int i = 0 ; i < number_field ; i++) {
    list << field_name[i];
    if (i < number_field-1) {
      list << ",";
    }
  }
  field_name_list_ = list.str();

  type_size_ = sizeof(BandTableRecord);
  field_offset_[0] = offsetof(BandTableRecord, band_number);
  field_offset_[1] = offsetof(BandTableRecord, wavelength);
  field_offset_[2] = offsetof(BandTableRecord, e0);
  field_offset_[3] = offsetof(BandTableRecord, grid_name);

  field_type_[0] = DFNT_INT32;
  field_type_[1] = DFNT_FLOAT32;
  field_type_[2] = DFNT_FLOAT64;
  field_type_[3] = DFNT_CHAR8;

  int32 field_order[number_field] = {1, 1, 1, grid_name_maxlen_};

  BandTableRecord record;
  field_size_[0] = sizeof(record.band_number);
  field_size_[1] = sizeof(record.wavelength);
  field_size_[2] = sizeof(record.e0);
  field_size_[3] = sizeof(record.grid_name);

//---------------------------------------------------------------------------
// Start vdata API
//---------------------------------------------------------------------------
  
  status = Vstart(fid_);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with Vstart");
  }

//---------------------------------------------------------------------------
// If the table does not exist, create a new table
//---------------------------------------------------------------------------

  int32 vdata_ref;
  vdata_ref = VSfind(fid_, table_name_.c_str());

  int32 vdata_id = FAIL;

  if (vdata_ref == 0) {

    vdata_id = VSattach(fid_, -1, "w");
    if (vdata_id == FAIL) {
      throw MSPI_EXCEPTIONm("Trouble with VSattach");
    }

    for (int i = 0 ; i < number_field ; i++) {
      status = VSfdefine(vdata_id, field_name[i], 
			field_type_[i], field_order[i]);
      if (status) {
	throw MSPI_EXCEPTIONm("Trouble with VSfdefine");
      }
    }

    status = VSsetname(vdata_id, table_name_.c_str());
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with VSsetname(read)");
    }

    for (int i = 0 ; i < number_field ; i++) {
      status = VSfdefine(vdata_id, field_name[i], 
			 field_type_[i], field_order[i]);
      if (status) {
	throw MSPI_EXCEPTIONm("Trouble with VSfdefine");
      }
    }

    status = VSsetfields(vdata_id, field_name_list_.c_str());
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with VSsetfields");
    }

    // Write dummy record.
    uint8 buffer[1000];
    int32 count = VSwrite(vdata_id, buffer, 1, FULL_INTERLACE);
    if (count != 1) {
      throw MSPI_EXCEPTIONm("Trouble with VSwrite");
    }

//---------------------------------------------------------------------------
// Otherwise, open existing table, and get the number of records.
//---------------------------------------------------------------------------

  } else {

    vdata_id = VSattach(fid_, vdata_ref, "r");
    if (vdata_id == FAIL) {
      throw MSPI_EXCEPTIONm("Trouble with VSattach(readonly)");
    }


  }

//---------------------------------------------------------------------------
// Get the number of records.  Ignore the first record.
//---------------------------------------------------------------------------

  int32 number_record = VSelts(vdata_id);
  if (number_record == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with VSelts");
  }
  number_record--;
    
//---------------------------------------------------------------------------
// For each record in the table...
//---------------------------------------------------------------------------

  for (int irecord = 0 ; irecord < number_record ; irecord++) {

//---------------------------------------------------------------------------
// Read this record.
//---------------------------------------------------------------------------
    
    uint8 buffer[1000];
    
    int32 seekpos = irecord+1;
    int32 pos = VSseek(vdata_id, seekpos);
    if (pos != seekpos) {
      std::ostringstream message;
      message << "Trouble with VSseek(read): seekpos="<< seekpos 
	      << ", pos="<< pos<<"\n";
      throw MSPI_EXCEPTIONm(message.str());
    }

    status = VSsetfields(vdata_id, field_name_list_.c_str());
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with VSsetfields");
    }

    int32 count = VSread(vdata_id, buffer, 1, FULL_INTERLACE);
    if (count != 1) {
      throw MSPI_EXCEPTIONm("Trouble with VSread");
    }

    BandTableRecord record;
    VOIDP record_ptrs[4];
    record_ptrs[0] = &(record.band_number);
    record_ptrs[1] = &(record.wavelength);
    record_ptrs[2] = &(record.e0);
    record_ptrs[3] = &(record.grid_name[0]);

    status = VSfpack(vdata_id, _HDF_VSUNPACK, NULL, buffer, 1000, 1, NULL, 
		     record_ptrs);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with VSfpack");
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

//---------------------------------------------------------------------------
// Detach from vdata.
//---------------------------------------------------------------------------

  VSdetach(vdata_id);

//---------------------------------------------------------------------------
// End vdata API
//---------------------------------------------------------------------------

  Vend(fid_);
}

/////////////////////////////////////////////////////////////////////////////
/// Add a new band number to the table.  If a record already exists
/// for the given band number, throw an exception.
/////////////////////////////////////////////////////////////////////////////

void 
MSPI::Shared::HDF4::BandTable::add(
  int Band_number, 
  float Wavelength, 
  double E0,
  const std::string& Grid_name
)
{
  intn status; 		// Status code returned by HDF calls

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
// Start vdata API
//---------------------------------------------------------------------------
  
  status = Vstart(fid_);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with Vstart");
  }

//---------------------------------------------------------------------------
// Attach to vdata.
//---------------------------------------------------------------------------

  int32 vdata_ref;
  vdata_ref = VSfind(fid_, table_name_.c_str());
  if (vdata_ref == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with VSfind");
  }

  int32 vdata_id = VSattach(fid_, vdata_ref, "w");
  if (vdata_id == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with VSattach(write)");
  }

//---------------------------------------------------------------------------
// Seek to the end of the vdata.
//---------------------------------------------------------------------------

  int32 number_record = VSelts(vdata_id);
  if (number_record == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with VSelts");
  }

  int32 seekpos = number_record;
  int32 pos = VSseek(vdata_id, seekpos);
  if (pos != seekpos) {
    std::ostringstream message;
    message << "Trouble with VSseek(read): seekpos="<< seekpos 
	    << ", pos="<< pos<<"\n";
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

  uint8 buffer[1000];

  VOIDP record_ptrs[4];
  record_ptrs[0] = &(record.band_number);
  record_ptrs[1] = &(record.wavelength);
  record_ptrs[2] = &(record.e0);
  record_ptrs[3] = &(record.grid_name[0]);
  
  status = VSfpack(vdata_id, _HDF_VSPACK, NULL, buffer, 1000, 1, NULL, 
		   record_ptrs);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with VSfpack");
  }

  status = VSsetfields(vdata_id, field_name_list_.c_str());
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with VSsetfields(write)");
  }

  int32 count = VSwrite(vdata_id, buffer, 1, FULL_INTERLACE);
  if (count != 1) {
    throw MSPI_EXCEPTIONm("Trouble with VSwrite");
  }

//---------------------------------------------------------------------------
// Add new record to table in memory.
//---------------------------------------------------------------------------

  band_set_.insert(Band_number);
  table_[Band_number] = record;

//---------------------------------------------------------------------------
// Detach from vdata.
//---------------------------------------------------------------------------

  VSdetach(vdata_id);

//---------------------------------------------------------------------------
// End vdata API
//---------------------------------------------------------------------------

  Vend(fid_);
}
