# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _geocal_quaternion.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_geocal_quaternion', [dirname(__file__)])
        except ImportError:
            import _geocal_quaternion
            return _geocal_quaternion
        if fp is not None:
            try:
                _mod = imp.load_module('_geocal_quaternion', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _geocal_quaternion = swig_import_helper()
    del swig_import_helper
else:
    import _geocal_quaternion
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
    __swig_destroy__ = _geocal_quaternion.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_geocal_quaternion.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_geocal_quaternion.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_geocal_quaternion.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_geocal_quaternion.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_geocal_quaternion.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_geocal_quaternion.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_geocal_quaternion.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_geocal_quaternion.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_geocal_quaternion.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_geocal_quaternion.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_geocal_quaternion.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_geocal_quaternion.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_geocal_quaternion.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_geocal_quaternion.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_geocal_quaternion.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_geocal_quaternion.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _geocal_quaternion.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_geocal_quaternion.SHARED_PTR_DISOWN_swigconstant(_geocal_quaternion)
SHARED_PTR_DISOWN = _geocal_quaternion.SHARED_PTR_DISOWN

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
class Quaternion_AutoDerivative_double(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, a, b, c, d):
        _geocal_quaternion.Quaternion_AutoDerivative_double_swiginit(self, _geocal_quaternion.new_Quaternion_AutoDerivative_double(a, b, c, d))

    @property
    def R_component_1(self):
        return self._v_R_component_1()


    @property
    def R_component_2(self):
        return self._v_R_component_2()


    @property
    def R_component_3(self):
        return self._v_R_component_3()


    @property
    def R_component_4(self):
        return self._v_R_component_4()

    from_matrix = staticmethod(_geocal_quaternion.Quaternion_AutoDerivative_double_from_matrix)

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.R_component_1,self.R_component_2,self.R_component_3,self.R_component_4)


    def __str__(self):
        return "Quaternion (" + str(self.R_component_1) + ", " + str(self.R_component_2) + ", " + str(self.R_component_3) + ", " + str(self.R_component_4) + ")"

    __swig_destroy__ = _geocal_quaternion.delete_Quaternion_AutoDerivative_double
Quaternion_AutoDerivative_double._v_R_component_1 = new_instancemethod(_geocal_quaternion.Quaternion_AutoDerivative_double__v_R_component_1, None, Quaternion_AutoDerivative_double)
Quaternion_AutoDerivative_double._v_R_component_2 = new_instancemethod(_geocal_quaternion.Quaternion_AutoDerivative_double__v_R_component_2, None, Quaternion_AutoDerivative_double)
Quaternion_AutoDerivative_double._v_R_component_3 = new_instancemethod(_geocal_quaternion.Quaternion_AutoDerivative_double__v_R_component_3, None, Quaternion_AutoDerivative_double)
Quaternion_AutoDerivative_double._v_R_component_4 = new_instancemethod(_geocal_quaternion.Quaternion_AutoDerivative_double__v_R_component_4, None, Quaternion_AutoDerivative_double)
Quaternion_AutoDerivative_double.__add__ = new_instancemethod(_geocal_quaternion.Quaternion_AutoDerivative_double___add__, None, Quaternion_AutoDerivative_double)
Quaternion_AutoDerivative_double.__sub__ = new_instancemethod(_geocal_quaternion.Quaternion_AutoDerivative_double___sub__, None, Quaternion_AutoDerivative_double)
Quaternion_AutoDerivative_double.__mul__ = new_instancemethod(_geocal_quaternion.Quaternion_AutoDerivative_double___mul__, None, Quaternion_AutoDerivative_double)
Quaternion_AutoDerivative_double.__div__ = new_instancemethod(_geocal_quaternion.Quaternion_AutoDerivative_double___div__, None, Quaternion_AutoDerivative_double)
Quaternion_AutoDerivative_double.conj = new_instancemethod(_geocal_quaternion.Quaternion_AutoDerivative_double_conj, None, Quaternion_AutoDerivative_double)
Quaternion_AutoDerivative_double.to_matrix = new_instancemethod(_geocal_quaternion.Quaternion_AutoDerivative_double_to_matrix, None, Quaternion_AutoDerivative_double)
Quaternion_AutoDerivative_double.__str__ = new_instancemethod(_geocal_quaternion.Quaternion_AutoDerivative_double___str__, None, Quaternion_AutoDerivative_double)
Quaternion_AutoDerivative_double_swigregister = _geocal_quaternion.Quaternion_AutoDerivative_double_swigregister
Quaternion_AutoDerivative_double_swigregister(Quaternion_AutoDerivative_double)

