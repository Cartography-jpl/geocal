// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "ibis_file.h"
%}
%pythoncode %{
import numpy as np
%}

%base_import(generic_object)
%geocal_shared_ptr(GeoCal::IbisFile);
namespace GeoCal {
class IbisFile;
template<class T> class IbisColumn {
public:
  const IbisFile& ibis_file() const;
  %python_attribute(column_index, int)
  %python_attribute(size_byte, int)
  std::string print_to_string() const;
  std::vector<T> data;
  bool update;
  %pythoncode {
def __getitem__(self, i):
  return self.data[i]

def __setitem__(self, i, v):
  self.update = True
  self.data[i] = v

  }
private:
  IbisColumn(const IbisColumn<T>&);
};
}

%template(IbisColumn_byte) GeoCal::IbisColumn<unsigned char>;
%template(IbisColumn_half) GeoCal::IbisColumn<short int>;
%template(IbisColumn_full) GeoCal::IbisColumn<int>;
%template(IbisColumn_float) GeoCal::IbisColumn<float>;
%template(IbisColumn_double) GeoCal::IbisColumn<double>;
%template(IbisColumn_string) GeoCal::IbisColumn<std::string>;

namespace GeoCal {
class IbisFile : public GenericObject {
public:
  enum access_type {READ, WRITE, UPDATE};
  enum data_type {VICAR_BYTE, VICAR_HALF, VICAR_FULL, VICAR_FLOAT,
		  VICAR_DOUBLE, VICAR_ASCII};
  IbisFile(const std::string& Fname, int Number_row, 
	   const std::vector<std::string>& Format, 
	   const std::string& Organization = "COLUMN");
  IbisFile(const std::string& Fname, access_type Access = READ);
  %python_attribute(access, access_type)
  void close();
  data_type column_data_type(int I) const;
  %python_attribute(file_name, std::string)
  %python_attribute(ibis_fh, int)
  %python_attribute(number_row, int)
  %python_attribute(number_col, int)
  %python_attribute(unit, int)
  void set_column_type(int I, data_type Dt);
  void mark_updated();
  void mark_updated(int I);
  void flush();
  std::string print_to_string() const;
  %pickle_serialization();
  %extend {
// We use the C types here rather than the Vicar typedef because SWIG wants
// to treat unsigned char differently than VicarByte, for example. So that
// SWIG knows how to convert these to native Ruby types, we use the C type 
// here. If we change the types of Vicar (e.g., port to a machine with 
// different size types), then this will cause a compile error and we'll
// need to fix this.
    GeoCal::IbisColumn<unsigned char>& column_byte(int I)
    { return $self->column<GeoCal::VicarByte>(I); }
    GeoCal::IbisColumn<short int>& column_half(int I)
    { return $self->column<GeoCal::VicarHalf>(I); }
    GeoCal::IbisColumn<int>& column_full(int I)
    { return $self->column<GeoCal::VicarFull>(I); }
    GeoCal::IbisColumn<float>& column_float(int I)
    { return $self->column<GeoCal::VicarFloat>(I); }
    GeoCal::IbisColumn<double>& column_double(int I)
    { return $self->column<GeoCal::VicarDouble>(I); }
    GeoCal::IbisColumn<std::string>& column_string(int I)
    { return $self->column<std::string>(I); }
  }
};
}

// List of things "import *" will include
%python_export("IbisFile", "IbisColumn_byte", "IbisColumn_half", "IbisColumn_full", "IbisColumn_float", "IbisColumn_double", "IbisColumn_string")


