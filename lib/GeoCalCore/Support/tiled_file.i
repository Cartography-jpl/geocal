// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "common.i"

%{
#include "tiled_file.h"
%}

namespace GeoCal {
template<std::size_t D> class TiledFileBase {
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

%template(TiledFileBase_2) TiledFileBase<2>;
}
