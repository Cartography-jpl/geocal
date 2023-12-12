# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _rsm_grid.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_rsm_grid')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_rsm_grid')
    _rsm_grid = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_rsm_grid', [dirname(__file__)])
        except ImportError:
            import _rsm_grid
            return _rsm_grid
        try:
            _mod = imp.load_module('_rsm_grid', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _rsm_grid = swig_import_helper()
    del swig_import_helper
else:
    import _rsm_grid
del _swig_python_version_info

try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

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


def _swig_getattr(self, class_type, name):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    raise AttributeError("'%s' object has no attribute '%s'" % (class_type.__name__, name))


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except __builtin__.Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)


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
except __builtin__.Exception:
    weakref_proxy = lambda x: x


SWIG_MODULE_ALREADY_DONE = _rsm_grid.SWIG_MODULE_ALREADY_DONE
class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _rsm_grid.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_rsm_grid.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_rsm_grid.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_rsm_grid.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_rsm_grid.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_rsm_grid.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_rsm_grid.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_rsm_grid.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_rsm_grid.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_rsm_grid.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_rsm_grid.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_rsm_grid.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_rsm_grid.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_rsm_grid.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_rsm_grid.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_rsm_grid.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_rsm_grid.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _rsm_grid.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _rsm_grid.SHARED_PTR_DISOWN

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
    return geocal_swig.serialize_function.serialize_read_binary(data)

def _new_from_serialization_dir(dir, data):
    curdir = os.getcwd()
    try:
      os.chdir(dir)
      return geocal_swig.serialize_function.serialize_read_binary(data)
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

