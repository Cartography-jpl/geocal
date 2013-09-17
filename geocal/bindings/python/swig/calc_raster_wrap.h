/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.9
 * 
 * This file is not intended to be easily readable and contains a number of 
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG 
 * interface file instead. 
 * ----------------------------------------------------------------------------- */

#ifndef SWIG_calc_raster_WRAP_H_
#define SWIG_calc_raster_WRAP_H_

#include <map>
#include <string>


class SwigDirector_CalcRaster : public GeoCal::CalcRaster, public Swig::Director {

public:
    SwigDirector_CalcRaster(PyObject *self, int Number_tile = 4);
    SwigDirector_CalcRaster(PyObject *self, int Nline, int Nsamp, int Number_tile = 4);
    SwigDirector_CalcRaster(PyObject *self, int Nline, int Nsamp, int Ntileln, int Ntilesmp, int Number_tile = 4);
    SwigDirector_CalcRaster(PyObject *self, GeoCal::MapInfo const &Mi, int Number_tile = 4);
    SwigDirector_CalcRaster(PyObject *self, GeoCal::RasterImage const &Img, int Number_tile = 4);
    virtual ~SwigDirector_CalcRaster();
    virtual boost::shared_ptr< GeoCal::RasterImage > overview(int Min_number_sample) const;
    virtual int number_tile_line() const;
    virtual int number_tile_sample() const;
    virtual int read(int Line, int Sample) const;
    virtual void write(int Line, int Sample, int Val);
    virtual void write(int Lstart, int Sstart, blitz::Array< int,2 > const &A);
    virtual blitz::Array< double,2 > read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const;
    virtual void calc(int Lstart, int Sstart) const;
    using GeoCal::CalcRaster::data_ptr;


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
          std::string msg = "Method in class CalcRaster doesn't exist, undefined ";
          msg += method_name;
          Swig::DirectorMethodException::raise(msg.c_str());
        }
        vtable[method_index] = method;
      }
      return method;
    }
private:
    mutable swig::SwigVar_PyObject vtable[8];
#endif

};


#endif
