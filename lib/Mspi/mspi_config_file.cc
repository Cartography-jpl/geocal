#include "mspi_config_file.h"
#include "ifstream_cs.h"
#include <sstream>

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Read the given configuration file. See the class comments for the
/// details of the format of this string.
//-----------------------------------------------------------------------

MspiConfigFile::MspiConfigFile(const std::string& Fname)
  : fname(Fname)
{
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
			   boost::regex("^((?:[^:]|\\n)*):\\s*([[:word:]-]+)\\s*$"));
  boost::sregex_iterator iend;
  for(; i != iend; ++i) {
    std::string key = (*i)[2];
    std::string val = (*i)[1];
    boost::trim(val);
    // Check to see if this is a duplicate. By convention, this is
    // treated as an error.
    if(have_key(key)) {
      Exception e;
      e << "Duplicate key '" << key << "' found in file '"
	<< Fname << "'";
      throw e;
    }
    key_to_value[key] = val;
  }
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
  Os << "MspiConfigFile:\n"
     << "  File name: " << file_name() << "\n";
}
