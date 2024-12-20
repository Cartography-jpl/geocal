/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (https://www.swig.org).
 * Version 4.1.1
 *
 * Do not make changes to this file unless you know what you are doing - modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

#ifndef SWIG_orbit_WRAP_H_
#define SWIG_orbit_WRAP_H_

#include <map>
#include <string>


class SwigDirector_Orbit : public GeoCal::Orbit, public Swig::Director {

public:
    SwigDirector_Orbit(PyObject *self,GeoCal::Time Min_time=GeoCal::Time::min_valid_time,GeoCal::Time Max_time=GeoCal::Time::max_valid_time);
    virtual ~SwigDirector_Orbit();
    virtual void add_observer(GeoCal::Observer< GeoCal::Orbit > &Obs);
    virtual void remove_observer(GeoCal::Observer< GeoCal::Orbit > &Obs);
    virtual blitz::Array< double,1 > parameter() const;
    virtual void parameter(blitz::Array< double,1 > const &V);
    virtual GeoCal::ArrayAd< double,1 > parameter_with_derivative() const;
    virtual void parameter_with_derivative(GeoCal::ArrayAd< double,1 > const &V);
    virtual std::vector< std::string,std::allocator< std::string > > parameter_name() const;
    virtual blitz::Array< double,1 > parameter_subset() const;
    virtual void parameter_subset(blitz::Array< double,1 > const &V);
    virtual GeoCal::ArrayAd< double,1 > parameter_with_derivative_subset() const;
    virtual void parameter_with_derivative_subset(GeoCal::ArrayAd< double,1 > const &V);
    virtual std::vector< std::string,std::allocator< std::string > > parameter_name_subset() const;
    virtual blitz::Array< bool,1 > parameter_mask() const;
    virtual std::string desc() const;
    virtual GeoCal::CartesianInertialLookVector ci_look_vector(GeoCal::Time T,GeoCal::ScLookVector const &Sl) const;
    virtual GeoCal::CartesianInertialLookVectorWithDerivative ci_look_vector(GeoCal::TimeWithDerivative const &T,GeoCal::ScLookVectorWithDerivative const &Sl) const;
    virtual GeoCal::CartesianFixedLookVector cf_look_vector(GeoCal::Time T,GeoCal::ScLookVector const &Sl) const;
    virtual GeoCal::CartesianFixedLookVectorWithDerivative cf_look_vector(GeoCal::TimeWithDerivative const &T,GeoCal::ScLookVectorWithDerivative const &Sl) const;
    virtual GeoCal::ScLookVector sc_look_vector(GeoCal::Time T,GeoCal::CartesianInertialLookVector const &Ci) const;
    virtual GeoCal::ScLookVectorWithDerivative sc_look_vector(GeoCal::TimeWithDerivative const &T,GeoCal::CartesianInertialLookVectorWithDerivative const &Ci) const;
    virtual GeoCal::ScLookVector sc_look_vector(GeoCal::Time T,GeoCal::CartesianFixedLookVector const &Cf) const;
    virtual GeoCal::ScLookVectorWithDerivative sc_look_vector(GeoCal::TimeWithDerivative const &T,GeoCal::CartesianFixedLookVectorWithDerivative const &Cf) const;
    virtual GeoCal::ScLookVector sc_look_vector(GeoCal::Time T,GeoCal::CartesianFixed const &Pt,boost::shared_ptr< GeoCal::Refraction > const &Ref=boost::shared_ptr< GeoCal::Refraction >(),boost::shared_ptr< GeoCal::VelocityAberration > const &Vabb=boost::shared_ptr< GeoCal::VelocityAberration >()) const;
    virtual boost::shared_ptr< GeoCal::CartesianInertial > position_ci(GeoCal::Time T) const;
    virtual boost::shared_ptr< GeoCal::CartesianFixed > position_cf(GeoCal::Time T) const;
    virtual boost::shared_ptr< GeoCal::OrbitData > orbit_data(GeoCal::Time T) const;
    virtual boost::shared_ptr< GeoCal::OrbitData > orbit_data(GeoCal::TimeWithDerivative const &T) const;
    virtual std::string print_to_string() const;
    using GeoCal::Orbit::notify_update_do;

/* Internal director utilities */
public:
    bool swig_get_inner(const char *swig_protected_method_name) const {
      std::map<std::string, bool>::const_iterator iv = swig_inner.find(swig_protected_method_name);
      return (iv != swig_inner.end() ? iv->second : false);
    }
    void swig_set_inner(const char *swig_protected_method_name, bool swig_val) const {
      swig_inner[swig_protected_method_name] = swig_val;
    }
private:
    mutable std::map<std::string, bool> swig_inner;

#if defined(SWIG_PYTHON_DIRECTOR_VTABLE)
/* VTable implementation */
    PyObject *swig_get_method(size_t method_index, const char *method_name) const {
      PyObject *method = vtable[method_index];
      if (!method) {
        swig::SwigVar_PyObject name = SWIG_Python_str_FromChar(method_name);
        method = PyObject_GetAttr(swig_get_self(), name);
        if (!method) {
          std::string msg = "Method in class Orbit doesn't exist, undefined ";
          msg += method_name;
          Swig::DirectorMethodException::raise(msg.c_str());
        }
        vtable[method_index] = method;
      }
      return method;
    }
private:
    mutable swig::SwigVar_PyObject vtable[30];
#endif

};


#endif
