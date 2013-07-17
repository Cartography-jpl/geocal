#include "mspi_config_file.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

MspiConfigFile::MspiConfigFile(const std::string& Filename) 
  : filename_(Filename)
{
  value_ = parse(Filename);
}

//-----------------------------------------------------------------------
/// Add content of the given file, replacing any existing variables 
/// with new values.
//-----------------------------------------------------------------------

void MspiConfigFile::add_replace(const std::string& Filename) 
{
  typedef std::map<std::string, std::vector<std::string> > mtype;
  mtype new_values = parse(Filename);

  for (mtype::iterator i = new_values.begin() ; i != new_values.end() ; ++i) {
    std::pair<mtype::iterator, bool> result = value_.insert(*i);
    if (!result.second)
      result.first->second = i->second;
  }
}

//-----------------------------------------------------------------------
/// Return parsed content of the given file.
//-----------------------------------------------------------------------

std::map<std::string, std::vector<std::string> >
MspiConfigFile::parse(const std::string& Filename) 
{
  
  std::map<std::string, std::vector<std::string> > result;
  std::ifstream infile(Filename.c_str(), std::ifstream::in);
  if (!infile.good())
    throw Exception("Trouble opening input file: " +  Filename);

  //---------------------------------------------------------------------
  // Initialize parser
  //---------------------------------------------------------------------

  NameValueParser parser;

  //---------------------------------------------------------------------
  // For each line in the input file...
  //---------------------------------------------------------------------

  std::string line;
  int line_counter = 0;
  while (std::getline(infile,line)) {
    line_counter++;

    //---------------------------------------------------------------------
    // Strip comments from this line.
    // '#' character and everything after it is a comment.
    //---------------------------------------------------------------------

    boost::match_results<std::string::const_iterator> match1;
    const boost::regex re1("^([^#]+)");
    if (!boost::regex_search(line,match1,re1)) {
      continue;
    }
    line.assign(match1[1].first,match1[1].second);
    
    //---------------------------------------------------------------------
    // Convert the line to tokens.
    //---------------------------------------------------------------------

    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(" \t", "[]:");
    tokenizer tok(line,sep);

    //---------------------------------------------------------------------
    // For each token...(begin loop)
    //---------------------------------------------------------------------

    for(tokenizer::iterator i=tok.begin(); i!=tok.end();++i) {

      //---------------------------------------------------------------------
      // Submit this token to the token parser.
      //---------------------------------------------------------------------

      try {
	bool success = parser.add_token(*i);

	//---------------------------------------------------------------------
	// If parsing is done, then add this name value pair, and reset the 
	// token parser the next cycle.
	// Don't allow the same name to be defined more than once.
	// Require values to have a size of at least 1.
	// Resubmit this token after resetting if it was not added successfully.
	//---------------------------------------------------------------------

	if (parser.done()) {
	  std::vector<std::string> value = parser.value();

	  if (value.size() < 1) {
	    std::ostringstream msg;
	    throw MSPI_EXCEPTIONm("Unexpected result: parser.value().size() < 1");
	  }

	  if (result.find(parser.name()) != result.end()) {
	    std::ostringstream msg;
	    msg << "Duplicate config parameter name: " << parser.name();
	    throw MSPI_EXCEPTIONm(msg.str());
	  }

	  result[parser.name()] = value;
	  parser.reset();

	  if (!success) {
	    parser.add_token(*i);
	  }

	}

      //---------------------------------------------------------------------
      // Catch any parse errors, and report the config file line number where
      // the error occurred.
      //---------------------------------------------------------------------

      } catch(MSPI::Shared::Exception& e) {
	std::ostringstream message;
	message << "Caught exception, ["<<e.what() << "], at line " 
		<< line_counter << " of " << Filename << ", line='" << line << "'";
	throw MSPI_EXCEPTIONm(message.str());
      }

      //---------------------------------------------------------------------
      // End loop for each token on this line.
      //---------------------------------------------------------------------

    }

    //---------------------------------------------------------------------
    // End loop for each line in the input file.
    //---------------------------------------------------------------------

  }

  return result;
}
