/*===========================================================================
=                                                                           =
=                               FieldMap                                    =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                                    Shared

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: FieldMap

=============================================================================
!END=======================================================================*/

#include "field_map.h"		// Definition of class.
#include <cstdarg>  		// Definitiion of va_list

/////////////////////////////////////////////////////////////////////////////
/// Set field names for a given grid number.  The number of "Fieldname"
/// arguments should be equal to Number_field.
/// Type is set to "float" with fill value -9999 for all fields.
/////////////////////////////////////////////////////////////////////////////

void 
MSPI::Shared::FieldMap::init_grid(
  int Grid_number,
  const char *Grid_name,
  int Number_field, 
  ...   // variable argument list of type (char *)
)
{

//---------------------------------------------------------------------------
// Initialize variable argument list parse
//---------------------------------------------------------------------------

  va_list vl;
  va_start(vl,Number_field);

//---------------------------------------------------------------------------
// Get field names from variable argument list.
//---------------------------------------------------------------------------

  std::vector<std::string> field_list(Number_field);
  for (int ifield = 0 ; ifield < Number_field ; ++ifield) {
    field_list[ifield] = std::string(va_arg(vl,char *));
  }

//---------------------------------------------------------------------------
// End variable argument list parser.
//---------------------------------------------------------------------------

  va_end(vl);

//---------------------------------------------------------------------------
// Initialize private attributes.
//---------------------------------------------------------------------------

  init_grid(Grid_number, 
	    Grid_name, 
	    field_list, 
	    std::vector<std::string>(Number_field, "float"),
	    std::vector<boost::any>(Number_field, -9999.0F));
}

/////////////////////////////////////////////////////////////////////////////
/// Set field names for a given grid number.
/// Type is set to "float" with fill value -9999 for all fields.
/////////////////////////////////////////////////////////////////////////////

void 
MSPI::Shared::FieldMap::init_grid(
  int Grid_number,
  const std::string& Grid_name, 
  const std::vector<std::string>& Field_names
)
{
  size_t number_field = Field_names.size();
  init_grid(Grid_number, 
	    Grid_name, 
	    Field_names, 
	    std::vector<std::string>(number_field, "float"),
	    std::vector<boost::any>(number_field, -9999.0F));
}

/////////////////////////////////////////////////////////////////////////////
/// Set field names for a given grid number.
/////////////////////////////////////////////////////////////////////////////

void 
MSPI::Shared::FieldMap::init_grid(
  int Grid_number,
  const std::string& Grid_name, 
  const std::vector<std::string>& Field_names,
  const std::vector<std::string>& Type,
  const std::vector<boost::any>& Fill
)
{
  //------------------------------------------------------------------------
  // Get the number of fields.
  //------------------------------------------------------------------------

  size_t number_field = Field_names.size();

  //------------------------------------------------------------------------
  // Argument check: Type.size() == number_field
  //------------------------------------------------------------------------

  if (Type.size() != number_field) {
    throw MSPI_EXCEPTIONm("Type.size() != number_field");
  }

  //------------------------------------------------------------------------
  // Argument check: Fill.size() == number_field
  //------------------------------------------------------------------------

  if (Fill.size() != number_field) {
    throw MSPI_EXCEPTIONm("Fill.size() != number_field");
  }

  //------------------------------------------------------------------------
  // Initialize private attributes.
  //------------------------------------------------------------------------

  grid_index_[Grid_number] = grid_numbers_.size();
  grid_name_to_index_[Grid_name] = grid_numbers_.size();

  grid_name_.push_back(Grid_name);
  fields_.push_back(Field_names);
  type_.push_back(Type);
  fill_.push_back(Fill);

  grid_numbers_.push_back(Grid_number);

  std::map<std::string,int> field_index;
  for (size_t ifield = 0 ; ifield < number_field ; ifield++) {
    field_index[Field_names[ifield]] = ifield;
  }
  field_index_.push_back(field_index);

}
