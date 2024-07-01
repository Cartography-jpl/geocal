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
};
}

// List of things "import *" will include
%python_export("ThreadPool")
