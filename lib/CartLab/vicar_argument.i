// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "vicar_argument.h"
%}
%base_import(generic_object)

%include "argcargv.i"

%geocal_shared_ptr(GeoCal::VicarArgument);
namespace GeoCal {
class VicarArgument: public GenericObject {
public:
  VicarArgument(int ARGC, char** ARGV, bool copy_primary_input_label = false);
  void write_out(const std::string& Keyword, int Val);
  void write_out(const std::string& Keyword, double Val);
  void write_out(const std::string& Keyword, const std::string& Val,
		 int Max_lin = 250);
  %extend {
    static std::string type_string(const std::string& Keyword)
    { std::string res1("b"); int res2;
      GeoCal::VicarArgument::type(Keyword, res1, res2);
      return res1;
    }
    static int type_count(const std::string& Keyword)
    { std::string res1("b"); int res2;
      GeoCal::VicarArgument::type(Keyword, res1, res2);
      return res2;
    }

     static std::vector<std::string> arg_str(const std::string& Keyword)
     { return GeoCal::VicarArgument::arg<std::vector<std::string> >(Keyword); }
     static std::vector<int> arg_int(const std::string& Keyword)
     { return GeoCal::VicarArgument::arg<std::vector<int> >(Keyword); }
     static std::vector<double> arg_real(const std::string& Keyword)
     { return GeoCal::VicarArgument::arg<std::vector<double> >(Keyword); }
  }
  std::string print_to_string() const;
#ifdef SWIGPYTHON
  // Provide a interface like t["key"][0] for python
%pythoncode %{
def __getitem__(self, key):
  k = str(key).upper()
  t = self.type_string(k)
  if(t == "STRING"):
    r = self.arg_str(k)
  elif(t == "REAL"):
    r = self.arg_real(k)
  elif(t == "INT"):
    r = self.arg_int(k)
  if(self.type_count(k) == 1):
    r = r[0]
  return r

def __setitem__(self, key, val):
  k = str(key).upper()
  t = self.type_string(k)
  if(t == "STRING"):
    self.write_out(k, str(val))
  elif(t == "REAL"):
    self.write_out(k, float(val))
  elif(t == "INT"):
    self.write_out(k, int(val))

%}
#endif
};
}


