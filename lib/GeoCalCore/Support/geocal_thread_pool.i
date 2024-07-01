// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "geocal_common.i"

%{
#include "geocal_thread_pool.h"
%}
%base_import(generic_object)

%geocal_shared_ptr(GeoCal::ThreadPool);

namespace GeoCal {
%nodefaultctor ThreadPool;
class ThreadPool : public GenericObject {
public:
  static const int default_number_thread = 10;
  static ThreadPool& thread_pool();
  %python_attribute_with_set(number_thread, int);
  // add_task is really not something we can expose to python, it is
  // very C++ specific.
  void wait_tasks_finish();
  virtual std::string print_to_string() const;
};
}

// List of things "import *" will include
%python_export("ThreadPool")
