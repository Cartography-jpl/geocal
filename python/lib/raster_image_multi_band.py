from geocal import *

def _new_from_init(cls, version, *args):
    if(cls.pickle_format_version() < version):
        raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__(*args)
    return inst

class RasterImageMB(geocal.RasterImageMultiBandVariable):
    '''This is a thin wrapper around RasterImageMultiBandVariable that gives
    a cleaner interface for python. In particular, we add support for
    pickling.'''
    def __init__(self, *img_list):
        geocal.RasterImageMultiBandVariable.__init__(self)
        for img in img_list:
            self.add_raster_image(img)
    
    @classmethod
    def pickle_format_version(cls):
        return 1

    def __reduce__(self):
        arg_list = [self.__class__, self.__class__.pickle_format_version()]
        for i in range(self.number_band):
            arg_list.append(self.raster_image(i))
        return _new_from_init, tuple(arg_list)

