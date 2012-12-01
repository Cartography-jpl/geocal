// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "statistic.h"
%}

%geocal_shared_ptr(Statistic);

namespace GeoCal {
class Statistic {
public:
  Statistic();
  void add(double x);
  Statistic& operator+=(double x);
  %python_attribute(min, double)
  %python_attribute(max, double)
  %python_attribute(mean, double)
  %python_attribute(sigma, double)
  %python_attribute(mean_abs, double)
  %python_attribute(min_index, int)
  %python_attribute(max_index, int)
  %python_attribute(count, int)
  std::string print_to_string() const;
};
}
