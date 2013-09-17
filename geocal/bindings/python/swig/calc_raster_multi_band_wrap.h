/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.9
 * 
 * This file is not intended to be easily readable and contains a number of 
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG 
 * interface file instead. 
 * ----------------------------------------------------------------------------- */

#ifndef SWIG_calc_raster_multi_band_WRAP_H_
#define SWIG_calc_raster_multi_band_WRAP_H_

#include <map>
#include <string>


class SwigDirector_CalcRasterMultiBand : public GeoCal::CalcRasterMultiBand, public Swig::Director {

public:
    SwigDirector_CalcRasterMultiBand(PyObject *self);
    SwigDirector_CalcRasterMultiBand(PyObject *self, GeoCal::RasterImage const &Img, int Nband, int Number_tile_line = -1, int Number_tile_sample = -1, int Number_tile = 4);
    SwigDirector_CalcRasterMultiBand(PyObject *self, int Nline, int Nsamp, int Nband, int Number_tile_line = -1, int Number_tile_sample = -1, int Number_tile = 4);
    SwigDirector_CalcRasterMultiBand(PyObject *self, GeoCal::MapInfo const &Mi, int Nband, int Number_tile_line = -1, int Number_tile_sample = -1, int Number_tile = 4);
    virtual ~SwigDirector_CalcRasterMultiBand();
    virtual int number_band() const;
    virtual boost::shared_ptr< GeoCal::RasterImage > raster_image_ptr(int band);
    virtual blitz::Array< double,3 > read_double(int Lstart, int Sstart, int Nline, int Nsamp) const;
    virtual void calc(int Lstart, int Sstart) const;
    using GeoCal::CalcRasterMultiBand::data_ptr;
    using GeoCal::CalcRasterMultiBand::initialize;


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
          std::string msg = "Method in class CalcRasterMultiBand doesn't exist, undefined ";
          msg += method_name;
          Swig::DirectorMethodException::raise(msg.c_str());
        }
        vtable[method_index] = method;
      }
      return method;
    }
private:
    mutable swig::SwigVar_PyObject vtable[4];
#endif

};


#endif
