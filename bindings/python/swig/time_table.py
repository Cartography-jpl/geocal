# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _time_table.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_time_table', [dirname(__file__)])
        except ImportError:
            import _time_table
            return _time_table
        if fp is not None:
            try:
                _mod = imp.load_module('_time_table', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _time_table = swig_import_helper()
    del swig_import_helper
else:
    import _time_table
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
    __swig_destroy__ = _time_table.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_time_table.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_time_table.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_time_table.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_time_table.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_time_table.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_time_table.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_time_table.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_time_table.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_time_table.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_time_table.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_time_table.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_time_table.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_time_table.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_time_table.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_time_table.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_time_table.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _time_table.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_time_table.SHARED_PTR_DISOWN_swigconstant(_time_table)
SHARED_PTR_DISOWN = _time_table.SHARED_PTR_DISOWN

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
import geocal_swig.with_parameter
import geocal_swig.geocal_time
class Vector_Time(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self):
        return self.iterator()

    def __init__(self, *args):
        _time_table.Vector_Time_swiginit(self, _time_table.new_Vector_Time(*args))

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _time_table.delete_Vector_Time
Vector_Time.iterator = new_instancemethod(_time_table.Vector_Time_iterator, None, Vector_Time)
Vector_Time.__nonzero__ = new_instancemethod(_time_table.Vector_Time___nonzero__, None, Vector_Time)
Vector_Time.__bool__ = new_instancemethod(_time_table.Vector_Time___bool__, None, Vector_Time)
Vector_Time.__len__ = new_instancemethod(_time_table.Vector_Time___len__, None, Vector_Time)
Vector_Time.pop = new_instancemethod(_time_table.Vector_Time_pop, None, Vector_Time)
Vector_Time.__getslice__ = new_instancemethod(_time_table.Vector_Time___getslice__, None, Vector_Time)
Vector_Time.__setslice__ = new_instancemethod(_time_table.Vector_Time___setslice__, None, Vector_Time)
Vector_Time.__delslice__ = new_instancemethod(_time_table.Vector_Time___delslice__, None, Vector_Time)
Vector_Time.__delitem__ = new_instancemethod(_time_table.Vector_Time___delitem__, None, Vector_Time)
Vector_Time.__getitem__ = new_instancemethod(_time_table.Vector_Time___getitem__, None, Vector_Time)
Vector_Time.__setitem__ = new_instancemethod(_time_table.Vector_Time___setitem__, None, Vector_Time)
Vector_Time.append = new_instancemethod(_time_table.Vector_Time_append, None, Vector_Time)
Vector_Time.empty = new_instancemethod(_time_table.Vector_Time_empty, None, Vector_Time)
Vector_Time.size = new_instancemethod(_time_table.Vector_Time_size, None, Vector_Time)
Vector_Time.clear = new_instancemethod(_time_table.Vector_Time_clear, None, Vector_Time)
Vector_Time.swap = new_instancemethod(_time_table.Vector_Time_swap, None, Vector_Time)
Vector_Time.get_allocator = new_instancemethod(_time_table.Vector_Time_get_allocator, None, Vector_Time)
Vector_Time.begin = new_instancemethod(_time_table.Vector_Time_begin, None, Vector_Time)
Vector_Time.end = new_instancemethod(_time_table.Vector_Time_end, None, Vector_Time)
Vector_Time.rbegin = new_instancemethod(_time_table.Vector_Time_rbegin, None, Vector_Time)
Vector_Time.rend = new_instancemethod(_time_table.Vector_Time_rend, None, Vector_Time)
Vector_Time.pop_back = new_instancemethod(_time_table.Vector_Time_pop_back, None, Vector_Time)
Vector_Time.erase = new_instancemethod(_time_table.Vector_Time_erase, None, Vector_Time)
Vector_Time.push_back = new_instancemethod(_time_table.Vector_Time_push_back, None, Vector_Time)
Vector_Time.front = new_instancemethod(_time_table.Vector_Time_front, None, Vector_Time)
Vector_Time.back = new_instancemethod(_time_table.Vector_Time_back, None, Vector_Time)
Vector_Time.assign = new_instancemethod(_time_table.Vector_Time_assign, None, Vector_Time)
Vector_Time.resize = new_instancemethod(_time_table.Vector_Time_resize, None, Vector_Time)
Vector_Time.insert = new_instancemethod(_time_table.Vector_Time_insert, None, Vector_Time)
Vector_Time.reserve = new_instancemethod(_time_table.Vector_Time_reserve, None, Vector_Time)
Vector_Time.capacity = new_instancemethod(_time_table.Vector_Time_capacity, None, Vector_Time)
Vector_Time_swigregister = _time_table.Vector_Time_swigregister
Vector_Time_swigregister(Vector_Time)

class Vector_Time2(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self):
        return self.iterator()

    def __init__(self, *args):
        _time_table.Vector_Time2_swiginit(self, _time_table.new_Vector_Time2(*args))
    __swig_destroy__ = _time_table.delete_Vector_Time2
Vector_Time2.iterator = new_instancemethod(_time_table.Vector_Time2_iterator, None, Vector_Time2)
Vector_Time2.__nonzero__ = new_instancemethod(_time_table.Vector_Time2___nonzero__, None, Vector_Time2)
Vector_Time2.__bool__ = new_instancemethod(_time_table.Vector_Time2___bool__, None, Vector_Time2)
Vector_Time2.__len__ = new_instancemethod(_time_table.Vector_Time2___len__, None, Vector_Time2)
Vector_Time2.pop = new_instancemethod(_time_table.Vector_Time2_pop, None, Vector_Time2)
Vector_Time2.__getslice__ = new_instancemethod(_time_table.Vector_Time2___getslice__, None, Vector_Time2)
Vector_Time2.__setslice__ = new_instancemethod(_time_table.Vector_Time2___setslice__, None, Vector_Time2)
Vector_Time2.__delslice__ = new_instancemethod(_time_table.Vector_Time2___delslice__, None, Vector_Time2)
Vector_Time2.__delitem__ = new_instancemethod(_time_table.Vector_Time2___delitem__, None, Vector_Time2)
Vector_Time2.__getitem__ = new_instancemethod(_time_table.Vector_Time2___getitem__, None, Vector_Time2)
Vector_Time2.__setitem__ = new_instancemethod(_time_table.Vector_Time2___setitem__, None, Vector_Time2)
Vector_Time2.append = new_instancemethod(_time_table.Vector_Time2_append, None, Vector_Time2)
Vector_Time2.empty = new_instancemethod(_time_table.Vector_Time2_empty, None, Vector_Time2)
Vector_Time2.size = new_instancemethod(_time_table.Vector_Time2_size, None, Vector_Time2)
Vector_Time2.clear = new_instancemethod(_time_table.Vector_Time2_clear, None, Vector_Time2)
Vector_Time2.swap = new_instancemethod(_time_table.Vector_Time2_swap, None, Vector_Time2)
Vector_Time2.get_allocator = new_instancemethod(_time_table.Vector_Time2_get_allocator, None, Vector_Time2)
Vector_Time2.begin = new_instancemethod(_time_table.Vector_Time2_begin, None, Vector_Time2)
Vector_Time2.end = new_instancemethod(_time_table.Vector_Time2_end, None, Vector_Time2)
Vector_Time2.rbegin = new_instancemethod(_time_table.Vector_Time2_rbegin, None, Vector_Time2)
Vector_Time2.rend = new_instancemethod(_time_table.Vector_Time2_rend, None, Vector_Time2)
Vector_Time2.pop_back = new_instancemethod(_time_table.Vector_Time2_pop_back, None, Vector_Time2)
Vector_Time2.erase = new_instancemethod(_time_table.Vector_Time2_erase, None, Vector_Time2)
Vector_Time2.push_back = new_instancemethod(_time_table.Vector_Time2_push_back, None, Vector_Time2)
Vector_Time2.front = new_instancemethod(_time_table.Vector_Time2_front, None, Vector_Time2)
Vector_Time2.back = new_instancemethod(_time_table.Vector_Time2_back, None, Vector_Time2)
Vector_Time2.assign = new_instancemethod(_time_table.Vector_Time2_assign, None, Vector_Time2)
Vector_Time2.resize = new_instancemethod(_time_table.Vector_Time2_resize, None, Vector_Time2)
Vector_Time2.insert = new_instancemethod(_time_table.Vector_Time2_insert, None, Vector_Time2)
Vector_Time2.reserve = new_instancemethod(_time_table.Vector_Time2_reserve, None, Vector_Time2)
Vector_Time2.capacity = new_instancemethod(_time_table.Vector_Time2_capacity, None, Vector_Time2)
Vector_Time2_swigregister = _time_table.Vector_Time2_swigregister
Vector_Time2_swigregister(Vector_Time2)

class TimeTable(geocal_swig.with_parameter.WithParameter):
    """

    This class is used to relate time to image line number and vice versa.

    Often the relationship is pretty simply, there is just a fixed time
    interval between one line and the next. However this class can be used
    to model any instrument complications that need to be modelled (e.g.,
    missing lines, drift in spacing).

    C++ includes: time_table.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr

    def image_coordinate(self, T, F):
        """

        virtual ImageCoordinate GeoCal::TimeTable::image_coordinate(Time T, const FrameCoordinate &F) const =0
        Convert from Time and FrameCoordinate to ImageCoordinate. 
        """
        return _time_table.TimeTable_image_coordinate(self, T, F)


    def image_coordinate_with_derivative(self, T, F):
        """

        virtual ImageCoordinateWithDerivative GeoCal::TimeTable::image_coordinate_with_derivative(const TimeWithDerivative &T, const FrameCoordinateWithDerivative &F)
        const =0
        Convert from Time and FrameCoordinate to ImageCoordinate. 
        """
        return _time_table.TimeTable_image_coordinate_with_derivative(self, T, F)


    def time(self, Ic):
        """

        virtual void GeoCal::TimeTable::time(const ImageCoordinate &Ic, Time &T, FrameCoordinate &F) const =0
        Convert from ImageCoordinate to Time and FrameCoordinate. 
        """
        return _time_table.TimeTable_time(self, Ic)


    def time_with_derivative(self, Ic):
        """

        virtual void GeoCal::TimeTable::time_with_derivative(const ImageCoordinateWithDerivative &Ic, TimeWithDerivative &T,
        FrameCoordinateWithDerivative &F) const =0
        Convert from ImageCoordinate to Time and FrameCoordinate. 
        """
        return _time_table.TimeTable_time_with_derivative(self, Ic)


    def _v_min_line(self):
        """

        virtual int GeoCal::TimeTable::min_line() const =0
        Minimum line table is valid for. 
        """
        return _time_table.TimeTable__v_min_line(self)


    @property
    def min_line(self):
        return self._v_min_line()


    def _v_max_line(self):
        """

        virtual int GeoCal::TimeTable::max_line() const =0
        Maximum line table is valid for. 
        """
        return _time_table.TimeTable__v_max_line(self)


    @property
    def max_line(self):
        return self._v_max_line()


    def _v_min_time(self):
        """

        virtual Time GeoCal::TimeTable::min_time() const =0
        Minimum time table is valid for. 
        """
        return _time_table.TimeTable__v_min_time(self)


    @property
    def min_time(self):
        return self._v_min_time()


    def _v_max_time(self):
        """

        virtual Time GeoCal::TimeTable::max_time() const =0
        Maximum time table is valid for. 
        """
        return _time_table.TimeTable__v_max_time(self)


    @property
    def max_time(self):
        return self._v_max_time()


    @property
    def parameter(self):
        return self._v_parameter()

    @parameter.setter
    def parameter(self, value):
      self._v_parameter(value)


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

    __swig_destroy__ = _time_table.delete_TimeTable
TimeTable.image_coordinate = new_instancemethod(_time_table.TimeTable_image_coordinate, None, TimeTable)
TimeTable.image_coordinate_with_derivative = new_instancemethod(_time_table.TimeTable_image_coordinate_with_derivative, None, TimeTable)
TimeTable.time = new_instancemethod(_time_table.TimeTable_time, None, TimeTable)
TimeTable.time_with_derivative = new_instancemethod(_time_table.TimeTable_time_with_derivative, None, TimeTable)
TimeTable._v_min_line = new_instancemethod(_time_table.TimeTable__v_min_line, None, TimeTable)
TimeTable._v_max_line = new_instancemethod(_time_table.TimeTable__v_max_line, None, TimeTable)
TimeTable._v_min_time = new_instancemethod(_time_table.TimeTable__v_min_time, None, TimeTable)
TimeTable._v_max_time = new_instancemethod(_time_table.TimeTable__v_max_time, None, TimeTable)
TimeTable._v_parameter = new_instancemethod(_time_table.TimeTable__v_parameter, None, TimeTable)
TimeTable._v_parameter_with_derivative = new_instancemethod(_time_table.TimeTable__v_parameter_with_derivative, None, TimeTable)
TimeTable._v_parameter_subset = new_instancemethod(_time_table.TimeTable__v_parameter_subset, None, TimeTable)
TimeTable._v_parameter_with_derivative_subset = new_instancemethod(_time_table.TimeTable__v_parameter_with_derivative_subset, None, TimeTable)
TimeTable.__str__ = new_instancemethod(_time_table.TimeTable___str__, None, TimeTable)
TimeTable_swigregister = _time_table.TimeTable_swigregister
TimeTable_swigregister(TimeTable)

class ConstantSpacingTimeTable(TimeTable):
    """

    This is a time table that has a constant spacing between lines.

    C++ includes: time_table.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Min_time, Max_time, Time_space=40.8e-3):
        """

        ConstantSpacingTimeTable::ConstantSpacingTimeTable(Time Min_time, Time Max_time, double Time_space=40.8e-3)
        Constructor, creates time table from Min_time to Max_time with given
        Time spacing.

        We adjust Max_time to exactly Min_time + i * Time_space, rounding to
        nearest integer i, so it ok if Max_time is a little sloppy. 
        """
        _time_table.ConstantSpacingTimeTable_swiginit(self, _time_table.new_ConstantSpacingTimeTable(Min_time, Max_time, Time_space))

    def _v_time_space(self):
        """

        double GeoCal::ConstantSpacingTimeTable::time_space() const

        """
        return _time_table.ConstantSpacingTimeTable__v_time_space(self)


    @property
    def time_space(self):
        return self._v_time_space()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _time_table.delete_ConstantSpacingTimeTable
