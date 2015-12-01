# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _material_detect.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_material_detect', [dirname(__file__)])
        except ImportError:
            import _material_detect
            return _material_detect
        if fp is not None:
            try:
                _mod = imp.load_module('_material_detect', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _material_detect = swig_import_helper()
    del swig_import_helper
else:
    import _material_detect
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
    __swig_destroy__ = _material_detect.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_material_detect.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_material_detect.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_material_detect.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_material_detect.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_material_detect.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_material_detect.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_material_detect.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_material_detect.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_material_detect.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_material_detect.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_material_detect.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_material_detect.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_material_detect.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_material_detect.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_material_detect.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_material_detect.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _material_detect.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_material_detect.SHARED_PTR_DISOWN_swigconstant(_material_detect)
SHARED_PTR_DISOWN = _material_detect.SHARED_PTR_DISOWN

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

import geocal_swig.calc_raster
import geocal_swig.raster_image_variable
import geocal_swig.raster_image
import geocal_swig.generic_object
class MaterialDetect(geocal_swig.calc_raster.CalcRaster):
    """

    This class is used to do change detection and identify the material
    for pixels that have changed.

    This duplicates what was done in the VICAR proc "detwvpan", and in
    particular the "f2ratio" process.

    We read a IBIS file that describes the thresholds for a number of
    material classes. For each material class, we do the following:

    First, the difference in the pan bands is compared to a threshold. The
    difference is normally calculated by DoughnutAverage, using the
    pandif_raster_image. But something else could be used, all this class
    cares about is having a difference it can compare against a threshold.

    Compare pan data to a shadow threshold. This masks at very dark pixels
    that we assume are in shadow.

    For each point, we calculate the required ratios between the
    multispectral bands, take the difference with the supplied class mean
    and divide by the class sigma.

    We sum the abs value of for each of the band ratios divided by the
    number of band ratios (i.e, we use a L1 norm). This is compared
    against a second threshold.

    We may have more than one class that passes the second threshold. In
    that case, we sort the classes first by a class priority (with the
    lower number being selected first). For ties, we then pick the class
    that has the smallest difference norm.

    C++ includes: material_detect.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Pan_data, Pan_diff, Mulspect, Pan_diff_threshold, Spectral_diff_threshold, Class_priority, Ibis_fname, Pan_shadow_threshold):
        """

        MaterialDetect::MaterialDetect(const boost::shared_ptr< RasterImage > &Pan_data, const
        boost::shared_ptr< RasterImage > &Pan_diff, const boost::shared_ptr<
        RasterImageMultiBand > &Mulspect, const blitz::Array< double, 1 >
        &Pan_diff_threshold, const blitz::Array< double, 1 >
        &Spectral_diff_threshold, const blitz::Array< int, 1 >
        &Class_priority, const std::string &Ibis_fname, double
        Pan_shadow_threshold)
        Constructor.

        This takes the pan difference band (normally pandif_raster_image from
        DoughnutAverage), the multispectral bands, the threshold for each
        material for the pan difference and spectral difference, the class
        priority for eahc class, and the name of the IBIS file to get the
        material information.

        The IBIS file should have at least 5 columns (which is all that we
        read). The first two columns should be full word values, and are the
        band indices (1 based, rather than the 0 based we use elsewhere). The
        third should be double (for no good reason, it just is), which is the
        class id. The fourth and fifth are also double, and are the mean and
        sigma for that band ratio.

        In generate, the class ID doesn't start from 0. We find the minimum
        value in the table, and use that as the index into Pan_diff_threshold
        etc. (so if first id is 8881 then the threshold for 8881 is
        Pan_diff_threshold[0] and 8885 is Pan_diff_threshold[4]). Obviously we
        could have just used a map instead, but this convention fits better
        with the current way we supply this values in the Shiva scripts. 
        """
        _material_detect.MaterialDetect_swiginit(self, _material_detect.new_MaterialDetect(Pan_data, Pan_diff, Mulspect, Pan_diff_threshold, Spectral_diff_threshold, Class_priority, Ibis_fname, Pan_shadow_threshold))

    def _v_closest_material_raster_image(self):
        """

        boost::shared_ptr< RasterImage > MaterialDetect::closest_material_raster_image() const
        This returns an image that gives the distance to the closest material
        (whose class id will vary from pixel to pixel).

        By convention we multiple this difference by 100 so it can be viewed
        more easily in xvd (which prefers integers). 
        """
        return _material_detect.MaterialDetect__v_closest_material_raster_image(self)


    @property
    def closest_material_raster_image(self):
        return self._v_closest_material_raster_image()


    @property
    def material_raster_image(self):
        return self._v_material_raster_image()


    def _v_material_raster_image(self, *args):
        """

        boost::shared_ptr< RasterImage > MaterialDetect::material_raster_image(int Class_id) const
        This returns a set image that gives the distance to the given
        material.

        By convention we multiple this difference by 100 so it can be viewed
        more easily in xvd (which prefers integers). 
        """
        return _material_detect.MaterialDetect__v_material_raster_image(self, *args)


    def closest_material_dif(self, Lstart, Sstart, Number_line, Number_sample):
        """

        blitz::Array< double, 2 > MaterialDetect::closest_material_dif(int Lstart, int Sstart, int Number_line, int Number_sample) const
        Calculate distance for every point from the nearest material.

        By convention we multiple this by 100. 
        """
        return _material_detect.MaterialDetect_closest_material_dif(self, Lstart, Sstart, Number_line, Number_sample)


    def material_dif(self, Lstart, Sstart, Number_line, Number_sample, Class_id):
        """

        blitz::Array< double, 2 > MaterialDetect::material_dif(int Lstart, int Sstart, int Number_line, int Number_sample, int
        Class_id) const
        Calculate the distance for every point from the given material.

        By convention we multiple this by 100. 
        """
        return _material_detect.MaterialDetect_material_dif(self, Lstart, Sstart, Number_line, Number_sample, Class_id)

    __swig_destroy__ = _material_detect.delete_MaterialDetect
MaterialDetect._v_closest_material_raster_image = new_instancemethod(_material_detect.MaterialDetect__v_closest_material_raster_image, None, MaterialDetect)
MaterialDetect._v_material_raster_image = new_instancemethod(_material_detect.MaterialDetect__v_material_raster_image, None, MaterialDetect)
MaterialDetect.closest_material_dif = new_instancemethod(_material_detect.MaterialDetect_closest_material_dif, None, MaterialDetect)
MaterialDetect.material_dif = new_instancemethod(_material_detect.MaterialDetect_material_dif, None, MaterialDetect)
MaterialDetect_swigregister = _material_detect.MaterialDetect_swigregister
MaterialDetect_swigregister(MaterialDetect)



