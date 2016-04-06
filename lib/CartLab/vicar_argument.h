#ifndef VICAR_ARGUMENT_H
#define VICAR_ARGUMENT_H
#include "geocal_exception.h"
#include <string>
#include <vector>
#include "printable.h"

namespace GeoCal {

/****************************************************************//**
  This provides access to arguments passed by VICAR.

  Note an important limitation of VICAR, you can only initialize the
  argument handling once. This means that if you try to create
  multiple VicarArgument objects, you'll trigger a TAE error when you
  try to create the second object.

  VICAR uses a somewhat unusual mechanism to pass arguments to a
  program called from the TAE. Rather than putting the arguments on
  the command line to appear in Argv, the arguments get passed down
  a pipe (usually file logical 4) in a compressed format. I'm not sure
  why exactly it does this, probably some historical artifact from
  when it was developed on VMS. In any case, in order to fit into
  VICAR we need to parse arguments the same way.

  This class handles the arguments. 
*******************************************************************/
class VicarArgument : public Printable<VicarArgument> {
public:
  VicarArgument(int Argc, char** Argv, bool copy_primary_input_label = false); 

  bool has_keyword(const std::string& Keyword);
  void type(const std::string& Keyword, std::string& Type, 
		   int& Count);
  void type(const std::string& Keyword, std::string& Type, 
		   int& Count, int& Maxlen);

//-----------------------------------------------------------------------
/// Stand alone version of argument parsing. This is just a typesafe
/// version of the Vicar RTL zvp call.
//-----------------------------------------------------------------------

  template<class T> T arg(const std::string& Keyword);
  void print(std::ostream& Os) const;
  void write_out(const std::string& Keyword, int Val);
  void write_out(const std::string& Keyword, double Val);
  void write_out(const std::string& Keyword, const std::string& Val, 
		 int Max_len = 250);

  int zvpw(char *name, void *value, int *count);
  int zvparmw(char *name, void *value, int *count, int *def,
		    int maxcnt, int length);
  int zvparmdw(char *name, void *value, int *count, int *def,
		     int maxcnt, int length);
};

template<> inline int VicarArgument::arg(const std::string& Keyword)
{
  std::string tp("b");		// Mac is flaky passing empty strings
				// around between shared
				// libraries. W/o this initialization
				// we crash with ruby.
  int count;
  type(Keyword, tp, count);
  if(count != 1)
    throw Exception("Count !=1 ");
  if(tp != "INT")
    throw Exception("Type '" + tp + "' is not INT");
  int res;
  int status = zvpw(const_cast<char*>(Keyword.c_str()), &res, &count);
  if(status != 1)
    throw Exception("Call to zvp failed");
  return res;
}

template<> inline std::string 
VicarArgument::arg(const std::string& Keyword)
{
  std::string tp("b");		// Mac is flaky passing empty strings
				// around between shared
				// libraries. W/o this initialization
				// we crash with ruby.
  int count, maxlen;
  type(Keyword, tp, count, maxlen);
  if(count != 1)
    throw Exception("Count !=1 ");
  if(tp != "STRING")
    throw Exception("Type '" + tp + "' is not STRING");
  std::vector<char> buf(maxlen + 1);
  int status = zvpw(const_cast<char*>(Keyword.c_str()), &buf[0], &count);
  if(status != 1)
    throw Exception("Call to zvp failed");
  return std::string(&buf[0]);
}

template<> inline double VicarArgument::arg(const std::string& Keyword)
{
  std::string tp("b");		// Mac is flaky passing empty strings
				// around between shared
				// libraries. W/o this initialization
				// we crash with ruby.
  int count;
  type(Keyword, tp, count);
  if(count != 1)
    throw Exception("Count !=1 ");
  if(tp != "REAL")
    throw Exception("Type '" + tp + "' is not REAL");
  double res;
  int def;
  int status = zvparmdw(const_cast<char*>(Keyword.c_str()), &res, &count, &def,
		       0, 0);
  if(status != 1)
    throw Exception("Call to zvparmd failed");
  return res;
}

template<> inline std::vector<int> 
VicarArgument::arg(const std::string& Keyword)
{
  std::string tp("b");		// Mac is flaky passing empty strings
				// around between shared
				// libraries. W/o this initialization
				// we crash with ruby.
  int count;
  type(Keyword, tp, count);
  if(tp != "INT")
    throw Exception("Type '" + tp + "' is not INT");
  std::vector<int> res(count);
  int status = zvpw(const_cast<char*>(Keyword.c_str()), &res[0], &count);
  if(status != 1)
    throw Exception("Call to zvp failed");
  return res;
}

template<> inline std::vector<double> 
VicarArgument::arg(const std::string& Keyword)
{
  std::string tp("b");		// Mac is flaky passing empty strings
				// around between shared
				// libraries. W/o this initialization
				// we crash with ruby.
  int count;
  type(Keyword, tp, count);
  if(tp != "REAL")
    throw Exception("Type '" + tp + "' is not REAL");
  std::vector<double> res(count);
  int def;
  int status = zvparmdw(const_cast<char*>(Keyword.c_str()), &res[0], &count, 
		       &def, 0, 0);
  if(status != 1)
    throw Exception("Call to zvparmd failed");
  return res;
}

template<> inline std::vector<std::string>
VicarArgument::arg(const std::string& Keyword)
{
  std::string tp("b");		// Mac is flaky passing empty strings
				// around between shared
				// libraries. W/o this initialization
				// we crash with ruby.
  int count, maxlen;
  type(Keyword, tp, count, maxlen);
  if(tp != "STRING")
    throw Exception("Type '" + tp + "' is not STRING");
  std::vector<char> buf((maxlen + 1) * count);
  int def;
  int status = zvparmw(const_cast<char*>(Keyword.c_str()), &buf[0], &count, 
		       &def, 0, maxlen + 1);
  if(status != 1)
    throw Exception("Call to zvparm failed");
  std::vector<std::string> res;
  for(int i = 0; i < count; ++i)
    res.push_back(std::string(&buf[i * (maxlen + 1)]));
  return res;
}

}
#endif