ConstantSpacingTimeTable._v_time_space = new_instancemethod(_time_table.ConstantSpacingTimeTable__v_time_space, None, ConstantSpacingTimeTable)
ConstantSpacingTimeTable_swigregister = _time_table.ConstantSpacingTimeTable_swigregister
ConstantSpacingTimeTable_swigregister(ConstantSpacingTimeTable)

class MeasuredTimeTable(TimeTable):
    """

    This is a time table that has a time associated with each line.

    C++ includes: time_table.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Time_list, Min_line=0):
        """

        MeasuredTimeTable::MeasuredTimeTable(const std::vector< Time > &Time_list, int Min_line=0)
        Constructor.

        This gives the time for every line. This list should be strictly
        ordered. The first time is for the given Min_line (default of 0). 
        """
        _time_table.MeasuredTimeTable_swiginit(self, _time_table.new_MeasuredTimeTable(Time_list, Min_line))

    def _v_size_time_list(self):
        """

        int GeoCal::MeasuredTimeTable::size_time_list() const
        List of times.

        Note that std::vector<Time> doesn't play well with python for reasons
        I've not bothered to track down. So instead we just provide access to
        the underlying list and have python set this up. 
        """
        return _time_table.MeasuredTimeTable__v_size_time_list(self)


    @property
    def size_time_list(self):
        return self._v_size_time_list()


    def time_list(self, i):
        """

        Time GeoCal::MeasuredTimeTable::time_list(int i) const

        """
        return _time_table.MeasuredTimeTable_time_list(self, i)


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _time_table.delete_MeasuredTimeTable
MeasuredTimeTable._v_size_time_list = new_instancemethod(_time_table.MeasuredTimeTable__v_size_time_list, None, MeasuredTimeTable)
MeasuredTimeTable.time_list = new_instancemethod(_time_table.MeasuredTimeTable_time_list, None, MeasuredTimeTable)
MeasuredTimeTable_swigregister = _time_table.MeasuredTimeTable_swigregister
MeasuredTimeTable_swigregister(MeasuredTimeTable)



