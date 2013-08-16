/*===========================================================================
=                                                                           =
=                              ConfigTable                                  =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory

             Copyright 2012, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

=============================================================================
!END=======================================================================*/

#include "config_table.h"	// Definition of class.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <sstream> 		// Definition of std::ostringstream
#include <set> 			// Definition of std::set

/////////////////////////////////////////////////////////////////////////////
/// Constructor
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::ConfigTable::ConfigTable(
  const ConfigFile& Config,
  const std::string& Table_name
) 
{
  //------------------------------------------------------------------------
  // Get table size.
  //------------------------------------------------------------------------

  {  
    std::ostringstream var;
    var << Table_name << ".size";
    std::vector<int> size = Config.get<int>(var.str(),2);
    number_row_ = size[0];
    number_column_ = size[1];
  }

  //------------------------------------------------------------------------
  // Get column names.
  //------------------------------------------------------------------------

  {  
    std::ostringstream var;
    var << Table_name << ".columns";
    column_name_ = Config.get<std::string>(var.str(),number_column_);
  }

  //------------------------------------------------------------------------
  // Allocate column vectors.
  // Check for duplicate column names.
  //------------------------------------------------------------------------

  std::set<std::string> uniq;
  for (int icol = 0 ; icol < number_column_; icol++) {
    const std::string& name = column_name_[icol];
    if (!uniq.insert(name).second) {
      throw MSPI_EXCEPTIONm("Duplicate column name "+name+" in table "+Table_name);
    }

    value_[name].resize(number_row_);
  }

  //------------------------------------------------------------------------
  // Get column values.
  //------------------------------------------------------------------------

  {
    std::ostringstream var;
    var << Table_name << ".table";
    std::vector<std::string> value =
      Config.get<std::string>(var.str(), number_column_ * number_row_);

    for (int icol = 0 ; icol < number_column_; icol++) {
      const std::string& name = column_name_[icol];
      for (int irow = 0 ; irow < number_row_; irow++) {
	
	value_[name][irow] = value[irow * number_column_ + icol];
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// Private method to retrieve reference to vector associated with the given
// name.  Throws an exception if the name is not defined.
/////////////////////////////////////////////////////////////////////////////

const std::vector<std::string>&
MSPI::Shared::ConfigTable::get(const std::string& Column_name) const
{
  std::map<std::string, std::vector<std::string> >::const_iterator f = value_.find(Column_name);
  if (f == value_.end()) {
    throw MSPI_EXCEPTIONm("Configuration parameter not found: '"+Column_name+"'");
  }
  return f->second;
}

/////////////////////////////////////////////////////////////////////////////
// Private function to convert an ascii string to a boolean.
// Throw an exception if the ascii string does not contain a recognizable
// boolean value.
/////////////////////////////////////////////////////////////////////////////

bool 
MSPI::Shared::ConfigTable::atobool(const std::string& String) const
{
  std::string copy = String;
  for (size_t i = 0 ; i < copy.size() ; i++) {
    copy[i] = tolower(copy[i]);
  }
  if (copy == "t" ||
      copy == "true" ||
      copy == ".true.") {
    return true;
  } else if (copy == "f" ||
	     copy == "false" ||
	     copy == ".false.") {
    return false;
  } else {
    throw MSPI_EXCEPTIONm("Boolean value not recognized for string: '"+String+"'");
  }
}

