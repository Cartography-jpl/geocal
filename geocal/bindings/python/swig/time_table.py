# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _time_table.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
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


SHARED_PTR_DISOWN = _time_table.SHARED_PTR_DISOWN
def _new_from_init(cls, version, *args):
    '''For use with pickle, covers common case where we just store the
    arguments needed to create an object. See for example HdfFile'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__(*args)
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

import geocal.generic_object
class TimeTable(geocal.generic_object.GenericObject):
    """
    This class is used to relate time to image line number and vice versa.

    Often the relationship is pretty simply, there is just a fixed time
    interval between one line and the next. However this class can be used
    to model any instrument complications that need to be modelled (e.g.,
    missing lines, drift in spacing).

    C++ includes: time_table.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def image_coordinate(self, *args):
        """
        virtual ImageCoordinate GeoCal::TimeTable::image_coordinate(Time T, const FrameCoordinate &F) const =0
        Convert from Time and FrameCoordinate to ImageCoordinate. 
        """
        return _time_table.TimeTable_image_coordinate(self, *args)

    def time(self, *args):
        """
        virtual void GeoCal::TimeTable::time(const ImageCoordinate &Ic, Time &T, FrameCoordinate &F) const =0
        Convert from ImageCoordinate to Time and FrameCoordinate. 
        """
        return _time_table.TimeTable_time(self, *args)

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

    __swig_destroy__ = _time_table.delete_TimeTable
TimeTable.image_coordinate = new_instancemethod(_time_table.TimeTable_image_coordinate,None,TimeTable)
TimeTable.time = new_instancemethod(_time_table.TimeTable_time,None,TimeTable)
TimeTable._v_min_line = new_instancemethod(_time_table.TimeTable__v_min_line,None,TimeTable)
TimeTable._v_max_line = new_instancemethod(_time_table.TimeTable__v_max_line,None,TimeTable)
TimeTable._v_min_time = new_instancemethod(_time_table.TimeTable__v_min_time,None,TimeTable)
TimeTable._v_max_time = new_instancemethod(_time_table.TimeTable__v_max_time,None,TimeTable)
TimeTable.__str__ = new_instancemethod(_time_table.TimeTable___str__,None,TimeTable)
TimeTable_swigregister = _time_table.TimeTable_swigregister
TimeTable_swigregister(TimeTable)

class ConstantSpacingTimeTable(TimeTable):
    """
    This is a time table that has a constant spacing between lines.

    C++ includes: time_table.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        ConstantSpacingTimeTable::ConstantSpacingTimeTable(Time Min_time, Time Max_time, double Time_space=40.8e-3)
        Constructor, creates time table from Min_time to Max_time with given
        Time spacing.

        We adjust Max_time to exactly Min_time + i * Time_space, rounding to
        nearest integer i, so it ok if Max_time is a little sloppy. 
        """
        _time_table.ConstantSpacingTimeTable_swiginit(self,_time_table.new_ConstantSpacingTimeTable(*args))
    def _v_time_space(self):
        """
        double GeoCal::ConstantSpacingTimeTable::time_space() const

        """
        return _time_table.ConstantSpacingTimeTable__v_time_space(self)

    @property
    def time_space(self):
        return self._v_time_space()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.min_time,self.max_time,self.time_space)

    __swig_destroy__ = _time_table.delete_ConstantSpacingTimeTable
ConstantSpacingTimeTable._v_time_space = new_instancemethod(_time_table.ConstantSpacingTimeTable__v_time_space,None,ConstantSpacingTimeTable)
ConstantSpacingTimeTable_swigregister = _time_table.ConstantSpacingTimeTable_swigregister
ConstantSpacingTimeTable_swigregister(ConstantSpacingTimeTable)



