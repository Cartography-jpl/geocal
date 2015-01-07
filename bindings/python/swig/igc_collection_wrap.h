/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.9
 * 
 * This file is not intended to be easily readable and contains a number of 
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG 
 * interface file instead. 
 * ----------------------------------------------------------------------------- */

#ifndef SWIG_igc_collection_WRAP_H_
#define SWIG_igc_collection_WRAP_H_

#include <map>
#include <string>


class SwigDirector_IgcCollection : public GeoCal::IgcCollection, public Swig::Director {

public:
    SwigDirector_IgcCollection(PyObject *self);
    virtual ~SwigDirector_IgcCollection();
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
    virtual int number_image() const;
    virtual boost::shared_ptr< GeoCal::GroundCoordinate > ground_coordinate(int Image_index, GeoCal::ImageCoordinate const &Ic) const;
    virtual boost::shared_ptr< GeoCal::GroundCoordinate > ground_coordinate_dem(int Image_index, GeoCal::ImageCoordinate const &Ic, GeoCal::Dem const &D) const;
    virtual boost::shared_ptr< GeoCal::Dem > dem(int Image_index) const;
    virtual GeoCal::ImageCoordinate image_coordinate(int Image_index, GeoCal::GroundCoordinate const &Gc) const;
    virtual blitz::Array< double,2 > image_coordinate_jac_cf(int Image_index, GeoCal::CartesianFixed const &Gc) const;
    virtual std::string title(int Image_index) const;
    virtual boost::shared_ptr< GeoCal::RasterImage > image(int Image_index) const;
    virtual boost::shared_ptr< GeoCal::ImageGroundConnection > image_ground_connection(int Image_index) const;
    virtual boost::shared_ptr< GeoCal::IgcCollection > subset(std::vector< int,std::allocator< int > > const &Index_set) const;


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
          std::string msg = "Method in class IgcCollection doesn't exist, undefined ";
          msg += method_name;
          Swig::DirectorMethodException::raise(msg.c_str());
        }
        vtable[method_index] = method;
      }
      return method;
    }
private:
    mutable swig::SwigVar_PyObject vtable[21];
#endif

};


#endif
