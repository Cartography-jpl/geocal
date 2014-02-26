#include "hdf_file.h"
using namespace GeoCal;
using namespace H5;

//-----------------------------------------------------------------------
/// Open the given file with the given mode.
//-----------------------------------------------------------------------

HdfFile::HdfFile(const std::string& Fname, Mode M)
: fname(Fname)
{
  unsigned int flag;
  switch(M) {
  case READ:
    flag = H5F_ACC_RDONLY;
    break;
  case READ_WRITE:
    flag = H5F_ACC_RDWR;
    break;
  case CREATE:
    flag = H5F_ACC_TRUNC;
    break;
  default:
    throw Exception("This shouldn't happen");
  }
  H5::Exception::dontPrint();	// We'll report exceptions ourself,
				// don't have HDF library print
				// Warning messages.
  try {
    h.reset(new H5File(Fname, flag));
  } catch(const H5::Exception& e) {
    Exception en;
    en << "While trying to open file '" << Fname 
       << "' a HDF 5 Exception thrown:\n"
       << "  " << e.getDetailMsg();
    throw en;
  }
}
    
//-----------------------------------------------------------------------
/// Determine of object is a group or dataset.
//-----------------------------------------------------------------------

bool HdfFile::is_group(const std::string& Objname) const
{
  H5G_stat_t statbuf;
  h->getObjinfo(Objname, statbuf);
  return statbuf.type == H5G_GROUP;
}

//-----------------------------------------------------------------------
/// Determine if object is present.
//-----------------------------------------------------------------------

bool HdfFile::is_present(const std::string& Objname, 
			 const H5::CommonFG& Parent) const
{
  try {
    H5G_stat_t statbuf;
    Parent.getObjinfo(Objname, statbuf);
  } catch(const H5::Exception& E) {
    return false;
  }
  return true;
}

//-----------------------------------------------------------------------
/// Determine if attribute is present.
//-----------------------------------------------------------------------

bool HdfFile::has_attribute(const std::string& Aname) const
{
  try {
    open_attribute(Aname);
  } catch(const H5::Exception& E) {
    return false;
  }
  return true;
}

//-----------------------------------------------------------------------
/// Open an attribute attached to a Group. This should have a name
/// like "Dimensions/Aerosol/Size"
//-----------------------------------------------------------------------

H5::Attribute HdfFile::open_attribute(const std::string& Aname) const
{
  size_t i = Aname.find_last_of('/');
  if(i == std::string::npos) {
    GeoCal::Exception e;
    e << "The attribute name '" << Aname << "' is not a valid name";
    throw e;
  }
  std::string gname = Aname.substr(0, i);
  std::string aname = Aname.substr(i + 1);
  if(is_group(gname))
    return h->openGroup(gname).openAttribute(aname);
  else
    return h->openDataSet(gname).openAttribute(aname);
}

void HdfFile::print(std::ostream& Os) const
{
  Os << "HdfFile: \n" 
     << "  File name: " << file_name() << "\n";
}
