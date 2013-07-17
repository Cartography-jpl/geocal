//===========================================================================
//                                                                          
//                              Hdf4Meta                                    
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//=============================================================================

#include "hdf4_meta.h" 		// Definition of class
#include <vector> 		// Definition of std::vector

/////////////////////////////////////////////////////////////////////////////
/// Write current metadata to an existing HDF4 file.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::Hdf4Meta::hdf4_export(
  int32 Group_id
) const
{
  for (const_it_type i = meta().begin() ; i != meta().end() ; ++i) {
    hdf4_export(Group_id, i->first, i->second);
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Read attributes from an HDF4 group.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::Meta
MSPI::Shared::Hdf4Meta::hdf4_import(
  int32 Group_id
)
{
  Hdf4Meta result;
  intn status;

  //------------------------------------------------------------------------
  // Get the number of attributes.
  //------------------------------------------------------------------------

  int32 number_attribute = Vnattrs(Group_id);
  if (number_attribute == -1) {
    throw MSPI_EXCEPTIONm("Trouble with Vnattrs");
  }

  //------------------------------------------------------------------------
  // Read each attribute.
  //------------------------------------------------------------------------

  for (int32 i = 0 ; i < number_attribute ; i++) {

    std::vector<char> attr_name(H4_MAX_NC_NAME+1,0);
    int32 type;
    int32 count;
    int32 size_bytes;
    status = Vattrinfo(Group_id, i, &attr_name[0], &type, &count, &size_bytes);
    if (status == -1) {
      throw MSPI_EXCEPTIONm("Trouble with Vattrinfo");
    }

    std::string name(&attr_name[0]);
    result.add_any(name, hdf4_import(Group_id, name));
  }

  return result;
}

/////////////////////////////////////////////////////////////////////////////
/// Write an attribute to an HDF4 file.  Overwrites existing attribute.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::Hdf4Meta::hdf4_export(
  int32 Group_id,
  const std::string& Name,
  const any_type& Value
)
{
  hdf4_type type = 
    boost::apply_visitor(hdf4_type_visitor(),Value);
  const void *p = boost::apply_visitor(const_pointer_visitor(), Value);

  intn status = Vsetattr(Group_id, Name.c_str(), type.id, type.count, p);
  if (status == -1) {
    throw MSPI_EXCEPTIONm("Trouble with Vsetattr");
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Read an attribute from an HDF4 file.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::Hdf4Meta::any_type
MSPI::Shared::Hdf4Meta::hdf4_import(
  int32 Group_id,
  const std::string& Name
)
{
  int32 attr_index = Vfindattr(Group_id, Name.c_str());
  if (attr_index == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with Vfindattr");
  }

  int32 data_type;
  int32 count;
  char attr_name[H4_MAX_NC_NAME];
  int32 size_bytes;
  
  intn status = Vattrinfo(Group_id, attr_index, attr_name, &data_type, &count,
			  &size_bytes);
  if (status == -1) {
    throw MSPI_EXCEPTIONm("Trouble with Vattrinfo");
  }

  any_type any = hdf4_any(hdf4_type(data_type, count));

  status = Vgetattr(Group_id, attr_index,
		    boost::apply_visitor(pointer_visitor(), any));
  if (status == FAIL) {
    throw MSPI_EXCEPTIONm("Trouble with Vgetattr");
  }

  return any;
}

