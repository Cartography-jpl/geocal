# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _igc_collection.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_igc_collection', [dirname(__file__)])
        except ImportError:
            import _igc_collection
            return _igc_collection
        if fp is not None:
            try:
                _mod = imp.load_module('_igc_collection', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _igc_collection = swig_import_helper()
    del swig_import_helper
else:
    import _igc_collection
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
    __swig_destroy__ = _igc_collection.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_igc_collection.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_igc_collection.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_igc_collection.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_igc_collection.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_igc_collection.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_igc_collection.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_igc_collection.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_igc_collection.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_igc_collection.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_igc_collection.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_igc_collection.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_igc_collection.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_igc_collection.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_igc_collection.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_igc_collection.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_igc_collection.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _igc_collection.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_igc_collection.SHARED_PTR_DISOWN_swigconstant(_igc_collection)
SHARED_PTR_DISOWN = _igc_collection.SHARED_PTR_DISOWN

import os

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

def _new_from_serialization_dir(dir, data):
    curdir = os.getcwd()
    try:
      os.chdir(dir)
      return geocal_swig.serialize_read_binary(data)
    finally:
      os.chdir(curdir)


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
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
class IgcCollection(geocal_swig.with_parameter.WithParameter):
    """

    This is a collection of ImageGroundConnection.

    C++ includes: igc_collection.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    __swig_destroy__ = _igc_collection.delete_IgcCollection

    def _v_number_image(self):
        """

        virtual int GeoCal::IgcCollection::number_image() const

        """
        return _igc_collection.IgcCollection__v_number_image(self)


    @property
    def number_image(self):
        return self._v_number_image()


    def collinearity_residual(self, Image_index, Gc, Ic_actual):
        """

        virtual blitz::Array<double, 1> GeoCal::IgcCollection::collinearity_residual(int Image_index, const GroundCoordinate &Gc, const ImageCoordinate
        &Ic_actual) const
        Return collinearity residual for given Image_index (see
        ImageGroundConnection for details on this. 
        """
        return _igc_collection.IgcCollection_collinearity_residual(self, Image_index, Gc, Ic_actual)


    def collinearity_residual_jacobian(self, Image_index, Gc, Ic_actual):
        """

        virtual blitz::Array<double, 2> GeoCal::IgcCollection::collinearity_residual_jacobian(int Image_index, const GroundCoordinate &Gc, const ImageCoordinate
        &Ic_actual) const
        Return jacobian of collinearity_residual.

        The parameters for this class should already have AutoDerivative
        extended for the jacobian (e.g., call add_identity_gradient()).

        We add the derivative wrt the CartesianFixed coordinates of the
        Ground_coor (X, Y, Z in that order), at the end of the Jacobian. So
        the total Jacobian is 2 x (number parameter + 3). 
        """
        return _igc_collection.IgcCollection_collinearity_residual_jacobian(self, Image_index, Gc, Ic_actual)


    def __ground_coordinate(self, Image_index, Ic):
        """

        virtual boost::shared_ptr<GroundCoordinate> GeoCal::IgcCollection::ground_coordinate(int Image_index, const ImageCoordinate &Ic) const
        Return ground coordinate that goes with a particular image coordinate.

        """
        return _igc_collection.IgcCollection___ground_coordinate(self, Image_index, Ic)


    def ground_coordinate_dem(self, Image_index, Ic, D):
        """

        virtual boost::shared_ptr<GroundCoordinate> GeoCal::IgcCollection::ground_coordinate_dem(int Image_index, const ImageCoordinate &Ic, const Dem &D) const
        Return ground coordinate that goes with a particular image coordinate.

        This version supplies a Dem to use. 
        """
        return _igc_collection.IgcCollection_ground_coordinate_dem(self, Image_index, Ic, D)


    def dem(self, Image_index):
        """

        virtual boost::shared_ptr<Dem> GeoCal::IgcCollection::dem(int Image_index) const
        Dem used by ground_coordinate. 
        """
        return _igc_collection.IgcCollection_dem(self, Image_index)


    def image_coordinate(self, Image_index, Gc):
        """

        virtual ImageCoordinate GeoCal::IgcCollection::image_coordinate(int Image_index, const GroundCoordinate &Gc) const
        Return image coordinate that goes with a particular GroundCoordinate.

        For some types of ImageGroundConnection, we might not be able to
        calculate image_coordinate for all values (e.g., Ipi might fail). In
        those cases, we will throw a ImageGroundConnectionFailed exception.
        This means that nothing is wrong, other than that we can't calculate
        the image_coordinate. Callers can catch this exception if they have
        some way of handling no image coordinate data. 
        """
        return _igc_collection.IgcCollection_image_coordinate(self, Image_index, Gc)


    def image_coordinate_jac_parm(self, Image_index, Gc):
        """

        virtual blitz::Array<double, 2> GeoCal::IgcCollection::image_coordinate_jac_parm(int Image_index, const CartesianFixed &Gc) const
        Return the Jacobian of the image coordinates with respect to the
        parameters. 
        """
        return _igc_collection.IgcCollection_image_coordinate_jac_parm(self, Image_index, Gc)


    def image_coordinate_jac_parm_fd(self, Image_index, Gc, Pstep):
        """

        blitz::Array< double, 2 > IgcCollection::image_coordinate_jac_parm_fd(int Image_index, const CartesianFixed &Gc, const blitz::Array<
        double, 1 > Pstep) const
        Return the Jacobian of the image coordinates with respect to the
        parameters, calculated by taking a finite difference. 
        """
        return _igc_collection.IgcCollection_image_coordinate_jac_parm_fd(self, Image_index, Gc, Pstep)


    def image_coordinate_jac_cf(self, Image_index, Gc):
        """

        virtual blitz::Array<double, 2> GeoCal::IgcCollection::image_coordinate_jac_cf(int Image_index, const CartesianFixed &Gc) const
        Return the Jacobian of the image coordinates with respect to the X, Y,
        and Z components of the CartesianFixed ground location. 
        """
        return _igc_collection.IgcCollection_image_coordinate_jac_cf(self, Image_index, Gc)


    def image_coordinate_jac_cf_fd(self, Image_index, Gc, Step_size):
        """

        virtual blitz::Array<double, 2> GeoCal::IgcCollection::image_coordinate_jac_cf_fd(int Image_index, const CartesianFixed &Gc, double Step_size) const
        Return the Jacobian of the image coordinates with respect to the X, Y,
        and Z components of the CartesianFixed ground location.

        Calculated as a finite difference with the given step size. 
        """
        return _igc_collection.IgcCollection_image_coordinate_jac_cf_fd(self, Image_index, Gc, Step_size)


    def title(self, Image_index):
        """

        virtual std::string GeoCal::IgcCollection::title(int Image_index) const
        Title that we can use to describe the image.

        This can be any string that is useful as a label. 
        """
        return _igc_collection.IgcCollection_title(self, Image_index)


    def image(self, Image_index):
        """

        virtual boost::shared_ptr<RasterImage> GeoCal::IgcCollection::image(int Image_index) const
        Underlying image (if present) 
        """
        return _igc_collection.IgcCollection_image(self, Image_index)


    def image_ground_connection(self, Image_index):
        """

        virtual boost::shared_ptr<ImageGroundConnection> GeoCal::IgcCollection::image_ground_connection(int Image_index) const  =0
        Image ground connection for given image index. 
        """
        return _igc_collection.IgcCollection_image_ground_connection(self, Image_index)


    def subset(self, Index_set):
        """

        virtual boost::shared_ptr<IgcCollection> GeoCal::IgcCollection::subset(const std::vector< int > &Index_set) const  =0
        Return IgcCollection for a subset of the data. 
        """
        return _igc_collection.IgcCollection_subset(self, Index_set)


    def _v_parameter(self, *args):
        """

        virtual void GeoCal::IgcCollection::parameter(const blitz::Array< double, 1 > &P)

        """
        return _igc_collection.IgcCollection__v_parameter(self, *args)


    @property
    def parameter(self):
        return self._v_parameter()

    @parameter.setter
    def parameter(self, value):
      self._v_parameter(value)


    def _v_parameter_with_derivative(self, *args):
        """

        virtual void GeoCal::IgcCollection::parameter_with_derivative(const ArrayAd< double, 1 > &P)

        """
        return _igc_collection.IgcCollection__v_parameter_with_derivative(self, *args)


    @property
    def parameter_with_derivative(self):
        return self._v_parameter_with_derivative()

    @parameter_with_derivative.setter
    def parameter_with_derivative(self, value):
      self._v_parameter_with_derivative(value)


    @property
    def parameter_name(self):
        return self._v_parameter_name()


    @property
    def parameter_subset(self):
        return self._v_parameter_subset()

    @parameter_subset.setter
    def parameter_subset(self, value):
      self._v_parameter_subset(value)


    @property
    def parameter_with_derivative_subset(self):
        return self._v_parameter_with_derivative_subset()

    @parameter_with_derivative_subset.setter
    def parameter_with_derivative_subset(self, value):
      self._v_parameter_with_derivative_subset(value)


    @property
    def parameter_name_subset(self):
        return self._v_parameter_name_subset()


    @property
    def parameter_mask(self):
        return self._v_parameter_mask()


    def ground_coordinate(self, image_index, ic, dem = None):
      '''Return ground coordinate for the given image coordinate. You can specify
       a dem to use, or we use the dem associated with the class.'''
      if(dem):
        return self.ground_coordinate_dem(image_index, ic, dem)
      else:
        return self.__ground_coordinate(image_index, ic)


    def __init__(self):
        if self.__class__ == IgcCollection:
            _self = None
        else:
            _self = self
        _igc_collection.IgcCollection_swiginit(self, _igc_collection.new_IgcCollection(_self, ))
    def __disown__(self):
        self.this.disown()
        _igc_collection.disown_IgcCollection(self)
        return weakref_proxy(self)
IgcCollection._v_number_image = new_instancemethod(_igc_collection.IgcCollection__v_number_image, None, IgcCollection)
IgcCollection.collinearity_residual = new_instancemethod(_igc_collection.IgcCollection_collinearity_residual, None, IgcCollection)
IgcCollection.collinearity_residual_jacobian = new_instancemethod(_igc_collection.IgcCollection_collinearity_residual_jacobian, None, IgcCollection)
IgcCollection.__ground_coordinate = new_instancemethod(_igc_collection.IgcCollection___ground_coordinate, None, IgcCollection)
IgcCollection.ground_coordinate_dem = new_instancemethod(_igc_collection.IgcCollection_ground_coordinate_dem, None, IgcCollection)
IgcCollection.dem = new_instancemethod(_igc_collection.IgcCollection_dem, None, IgcCollection)
IgcCollection.image_coordinate = new_instancemethod(_igc_collection.IgcCollection_image_coordinate, None, IgcCollection)
IgcCollection.image_coordinate_jac_parm = new_instancemethod(_igc_collection.IgcCollection_image_coordinate_jac_parm, None, IgcCollection)
IgcCollection.image_coordinate_jac_parm_fd = new_instancemethod(_igc_collection.IgcCollection_image_coordinate_jac_parm_fd, None, IgcCollection)
IgcCollection.image_coordinate_jac_cf = new_instancemethod(_igc_collection.IgcCollection_image_coordinate_jac_cf, None, IgcCollection)
IgcCollection.image_coordinate_jac_cf_fd = new_instancemethod(_igc_collection.IgcCollection_image_coordinate_jac_cf_fd, None, IgcCollection)
IgcCollection.title = new_instancemethod(_igc_collection.IgcCollection_title, None, IgcCollection)
IgcCollection.image = new_instancemethod(_igc_collection.IgcCollection_image, None, IgcCollection)
IgcCollection.image_ground_connection = new_instancemethod(_igc_collection.IgcCollection_image_ground_connection, None, IgcCollection)
IgcCollection.subset = new_instancemethod(_igc_collection.IgcCollection_subset, None, IgcCollection)
IgcCollection.__str__ = new_instancemethod(_igc_collection.IgcCollection___str__, None, IgcCollection)
IgcCollection._v_parameter = new_instancemethod(_igc_collection.IgcCollection__v_parameter, None, IgcCollection)
IgcCollection._v_parameter_with_derivative = new_instancemethod(_igc_collection.IgcCollection__v_parameter_with_derivative, None, IgcCollection)
IgcCollection._v_parameter_name = new_instancemethod(_igc_collection.IgcCollection__v_parameter_name, None, IgcCollection)
IgcCollection._v_parameter_subset = new_instancemethod(_igc_collection.IgcCollection__v_parameter_subset, None, IgcCollection)
IgcCollection._v_parameter_with_derivative_subset = new_instancemethod(_igc_collection.IgcCollection__v_parameter_with_derivative_subset, None, IgcCollection)
IgcCollection._v_parameter_name_subset = new_instancemethod(_igc_collection.IgcCollection__v_parameter_name_subset, None, IgcCollection)
IgcCollection._v_parameter_mask = new_instancemethod(_igc_collection.IgcCollection__v_parameter_mask, None, IgcCollection)
IgcCollection_swigregister = _igc_collection.IgcCollection_swigregister
IgcCollection_swigregister(IgcCollection)



