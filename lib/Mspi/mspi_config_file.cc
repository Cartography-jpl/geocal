#include "mspi_config_file.h"
#include "ifstream_cs.h"
#include "geocal_exception.h"
#include <sstream>
#include <iostream>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Read the given configuration file. See the class comments for the
/// details of the format of this string.
//-----------------------------------------------------------------------

MspiConfigFile::MspiConfigFile(const std::string& Fname)
  : fname(Fname)
{
  // Read in the whole file. We could do this line by line, but these
  // files tend to be shorter and it is easier just to read
  // everything. This class strips off all the comments, so we don't
  // need to worry about them.

  IfstreamCs in(Fname);
  std::stringstream buf;
  buf << in.rdbuf();

  // Now break up into keyword/value pairs
  boost::regex rex("\\`((?:[^:]|\\n)*):\\s*(\\w+)\\s*$");
  std::string s = buf.str();
  boost::smatch m;
  while(boost::regex_search(s, m, rex)) {
    std::string key = m[2];
    std::string val = m[1];
    boost::trim(val);
    // Check to see if this is a duplicate. By convention, this is
    // treated as an error.
    if(key_to_value.count(key) != 0) {
      Exception e;
      e << "Duplicate key '" << key << "' found in file '"
	<< Fname << "'";
      throw e;
    }
    key_to_value[key] = val;
    s = s.substr(m[0].str().size());
  }
}

//-----------------------------------------------------------------------
/// Find the given keyword, and return string associated with it.
//-----------------------------------------------------------------------

const std::string& MspiConfigFile::value_string
(const std::string& Keyword) const
{
  if(key_to_value.count(Keyword) == 0) {
    Exception e;
    e << "The keyword '" << Keyword << "' is not found in the file '"
      << fname << "\n";
  }
  return key_to_value.find(Keyword)->second;
}

void MspiConfigFile::print(std::ostream& Os) const
{
  Os << "MspiConfigFile:\n"
     << "  File name: " << file_name() << "\n";
}
