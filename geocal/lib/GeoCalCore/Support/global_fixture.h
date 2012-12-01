#ifndef GLOBAL_FIXTURE_H
#define GLOBAL_FIXTURE_H
#include <string>

namespace GeoCal {
/****************************************************************//**
  This is a global fixture that is available to all unit tests.
*******************************************************************/
class GlobalFixture {
public:
  GlobalFixture();
  virtual ~GlobalFixture() { /* Nothing to do now */ }
  std::string test_data_dir() const;
  std::string shiva_test_data_dir() const 
  { return test_data_dir() + "shiva_test_case/"; }
  std::string stereo_test_data_dir() const 
  { return test_data_dir() + "Stereo/"; }
private:
  void set_default_value();
};
}
#endif
