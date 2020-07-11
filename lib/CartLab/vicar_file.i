// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "vicar_file.h"
#include "ecr.h"
#include "image_ground_connection.h"
%}
%base_import(generic_object)
%import "map_info.i"
%import "geocal_rpc.i"
%import "rsm.i"
%geocal_shared_ptr(GeoCal::VicarFile);
namespace GeoCal {
class VicarFile : public GenericObject {
public:
  enum label_type {VICAR_INT, VICAR_REAL, VICAR_STRING};
  enum data_type {VICAR_BYTE, VICAR_HALF, VICAR_FULL, VICAR_FLOAT,
		  VICAR_DOUBLE};
  enum access_type {READ, WRITE, UPDATE};
  enum compression {NONE, BASIC, BASIC2};
  enum rsm_file_type {RSM_XML_FILE, RSM_NITF_FILE};
  enum glas_gfm_file_type {GLAS_GFM_XML_FILE, GLAS_GFM_NITF_FILE};
  VicarFile(const std::string& Fname, access_type Access = READ);
  VicarFile(const std::string& Fname, int Number_line, int Number_sample,
	    const std::string& Type = "BYTE");
  VicarFile(const std::string& Fname, int Number_line, int Number_sample,
	    int Number_band,
	    const std::string& Type = "BYTE", 
	    const std::string& Org = "BSQ");
  VicarFile(int Instance, access_type Access = READ, 
	    const std::string& Name = "INP");
  VicarFile(int Instance, int Number_line, int Number_sample,
	    int Number_band = 1,
	    const std::string& Type = "BYTE",
	    const std::string& Name = "OUT",
	    const std::string& Org = "BSQ",
	    compression C = NONE);
  ~VicarFile();
  static bool vicar_available();
  %python_attribute(access, access_type)
  %python_attribute(force_area_pixel, bool)
  %python_attribute(file_name, std::string)
  static bool is_vicar_file(const std::string& Fname);
  static int file_name_to_unit(const std::string& Fname);
  %python_attribute(number_band, int)
  %python_attribute(number_line, int)
  %python_attribute(number_sample, int)
  %python_attribute(type, data_type)
  %python_attribute(unit, int)
  %python_attribute_with_set(map_info, MapInfo)
  %python_attribute_with_set(rpc, Rpc)
  %python_attribute_with_set(rsm, boost::shared_ptr<Rsm>)
  %python_attribute_with_set(igc_glas_gfm, boost::shared_ptr<ImageGroundConnection>)
%pythoncode {
def rsm_save_xml(self, value):
  self._v_rsm(value, self.RSM_XML_FILE)
}
%pythoncode {
def igc_glas_gfm_save_xml(self, value):
  self._v_igc_glas_gfm(value, self.GLAS_GFM_XML_FILE)
}

  void close();
  void flush() const;
  std::string print_to_string() const;
  void label_delete(const std::string& F, const std::string& Property = "");
  void label_set(const std::string& F, 
		 int Val,
		 const std::string& Property = "");
  void label_set(const std::string& F, 
		 float Val,
		 const std::string& Property = "");
  void label_set(const std::string& F, 
		 double Val,
		 const std::string& Property = "");
  void label_set(const std::string& F, 
		 const std::string& Val,
		 const std::string& Property = "");
  %extend {
     int ltype(const std::string& N) 
	{return $self->label_type().find(N)->second;}
     std::vector<int> label_int(const std::string& N,
         const std::string& Prop = "")
	{return $self->label<std::vector<int> >(N, Prop);}
     std::vector<float> label_float(const std::string& N, 
        const std::string& Prop = "")
	{return $self->label<std::vector<float> >(N, Prop);}
     std::vector<double> label_double(const std::string& N, 
        const std::string& Prop = "")
	{return $self->label<std::vector<double> >(N, Prop);}
     std::vector<std::string> label_string(const std::string& N,
        const std::string& Prop = "")
	{return $self->label<std::vector<std::string> >(N, Prop);}
     std::vector<std::string> label_list()
     { std::vector<std::string> res;
       for(std::map<std::string, int>::const_iterator i = 
           $self->label_type().begin();
           i != $self->label_type().end(); ++i)
        res.push_back((*i).first);
      return res;
     }
     std::vector<std::string> property_list() const
     { return std::vector<std::string>($self->property().begin(), $self->property().end());}

  }	
  %pythoncode {
def __getitem__(self, key):
    if(isinstance(key, list) or isinstance(key, tuple)):
      prop, ky = key
      tp = self.ltype(prop + " " + ky)
      if(tp == VicarFile.VICAR_INT):
         return self.label_int(ky, prop)
      elif(tp == VicarFile.VICAR_REAL):
         return self.label_float(ky, prop)
      elif(tp == VicarFile.VICAR_DOUBLE):
         return self.label_double(ky, prop)
      else:
         return self.label_string(ky, prop)
    else:
      tp = self.ltype(key)
      if(tp == VicarFile.VICAR_INT):
         return self.label_int(key)
      elif(tp == VicarFile.VICAR_REAL):
         return self.label_float(key)
      elif(tp == VicarFile.VICAR_DOUBLE):
         return self.label_double(key)
      else:
         return self.label_string(key)

def __setitem__(self, key, v):
    if(isinstance(key, list) or isinstance(key, tuple)):
      prop, ky = key
      self.label_set(ky, v, prop)
    else:
      self.label_set(key, v)
  }
  %pickle_serialization();
};
}
// List of things "import *" will include
%python_export("VicarFile")
