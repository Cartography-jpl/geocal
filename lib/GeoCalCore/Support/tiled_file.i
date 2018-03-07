// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "geocal_common.i"

%{
#include "tiled_file.h"
%}

%base_import(generic_object)

namespace GeoCal {
  template<std::size_t D> class TiledFileBase : public GenericObject {
public:
  typedef boost::multi_array_types::index index;
  %python_attribute(number_swap, int)
  void reset_number_swap();
  %python_attribute(size, boost::array<index, D>)
  %python_attribute(tile_size, boost::array<index, D>)
  virtual int get_int(const boost::array<index, D>& Index) const = 0;
  virtual double get_double(const boost::array<index, D>& Index) const = 0;
  virtual void read_int(const boost::array<index, D>& Min_index, 
			const boost::array<index, D>& Max_index, 
			int* Res) const = 0;
  virtual void read_double(const boost::array<index, D>& Min_index, 
			   const boost::array<index, D>& Max_index, 
			   double* Res) const = 0;
  virtual void write_int(const boost::array<index, D>& Index, int Val) = 0;
  virtual void write_double(const boost::array<index, D>& Index, 
			    double Val) = 0;
};

}

%geocal_shared_ptr(GeoCal::TiledFileBase<2>);
%template(TiledFileBase_2) GeoCal::TiledFileBase<2>;
// List of things "import *" will include
%python_export("TiledFileBase_2")
