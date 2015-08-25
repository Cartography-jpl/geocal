# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _igc_multiple_pass.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_igc_multiple_pass', [dirname(__file__)])
        except ImportError:
            import _igc_multiple_pass
            return _igc_multiple_pass
        if fp is not None:
            try:
                _mod = imp.load_module('_igc_multiple_pass', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _igc_multiple_pass = swig_import_helper()
    del swig_import_helper
else:
    import _igc_multiple_pass
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


SHARED_PTR_DISOWN = _igc_multiple_pass.SHARED_PTR_DISOWN
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
class IgcMultiplePass(geocal_swig.igc_collection.IgcCollection,geocal_swig.with_parameter.WithParameterNested):
    """
    This is a IgcCollection that stitches a collection of IgcCollection
    together.

    The use case is multiple passes , e.g., AirMSPI having multiple
    collections in a day. Typically you will have a shared set of
    parameters that you are jointly fitting for, e.g., fitting for the
    camera parameters over multiple passes of AirMSPI. This class is a
    WithParameterNested, but it doesn't automatically add anything into
    the parameter list. The normal case is you would create the
    IgcMultiplePass, and then add in the objects you want. For
    example:boost::shared_ptr<IgcCollection> igc_pass1(new
    AirMspiIgcCollection (orb1, cam, gim, Dem, l1b1_file_name1));
    boost::shared_ptr<IgcCollection> igc_pass2(new AirMspiIgcCollection
    (orb2, cam, gim, Dem, l1b1_file_name2)); IgcMultiplePass igc;
    igc.add_igc(igc_pass1); igc.add_igc(igc_pass2); igc.add_object(cam);
    igc.add_object(gim);

    The images are stacked together. So if the first IgcCollection Igc1
    has 5 image, the second Igc2 6, the third Igc3 2 then in the
    IgcMultiplePass image 0 is 0th image in Igc1, image 5 is the 0th image
    in Igc2, image 12 is image 1 of Igc3. The total number of images is
    13.

    C++ includes: igc_multiple_pass.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::IgcMultiplePass::IgcMultiplePass(const std::vector< boost::shared_ptr< IgcCollection > > &Igc_list)
        Constructor that takes a list of IgcCollection to initialize with. 
        """
        _igc_multiple_pass.IgcMultiplePass_swiginit(self,_igc_multiple_pass.new_IgcMultiplePass(*args))
    def add_igc(self, *args):
        """
        void GeoCal::IgcMultiplePass::add_igc(const boost::shared_ptr< IgcCollection > Igccol)

        """
        return _igc_multiple_pass.IgcMultiplePass_add_igc(self, *args)

    def image_ground_connection(self, *args):
        """
        boost::shared_ptr< ImageGroundConnection > IgcMultiplePass::image_ground_connection(int Image_index) const

        """
        return _igc_multiple_pass.IgcMultiplePass_image_ground_connection(self, *args)

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _igc_multiple_pass.delete_IgcMultiplePass
IgcMultiplePass.add_igc = new_instancemethod(_igc_multiple_pass.IgcMultiplePass_add_igc,None,IgcMultiplePass)
IgcMultiplePass.image_ground_connection = new_instancemethod(_igc_multiple_pass.IgcMultiplePass_image_ground_connection,None,IgcMultiplePass)
IgcMultiplePass_swigregister = _igc_multiple_pass.IgcMultiplePass_swigregister
IgcMultiplePass_swigregister(IgcMultiplePass)



