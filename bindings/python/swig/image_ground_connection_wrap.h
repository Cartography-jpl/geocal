/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.9
 * 
 * This file is not intended to be easily readable and contains a number of 
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG 
 * interface file instead. 
 * ----------------------------------------------------------------------------- */

#ifndef SWIG_image_ground_connection_WRAP_H_
#define SWIG_image_ground_connection_WRAP_H_

#include <map>
#include <string>


class SwigDirector_ImageGroundConnection : public GeoCal::ImageGroundConnection, public Swig::Director {

public:
    SwigDirector_ImageGroundConnection(PyObject *self, boost::shared_ptr< GeoCal::Dem > const &d, boost::shared_ptr< GeoCal::RasterImage > const &Img, boost::shared_ptr< GeoCal::RasterImageMultiBand > const &Img_mb, std::string const &Title);
    SwigDirector_ImageGroundConnection(PyObject *self, boost::shared_ptr< GeoCal::Dem > const &d, boost::shared_ptr< GeoCal::RasterImage > const &Img, boost::shared_ptr< GeoCal::RasterImageMultiBand > const &Img_mb, std::string const &Title, boost::shared_ptr< GeoCal::ImageMask > const &Img_mask);
    SwigDirector_ImageGroundConnection(PyObject *self, boost::shared_ptr< GeoCal::Dem > const &d, boost::shared_ptr< GeoCal::RasterImage > const &Img, boost::shared_ptr< GeoCal::RasterImageMultiBand > const &Img_mb, std::string const &Title, boost::shared_ptr< GeoCal::ImageMask > const &Img_mask, boost::shared_ptr< GeoCal::GroundMask > const &Ground_mask);
    SwigDirector_ImageGroundConnection(PyObject *self);
    virtual ~SwigDirector_ImageGroundConnection();
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
    virtual boost::shared_ptr< GeoCal::CartesianFixedLookVector > cf_look_vector_lv(GeoCal::ImageCoordinate const &Ic) const;
    virtual boost::shared_ptr< GeoCal::CartesianFixed > cf_look_vector_pos(GeoCal::ImageCoordinate const &Ic) const;
    virtual blitz::Array< double,1 > collinearity_residual(GeoCal::GroundCoordinate const &Gc, GeoCal::ImageCoordinate const &Ic_actual) const;
    virtual blitz::Array< double,2 > collinearity_residual_jacobian(GeoCal::GroundCoordinate const &Gc, GeoCal::ImageCoordinate const &Ic_actual) const;
    virtual boost::shared_ptr< GeoCal::GroundCoordinate > ground_coordinate(GeoCal::ImageCoordinate const &Ic) const;
    virtual boost::shared_ptr< GeoCal::GroundCoordinate > ground_coordinate_dem(GeoCal::ImageCoordinate const &Ic, GeoCal::Dem const &D) const;
    virtual boost::shared_ptr< GeoCal::GroundCoordinate > ground_coordinate_approx_height(GeoCal::ImageCoordinate const &Ic, double H) const;
    virtual GeoCal::ImageCoordinate image_coordinate(GeoCal::GroundCoordinate const &Gc) const;
    virtual void image_coordinate_with_status(GeoCal::GroundCoordinate const &Gc, GeoCal::ImageCoordinate &OUTPUT2, bool &OUTPUT) const;
    virtual blitz::Array< double,2 > image_coordinate_jac_cf(GeoCal::CartesianFixed const &Gc) const;
    virtual blitz::Array< double,2 > image_coordinate_jac_cf_fd(GeoCal::CartesianFixed const &Gc, double Step_size) const;
    virtual blitz::Array< double,2 > image_coordinate_jac_parm(GeoCal::GroundCoordinate const &Gc) const;
    virtual int number_line() const;
    virtual int number_sample() const;
    virtual int number_band() const;
    virtual double resolution_meter(GeoCal::ImageCoordinate const &Ic) const;
    virtual double resolution_meter() const;
    virtual double footprint_resolution_line(int Line, int Sample) const;
    virtual double footprint_resolution_sample(int Line, int Sample) const;
    virtual blitz::Array< double,7 > cf_look_vector_arr(int ln_start, int smp_start, int nline, int nsamp, int nsubpixel_line = 1, int nsubpixel_sample = 1, int nintegration_step = 1) const;
    using GeoCal::ImageGroundConnection::initialize;


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
      }
      return method;
    }
private:
    mutable swig::SwigVar_PyObject vtable[34];
#endif

};


#endif
