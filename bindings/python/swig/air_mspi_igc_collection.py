# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _air_mspi_igc_collection.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_air_mspi_igc_collection', [dirname(__file__)])
        except ImportError:
            import _air_mspi_igc_collection
            return _air_mspi_igc_collection
        if fp is not None:
            try:
                _mod = imp.load_module('_air_mspi_igc_collection', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _air_mspi_igc_collection = swig_import_helper()
    del swig_import_helper
else:
    import _air_mspi_igc_collection
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


SHARED_PTR_DISOWN = _air_mspi_igc_collection.SHARED_PTR_DISOWN
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

import geocal_swig.igc_collection
import geocal_swig.generic_object
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
import geocal_swig.orbit
import geocal_swig.observer
import geocal_swig.quaternion_camera
class AirMspiIgcCollection(geocal_swig.igc_collection.IgcCollection):
    """
    This is an IgcCollection for AirMspi.

    C++ includes: air_mspi_igc_collection.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        AirMspiIgcCollection::AirMspiIgcCollection(const boost::shared_ptr< Orbit > &Orb, const boost::shared_ptr<
        MspiCamera > &Cam, const boost::shared_ptr< MspiGimbal > &Gim, const
        boost::shared_ptr< Dem > &D, const std::vector< std::string >
        &L1b1_file_name, const std::string &Swath_to_use="660-I", int
        Dem_resolution=10, const std::string &Base_directory=".")
        This create a AirMspiIgcCollection by directly giving the various
        pieces needed to construct it. 
        """
        _air_mspi_igc_collection.AirMspiIgcCollection_swiginit(self,_air_mspi_igc_collection.new_AirMspiIgcCollection(*args))
    @property
    def number_image(self):
        return self._v_number_image()

    def image_ground_connection(self, *args):
        """
        virtual boost::shared_ptr<ImageGroundConnection> GeoCal::AirMspiIgcCollection::image_ground_connection(int Image_index) const

        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_image_ground_connection(self, *args)

    def have_config(self, *args):
        """
        bool GeoCal::AirMspiIgcCollection::have_config(int Index, const std::string &Keyword) const
        Do we have keyword in configuration value given view number? 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_have_config(self, *args)

    def orbit(self, *args):
        """
        boost::shared_ptr<Orbit> GeoCal::AirMspiIgcCollection::orbit(int Index) const
        Return specific orbit we are using. 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_orbit(self, *args)

    def camera(self, *args):
        """
        boost::shared_ptr<MspiCamera> GeoCal::AirMspiIgcCollection::camera(int Index) const
        Return specific camera we are using, needed for some routines that
        depend on the details of MspiCamera. 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_camera(self, *args)

    def gimbal(self, *args):
        """
        boost::shared_ptr<MspiGimbal> GeoCal::AirMspiIgcCollection::gimbal(int Index) const
        Return specific gimbal we are using, needed for some routines that
        depend on the details of MspiGimbal. 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_gimbal(self, *args)

    def time_table(self, *args):
        """
        boost::shared_ptr<TimeTable> GeoCal::AirMspiIgcCollection::time_table(int Index) const
        Return specific time table we are using, needed for some routines that
        depend on the details of the time table. 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_time_table(self, *args)

    def number_band(self, *args):
        """
        int GeoCal::AirMspiIgcCollection::number_band(int Index) const
        Return number of bands. 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_number_band(self, *args)

    def band(self, *args):
        """
        void GeoCal::AirMspiIgcCollection::band(int Index, int B)
        Set band that we are using. 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_band(self, *args)

    def min_l1b1_line(self, *args):
        """
        int GeoCal::AirMspiIgcCollection::min_l1b1_line(int Index) const
        Return the minimum line in the l1b1 file that we have all the data to
        process.

        This looks at the both the l1b1 file itself and also the coverage of
        the navigation file. It can also be shrunk by the user supplying the
        range in the configuration files. 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_min_l1b1_line(self, *args)

    def max_l1b1_line(self, *args):
        """
        int GeoCal::AirMspiIgcCollection::max_l1b1_line(int Index) const
        Return the maximum line in the l1b1 file that we have all the data to
        process.

        This looks at the both the l1b1 file itself and also the coverage of
        the navigation file. It can also be shrunk by the user supplying the
        range in the configuration files. 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_max_l1b1_line(self, *args)

    def view_number_to_image_index(self, *args):
        """
        int AirMspiIgcCollection::view_number_to_image_index(int View_number) const
        Go from view number (found in the l1b1 table file) to the index number
        matching it. 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_view_number_to_image_index(self, *args)

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _air_mspi_igc_collection.delete_AirMspiIgcCollection
AirMspiIgcCollection.image_ground_connection = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_image_ground_connection,None,AirMspiIgcCollection)
AirMspiIgcCollection.have_config = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_have_config,None,AirMspiIgcCollection)
AirMspiIgcCollection.config_value_double = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_config_value_double,None,AirMspiIgcCollection)
AirMspiIgcCollection.config_value_int = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_config_value_int,None,AirMspiIgcCollection)
AirMspiIgcCollection.config_value_string = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_config_value_string,None,AirMspiIgcCollection)
AirMspiIgcCollection.orbit = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_orbit,None,AirMspiIgcCollection)
AirMspiIgcCollection.camera = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_camera,None,AirMspiIgcCollection)
AirMspiIgcCollection.gimbal = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_gimbal,None,AirMspiIgcCollection)
AirMspiIgcCollection.time_table = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_time_table,None,AirMspiIgcCollection)
AirMspiIgcCollection.number_band = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_number_band,None,AirMspiIgcCollection)
AirMspiIgcCollection.band = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_band,None,AirMspiIgcCollection)
AirMspiIgcCollection.min_l1b1_line = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_min_l1b1_line,None,AirMspiIgcCollection)
AirMspiIgcCollection.max_l1b1_line = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_max_l1b1_line,None,AirMspiIgcCollection)
AirMspiIgcCollection.view_number_to_image_index = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_view_number_to_image_index,None,AirMspiIgcCollection)
AirMspiIgcCollection_swigregister = _air_mspi_igc_collection.AirMspiIgcCollection_swigregister
AirMspiIgcCollection_swigregister(AirMspiIgcCollection)