import geocal_swig.rsm_base
import geocal_swig.generic_object
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
class RsmGrid(geocal_swig.rsm_base.RsmBase):
    """

    Use a interpolation grid to map from ground to image.

    Right now we only do a linear interpolation. The RSM documentation
    suggests doing Lagrange interpolation of various orders, we can
    implement that in the future.

    This can contain missing data, either because it is outside of the
    image or because the initial ImageGroundConnection failed for a
    particular point. We record these as NaNs.

    Right now this just carries through, we should probably add some kind
    of a status for image_coordinate.

    We plan on using the same grid for a full ground to image map, or for
    doing a correction.

    C++ includes: rsm_grid.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        GeoCal::RsmGrid::RsmGrid(int N_x, int N_y, int N_z, bool Ignore_igc_error_in_fit=false, int
        Total_number_row_digit=11, int Total_number_col_digit=11, int
        Number_fractional_row_digit=2, int Number_fractional_col_digit=2, int
        Row_section_number=1, int Col_section_number=1, const std::string
        &Image_identifier="", const std::string
        &Rsm_support_data_edition="fake-1")

        """
        _rsm_grid.RsmGrid_swiginit(self, _rsm_grid.new_RsmGrid(*args))

    def fit_corr(self, IGc, Cconv, Rb):
        """

        void RsmGrid::fit_corr(const ImageGroundConnection &IGc, const CoordinateConverter &Cconv,
        const RsmBase &Rb)
        Generate a RsmGrid that gives a correction to another RsmBase. 
        """
        return _rsm_grid.RsmGrid_fit_corr(self, IGc, Cconv, Rb)


    def _v_ignore_igc_error_in_fit(self, *args):
        """

        void GeoCal::RsmGrid::ignore_igc_error_in_fit(bool V)
        If true, ignore igc errors in fit. 
        """
        return _rsm_grid.RsmGrid__v_ignore_igc_error_in_fit(self, *args)


    @property
    def ignore_igc_error_in_fit(self):
        return self._v_ignore_igc_error_in_fit()

    @ignore_igc_error_in_fit.setter
    def ignore_igc_error_in_fit(self, value):
      self._v_ignore_igc_error_in_fit(value)


    def _v_line_grid(self):
        """

        const blitz::Array<double, 3>& GeoCal::RsmGrid::line_grid() const
        Grid of Line values. 
        """
        return _rsm_grid.RsmGrid__v_line_grid(self)


    @property
    def line_grid(self):
        return self._v_line_grid()


    def _v_sample_grid(self):
        """

        const blitz::Array<double, 3>& GeoCal::RsmGrid::sample_grid() const
        Grid of sample values. 
        """
        return _rsm_grid.RsmGrid__v_sample_grid(self)


    @property
    def sample_grid(self):
        return self._v_sample_grid()


    def image_coordinate(self, *args):
        """

        virtual blitz::Array<double, 4> GeoCal::RsmGrid::image_coordinate(const blitz::Array< double, 3 > &X, const blitz::Array< double, 3 >
        &Y, const blitz::Array< double, 3 > &Z) const

        """
        return _rsm_grid.RsmGrid_image_coordinate(self, *args)


    def image_coordinate_jacobian(self, *args):
        """

        virtual blitz::Array<double, 2> GeoCal::RsmGrid::image_coordinate_jacobian(double X, double Y, double Z) const

        """
        return _rsm_grid.RsmGrid_image_coordinate_jacobian(self, *args)


    def number_x(self, Zindex):
        """

        int GeoCal::RsmGrid::number_x(int Zindex) const
        Number of X values in grid.

        This can potentially depend on the z axis value. 
        """
        return _rsm_grid.RsmGrid_number_x(self, Zindex)


    def number_y(self, Zindex):
        """

        int GeoCal::RsmGrid::number_y(int Zindex) const
        Number of Y values in grid.

        This can potentially depend on the z axis value. 
        """
        return _rsm_grid.RsmGrid_number_y(self, Zindex)


    def x_offset(self, Zindex):
        """

        int GeoCal::RsmGrid::x_offset(int Zindex) const
        Offset in X pixels of particular z_index grid relative to the initial
        grid. 
        """
        return _rsm_grid.RsmGrid_x_offset(self, Zindex)


    def y_offset(self, Zindex):
        """

        int GeoCal::RsmGrid::y_offset(int Zindex) const
        Offset in Y pixels of particular z_index grid relative to the initial
        grid. 
        """
        return _rsm_grid.RsmGrid_y_offset(self, Zindex)


    def _v_number_z(self):
        """

        int GeoCal::RsmGrid::number_z() const
        Number of Z values in grid. 
        """
        return _rsm_grid.RsmGrid__v_number_z(self)


    @property
    def number_z(self):
        return self._v_number_z()


    def _v_x_start(self):
        """

        double GeoCal::RsmGrid::x_start() const
        First X value in grid.

        This is for the first index, use x_offset for other z index values. 
        """
        return _rsm_grid.RsmGrid__v_x_start(self)


    @property
    def x_start(self):
        return self._v_x_start()


    def _v_x_delta(self):
        """

        double GeoCal::RsmGrid::x_delta() const
        Spacing of X in grid. 
        """
        return _rsm_grid.RsmGrid__v_x_delta(self)


    @property
    def x_delta(self):
        return self._v_x_delta()


    def _v_y_start(self):
        """

        double GeoCal::RsmGrid::y_start() const
        First Y value in grid. 
        """
        return _rsm_grid.RsmGrid__v_y_start(self)


    @property
    def y_start(self):
        return self._v_y_start()


    def _v_y_delta(self):
        """

        double GeoCal::RsmGrid::y_delta() const
        Spacing of Y in grid. 
        """
        return _rsm_grid.RsmGrid__v_y_delta(self)


    @property
    def y_delta(self):
        return self._v_y_delta()


    def _v_z_start(self):
        """

        double GeoCal::RsmGrid::z_start() const
        First Z value in grid. 
        """
        return _rsm_grid.RsmGrid__v_z_start(self)


    @property
    def z_start(self):
        return self._v_z_start()


    def _v_z_delta(self):
        """

        double GeoCal::RsmGrid::z_delta() const
        Spacing of Z in grid. 
        """
        return _rsm_grid.RsmGrid__v_z_delta(self)


    @property
    def z_delta(self):
        return self._v_z_delta()


    def _v_total_number_row_digit(self, *args):
        """

        void GeoCal::RsmGrid::total_number_row_digit(int v)

        """
        return _rsm_grid.RsmGrid__v_total_number_row_digit(self, *args)


    @property
    def total_number_row_digit(self):
        return self._v_total_number_row_digit()

    @total_number_row_digit.setter
    def total_number_row_digit(self, value):
      self._v_total_number_row_digit(value)


    def _v_total_number_col_digit(self, *args):
        """

        void GeoCal::RsmGrid::total_number_col_digit(int v)

        """
        return _rsm_grid.RsmGrid__v_total_number_col_digit(self, *args)


    @property
    def total_number_col_digit(self):
        return self._v_total_number_col_digit()

    @total_number_col_digit.setter
    def total_number_col_digit(self, value):
      self._v_total_number_col_digit(value)


    def _v_number_fractional_row_digit(self, *args):
        """

        void GeoCal::RsmGrid::number_fractional_row_digit(int v)

        """
        return _rsm_grid.RsmGrid__v_number_fractional_row_digit(self, *args)


    @property
    def number_fractional_row_digit(self):
        return self._v_number_fractional_row_digit()

    @number_fractional_row_digit.setter
    def number_fractional_row_digit(self, value):
      self._v_number_fractional_row_digit(value)


    def _v_number_fractional_col_digit(self, *args):
        """

        void GeoCal::RsmGrid::number_fractional_col_digit(int v)

        """
        return _rsm_grid.RsmGrid__v_number_fractional_col_digit(self, *args)


    @property
    def number_fractional_col_digit(self):
        return self._v_number_fractional_col_digit()

    @number_fractional_col_digit.setter
    def number_fractional_col_digit(self, value):
      self._v_number_fractional_col_digit(value)


    def extrapolate_x_direction(self):
        """

        void RsmGrid::extrapolate_x_direction()
        Extrapolate data in the x/sample direction. 
        """
        return _rsm_grid.RsmGrid_extrapolate_x_direction(self)


    def extrapolate_y_direction(self):
        """

        void RsmGrid::extrapolate_y_direction()
        Extrapolate data in the y/line direction. 
        """
        return _rsm_grid.RsmGrid_extrapolate_y_direction(self)


    def extrapolate_z_direction(self):
        """

        void RsmGrid::extrapolate_z_direction()
        Extrapolate data in the z/height direction. 
        """
        return _rsm_grid.RsmGrid_extrapolate_z_direction(self)


    def tre_size(self):
        """

        int GeoCal::RsmGrid::tre_size() const
        Calculate the TRE size that we will try to write, so we can check if
        it is too big.

        This is the size that will go into the CEL field (so the total TRE
        size - 11). 
        """
        return _rsm_grid.RsmGrid_tre_size(self)


    def tre_string(self):
        """

        std::string RsmGrid::tre_string() const
        Write to TRE string.

        Note also that the TRE has a fixed precision which is less than the
        machine precision. Writing a RsmGrid and then reading it from a TRE
        does not in general give the exact same RsmGrid, rather just one that
        is close.

        Note that this is all the fields except the CETAG and CEL (the front
        two). It is convenient to treat those special. (We can revisit this in
        the future if we need to). 
        """
        return _rsm_grid.RsmGrid_tre_string(self)


    def read_tre_string(Tre_in):
        """

        boost::shared_ptr< RsmGrid > RsmGrid::read_tre_string(const std::string &Tre_in)
        Read a TRE string.

        This should have all the TRE except for the front CETAG and CEL. It is
        convenient to treat these fields as special. (We can revisit this in
        the future if we need to). 
        """
        return _rsm_grid.RsmGrid_read_tre_string(Tre_in)

    read_tre_string = staticmethod(read_tre_string)

    def __reduce__(self):
    #Special handling for when we are doing boost serialization, we set
    #"this" to None
      if(self.this is None):
        return super().__reduce__()
      return _new_from_serialization, (geocal_swig.serialize_function.serialize_write_binary(self),)

    __swig_destroy__ = _rsm_grid.delete_RsmGrid
