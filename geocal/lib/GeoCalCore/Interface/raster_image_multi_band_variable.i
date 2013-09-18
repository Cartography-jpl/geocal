// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "common.i"

%{
#include "raster_image_multi_band_variable.h"
%}
%base_import(raster_image_multi_band)
%import "raster_image.i"

%geocal_shared_ptr(GeoCal::RasterImageMultiBandVariable);

%pythoncode {
def _create_rimb(cls, version, *args):
    '''For use with pickle, covers common case where we just store the
    arguments needed to create an object. See for example HdfFile'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__()
    for i in args:
        inst.add_raster_image(i)
    return inst
}

namespace GeoCal {
class RasterImageMultiBandVariable: public RasterImageMultiBand {
public:
  %rename(raster_image) raster_image_ptr;
  RasterImageMultiBandVariable();
  RasterImageMultiBandVariable
  (const std::vector<boost::shared_ptr<RasterImage> >& Raster_image);
  void add_raster_image(const boost::shared_ptr<RasterImage>& Ri);
  virtual boost::shared_ptr<RasterImage> raster_image_ptr(int band);

  %pythoncode {
@classmethod
def pickle_format_version(cls):
  return 1

def __reduce__(self):
   arg_list = [self.__class__, self.__class__.pickle_format_version()]
   for i in range(self.number_band):
      arg_list.append(self.raster_image(i))
   return _create_rimb, tuple(arg_list)
}
};
}
