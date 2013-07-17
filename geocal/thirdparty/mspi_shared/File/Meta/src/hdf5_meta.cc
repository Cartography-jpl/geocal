//===========================================================================
//                                                                          
//                              Hdf5Meta                                    
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//=============================================================================

#include "hdf5_meta.h" 		// Definition of class
#include <vector> 		// Definition of std::vector

/////////////////////////////////////////////////////////////////////////////
/// Write current metadata to an existing HDF5 group.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::Hdf5Meta::hdf5_export(
  hid_t Group_id
) const
{
  for (const_it_type i = meta().begin() ; i != meta().end() ; ++i) {
    hdf5_export(Group_id, i->first, i->second);
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Operator for retrieving list of attribute names.
/////////////////////////////////////////////////////////////////////////////

namespace {
  herr_t read_op(
    hid_t Group_id, 
    const char *Name, 
    const H5A_info_t* Info,
    void *List
  )
  {
    if (List != NULL) {
      static_cast<std::vector<std::string> *>(List)->push_back(Name);
    }
    return 0;
  }
}	       

/////////////////////////////////////////////////////////////////////////////
/// Read attributes from an HDF5 group.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::Meta
MSPI::Shared::Hdf5Meta::hdf5_import(
  hid_t Group_id
)
{
  Hdf5Meta result;

  //------------------------------------------------------------------------
  // Get the list of attribute names.
  //------------------------------------------------------------------------

  std::vector<std::string> attribute_names;

  hsize_t n = 0;
  herr_t status = H5Aiterate(Group_id,
			     H5_INDEX_CRT_ORDER,
			     H5_ITER_INC,
			     &n,
			     &read_op,
			     &attribute_names);
  if (status < 0) {
    std::ostringstream msg;
    msg << "Trouble with H5Aiterate: n=" << n;
    throw MSPI_EXCEPTIONm(msg.str());
  }

  //------------------------------------------------------------------------
  // Read each attribute.
  //------------------------------------------------------------------------

  for (size_t i = 0 ; i < attribute_names.size() ; i++) {
    const std::string& name = attribute_names[i];
    result.add_any(name, hdf5_import(Group_id, name));
  }

  return result;
}

/////////////////////////////////////////////////////////////////////////////
/// Write an attribute to an HDF5 file.  Overwrites existing attribute.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::Hdf5Meta::hdf5_export(
  hid_t Group_id,
  const std::string& Name,
  const any_type& Value
)
{
  
  if (bool_H5Aexists(Group_id, Name.c_str())) {
    herr_t status = H5Adelete(Group_id, Name.c_str());
    if (status) {
      throw MSPI_EXCEPTIONm("Trouble with H5Adelete");
    }
  }
  
  hid_t dataspace_id = H5Screate(H5S_SCALAR);
  if (dataspace_id < 0) {
    throw MSPI_EXCEPTIONm("Trouble with H5Screate");
  }

  H5::DataType type = boost::apply_visitor(hdf5_type_visitor(),Value);
    
  hid_t attribute_id = H5Acreate(Group_id, 
				 Name.c_str(),
				 type.getId(), dataspace_id, 
				 H5P_DEFAULT, H5P_DEFAULT);
  if (attribute_id < 0) {
    std::ostringstream msg;
    msg << "Trouble with H5Acreate: name = " << Name;
    throw MSPI_EXCEPTIONm(msg.str());
  }

  herr_t status = H5Awrite(attribute_id, type.getId(),
			   boost::apply_visitor(const_pointer_visitor(), Value));
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with H5Awrite");
  }

  status = H5Aclose(attribute_id);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with H5Aclose");
  }

  status = H5Sclose(dataspace_id);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with H5Sclose");
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Read an attribute from an HDF5 file.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::Hdf5Meta::any_type
MSPI::Shared::Hdf5Meta::hdf5_import(
  hid_t Group_id,
  const std::string& Name
)
{
  hid_t attribute_id = H5Aopen_name(Group_id, Name.c_str());
  if (attribute_id < 0) {
    throw MSPI_EXCEPTIONm("Trouble with H5Aopen_name");
  }

  hid_t type_id = H5Aget_type(attribute_id);
  if (type_id < 0) {
    throw MSPI_EXCEPTIONm("Trouble with H5Aget_type");
  }

  H5::DataType type(type_id);

  any_type any = hdf5_any(type);

  //  std::cerr << "<" << any.type().name() << "> " <<  Name << " " << hdf5_type_debug(type.getId()) << " (import)\n";

  herr_t status = H5Aread(attribute_id, type.getId(), 
			  boost::apply_visitor(pointer_visitor(), any));

  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with H5Aread");
  }

  status = H5Aclose(attribute_id);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with H5Aclose");
  }

  status = H5Tclose(type_id);
  if (status) {
    throw MSPI_EXCEPTIONm("Trouble with H5Tclose");
  }

  return any;
}

