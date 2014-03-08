#ifndef MSPI_CONFIG_FILE_H
#define MSPI_CONFIG_FILE_H
#include "printable.h"
#include <boost/lexical_cast.hpp>
#include <string>
#include <map>

namespace GeoCal {
/****************************************************************//**
  MSPI uses ASCII configuration files.

  The files can contain comments, which start with a "#" and go to
  the end of the line.

  Everything else should be in a form like:
  <value 1> <value 2> ... : <keyword name>

  It is perfectly ok for there to be multiple lines before the 
  variable name, so something like:

  <value 1> <value 2>
  <value 3> <value 4> : <keyword name>

  Anything at the end of the file is ignored.
*******************************************************************/

class MspiConfigFile: public Printable<MspiConfigFile>  {
public:
  explicit MspiConfigFile(const std::string& Fname);
  virtual ~MspiConfigFile() {}

//-----------------------------------------------------------------------
/// File name for MspiConfigFile.
//-----------------------------------------------------------------------

  const std::string& file_name() const {return fname;}

//-----------------------------------------------------------------------
/// Return the value for the given keyword, lexical_cast to the given
/// type.
//-----------------------------------------------------------------------
  template<class T> T value(const std::string& Keyword) const
  {
    return boost::lexical_cast<T>(value_string(Keyword));
  }
  virtual void print(std::ostream& Os) const;
private:
  const std::string& value_string(const std::string& Keyword) const;
  std::map<std::string, std::string> key_to_value;
  std::string fname;
};
}
#endif
