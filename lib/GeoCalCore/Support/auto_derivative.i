// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "std_vector.i"
%include "common.i"

%{
#include "auto_derivative.h"
namespace GeoCal {
}

%}
%base_import(generic_object)
namespace GeoCal {
template<class T> class AutoDerivative;
template<class T> class AutoDerivativeRef : public GenericObject
{
public:
  AutoDerivativeRef(T& V, blitz::Array<T,1>& FORCE_COPY);
  %python_attribute(value, T)
  %python_attribute(gradient, blitz::Array<T, 1>)
  std::string print_to_string() const;
};

template<class T> class AutoDerivative : public GenericObject
{
public:
  typedef T value_type;
  AutoDerivative();
  AutoDerivative(const T& Val, const blitz::Array<T, 1>& FORCE_COPY);
  AutoDerivative(const T& Val, int i_th, int nvars);
  AutoDerivative(const T& Val);
  AutoDerivative(const AutoDerivative<T>& D);
  AutoDerivative(const AutoDerivativeRef<T>& V);
  %python_attribute(number_variable, int)
  %python_attribute(is_constant, bool)
  bool operator<(const AutoDerivative<T>& V) const;
  bool operator<(const T& V) const;
  bool operator>(const AutoDerivative<T>& V) const;
  bool operator>(const T& V) const;
  bool operator==(const AutoDerivative<T>& V) const;
  bool operator==(const T& V) const;
  AutoDerivative<T> operator+=(const AutoDerivative<T>& V);
  AutoDerivative<T> operator+=(const T& V);
  AutoDerivative<T> operator-=(const AutoDerivative<T>& V);
  AutoDerivative<T> operator-=(const T& V);
  AutoDerivative<T> operator*=(const AutoDerivative<T>& V);
  AutoDerivative<T> operator*=(const T& V);
  AutoDerivative<T> operator/=(const AutoDerivative<T>& V);
  AutoDerivative<T> operator/=(const T& V);
  std::string print_to_string() const;
  %extend {
    T _value() const { return $self->value();}
    void _value_set(T V) { $self->value() = V;}
    blitz::Array<T, 1> _gradient() const { return $self->gradient();}
    void _gradient_set(blitz::Array<T, 1>& V) { $self->gradient().reference(V.copy());}
    AutoDerivative<T> __add__(const AutoDerivative<T>& Y) 
    { return *$self + Y; }
    AutoDerivative<T> __add__(const T& Y) 
    { return *$self + Y; }
    AutoDerivative<T> __radd__(const T& X) 
    { return X + *$self;}
    AutoDerivative<T> __sub__(const AutoDerivative<T>& Y) 
    { return *$self - Y; }
    AutoDerivative<T> __sub__(const T& Y) 
    { return *$self - Y; }
    AutoDerivative<T> __rsub__(const T& X) 
    { return X - *$self;}
    AutoDerivative<T> __mul__(const AutoDerivative<T>& Y) 
    { return *$self * Y; }
    AutoDerivative<T> __mul__(const T& Y) 
    { return *$self * Y; }
    AutoDerivative<T> __rmul__(const T& X) 
    { return X * *$self;}
    AutoDerivative<T> __div__(const AutoDerivative<T>& Y) 
    { return *$self / Y; }
    AutoDerivative<T> __div__(const T& Y) 
    { return *$self / Y; }
    AutoDerivative<T> __rdiv__(const T& X) 
    { return X / *$self;}
    AutoDerivative<T> __pow__(const T& X) 
    { return std::pow(*$self,  X);}
    AutoDerivative<T> __rpow__(const T& X) 
    { return std::pow(X, *$self);}
  }
  %pythoncode {
@property
def value(self):
  return self._value()

@value.setter
def value(self, val):
  self._value_set(val)

@property
def gradient(self):
  return self._gradient()

@gradient.setter
def gradient(self,val):
    self._gradient_set(val)
  }

};
}

%geocal_shared_ptr(GeoCal::AutoDerivative<double>)
%geocal_shared_ptr(GeoCal::AutoDerivativeRef<double>)
%template(AutoDerivativeDouble) GeoCal::AutoDerivative<double>;
%template(AutoDerivativeRefDouble) GeoCal::AutoDerivativeRef<double>;

namespace std {
GeoCal::AutoDerivative<double> sqrt(const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> log(const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> log10(const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> exp(const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> sin(const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> asin(const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> cos(const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> acos(const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> tan(const GeoCal::AutoDerivative<double>& x);
GeoCal::AutoDerivative<double> atan(const GeoCal::AutoDerivative<double>& x);
}

%template(vector_auto_derivative) std::vector<GeoCal::AutoDerivative<double> >;
%array_template(BlitzArray_AutoDerivativeDouble_1, GeoCal::AutoDerivative<double>, 1);
%boost_array_template(Array_AutoDerivativeDouble_3, GeoCal::AutoDerivative<double>, 3)




