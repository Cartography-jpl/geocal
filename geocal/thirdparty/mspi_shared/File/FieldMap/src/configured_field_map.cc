//===========================================================================
//                                                                          
//                              ConfiguredFieldMap                          
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#include "configured_field_map.h" // Definition of class.
#include "MSPI-Shared/Config/src/config_file.h"
				// Definition of ConfigFile
#include <sstream>  		// Definition of std::ostringstream
#include <string> 		// Definition of std::string
#include "MSPI-Shared/Config/src/config_table.h"
				// Definition of ConfigTable

/////////////////////////////////////////////////////////////////////////////
/// Construct field map using external configuration file.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::ConfiguredFieldMap::ConfiguredFieldMap(
  const std::string& Filename
)
{

//---------------------------------------------------------------------------
// Read configuration file.
//---------------------------------------------------------------------------

  ConfigFile config(Filename);

//---------------------------------------------------------------------------
// Get granule id of input file.
//---------------------------------------------------------------------------
  
  granule_id_ = config.get<std::string>("granule_id");

//---------------------------------------------------------------------------
// Read grid table.
//---------------------------------------------------------------------------

  ConfigTable grids(config, "grids");

//---------------------------------------------------------------------------
// Get the grid numbers and number of fields in each grid.
//---------------------------------------------------------------------------

  int number_grid = grids.number_row();
  std::vector<int> grid_number = grids.get<int>("grid_number",number_grid);
  std::vector<std::string> 
    grid_names = grids.get<std::string>("grid_name",number_grid);

//---------------------------------------------------------------------------
// Read field names, data types, and fill values for each grid.
//---------------------------------------------------------------------------

  for (int igrid = 0 ; igrid < number_grid ; ++igrid) {

    ConfigTable fields(config, grid_names[igrid]);

    int number_field = fields.number_row();

    std::vector<std::string> 
      field_names = fields.get<std::string>("field_name", number_field);

    std::vector<std::string> 
      types = fields.get<std::string>("type",number_field);
    
    std::vector<boost::any> fill_values(number_field);

    for (int ifield = 0 ; ifield < number_field ; ifield++) {
      if (types[ifield] == "int") {
	fill_values[ifield] = fields.get_cell<int>("fill", ifield);
      } else if (types[ifield] == "float") {
	fill_values[ifield] = fields.get_cell<float>("fill", ifield);
      } else if (types[ifield] == "double") {
	fill_values[ifield] = fields.get_cell<double>("fill", ifield);
      } else if (types[ifield] == "uint16") {
	fill_values[ifield] = fields.get_cell<unsigned short>("fill", ifield);
      } else {
	throw MSPI_EXCEPTIONm("Unsupported field type: "+types[ifield]);
      }
    }

    init_grid(grid_number[igrid], 
	      grid_names[igrid], 
	      field_names,
	      types,
	      fill_values);
  }
}
