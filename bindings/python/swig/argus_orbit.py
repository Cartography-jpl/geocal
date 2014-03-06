# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _argus_orbit.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_argus_orbit', [dirname(__file__)])
        except ImportError:
            import _argus_orbit
            return _argus_orbit
        if fp is not None:
            try:
                _mod = imp.load_module('_argus_orbit', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _argus_orbit = swig_import_helper()
    del swig_import_helper
else:
    import _argus_orbit
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


def _swig_setattr_nondynamic_method(set):
    def set_attr(self,name,value):
        if (name == "thisown"): return self.this.own(value)
        if hasattr(self,name) or (name == "this"):
            set(self,name,value)
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
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _argus_orbit.delete_SwigPyIterator
    def __iter__(self): return self
SwigPyIterator.value = new_instancemethod(_argus_orbit.SwigPyIterator_value,None,SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_argus_orbit.SwigPyIterator_incr,None,SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_argus_orbit.SwigPyIterator_decr,None,SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_argus_orbit.SwigPyIterator_distance,None,SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_argus_orbit.SwigPyIterator_equal,None,SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_argus_orbit.SwigPyIterator_copy,None,SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_argus_orbit.SwigPyIterator_next,None,SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_argus_orbit.SwigPyIterator___next__,None,SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_argus_orbit.SwigPyIterator_previous,None,SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_argus_orbit.SwigPyIterator_advance,None,SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_argus_orbit.SwigPyIterator___eq__,None,SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_argus_orbit.SwigPyIterator___ne__,None,SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_argus_orbit.SwigPyIterator___iadd__,None,SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_argus_orbit.SwigPyIterator___isub__,None,SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_argus_orbit.SwigPyIterator___add__,None,SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_argus_orbit.SwigPyIterator___sub__,None,SwigPyIterator)
SwigPyIterator_swigregister = _argus_orbit.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _argus_orbit.SHARED_PTR_DISOWN
def _new_from_init(cls, version, *args):
    '''For use with pickle, covers common case where we just store the
    arguments needed to create an object. See for example HdfFile'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__(*args)
    return inst

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

import geocal_swig.orbit_quaternion_list
import geocal_swig.orbit
import geocal_swig.generic_object
import geocal_swig.aircraft_orbit_data
import geocal_swig.raster_image_tiled_file
import geocal_swig.raster_image_variable
import geocal_swig.raster_image
import geocal_swig.geocal_exception
class ArgusOrbitData(geocal_swig.aircraft_orbit_data.AircraftOrbitData):
    """
    This is a single ARGUS navigation file record.

    In addition to the normal things in an OrbitData class, this contains
    some other metadata. This matches what is stored in the ARGUS CSV
    navigation file. The additional information is the file name of the
    JPEG image file collected by the camera and the camera number.

    Note that we don't have access to a direct measurement of the
    velocity. Rather than trying to do some clever estimate of the
    velocity, we just set it to 0.

    C++ includes: argus_orbit.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::ArgusOrbitData::ArgusOrbitData(const Time &Tm, const std::string &File_name, int Camera_number,
        const Geodetic &Position, const boost::array< double, 3 > &Vel_fixed,
        double Roll, double Pitch, double Heading)
        Constructor. 
        """
        _argus_orbit.ArgusOrbitData_swiginit(self,_argus_orbit.new_ArgusOrbitData(*args))
    def save_ortho(self, *args):
        """
        void ArgusOrbitData::save_ortho(const MapInfo &Mi, const Camera &Cam, const Dem &D, const std::string
        &Fname, const std::string &Type, int Border=10, int Grid_spacing=1)
        const
        Save an three color orthorectified image to the given file.

        The type should be "img", "tif", "tifjpeg" or "vicar" ("tif"
        uses LZW lossless compression, "tifjpeg" uses jpeg lossy
        compression).

        The MapInfo governs the map projection and pixel size used. We however
        only write out the subset of MapInfo that is needed to cover the
        footprint of this orthorectified image.

        To speed up the processing, you can give a Grid_spacing > 1. We
        calculate the position in the input image exactly at the Grid_spacing
        locations, and do a bilinear interpolation in between. If the
        Grid_spacing is 1, then no interpolation is done.

        This is a somewhat specific function, but I had similar functionality
        in Ruby code working with ARGUS data. Ruby is a nice language, but
        just sucks at garbage collection. So we move this functionality into
        this routine where we can control the lifetimes of all of the data
        involved. 
        """
        return _argus_orbit.ArgusOrbitData_save_ortho(self, *args)

    def add_ortho_to_image(self, *args):
        """
        void ArgusOrbitData::add_ortho_to_image(const boost::shared_ptr< RasterImage > &M1, const boost::shared_ptr<
        RasterImage > &M2, const boost::shared_ptr< RasterImage > &M3, const
        Camera &Cam, const Dem &D, int Border=10) const
        Add three color orthorectified image to an existing set of three
        images.

        We currently don't try to do any feathering, instead we simply add
        imagery if it is currently fill (value of 0), or crop the data if it
        already has data. 
        """
        return _argus_orbit.ArgusOrbitData_add_ortho_to_image(self, *args)

    def mosaic(*args):
        """
        void ArgusOrbitData::mosaic(const std::vector< boost::shared_ptr< ArgusOrbitData > > &Od, const
        std::vector< boost::shared_ptr< Camera > > &Cam, const Dem &D, const
        MapInfo &Mi, const std::string &Fname, const std::string &Type, int
        Border=10)
        Generate a mosaic that fits the given MapInfo, sampling each of the
        orbit data.

        We save this to the given file, with the given Type. 
        """
        return _argus_orbit.ArgusOrbitData_mosaic(*args)

    mosaic = staticmethod(mosaic)
    def image(self, band=1):
        """
        boost::shared_ptr< GdalRasterImage > ArgusOrbitData::image(int band=1) const
        Access image for the given band.

        The band is 1 based, because that is what Gdal uses. 
        """
        return _argus_orbit.ArgusOrbitData_image(self, band)

    def _v_file_name(self):
        """
        const std::string& GeoCal::ArgusOrbitData::file_name() const
        File with JPEG data. 
        """
        return _argus_orbit.ArgusOrbitData__v_file_name(self)

    @property
    def file_name(self):
        return self._v_file_name()

    def _v_camera_number(self):
        """
        int GeoCal::ArgusOrbitData::camera_number() const
        Camera number. This is 1 - 13. 
        """
        return _argus_orbit.ArgusOrbitData__v_camera_number(self)

    @property
    def camera_number(self):
        return self._v_camera_number()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.time,self.file_name,self.camera_number,self.position_geodetic,self.vector_cf,self.roll,self.pitch,self.heading)

    __swig_destroy__ = _argus_orbit.delete_ArgusOrbitData
ArgusOrbitData.save_ortho = new_instancemethod(_argus_orbit.ArgusOrbitData_save_ortho,None,ArgusOrbitData)
ArgusOrbitData.add_ortho_to_image = new_instancemethod(_argus_orbit.ArgusOrbitData_add_ortho_to_image,None,ArgusOrbitData)
ArgusOrbitData.image = new_instancemethod(_argus_orbit.ArgusOrbitData_image,None,ArgusOrbitData)
ArgusOrbitData._v_file_name = new_instancemethod(_argus_orbit.ArgusOrbitData__v_file_name,None,ArgusOrbitData)
ArgusOrbitData._v_camera_number = new_instancemethod(_argus_orbit.ArgusOrbitData__v_camera_number,None,ArgusOrbitData)
ArgusOrbitData_swigregister = _argus_orbit.ArgusOrbitData_swigregister
ArgusOrbitData_swigregister(ArgusOrbitData)

def ArgusOrbitData_mosaic(*args):
  """
    void ArgusOrbitData::mosaic(const std::vector< boost::shared_ptr< ArgusOrbitData > > &Od, const
    std::vector< boost::shared_ptr< Camera > > &Cam, const Dem &D, const
    MapInfo &Mi, const std::string &Fname, const std::string &Type, int
    Border=10)
    Generate a mosaic that fits the given MapInfo, sampling each of the
    orbit data.

    We save this to the given file, with the given Type. 
    """
  return _argus_orbit.ArgusOrbitData_mosaic(*args)

class ArgusOrbit(geocal_swig.orbit_quaternion_list.OrbitQuaternionList):
    """
    This read a CSV navigation file and uses it to generate a Orbit.

    We access the data in two ways. The first is the standard time index,
    giving a OrbitData for that time (interpolating if needed). The second
    is by "row" and camera number. All the orbit data for a given row
    were acquired at nearly the same time. A row contains data for 1 or
    more cameras - once the instrument is fully working a row has data for
    cameras 1 through 13. We return a ArgusOrbitData, which in addition to
    having the normal OrbitData stuff has additional metadata information.

    C++ includes: argus_orbit.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        ArgusOrbit::ArgusOrbit(const std::string &Fname)
        Open the given file and use it to provide Orbit information.

        This file should be a CSV file containing one record per line, with
        the fields "File,Camera,Time,Lat,Lon,Alt,Roll,Pitch,Heading". The
        first line is assumed to be a header, and is discarded. The second
        line is the epoch that the times are measured relative to.

        Orbit data, indexed by the Time. This is a multimap because it is
        entirely possible to have multiple entries in the navigation file with
        the same time, e.g., two cameras collect data at the same time. 
        """
        _argus_orbit.ArgusOrbit_swiginit(self,_argus_orbit.new_ArgusOrbit(*args))
    def focal_length(self, *args):
        """
        double ArgusOrbit::focal_length(int camera_num) const
        This finds the first row with orbit and image data for the given
        camera.

        We then read the metadata from the image do determine the reported
        focal length. If we don't have any orbit or image data for the camera,
        we return -1. 
        """
        return _argus_orbit.ArgusOrbit_focal_length(self, *args)

    def _v_number_row(self):
        """
        int GeoCal::ArgusOrbit::number_row() const
        Number of rows of data we have. 
        """
        return _argus_orbit.ArgusOrbit__v_number_row(self)

    @property
    def number_row(self):
        return self._v_number_row()

    def nav(self, *args):
        """
        boost::shared_ptr< ArgusOrbitData > ArgusOrbit::nav(int row, int camera_num) const
        Return the ArgusOrbitData for the given row and camera, or if we don't
        have any data there return a null pointer. 
        """
        return _argus_orbit.ArgusOrbit_nav(self, *args)

    def _v_file_name(self):
        """
        const std::string& GeoCal::ArgusOrbit::file_name() const

        """
        return _argus_orbit.ArgusOrbit__v_file_name(self)

    @property
    def file_name(self):
        return self._v_file_name()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.file_name)

    __swig_destroy__ = _argus_orbit.delete_ArgusOrbit
ArgusOrbit.focal_length = new_instancemethod(_argus_orbit.ArgusOrbit_focal_length,None,ArgusOrbit)
ArgusOrbit._v_number_row = new_instancemethod(_argus_orbit.ArgusOrbit__v_number_row,None,ArgusOrbit)
ArgusOrbit.nav = new_instancemethod(_argus_orbit.ArgusOrbit_nav,None,ArgusOrbit)
ArgusOrbit._v_file_name = new_instancemethod(_argus_orbit.ArgusOrbit__v_file_name,None,ArgusOrbit)
ArgusOrbit_swigregister = _argus_orbit.ArgusOrbit_swigregister
ArgusOrbit_swigregister(ArgusOrbit)

class Vector_ArgusOrbitData(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self): return self.iterator()
    def __init__(self, *args): 
        _argus_orbit.Vector_ArgusOrbitData_swiginit(self,_argus_orbit.new_Vector_ArgusOrbitData(*args))
    __swig_destroy__ = _argus_orbit.delete_Vector_ArgusOrbitData
Vector_ArgusOrbitData.iterator = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_iterator,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.__nonzero__ = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData___nonzero__,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.__bool__ = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData___bool__,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.__len__ = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData___len__,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.pop = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_pop,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.__getslice__ = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData___getslice__,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.__setslice__ = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData___setslice__,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.__delslice__ = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData___delslice__,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.__delitem__ = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData___delitem__,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.__getitem__ = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData___getitem__,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.__setitem__ = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData___setitem__,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.append = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_append,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.empty = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_empty,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.size = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_size,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.clear = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_clear,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.swap = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_swap,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.get_allocator = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_get_allocator,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.begin = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_begin,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.end = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_end,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.rbegin = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_rbegin,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.rend = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_rend,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.pop_back = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_pop_back,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.erase = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_erase,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.push_back = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_push_back,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.front = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_front,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.back = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_back,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.assign = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_assign,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.resize = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_resize,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.insert = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_insert,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.reserve = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_reserve,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData.capacity = new_instancemethod(_argus_orbit.Vector_ArgusOrbitData_capacity,None,Vector_ArgusOrbitData)
Vector_ArgusOrbitData_swigregister = _argus_orbit.Vector_ArgusOrbitData_swigregister
Vector_ArgusOrbitData_swigregister(Vector_ArgusOrbitData)


