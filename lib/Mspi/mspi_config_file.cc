#include "mspi_config_file.h"
#include "ifstream_cs.h"
#include <sstream>
#include <set>
#include <boost/foreach.hpp>
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void MspiConfigFile::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(MspiConfigFile);
  ar & GEOCAL_NVP(key_to_value);
}

template<class Archive>
void MspiConfigTable::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(MspiConfigTable);
  ar & GEOCAL_NVP_(column_to_index) & GEOCAL_NVP(data);
}

GEOCAL_IMPLEMENT(MspiConfigFile);
GEOCAL_IMPLEMENT(MspiConfigTable);
#endif

//-----------------------------------------------------------------------
/// Read the given configuration file. See the class comments for the
/// details of the format of this string.
//-----------------------------------------------------------------------

MspiConfigFile::MspiConfigFile(const std::string& Fname)
{
  add_file(Fname);
}

//-----------------------------------------------------------------------
/// Add a file, replacing any keywords already in this class from
/// earlier files.
//-----------------------------------------------------------------------

void MspiConfigFile::add_file(const std::string& Fname)
{
  // We use this to look for duplicates.
  std::set<std::string> new_key;

  // Read in the whole file. We could do this line by line, but these
  // files tend to be short and it is easier just to read
  // everything. IfstreamCs strips off all the comments, so we don't
  // need to worry about them.

  IfstreamCs in(Fname);
  if(!in.good())
    throw Exception("Trouble reading file " + Fname);
  std::stringstream buf;
  buf << in.rdbuf();
  if(!in.good())
    throw Exception("Trouble reading file " + Fname);

  // Now break up into keyword/value pairs
  std::string s = buf.str();
  boost::sregex_iterator i(s.begin(), s.end(), 
			   boost::regex("^((?:\".*\")?(?:[^:]|\\n)*):\\s*([[:word:]\\-\\.]+)\\s*$"));
  boost::sregex_iterator iend;
  for(; i != iend; ++i) {
    std::string key = (*i)[2];
    std::string val = (*i)[1];
    boost::trim(val);
    // Strip of leading and trailing '"' if this is in the string.
    if(val[0] == '"' && val[val.length()-1] == '"')
      val = val.substr(1, val.length() - 2);
    // Check to see if this is a duplicate. By convention, this is
    // treated as an error.
    if(new_key.count(key) != 0) {
      Exception e;
      e << "Duplicate key '" << key << "' found in file '"
	<< Fname << "'";
      throw e;
    }
    new_key.insert(key);
    key_to_value[key] = val;
  }
}

//-----------------------------------------------------------------------
/// Create a MspiConfigTable for the given table name in the given file.
//-----------------------------------------------------------------------

MspiConfigTable::MspiConfigTable(const MspiConfigFile& Config, 
				 const std::string& Table_name)
: data(Config.value<blitz::Array<std::string, 2> >(Table_name + ".table"))
{
  std::vector<std::string> column_name = 
    Config.value<std::vector<std::string> >(Table_name + ".columns");
  int index = 0;
  BOOST_FOREACH(std::string cname, column_name)
    column_to_index_[cname] = index++;
}

//-----------------------------------------------------------------------
/// Find the given keyword, and return string associated with it.
//-----------------------------------------------------------------------

const std::string& MspiConfigFile::value_string
(const std::string& Keyword) const
{
  if(!have_key(Keyword))
    throw Exception("The keyword is not found in the file");
  return key_to_value.find(Keyword)->second;
}

void MspiConfigFile::print(std::ostream& Os) const
{
  Os << "MspiConfigFile";
}
