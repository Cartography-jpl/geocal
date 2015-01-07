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
class AirMspiIgcCollection(geocal_swig.igc_collection.IgcCollection):
    """
    This is an IgcCollection for AirMspi.

    C++ includes: air_mspi_igc_collection.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        AirMspiIgcCollection::AirMspiIgcCollection(const std::string &Master_config_file, const std::string
        &Orbit_file_name, const std::string &L1b1_table, const std::string
        &Base_directory=".")
        This creates a AirMspiIgcCollection by reading the given master config
        file.

        Various files found in the input files can have relative paths. You
        can specify the base directory these paths are relative to, the
        default is the current directory. 
        """
        _air_mspi_igc_collection.AirMspiIgcCollection_swiginit(self,_air_mspi_igc_collection.new_AirMspiIgcCollection(*args))
    @property
    def number_image(self):
        return self._v_number_image()

    def image_ground_connection(self, *args):
        """
        boost::shared_ptr< ImageGroundConnection > AirMspiIgcCollection::image_ground_connection(int Image_index) const

        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_image_ground_connection(self, *args)

    def view_number(self, *args):
        """
        int GeoCal::AirMspiIgcCollection::view_number(int Index) const
        Return view number. 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_view_number(self, *args)

    def view_name(self, *args):
        """
        std::string GeoCal::AirMspiIgcCollection::view_name(int Index) const
        Return view name. 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_view_name(self, *args)

    def view_time(self, *args):
        """
        std::string GeoCal::AirMspiIgcCollection::view_time(int Index) const
        Return view name. 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_view_time(self, *args)

    def l1b1_file_name(self, *args):
        """
        std::string AirMspiIgcCollection::l1b1_file_name(int Index) const
        Return L1B1 file name. 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_l1b1_file_name(self, *args)

    def l1b1_granule_id(self, *args):
        """
        std::string GeoCal::AirMspiIgcCollection::l1b1_granule_id(int Index) const
        Return l1b1 granule id. 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_l1b1_granule_id(self, *args)

    def geolocation_stage(self, *args):
        """
        std::string GeoCal::AirMspiIgcCollection::geolocation_stage(int Index) const
        Return geolocation stage. 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_geolocation_stage(self, *args)

    def target_type(self, *args):
        """
        std::string GeoCal::AirMspiIgcCollection::target_type(int Index) const
        Return target type. 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_target_type(self, *args)

    def min_l1b1_line(self, *args):
        """
        int GeoCal::AirMspiIgcCollection::min_l1b1_line(int Index) const
        Return min L1B1 line. 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_min_l1b1_line(self, *args)

    def max_l1b1_line(self, *args):
        """
        int GeoCal::AirMspiIgcCollection::max_l1b1_line(int Index) const
        Return max L1B1 line. 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_max_l1b1_line(self, *args)

    def view_resolution(self, *args):
        """
        double GeoCal::AirMspiIgcCollection::view_resolution(int Index) const
        Return view resolution.

        Note this is metadata passed in as a configuration, not the actual
        resolution of the l1b1 data on the ground (use ImageGroundConnection
        resolution_meter or footprint_resolution for that). 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection_view_resolution(self, *args)

    def _v_max_view_resolution(self):
        """
        double GeoCal::AirMspiIgcCollection::max_view_resolution() const
        Return maximum view resolution. 
        """
        return _air_mspi_igc_collection.AirMspiIgcCollection__v_max_view_resolution(self)

    @property
    def max_view_resolution(self):
        return self._v_max_view_resolution()

    def _v_l1b2_hdf_chunk_size_x(self):
        """
        int GeoCal::AirMspiIgcCollection::l1b2_hdf_chunk_size_x() const
        Return HDF chunk size that we've been requested to use for the L1B2.

        """
        return _air_mspi_igc_collection.AirMspiIgcCollection__v_l1b2_hdf_chunk_size_x(self)

    @property
    def l1b2_hdf_chunk_size_x(self):
        return self._v_l1b2_hdf_chunk_size_x()

    def _v_l1b2_hdf_chunk_size_y(self):
        """
        int GeoCal::AirMspiIgcCollection::l1b2_hdf_chunk_size_y() const

        """
        return _air_mspi_igc_collection.AirMspiIgcCollection__v_l1b2_hdf_chunk_size_y(self)

    @property
    def l1b2_hdf_chunk_size_y(self):
        return self._v_l1b2_hdf_chunk_size_y()

    __swig_destroy__ = _air_mspi_igc_collection.delete_AirMspiIgcCollection
AirMspiIgcCollection.image_ground_connection = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_image_ground_connection,None,AirMspiIgcCollection)
AirMspiIgcCollection.view_number = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_view_number,None,AirMspiIgcCollection)
AirMspiIgcCollection.view_name = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_view_name,None,AirMspiIgcCollection)
AirMspiIgcCollection.view_time = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_view_time,None,AirMspiIgcCollection)
AirMspiIgcCollection.l1b1_file_name = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_l1b1_file_name,None,AirMspiIgcCollection)
AirMspiIgcCollection.l1b1_granule_id = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_l1b1_granule_id,None,AirMspiIgcCollection)
AirMspiIgcCollection.geolocation_stage = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_geolocation_stage,None,AirMspiIgcCollection)
AirMspiIgcCollection.target_type = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_target_type,None,AirMspiIgcCollection)
AirMspiIgcCollection.min_l1b1_line = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_min_l1b1_line,None,AirMspiIgcCollection)
AirMspiIgcCollection.max_l1b1_line = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_max_l1b1_line,None,AirMspiIgcCollection)
AirMspiIgcCollection.view_resolution = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection_view_resolution,None,AirMspiIgcCollection)
AirMspiIgcCollection._v_max_view_resolution = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection__v_max_view_resolution,None,AirMspiIgcCollection)
AirMspiIgcCollection._v_l1b2_hdf_chunk_size_x = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection__v_l1b2_hdf_chunk_size_x,None,AirMspiIgcCollection)
AirMspiIgcCollection._v_l1b2_hdf_chunk_size_y = new_instancemethod(_air_mspi_igc_collection.AirMspiIgcCollection__v_l1b2_hdf_chunk_size_y,None,AirMspiIgcCollection)
AirMspiIgcCollection_swigregister = _air_mspi_igc_collection.AirMspiIgcCollection_swigregister
AirMspiIgcCollection_swigregister(AirMspiIgcCollection)