def Quaternion_AutoDerivative_double_from_matrix(Mat):
    return _geocal_quaternion.Quaternion_AutoDerivative_double_from_matrix(Mat)
Quaternion_AutoDerivative_double_from_matrix = _geocal_quaternion.Quaternion_AutoDerivative_double_from_matrix


def quaternion_to_matrix(q):
    """

    blitz::Array<T, 2> GeoCal::quaternion_to_matrix(const boost::math::quaternion< T > &q)
    Convert a quaternion to a rotation matrix.

    This is an "active" transformation. For "passive", just reverse
    the sign of A 
    """
    return _geocal_quaternion.quaternion_to_matrix(q)

def matrix_to_quaternion(m):
    """

    boost::math::quaternion<T> GeoCal::matrix_to_quaternion(const T m[3][3])

    """
    return _geocal_quaternion.matrix_to_quaternion(m)

def quat_to_ypr(qin):
    """

    void GeoCal::quat_to_ypr(const boost::math::quaternion< T > &qin, T &yaw, T &pitch, T &roll)
    Return the Euler angles that make up the quaternion rotation (yaw,
    pitch, roll, so quat_rot("xyz", pitch, roll, yaw) = qin. 
    """
    return _geocal_quaternion.quat_to_ypr(qin)

def quat_to_euler(qin):
    """

    void GeoCal::quat_to_euler(const boost::math::quaternion< T > &qin, T &epsilon, T &beta, T
    &delta)
    Return the Euler angles that make up the quaternion rotation (epsilon,
    beta, delta so quat_rot("zyx", epsilon,beta, delta) = qin. 
    """
    return _geocal_quaternion.quat_to_euler(qin)

def determine_quat_rot(V1, V2):
    """

    boost::math::quaternion<T> GeoCal::determine_quat_rot(const boost::array< T, 3 > &V1, const boost::array< T, 3 > &V2)
    Determine quaternion that will rotate a vector v1 to point at v2.

    Note that this isn't actually unique, but this is the 'shortest arc'
    solution. 
    """
    return _geocal_quaternion.determine_quat_rot(V1, V2)

def quat_rot_x(*args):
    """

    boost::math::quaternion<T> GeoCal::quat_rot_x(const T &A)
    Quaternion that rotate about x.

    If you don't happen to remember off the top of your head, you rotate
    an angle 'a' around an axis 'u' by the quaternion cos(a / 2) + sin(a /
    2) * u.

    This is an "active" transformation. For "passive", just reverse
    the sign of A 
    """
    return _geocal_quaternion.quat_rot_x(*args)

def quat_rot_y(*args):
    """

    boost::math::quaternion<T> GeoCal::quat_rot_y(const T &A)
    Quaternion that rotate about y.

    If you don't happen to remember off the top of your head, you rotate
    an angle 'a' around an axis 'u' by the quaternion cos(a / 2) + sin(a /
    2) * u.

    This is an "active" transformation. For "passive", just reverse
    the sign of A 
    """
    return _geocal_quaternion.quat_rot_y(*args)

def quat_rot_z(*args):
    """

    boost::math::quaternion<T> GeoCal::quat_rot_z(const T &A)
    Quaternion that rotate about z.

    If you don't happen to remember off the top of your head, you rotate
    an angle 'a' around an axis 'u' by the quaternion cos(a / 2) + sin(a /
    2) * u.

    This is an "active" transformation. For "passive", just reverse
    the sign of A 
    """
    return _geocal_quaternion.quat_rot_z(*args)

def quat_rot(*args):
    """

    boost::math::quaternion<T> GeoCal::quat_rot(const std::string &Rot, const T &A1, const T &A2, const T &A3, const
    T &A4, const T &A5, const T &A6, const T &A7)
    Do a rotation about the seven axis given.

    This is an "active" transformation. For "passive", just reverse
    the sign of A 
    """
    return _geocal_quaternion.quat_rot(*args)


