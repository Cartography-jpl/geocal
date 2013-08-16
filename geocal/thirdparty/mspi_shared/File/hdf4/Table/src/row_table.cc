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
#include <sstream>  		// Definition of std::ostringstream

#define MAX_FIELDNAME_SIZE 1024 // Size of buffer to allocate for each field name.
                                // NOTE: The HDF5 library version 1.6.7 overruns 
                                // an internal buffer when the size of a field
                                // name exceeds 255 characters.
                                // HDF Help Desk has been notified of the problem.

//***************************************************************************
// Default constructor
//***************************************************************************

MSPI::Shared::HDF4::RowTable::RowTable()
  : fid_(FAIL),
    table_name_("Row Table")
{
}

//***************************************************************************
// Constructor definition.
//***************************************************************************

MSPI::Shared::HDF4::RowTable::RowTable(int32 Fid)
  : fid_(Fid),
    table_name_("Row Table"),
    is_empty_(false)
{
  intn status; 		// Status code returned by HDF calls

//---------------------------------------------------------------------------
// Initialize table parameters.
//---------------------------------------------------------------------------

  field_name_[0] = "Row Number";
  field_name_[1] = "Wavelength";
  field_name_[2] = "Polarization Angle";
  field_name_[3] = "Swath name";

  std::ostringstream list;
  for (int i = 0 ; i < number_field_ ; i++) {
    list << field_name_[i];
    if (i < number_field_-1) {
      list << ",";
    }
  }
  field_name_list_ = list.str();

  type_size_ = sizeof(RowTableRecord);
  field_offset_[0] = offsetof(RowTableRecord, row_number);
  field_offset_[1] = offsetof(RowTableRecord, wavelength);
  field_offset_[2] = offsetof(RowTableRecord, polarization_angle);
  field_offset_[3] = offsetof(RowTableRecord, swath_name);

  field_type_[0] = DFNT_INT32;
  field_type_[1] = DFNT_FLOAT32;
  field_type_[2] = DFNT_FLOAT32;
  field_type_[3] = DFNT_CHAR8;

  field_order_[0] = 1;
  field_order_[1] = 1;
  field_order_[2] = 1;
  field_order_[3] = swath_name_maxlen_;

  RowTableRecord record;
  field_size_[0] = sizeof(record.row_number);
  field_size_[1] = sizeof(record.wavelength);
  field_size_[2] = sizeof(record.polarization_angle);
  field_size_[3] = sizeof(record.swath_name);

//---------------------------------------------------------------------------
// Start vdata API
//---------------------------------------------------------------------------
  
  status = Vstart(fid_);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with Vstart");
  }

//---------------------------------------------------------------------------
// If the table does not exist, create a new table
// Note that we have to write a dummy record into the vdata because
// HDF4 won't allow an empty vdata.  An additional attribute "IsEmpty" 
// is used to distinguish an empty vdata from a vdata with one record.
//---------------------------------------------------------------------------

  int32 vdata_ref;
  vdata_ref = VSfind(fid_, table_name_.c_str());

  int32 vdata_id = FAIL;

  if (vdata_ref == 0) {

    vdata_id = VSattach(fid_, -1, "w");
    if (vdata_id == FAIL) {
      throw MSPI_EXCEPTIONm("Trouble with VSattach");
    }

    for (int i = 0 ; i < number_field_ ; i++) {
      status = VSfdefine(vdata_id, field_name_[i], 
			field_type_[i], field_order_[i]);
      if (status) {
	throw MSPI_EXCEPTIONm("Trouble with VSfdefine");
      }
    }

    status = VSsetname(vdata_id, table_name_.c_str());
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with VSsetname(read)");
    }

    for (int i = 0 ; i < number_field_ ; i++) {
      status = VSfdefine(vdata_id, field_name_[i], 
			 field_type_[i], field_order_[i]);
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

    // Set IsEmpty attribute.
    unsigned char flag = 1;
    status = VSsetattr(vdata_id, _HDF_VDATA, "IsEmpty", DFNT_UINT8, 1, &flag);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with VSsetattr");
    }

//---------------------------------------------------------------------------
// Otherwise, open existing table.
//---------------------------------------------------------------------------

  } else {

    vdata_id = VSattach(fid_, vdata_ref, "r");
    if (vdata_id == FAIL) {
      throw MSPI_EXCEPTIONm("Trouble with VSattach(readonly)");
    }
  }

