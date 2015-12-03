# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _raster_image_multi_band.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_raster_image_multi_band', [dirname(__file__)])
        except ImportError:
            import _raster_image_multi_band
            return _raster_image_multi_band
        if fp is not None:
            try:
                _mod = imp.load_module('_raster_image_multi_band', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _raster_image_multi_band = swig_import_helper()
    del swig_import_helper
else:
    import _raster_image_multi_band
del version_info
try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.


def _swig_setattr_nondynamic(self, class_type, name, value, static=1):
    if (name == "thisown"):
        return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name, None)
    if method:
        return method(self, value)
    if (not static):
        object.__setattr__(self, name, value)
    else:
        raise AttributeError("You cannot add attributes to %s" % self)


def _swig_setattr(self, class_type, name, value):
    return _swig_setattr_nondynamic(self, class_type, name, value, 0)


def _swig_getattr_nondynamic(self, class_type, name, static=1):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    if (not static):
        return object.__getattr__(self, name)
    else:
        raise AttributeError(name)

def _swig_getattr(self, class_type, name):
    return _swig_getattr_nondynamic(self, class_type, name, 0)


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object:
        pass
    _newclass = 0



def _swig_setattr_nondynamic_method(set):
    def set_attr(self, name, value):
        if (name == "thisown"):
            return self.this.own(value)
        if hasattr(self, name) or (name == "this"):
            set(self, name, value)
        else:
            raise AttributeError("You cannot add attributes to %s" % self)
    return set_attr


try:
    import weakref
    weakref_proxy = weakref.proxy
except:
    weakref_proxy = lambda x: x


class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _raster_image_multi_band.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_raster_image_multi_band.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_raster_image_multi_band.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_raster_image_multi_band.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_raster_image_multi_band.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_raster_image_multi_band.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_raster_image_multi_band.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_raster_image_multi_band.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_raster_image_multi_band.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_raster_image_multi_band.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_raster_image_multi_band.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_raster_image_multi_band.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_raster_image_multi_band.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_raster_image_multi_band.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_raster_image_multi_band.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_raster_image_multi_band.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_raster_image_multi_band.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _raster_image_multi_band.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_raster_image_multi_band.SHARED_PTR_DISOWN_swigconstant(_raster_image_multi_band)
SHARED_PTR_DISOWN = _raster_image_multi_band.SHARED_PTR_DISOWN

def _new_from_init(cls, version, *args):
    '''For use with pickle, covers common case where we just store the
    arguments needed to create an object. See for example HdfFile'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__(*args)
    return inst

def _new_from_serialization(data):
    return geocal_swig.serialize_read_binary(data)

def _new_vector(cls, version, lst):
    '''Create a vector from a list.'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__()
    for i in lst:
       inst.append(i)
    return inst

def _new_from_set(cls, version, *args):
    '''For use with pickle, covers common case where we use a set function 
    to assign the value'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__()
    inst.set(*args)
    return inst

import geocal_swig.generic_object
class RasterImageMultiBand(geocal_swig.generic_object.GenericObject):
    """

    This is a set of RasterImages, one for each band.

    C++ includes: raster_image_multi_band.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr

    def _v_number_band(self):
        """

        virtual int GeoCal::RasterImageMultiBand::number_band() const =0
        Return number of bands. 
        """
        return _raster_image_multi_band.RasterImageMultiBand__v_number_band(self)


    @property
    def number_band(self):
        return self._v_number_band()


    def raster_image(self, band):
        """

        virtual boost::shared_ptr<RasterImage> GeoCal::RasterImageMultiBand::raster_image_ptr(int band) const =0

        """
        return _raster_image_multi_band.RasterImageMultiBand_raster_image(self, band)


    def read(self, Lstart, Sstart, Nline, Nsamp):
        """

        Array< int, 3 > RasterImageMultiBand::read(int Lstart, int Sstart, int Nline, int Nsamp) const
        Read the data from the same location in each image, and put into an
        array.

        This will be number_band x number_line x number_sample. 
        """
        return _raster_image_multi_band.RasterImageMultiBand_read(self, Lstart, Sstart, Nline, Nsamp)


    def read_double(self, Lstart, Sstart, Nline, Nsamp):
        """

        Array< double, 3 > RasterImageMultiBand::read_double(int Lstart, int Sstart, int Nline, int Nsamp) const
        Read the data from the same location in each image, and put into an
        array.

        This will be number_band x number_line x number_sample. 
        """
        return _raster_image_multi_band.RasterImageMultiBand_read_double(self, Lstart, Sstart, Nline, Nsamp)


    def write(self, Lstart, Sstart, Data):
        """

        void RasterImageMultiBand::write(int Lstart, int Sstart, const blitz::Array< int, 3 > &Data)
        Write the data to the same location in each image.

        Data should be number_band() x nline x nsamp. 
        """
        return _raster_image_multi_band.RasterImageMultiBand_write(self, Lstart, Sstart, Data)


    def overview(self, Min_number_sample):
        """

        RasterImageMultiBandVariable RasterImageMultiBand::overview(int Min_number_sample) const
        Create a RasterImageMultiBandVariable by finding the overview (if any)
        for each of the raster bands.

        The bands will all be the same size, so if the overviews don't match
        in size we don't use them. If we can't find overviews, then the
        RasterImageMultiBandVariable will just point to the same data as this
        object does. 
        """
        return _raster_image_multi_band.RasterImageMultiBand_overview(self, Min_number_sample)

    __swig_destroy__ = _raster_image_multi_band.delete_RasterImageMultiBand
RasterImageMultiBand._v_number_band = new_instancemethod(_raster_image_multi_band.RasterImageMultiBand__v_number_band, None, RasterImageMultiBand)
RasterImageMultiBand.raster_image = new_instancemethod(_raster_image_multi_band.RasterImageMultiBand_raster_image, None, RasterImageMultiBand)
RasterImageMultiBand.read = new_instancemethod(_raster_image_multi_band.RasterImageMultiBand_read, None, RasterImageMultiBand)
RasterImageMultiBand.read_double = new_instancemethod(_raster_image_multi_band.RasterImageMultiBand_read_double, None, RasterImageMultiBand)
RasterImageMultiBand.write = new_instancemethod(_raster_image_multi_band.RasterImageMultiBand_write, None, RasterImageMultiBand)
RasterImageMultiBand.__str__ = new_instancemethod(_raster_image_multi_band.RasterImageMultiBand___str__, None, RasterImageMultiBand)
RasterImageMultiBand.overview = new_instancemethod(_raster_image_multi_band.RasterImageMultiBand_overview, None, RasterImageMultiBand)
RasterImageMultiBand_swigregister = _raster_image_multi_band.RasterImageMultiBand_swigregister
RasterImageMultiBand_swigregister(RasterImageMultiBand)


def copy_raster(*args):
    """

    void GeoCal::copy(const RasterImageMultiBand &Img_in, RasterImageMultiBand &Img_out,
    bool Log_progress=false)
    This copies one multiband image to another.

    The images should be the same size. We also assume that all the bands
    are the same size.

    Setting Diagnostic to true causes messages to be printed as we do the
    copying. 
    """
    return _raster_image_multi_band.copy_raster(*args)


