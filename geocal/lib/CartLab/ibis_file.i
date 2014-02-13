// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

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
  int column_index() const;
  int size_byte() const;
  std::string print_to_string() const;
  std::vector<T> data;
  bool update;
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
  void flush();
  std::string print_to_string() const;
  %pickle_init(1, self.file_name, self.access)
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
#ifdef SWIGPYTHON
// Provide a simple "column" function that returns a numpy array of
// data of whatever the underlying type is.
%pythoncode %{
def __getitem__(self, index):
  return self.column(index[1])[index[0]]

def column(self, cindex):
    '''This return a numpy.array of whatever type the given column is. This
    reads all the data.'''
    t = self.column_data_type(cindex)
    if(t == IbisFile.VICAR_BYTE):
        return np.array(self.column_byte(cindex).data)
    elif(t == IbisFile.VICAR_HALF):
        return np.array(self.column_half(cindex).data)
    elif(t == IbisFile.VICAR_FULL):
        return np.array(self.column_full(cindex).data)
    elif(t == IbisFile.VICAR_FLOAT):
        return np.array(self.column_float(cindex).data)
    elif(t == IbisFile.VICAR_DOUBLE):
        return np.array(self.column_double(cindex).data)
    elif(t == IbisFile.VICAR_ASCII):
        return np.array(self.column_string(cindex).data)
    else:
        raise "Unrecognized type %d" % t
%}
#endif
};
}
