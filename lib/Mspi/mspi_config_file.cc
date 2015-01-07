#include "mspi_config_file.h"
#include "ifstream_cs.h"
#include <sstream>
#include <set>
#include <boost/foreach.hpp>

using namespace GeoCal;

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
  std::stringstream buf;
  buf << in.rdbuf();

  // Now break up into keyword/value pairs
  std::string s = buf.str();
  boost::sregex_iterator i(s.begin(), s.end(), 
			   boost::regex("^((?:[^:]|\\n)*):\\s*([[:word:]\\-\\.]+)\\s*$"));
  boost::sregex_iterator iend;
  for(; i != iend; ++i) {
    std::string key = (*i)[2];
    std::string val = (*i)[1];
    boost::trim(val);
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
