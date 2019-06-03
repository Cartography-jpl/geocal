#ifndef AUTO_DERIVATIVE_BEFORE_BLITZ_H
#define AUTO_DERIVATIVE_BEFORE_BLITZ_H
#include "geocal_exception.h"
#include "printable.h"
#include <cmath>
#include <boost/operators.hpp>
#include <boost/foreach.hpp>

// Because of the way blitz expands things, we need to declare these
// functions before including blitz.
namespace GeoCal {
  template<class T> class AutoDerivative;
}
namespace std {			// Math functions are in std:: namespace.
GeoCal::AutoDerivative<double> 
sqrt(const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> 
log(const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> 
log10(const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> 
exp(const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> 
sin(const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> 
asin(const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> 
cos(const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> 
acos(const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> 
tan(const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> 
atan(const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> 
atan2(const GeoCal::AutoDerivative<double>& y, 
      const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> 
pow(const GeoCal::AutoDerivative<double>& x, const double y);
GeoCal::AutoDerivative<double> 
pow(const double x, const GeoCal::AutoDerivative<double>& y);
}
namespace blitz {
GeoCal::AutoDerivative<double> 
pow2(const GeoCal::AutoDerivative<double>& x);
}
#endif