RsmGrid.fit_corr = new_instancemethod(_rsm_grid.RsmGrid_fit_corr, None, RsmGrid)
RsmGrid._v_ignore_igc_error_in_fit = new_instancemethod(_rsm_grid.RsmGrid__v_ignore_igc_error_in_fit, None, RsmGrid)
RsmGrid._v_line_grid = new_instancemethod(_rsm_grid.RsmGrid__v_line_grid, None, RsmGrid)
RsmGrid._v_sample_grid = new_instancemethod(_rsm_grid.RsmGrid__v_sample_grid, None, RsmGrid)
RsmGrid.image_coordinate = new_instancemethod(_rsm_grid.RsmGrid_image_coordinate, None, RsmGrid)
RsmGrid.image_coordinate_jacobian = new_instancemethod(_rsm_grid.RsmGrid_image_coordinate_jacobian, None, RsmGrid)
RsmGrid.number_x = new_instancemethod(_rsm_grid.RsmGrid_number_x, None, RsmGrid)
RsmGrid.number_y = new_instancemethod(_rsm_grid.RsmGrid_number_y, None, RsmGrid)
RsmGrid.x_offset = new_instancemethod(_rsm_grid.RsmGrid_x_offset, None, RsmGrid)
RsmGrid.y_offset = new_instancemethod(_rsm_grid.RsmGrid_y_offset, None, RsmGrid)
RsmGrid._v_number_z = new_instancemethod(_rsm_grid.RsmGrid__v_number_z, None, RsmGrid)
RsmGrid._v_x_start = new_instancemethod(_rsm_grid.RsmGrid__v_x_start, None, RsmGrid)
RsmGrid._v_x_delta = new_instancemethod(_rsm_grid.RsmGrid__v_x_delta, None, RsmGrid)
RsmGrid._v_y_start = new_instancemethod(_rsm_grid.RsmGrid__v_y_start, None, RsmGrid)
RsmGrid._v_y_delta = new_instancemethod(_rsm_grid.RsmGrid__v_y_delta, None, RsmGrid)
RsmGrid._v_z_start = new_instancemethod(_rsm_grid.RsmGrid__v_z_start, None, RsmGrid)
RsmGrid._v_z_delta = new_instancemethod(_rsm_grid.RsmGrid__v_z_delta, None, RsmGrid)
RsmGrid._v_total_number_row_digit = new_instancemethod(_rsm_grid.RsmGrid__v_total_number_row_digit, None, RsmGrid)
RsmGrid._v_total_number_col_digit = new_instancemethod(_rsm_grid.RsmGrid__v_total_number_col_digit, None, RsmGrid)
RsmGrid._v_number_fractional_row_digit = new_instancemethod(_rsm_grid.RsmGrid__v_number_fractional_row_digit, None, RsmGrid)
RsmGrid._v_number_fractional_col_digit = new_instancemethod(_rsm_grid.RsmGrid__v_number_fractional_col_digit, None, RsmGrid)
RsmGrid.extrapolate_x_direction = new_instancemethod(_rsm_grid.RsmGrid_extrapolate_x_direction, None, RsmGrid)
RsmGrid.extrapolate_y_direction = new_instancemethod(_rsm_grid.RsmGrid_extrapolate_y_direction, None, RsmGrid)
RsmGrid.extrapolate_z_direction = new_instancemethod(_rsm_grid.RsmGrid_extrapolate_z_direction, None, RsmGrid)
RsmGrid.tre_size = new_instancemethod(_rsm_grid.RsmGrid_tre_size, None, RsmGrid)
RsmGrid.tre_string = new_instancemethod(_rsm_grid.RsmGrid_tre_string, None, RsmGrid)
RsmGrid_swigregister = _rsm_grid.RsmGrid_swigregister
RsmGrid_swigregister(RsmGrid)

def RsmGrid_read_tre_string(Tre_in):
    """

    boost::shared_ptr< RsmGrid > RsmGrid::read_tre_string(const std::string &Tre_in)
    Read a TRE string.

    This should have all the TRE except for the front CETAG and CEL. It is
    convenient to treat these fields as special. (We can revisit this in
    the future if we need to). 
    """
    return _rsm_grid.RsmGrid_read_tre_string(Tre_in)


__all__ = ["RsmGrid"]



