# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _srtm_dem.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_srtm_dem', [dirname(__file__)])
        except ImportError:
            import _srtm_dem
            return _srtm_dem
        if fp is not None:
            try:
                _mod = imp.load_module('_srtm_dem', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _srtm_dem = swig_import_helper()
    del swig_import_helper
else:
    import _srtm_dem
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
    __swig_destroy__ = _srtm_dem.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_srtm_dem.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_srtm_dem.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_srtm_dem.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_srtm_dem.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_srtm_dem.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_srtm_dem.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_srtm_dem.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_srtm_dem.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_srtm_dem.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_srtm_dem.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_srtm_dem.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_srtm_dem.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_srtm_dem.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_srtm_dem.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_srtm_dem.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_srtm_dem.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _srtm_dem.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_srtm_dem.SHARED_PTR_DISOWN_swigconstant(_srtm_dem)
SHARED_PTR_DISOWN = _srtm_dem.SHARED_PTR_DISOWN

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

import geocal_swig.dem_map_info
import geocal_swig.dem
import geocal_swig.generic_object
import geocal_swig.cart_lab_multifile
import geocal_swig.raster_multifile
import geocal_swig.raster_image_variable
import geocal_swig.raster_image
class SrtmDemData(geocal_swig.cart_lab_multifile.VicarCartLabMultifile):
    """

    This is used the read the SRTM data.

    This gets used by SrtmDem. Although you can use this class directly,
    generally uou'll use this through SrtmDem class.

    C++ includes: srtm_dem.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Dir, No_coverage_is_error=True, Number_line_per_tile=-1, Number_sample_per_tile=-1, Number_tile_each_file=4, Number_file=4, Favor_memory_mapped=True, Force_area_pixel=True):
        """

        SrtmDemData::SrtmDemData(const std::string &Dir, bool No_coverage_is_error=true, int
        Number_line_per_tile=-1, int Number_sample_per_tile=-1, int
        Number_tile_each_file=4, int Number_file=4, bool
        Favor_memory_mapped=true, bool Force_area_pixel=true)
        Constructor.

        You can provide the directory to look for SRTM DEM data, or if you
        leave this blank we use the value of environment variable ELEV_ROOT.

        The SRTM doesn't cover the whole globe. If you ask for a point outside
        of the area this can either be treated as an error, or alternatively
        you can return a value of 0 instead. This is controlled by
        No_coverage_is_error.

        There are two kinds of tiling going on. At the top level, we have a
        number of files open at one time, given by Number_file. For each file,
        we read it with tiles with the given Number_line_per_tile x
        Number_sample_per_tile, having up to Number_tile_each_file tiles. If
        the Number_line_per_tile or Number_sample_per_tile is -1 we read the
        entire file. 
        """
        _srtm_dem.SrtmDemData_swiginit(self, _srtm_dem.new_SrtmDemData(Dir, No_coverage_is_error, Number_line_per_tile, Number_sample_per_tile, Number_tile_each_file, Number_file, Favor_memory_mapped, Force_area_pixel))

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _srtm_dem.delete_SrtmDemData
SrtmDemData_swigregister = _srtm_dem.SrtmDemData_swigregister
SrtmDemData_swigregister(SrtmDemData)

class SrtmDem(geocal_swig.dem_map_info.DemMapInfo):
    """

    This class provides access to the SRTM.

    Note comment from Tom about this data:

    The srtm_filled data were prepared back in 2003. The goal was to
    create a quick and dirty global data set, which means the data came
    from a variety of sources, scales, datums, and accuracy. At that time,
    a half pixel error was well below the threshold of concern (15-30 was
    high resolution back then). So, point and area data were often mixed
    in the void filling process, and some regional areas (not covered by
    srtm) were carved into quadrangles and retained their individual pixel
    types. I know its a mess, but the bottom line is that the data are not
    accurate to half a pixel, which makes the 'point vs. area' discuss not
    really relevant.

    The SRTM VICAR files incorrectly label the data as "pixel is point",
    when really this is "pixel is area". This matters, because it is a
    1/2 pixel shift. The code overrides what the VICAR files says, and
    treats it as "area" rather than "pixel".

    C++ includes: srtm_dem.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        SrtmDem::SrtmDem(const std::string &Dirbase="", bool Outside_dem_is_error=true,
        const boost::shared_ptr< Datum > &D=boost::shared_ptr< Datum >(new
        DatumGeoid96()))
        Constructor.

        You can provide the directory where there data can be found. If you
        leave this as a blank string, we use ${ELEV_ROOT} as the directory and
        ${ELEV_ROOT}/L2_dem_db.int as the file. 
        """
        _srtm_dem.SrtmDem_swiginit(self, _srtm_dem.new_SrtmDem(*args))

    def elevation(self, Y_index, X_index):
        """

        virtual double GeoCal::SrtmDem::elevation(int Y_index, int X_index) const
        Return height in meters relative to datum(). 
        """
        return _srtm_dem.SrtmDem_elevation(self, Y_index, X_index)


    def _v_directory_base(self):
        """

        const std::string& GeoCal::SrtmDem::directory_base() const
        Directory base that we read SRTM data from. 
        """
        return _srtm_dem.SrtmDem__v_directory_base(self)


    @property
    def directory_base(self):
        return self._v_directory_base()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _srtm_dem.delete_SrtmDem
SrtmDem.elevation = new_instancemethod(_srtm_dem.SrtmDem_elevation, None, SrtmDem)
SrtmDem._v_directory_base = new_instancemethod(_srtm_dem.SrtmDem__v_directory_base, None, SrtmDem)
SrtmDem_swigregister = _srtm_dem.SrtmDem_swigregister
SrtmDem_swigregister(SrtmDem)