//---------------------------------------------------------------------------
// Read IsEmpty attribute
//---------------------------------------------------------------------------

  {
    intn attr_id = VSfindattr(vdata_id, _HDF_VDATA, "IsEmpty");
    if (attr_id == FAIL) {
	throw MSPI_EXCEPTIONm("Trouble with VSfindattr(IsEmpty)");
    }

    int32 datatype;
    int32 count;
    intn status = VSattrinfo(vdata_id, _HDF_VDATA, attr_id, NULL, 
			     &datatype, &count, NULL);
    if (status == FAIL) {
	throw MSPI_EXCEPTIONm("Trouble with VSfindattr(IsEmpty)");
    }

    if (datatype != DFNT_UINT8) {
	throw MSPI_EXCEPTIONm("Unexpected datatype for IsEmpty attribute");
    }
    if (count != 1) {
	throw MSPI_EXCEPTIONm("Unexpected count for IsEmpty attribute");
    }

    unsigned char flag;
    status = VSgetattr(vdata_id, _HDF_VDATA, attr_id, &flag);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with VSsetattr");
    }

    if (flag) {
      is_empty_ = true;
    }
  }

//---------------------------------------------------------------------------
// Determine the number of records.
//---------------------------------------------------------------------------

  int32 number_record = VSelts(vdata_id);
  if (number_record == FAIL) {
	throw MSPI_EXCEPTIONm("Trouble with VSelts");
  }

  if (is_empty_ && number_record > 1) {
    throw MSPI_EXCEPTIONm("Unexpected condition: (IsEmpty==true) and (VSelts > 1)");
  }
  
  if (is_empty_) {
    number_record = 0;
  }
      
//---------------------------------------------------------------------------
// For each record in the table...
//---------------------------------------------------------------------------

  for (int irecord = 0 ; irecord < number_record ; irecord++) {

//---------------------------------------------------------------------------
// Read this record.
//---------------------------------------------------------------------------

    uint8 buffer[1000];
    
    int32 seekpos = irecord;
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

    RowTableRecord record;
    VOIDP record_ptrs[number_field_];
    record_ptrs[0] = &(record.row_number);
    record_ptrs[1] = &(record.wavelength);
    record_ptrs[2] = &(record.polarization_angle);
    record_ptrs[3] = &(record.swath_name[0]);

    status = VSfpack(vdata_id, _HDF_VSUNPACK, NULL, buffer, 1000, 1, NULL, 
		     record_ptrs);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with VSfpack");
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

//---------------------------------------------------------------------------
// Detach from vdata.
//---------------------------------------------------------------------------

  VSdetach(vdata_id);

//---------------------------------------------------------------------------
// End vdata API
//---------------------------------------------------------------------------

  Vend(fid_);

}

//***************************************************************************
// Destructor definition.
//***************************************************************************

MSPI::Shared::HDF4::RowTable::~RowTable()
{
}


//***************************************************************************
// Public method.
//***************************************************************************

void 
MSPI::Shared::HDF4::RowTable::add(
  int Row_number, 
  float Wavelength, 
  float Polarization_angle, 
  const std::string& Swath_name
)
{
  intn status; 		// Status code returned by HDF calls

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
  if (is_empty_) {
    number_record = 0;
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

  RowTableRecord record;
  record.row_number = Row_number;
  record.wavelength = Wavelength;
  record.polarization_angle = Polarization_angle;
  strncpy(record.swath_name, Swath_name.c_str(), swath_name_maxlen_);

  uint8 buffer[1000];

  VOIDP record_ptrs[number_field_];
  record_ptrs[0] = &(record.row_number);
  record_ptrs[1] = &(record.wavelength);
  record_ptrs[2] = &(record.polarization_angle);
  record_ptrs[3] = &(record.swath_name[0]);
  
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
// Clear IsEmpty attribute, if it isn't already cleared.
//---------------------------------------------------------------------------

  if (is_empty_) {
    unsigned char flag = 0;
    status = VSsetattr(vdata_id, _HDF_VDATA, "IsEmpty", DFNT_UINT8, 1, &flag);
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with VSsetattr");
    }
    is_empty_ = false;
  }

//---------------------------------------------------------------------------
// Add new record to table in memory.
//---------------------------------------------------------------------------

  row_set_.insert(Row_number);
  table_[Row_number] = record;

//---------------------------------------------------------------------------
// Detach from vdata.
//---------------------------------------------------------------------------

  VSdetach(vdata_id);

//---------------------------------------------------------------------------
// End vdata API
//---------------------------------------------------------------------------

  Vend(fid_);
}
