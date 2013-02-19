/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 * 
 * This file is not intended to be easily readable and contains a number of 
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG 
 * interface file instead. 
 * ----------------------------------------------------------------------------- */

#ifndef SWIG_geocal_WRAP_H_
#define SWIG_geocal_WRAP_H_

#include <map>
#include <string>


class SwigDirector_ImageGroundConnection : public GeoCal::ImageGroundConnection, public Swig::Director {

public:
    SwigDirector_ImageGroundConnection(PyObject *self, boost::shared_ptr< GeoCal::Dem > const d, boost::shared_ptr< GeoCal::RasterImage > const &Img, std::string const &Title);
    SwigDirector_ImageGroundConnection(PyObject *self);
    virtual ~SwigDirector_ImageGroundConnection();
    virtual void cf_look_vector(GeoCal::ImageCoordinate const &Ic, GeoCal::CartesianFixedLookVector &OUTPUT2, boost::shared_ptr< GeoCal::CartesianFixed > &OUTPUT) const;
    virtual boost::shared_ptr< GeoCal::GroundCoordinate > ground_coordinate(GeoCal::ImageCoordinate const &Ic) const;
    virtual boost::shared_ptr< GeoCal::GroundCoordinate > ground_coordinate_dem(GeoCal::ImageCoordinate const &Ic, GeoCal::Dem const &D) const;
    virtual GeoCal::ImageCoordinate image_coordinate(GeoCal::GroundCoordinate const &Gc) const;
    virtual blitz::Array< double,2 > image_coordinate_jac_ecr(GeoCal::Ecr const &Gc) const;
    virtual blitz::Array< double,2 > image_coordinate_jac_parm(GeoCal::GroundCoordinate const &Gc) const;
    virtual int number_line() const;
    virtual int number_sample() const;
    virtual double resolution_meter(GeoCal::ImageCoordinate const &Ic) const;
    virtual double resolution_meter() const;


/* Internal Director utilities */
public:
    bool swig_get_inner(const char* swig_protected_method_name) const {
      std::map<std::string, bool>::const_iterator iv = swig_inner.find(swig_protected_method_name);
      return (iv != swig_inner.end() ? iv->second : false);
    }

    void swig_set_inner(const char* swig_protected_method_name, bool val) const
    { swig_inner[swig_protected_method_name] = val;}

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
          std::string msg = "Method in class ImageGroundConnection doesn't exist, undefined ";
          msg += method_name;
          Swig::DirectorMethodException::raise(msg.c_str());
        }
        vtable[method_index] = method;
      };
      return method;
    }
private:
    mutable swig::SwigVar_PyObject vtable[10];
#endif

};


class SwigDirector_Orbit : public GeoCal::Orbit, public Swig::Director {

public:
    SwigDirector_Orbit(PyObject *self, GeoCal::Time Min_time = GeoCal::Time::min_valid_time, GeoCal::Time Max_time = GeoCal::Time::max_valid_time);
    virtual ~SwigDirector_Orbit();
    virtual GeoCal::CartesianInertialLookVector ci_look_vector(GeoCal::Time T, GeoCal::ScLookVector const &Sl) const;
    virtual GeoCal::CartesianFixedLookVector cf_look_vector(GeoCal::Time T, GeoCal::ScLookVector const &Sl) const;
    virtual GeoCal::ScLookVector sc_look_vector(GeoCal::Time T, GeoCal::CartesianInertialLookVector const &Ci) const;
    virtual GeoCal::ScLookVector sc_look_vector(GeoCal::Time T, GeoCal::CartesianFixedLookVector const &Cf) const;
    virtual boost::shared_ptr< GeoCal::CartesianInertial > position_ci(GeoCal::Time T) const;
    virtual boost::shared_ptr< GeoCal::CartesianFixed > position_cf(GeoCal::Time T) const;
    virtual boost::shared_ptr< GeoCal::OrbitData > orbit_data(GeoCal::Time T) const;
    using GeoCal::Orbit::interpolate;


/* Internal Director utilities */
public:
    bool swig_get_inner(const char* swig_protected_method_name) const {
      std::map<std::string, bool>::const_iterator iv = swig_inner.find(swig_protected_method_name);
      return (iv != swig_inner.end() ? iv->second : false);
    }

    void swig_set_inner(const char* swig_protected_method_name, bool val) const
    { swig_inner[swig_protected_method_name] = val;}

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
      };
      return method;
    }
private:
    mutable swig::SwigVar_PyObject vtable[7];
#endif

};


#endif
