# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _swig_std.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_swig_std')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_swig_std')
    _swig_std = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_swig_std', [dirname(__file__)])
        except ImportError:
            import _swig_std
            return _swig_std
        try:
            _mod = imp.load_module('_swig_std', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _swig_std = swig_import_helper()
    del swig_import_helper
else:
    import _swig_std
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


class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _swig_std.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_swig_std.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_swig_std.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_swig_std.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_swig_std.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_swig_std.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_swig_std.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_swig_std.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_swig_std.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_swig_std.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_swig_std.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_swig_std.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_swig_std.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_swig_std.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_swig_std.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_swig_std.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_swig_std.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _swig_std.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

class ios_base(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined")
    __repr__ = _swig_repr
    erase_event = _swig_std.ios_base_erase_event
    imbue_event = _swig_std.ios_base_imbue_event
    copyfmt_event = _swig_std.ios_base_copyfmt_event
    sync_with_stdio = staticmethod(_swig_std.ios_base_sync_with_stdio)
    xalloc = staticmethod(_swig_std.ios_base_xalloc)
    __swig_destroy__ = _swig_std.delete_ios_base
ios_base.register_callback = new_instancemethod(_swig_std.ios_base_register_callback, None, ios_base)
ios_base.flags = new_instancemethod(_swig_std.ios_base_flags, None, ios_base)
ios_base.setf = new_instancemethod(_swig_std.ios_base_setf, None, ios_base)
ios_base.unsetf = new_instancemethod(_swig_std.ios_base_unsetf, None, ios_base)
ios_base.precision = new_instancemethod(_swig_std.ios_base_precision, None, ios_base)
ios_base.width = new_instancemethod(_swig_std.ios_base_width, None, ios_base)
ios_base.imbue = new_instancemethod(_swig_std.ios_base_imbue, None, ios_base)
ios_base.getloc = new_instancemethod(_swig_std.ios_base_getloc, None, ios_base)
ios_base.iword = new_instancemethod(_swig_std.ios_base_iword, None, ios_base)
ios_base.pword = new_instancemethod(_swig_std.ios_base_pword, None, ios_base)
ios_base_swigregister = _swig_std.ios_base_swigregister
ios_base_swigregister(ios_base)
cvar = _swig_std.cvar
ios_base.boolalpha = _swig_std.cvar.ios_base_boolalpha
ios_base.dec = _swig_std.cvar.ios_base_dec
ios_base.fixed = _swig_std.cvar.ios_base_fixed
ios_base.hex = _swig_std.cvar.ios_base_hex
ios_base.internal = _swig_std.cvar.ios_base_internal
ios_base.left = _swig_std.cvar.ios_base_left
ios_base.oct = _swig_std.cvar.ios_base_oct
ios_base.right = _swig_std.cvar.ios_base_right
ios_base.scientific = _swig_std.cvar.ios_base_scientific
ios_base.showbase = _swig_std.cvar.ios_base_showbase
ios_base.showpoint = _swig_std.cvar.ios_base_showpoint
ios_base.showpos = _swig_std.cvar.ios_base_showpos
ios_base.skipws = _swig_std.cvar.ios_base_skipws
ios_base.unitbuf = _swig_std.cvar.ios_base_unitbuf
ios_base.uppercase = _swig_std.cvar.ios_base_uppercase
ios_base.adjustfield = _swig_std.cvar.ios_base_adjustfield
ios_base.basefield = _swig_std.cvar.ios_base_basefield
ios_base.floatfield = _swig_std.cvar.ios_base_floatfield
ios_base.badbit = _swig_std.cvar.ios_base_badbit
ios_base.eofbit = _swig_std.cvar.ios_base_eofbit
ios_base.failbit = _swig_std.cvar.ios_base_failbit
ios_base.goodbit = _swig_std.cvar.ios_base_goodbit
ios_base.app = _swig_std.cvar.ios_base_app
ios_base.ate = _swig_std.cvar.ios_base_ate
ios_base.binary = _swig_std.cvar.ios_base_binary
ios_base.ios_base_in = _swig_std.cvar.ios_base_ios_base_in
ios_base.out = _swig_std.cvar.ios_base_out
ios_base.trunc = _swig_std.cvar.ios_base_trunc
ios_base.beg = _swig_std.cvar.ios_base_beg
ios_base.cur = _swig_std.cvar.ios_base_cur
ios_base.end = _swig_std.cvar.ios_base_end

def ios_base_sync_with_stdio(__sync=True):
    return _swig_std.ios_base_sync_with_stdio(__sync)
ios_base_sync_with_stdio = _swig_std.ios_base_sync_with_stdio

def ios_base_xalloc():
    return _swig_std.ios_base_xalloc()
ios_base_xalloc = _swig_std.ios_base_xalloc

class ios(ios_base):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, __sb):
        _swig_std.ios_swiginit(self, _swig_std.new_ios(__sb))
    __swig_destroy__ = _swig_std.delete_ios
ios.rdstate = new_instancemethod(_swig_std.ios_rdstate, None, ios)
ios.clear = new_instancemethod(_swig_std.ios_clear, None, ios)
ios.setstate = new_instancemethod(_swig_std.ios_setstate, None, ios)
ios.good = new_instancemethod(_swig_std.ios_good, None, ios)
ios.eof = new_instancemethod(_swig_std.ios_eof, None, ios)
ios.fail = new_instancemethod(_swig_std.ios_fail, None, ios)
ios.bad = new_instancemethod(_swig_std.ios_bad, None, ios)
ios.exceptions = new_instancemethod(_swig_std.ios_exceptions, None, ios)
ios.tie = new_instancemethod(_swig_std.ios_tie, None, ios)
ios.rdbuf = new_instancemethod(_swig_std.ios_rdbuf, None, ios)
ios.copyfmt = new_instancemethod(_swig_std.ios_copyfmt, None, ios)
ios.fill = new_instancemethod(_swig_std.ios_fill, None, ios)
ios.imbue = new_instancemethod(_swig_std.ios_imbue, None, ios)
ios.narrow = new_instancemethod(_swig_std.ios_narrow, None, ios)
ios.widen = new_instancemethod(_swig_std.ios_widen, None, ios)
ios_swigregister = _swig_std.ios_swigregister
ios_swigregister(ios)

class ostream(ios):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, __sb):
        _swig_std.ostream_swiginit(self, _swig_std.new_ostream(__sb))
    __swig_destroy__ = _swig_std.delete_ostream
ostream.__lshift__ = new_instancemethod(_swig_std.ostream___lshift__, None, ostream)
ostream.put = new_instancemethod(_swig_std.ostream_put, None, ostream)
ostream.write = new_instancemethod(_swig_std.ostream_write, None, ostream)
ostream.flush = new_instancemethod(_swig_std.ostream_flush, None, ostream)
ostream.tellp = new_instancemethod(_swig_std.ostream_tellp, None, ostream)
ostream.seekp = new_instancemethod(_swig_std.ostream_seekp, None, ostream)
ostream_swigregister = _swig_std.ostream_swigregister
ostream_swigregister(ostream)
cin = cvar.cin
cout = cvar.cout
cerr = cvar.cerr
clog = cvar.clog

class istream(ios):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, __sb):
        _swig_std.istream_swiginit(self, _swig_std.new_istream(__sb))
    __swig_destroy__ = _swig_std.delete_istream
istream.__rshift__ = new_instancemethod(_swig_std.istream___rshift__, None, istream)
istream.gcount = new_instancemethod(_swig_std.istream_gcount, None, istream)
istream.get = new_instancemethod(_swig_std.istream_get, None, istream)
istream.getline = new_instancemethod(_swig_std.istream_getline, None, istream)
istream.ignore = new_instancemethod(_swig_std.istream_ignore, None, istream)
istream.peek = new_instancemethod(_swig_std.istream_peek, None, istream)
istream.read = new_instancemethod(_swig_std.istream_read, None, istream)
istream.readsome = new_instancemethod(_swig_std.istream_readsome, None, istream)
istream.putback = new_instancemethod(_swig_std.istream_putback, None, istream)
istream.unget = new_instancemethod(_swig_std.istream_unget, None, istream)
istream.sync = new_instancemethod(_swig_std.istream_sync, None, istream)
istream.tellg = new_instancemethod(_swig_std.istream_tellg, None, istream)
istream.seekg = new_instancemethod(_swig_std.istream_seekg, None, istream)
istream_swigregister = _swig_std.istream_swigregister
istream_swigregister(istream)

class iostream(istream, ostream):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, __sb):
        _swig_std.iostream_swiginit(self, _swig_std.new_iostream(__sb))
    __swig_destroy__ = _swig_std.delete_iostream
iostream_swigregister = _swig_std.iostream_swigregister
iostream_swigregister(iostream)

endl_cb_ptr = _swig_std.endl_cb_ptr

def endl(arg1):
    return _swig_std.endl(arg1)
endl = _swig_std.endl
ends_cb_ptr = _swig_std.ends_cb_ptr

def ends(arg1):
    return _swig_std.ends(arg1)
ends = _swig_std.ends
flush_cb_ptr = _swig_std.flush_cb_ptr

def flush(arg1):
    return _swig_std.flush(arg1)
flush = _swig_std.flush
class vector_double(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self):
        return self.iterator()

    def __init__(self, *args):
        _swig_std.vector_double_swiginit(self, _swig_std.new_vector_double(*args))
    __swig_destroy__ = _swig_std.delete_vector_double
vector_double.iterator = new_instancemethod(_swig_std.vector_double_iterator, None, vector_double)
vector_double.__nonzero__ = new_instancemethod(_swig_std.vector_double___nonzero__, None, vector_double)
vector_double.__bool__ = new_instancemethod(_swig_std.vector_double___bool__, None, vector_double)
vector_double.__len__ = new_instancemethod(_swig_std.vector_double___len__, None, vector_double)
vector_double.__getslice__ = new_instancemethod(_swig_std.vector_double___getslice__, None, vector_double)
vector_double.__setslice__ = new_instancemethod(_swig_std.vector_double___setslice__, None, vector_double)
vector_double.__delslice__ = new_instancemethod(_swig_std.vector_double___delslice__, None, vector_double)
vector_double.__delitem__ = new_instancemethod(_swig_std.vector_double___delitem__, None, vector_double)
vector_double.__getitem__ = new_instancemethod(_swig_std.vector_double___getitem__, None, vector_double)
vector_double.__setitem__ = new_instancemethod(_swig_std.vector_double___setitem__, None, vector_double)
vector_double.pop = new_instancemethod(_swig_std.vector_double_pop, None, vector_double)
vector_double.append = new_instancemethod(_swig_std.vector_double_append, None, vector_double)
vector_double.empty = new_instancemethod(_swig_std.vector_double_empty, None, vector_double)
vector_double.size = new_instancemethod(_swig_std.vector_double_size, None, vector_double)
vector_double.swap = new_instancemethod(_swig_std.vector_double_swap, None, vector_double)
vector_double.begin = new_instancemethod(_swig_std.vector_double_begin, None, vector_double)
vector_double.end = new_instancemethod(_swig_std.vector_double_end, None, vector_double)
vector_double.rbegin = new_instancemethod(_swig_std.vector_double_rbegin, None, vector_double)
vector_double.rend = new_instancemethod(_swig_std.vector_double_rend, None, vector_double)
vector_double.clear = new_instancemethod(_swig_std.vector_double_clear, None, vector_double)
vector_double.get_allocator = new_instancemethod(_swig_std.vector_double_get_allocator, None, vector_double)
vector_double.pop_back = new_instancemethod(_swig_std.vector_double_pop_back, None, vector_double)
vector_double.erase = new_instancemethod(_swig_std.vector_double_erase, None, vector_double)
vector_double.push_back = new_instancemethod(_swig_std.vector_double_push_back, None, vector_double)
vector_double.front = new_instancemethod(_swig_std.vector_double_front, None, vector_double)
vector_double.back = new_instancemethod(_swig_std.vector_double_back, None, vector_double)
vector_double.assign = new_instancemethod(_swig_std.vector_double_assign, None, vector_double)
vector_double.resize = new_instancemethod(_swig_std.vector_double_resize, None, vector_double)
vector_double.insert = new_instancemethod(_swig_std.vector_double_insert, None, vector_double)
vector_double.reserve = new_instancemethod(_swig_std.vector_double_reserve, None, vector_double)
vector_double.capacity = new_instancemethod(_swig_std.vector_double_capacity, None, vector_double)
vector_double_swigregister = _swig_std.vector_double_swigregister
vector_double_swigregister(vector_double)

class vector_string(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self):
        return self.iterator()

    def __init__(self, *args):
        _swig_std.vector_string_swiginit(self, _swig_std.new_vector_string(*args))
    __swig_destroy__ = _swig_std.delete_vector_string
vector_string.iterator = new_instancemethod(_swig_std.vector_string_iterator, None, vector_string)
vector_string.__nonzero__ = new_instancemethod(_swig_std.vector_string___nonzero__, None, vector_string)
vector_string.__bool__ = new_instancemethod(_swig_std.vector_string___bool__, None, vector_string)
vector_string.__len__ = new_instancemethod(_swig_std.vector_string___len__, None, vector_string)
vector_string.__getslice__ = new_instancemethod(_swig_std.vector_string___getslice__, None, vector_string)
vector_string.__setslice__ = new_instancemethod(_swig_std.vector_string___setslice__, None, vector_string)
vector_string.__delslice__ = new_instancemethod(_swig_std.vector_string___delslice__, None, vector_string)
vector_string.__delitem__ = new_instancemethod(_swig_std.vector_string___delitem__, None, vector_string)
vector_string.__getitem__ = new_instancemethod(_swig_std.vector_string___getitem__, None, vector_string)
vector_string.__setitem__ = new_instancemethod(_swig_std.vector_string___setitem__, None, vector_string)
vector_string.pop = new_instancemethod(_swig_std.vector_string_pop, None, vector_string)
vector_string.append = new_instancemethod(_swig_std.vector_string_append, None, vector_string)
vector_string.empty = new_instancemethod(_swig_std.vector_string_empty, None, vector_string)
vector_string.size = new_instancemethod(_swig_std.vector_string_size, None, vector_string)
vector_string.swap = new_instancemethod(_swig_std.vector_string_swap, None, vector_string)
vector_string.begin = new_instancemethod(_swig_std.vector_string_begin, None, vector_string)
vector_string.end = new_instancemethod(_swig_std.vector_string_end, None, vector_string)
vector_string.rbegin = new_instancemethod(_swig_std.vector_string_rbegin, None, vector_string)
vector_string.rend = new_instancemethod(_swig_std.vector_string_rend, None, vector_string)
vector_string.clear = new_instancemethod(_swig_std.vector_string_clear, None, vector_string)
vector_string.get_allocator = new_instancemethod(_swig_std.vector_string_get_allocator, None, vector_string)
vector_string.pop_back = new_instancemethod(_swig_std.vector_string_pop_back, None, vector_string)
vector_string.erase = new_instancemethod(_swig_std.vector_string_erase, None, vector_string)
vector_string.push_back = new_instancemethod(_swig_std.vector_string_push_back, None, vector_string)
vector_string.front = new_instancemethod(_swig_std.vector_string_front, None, vector_string)
vector_string.back = new_instancemethod(_swig_std.vector_string_back, None, vector_string)
vector_string.assign = new_instancemethod(_swig_std.vector_string_assign, None, vector_string)
vector_string.resize = new_instancemethod(_swig_std.vector_string_resize, None, vector_string)
vector_string.insert = new_instancemethod(_swig_std.vector_string_insert, None, vector_string)
vector_string.reserve = new_instancemethod(_swig_std.vector_string_reserve, None, vector_string)
vector_string.capacity = new_instancemethod(_swig_std.vector_string_capacity, None, vector_string)
vector_string_swigregister = _swig_std.vector_string_swigregister
vector_string_swigregister(vector_string)

class vector_unsigned_char(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self):
        return self.iterator()

    def __init__(self, *args):
        _swig_std.vector_unsigned_char_swiginit(self, _swig_std.new_vector_unsigned_char(*args))
    __swig_destroy__ = _swig_std.delete_vector_unsigned_char
vector_unsigned_char.iterator = new_instancemethod(_swig_std.vector_unsigned_char_iterator, None, vector_unsigned_char)
vector_unsigned_char.__nonzero__ = new_instancemethod(_swig_std.vector_unsigned_char___nonzero__, None, vector_unsigned_char)
vector_unsigned_char.__bool__ = new_instancemethod(_swig_std.vector_unsigned_char___bool__, None, vector_unsigned_char)
vector_unsigned_char.__len__ = new_instancemethod(_swig_std.vector_unsigned_char___len__, None, vector_unsigned_char)
vector_unsigned_char.__getslice__ = new_instancemethod(_swig_std.vector_unsigned_char___getslice__, None, vector_unsigned_char)
vector_unsigned_char.__setslice__ = new_instancemethod(_swig_std.vector_unsigned_char___setslice__, None, vector_unsigned_char)
vector_unsigned_char.__delslice__ = new_instancemethod(_swig_std.vector_unsigned_char___delslice__, None, vector_unsigned_char)
vector_unsigned_char.__delitem__ = new_instancemethod(_swig_std.vector_unsigned_char___delitem__, None, vector_unsigned_char)
vector_unsigned_char.__getitem__ = new_instancemethod(_swig_std.vector_unsigned_char___getitem__, None, vector_unsigned_char)
vector_unsigned_char.__setitem__ = new_instancemethod(_swig_std.vector_unsigned_char___setitem__, None, vector_unsigned_char)
vector_unsigned_char.pop = new_instancemethod(_swig_std.vector_unsigned_char_pop, None, vector_unsigned_char)
vector_unsigned_char.append = new_instancemethod(_swig_std.vector_unsigned_char_append, None, vector_unsigned_char)
vector_unsigned_char.empty = new_instancemethod(_swig_std.vector_unsigned_char_empty, None, vector_unsigned_char)
vector_unsigned_char.size = new_instancemethod(_swig_std.vector_unsigned_char_size, None, vector_unsigned_char)
vector_unsigned_char.swap = new_instancemethod(_swig_std.vector_unsigned_char_swap, None, vector_unsigned_char)
vector_unsigned_char.begin = new_instancemethod(_swig_std.vector_unsigned_char_begin, None, vector_unsigned_char)
vector_unsigned_char.end = new_instancemethod(_swig_std.vector_unsigned_char_end, None, vector_unsigned_char)
vector_unsigned_char.rbegin = new_instancemethod(_swig_std.vector_unsigned_char_rbegin, None, vector_unsigned_char)
vector_unsigned_char.rend = new_instancemethod(_swig_std.vector_unsigned_char_rend, None, vector_unsigned_char)
vector_unsigned_char.clear = new_instancemethod(_swig_std.vector_unsigned_char_clear, None, vector_unsigned_char)
vector_unsigned_char.get_allocator = new_instancemethod(_swig_std.vector_unsigned_char_get_allocator, None, vector_unsigned_char)
vector_unsigned_char.pop_back = new_instancemethod(_swig_std.vector_unsigned_char_pop_back, None, vector_unsigned_char)
vector_unsigned_char.erase = new_instancemethod(_swig_std.vector_unsigned_char_erase, None, vector_unsigned_char)
vector_unsigned_char.push_back = new_instancemethod(_swig_std.vector_unsigned_char_push_back, None, vector_unsigned_char)
vector_unsigned_char.front = new_instancemethod(_swig_std.vector_unsigned_char_front, None, vector_unsigned_char)
vector_unsigned_char.back = new_instancemethod(_swig_std.vector_unsigned_char_back, None, vector_unsigned_char)
vector_unsigned_char.assign = new_instancemethod(_swig_std.vector_unsigned_char_assign, None, vector_unsigned_char)
vector_unsigned_char.resize = new_instancemethod(_swig_std.vector_unsigned_char_resize, None, vector_unsigned_char)
vector_unsigned_char.insert = new_instancemethod(_swig_std.vector_unsigned_char_insert, None, vector_unsigned_char)
vector_unsigned_char.reserve = new_instancemethod(_swig_std.vector_unsigned_char_reserve, None, vector_unsigned_char)
vector_unsigned_char.capacity = new_instancemethod(_swig_std.vector_unsigned_char_capacity, None, vector_unsigned_char)
vector_unsigned_char_swigregister = _swig_std.vector_unsigned_char_swigregister
vector_unsigned_char_swigregister(vector_unsigned_char)

class vector_short_int(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self):
        return self.iterator()

    def __init__(self, *args):
        _swig_std.vector_short_int_swiginit(self, _swig_std.new_vector_short_int(*args))
    __swig_destroy__ = _swig_std.delete_vector_short_int
vector_short_int.iterator = new_instancemethod(_swig_std.vector_short_int_iterator, None, vector_short_int)
vector_short_int.__nonzero__ = new_instancemethod(_swig_std.vector_short_int___nonzero__, None, vector_short_int)
vector_short_int.__bool__ = new_instancemethod(_swig_std.vector_short_int___bool__, None, vector_short_int)
vector_short_int.__len__ = new_instancemethod(_swig_std.vector_short_int___len__, None, vector_short_int)
vector_short_int.__getslice__ = new_instancemethod(_swig_std.vector_short_int___getslice__, None, vector_short_int)
vector_short_int.__setslice__ = new_instancemethod(_swig_std.vector_short_int___setslice__, None, vector_short_int)
vector_short_int.__delslice__ = new_instancemethod(_swig_std.vector_short_int___delslice__, None, vector_short_int)
vector_short_int.__delitem__ = new_instancemethod(_swig_std.vector_short_int___delitem__, None, vector_short_int)
vector_short_int.__getitem__ = new_instancemethod(_swig_std.vector_short_int___getitem__, None, vector_short_int)
vector_short_int.__setitem__ = new_instancemethod(_swig_std.vector_short_int___setitem__, None, vector_short_int)
vector_short_int.pop = new_instancemethod(_swig_std.vector_short_int_pop, None, vector_short_int)
vector_short_int.append = new_instancemethod(_swig_std.vector_short_int_append, None, vector_short_int)
vector_short_int.empty = new_instancemethod(_swig_std.vector_short_int_empty, None, vector_short_int)
vector_short_int.size = new_instancemethod(_swig_std.vector_short_int_size, None, vector_short_int)
vector_short_int.swap = new_instancemethod(_swig_std.vector_short_int_swap, None, vector_short_int)
vector_short_int.begin = new_instancemethod(_swig_std.vector_short_int_begin, None, vector_short_int)
vector_short_int.end = new_instancemethod(_swig_std.vector_short_int_end, None, vector_short_int)
vector_short_int.rbegin = new_instancemethod(_swig_std.vector_short_int_rbegin, None, vector_short_int)
vector_short_int.rend = new_instancemethod(_swig_std.vector_short_int_rend, None, vector_short_int)
vector_short_int.clear = new_instancemethod(_swig_std.vector_short_int_clear, None, vector_short_int)
vector_short_int.get_allocator = new_instancemethod(_swig_std.vector_short_int_get_allocator, None, vector_short_int)
vector_short_int.pop_back = new_instancemethod(_swig_std.vector_short_int_pop_back, None, vector_short_int)
vector_short_int.erase = new_instancemethod(_swig_std.vector_short_int_erase, None, vector_short_int)
vector_short_int.push_back = new_instancemethod(_swig_std.vector_short_int_push_back, None, vector_short_int)
vector_short_int.front = new_instancemethod(_swig_std.vector_short_int_front, None, vector_short_int)
vector_short_int.back = new_instancemethod(_swig_std.vector_short_int_back, None, vector_short_int)
vector_short_int.assign = new_instancemethod(_swig_std.vector_short_int_assign, None, vector_short_int)
vector_short_int.resize = new_instancemethod(_swig_std.vector_short_int_resize, None, vector_short_int)
vector_short_int.insert = new_instancemethod(_swig_std.vector_short_int_insert, None, vector_short_int)
vector_short_int.reserve = new_instancemethod(_swig_std.vector_short_int_reserve, None, vector_short_int)
vector_short_int.capacity = new_instancemethod(_swig_std.vector_short_int_capacity, None, vector_short_int)
vector_short_int_swigregister = _swig_std.vector_short_int_swigregister
vector_short_int_swigregister(vector_short_int)

class vector_int(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self):
        return self.iterator()

    def __init__(self, *args):
        _swig_std.vector_int_swiginit(self, _swig_std.new_vector_int(*args))
    __swig_destroy__ = _swig_std.delete_vector_int
vector_int.iterator = new_instancemethod(_swig_std.vector_int_iterator, None, vector_int)
vector_int.__nonzero__ = new_instancemethod(_swig_std.vector_int___nonzero__, None, vector_int)
vector_int.__bool__ = new_instancemethod(_swig_std.vector_int___bool__, None, vector_int)
vector_int.__len__ = new_instancemethod(_swig_std.vector_int___len__, None, vector_int)
vector_int.__getslice__ = new_instancemethod(_swig_std.vector_int___getslice__, None, vector_int)
vector_int.__setslice__ = new_instancemethod(_swig_std.vector_int___setslice__, None, vector_int)
vector_int.__delslice__ = new_instancemethod(_swig_std.vector_int___delslice__, None, vector_int)
vector_int.__delitem__ = new_instancemethod(_swig_std.vector_int___delitem__, None, vector_int)
vector_int.__getitem__ = new_instancemethod(_swig_std.vector_int___getitem__, None, vector_int)
vector_int.__setitem__ = new_instancemethod(_swig_std.vector_int___setitem__, None, vector_int)
vector_int.pop = new_instancemethod(_swig_std.vector_int_pop, None, vector_int)
vector_int.append = new_instancemethod(_swig_std.vector_int_append, None, vector_int)
vector_int.empty = new_instancemethod(_swig_std.vector_int_empty, None, vector_int)
vector_int.size = new_instancemethod(_swig_std.vector_int_size, None, vector_int)
vector_int.swap = new_instancemethod(_swig_std.vector_int_swap, None, vector_int)
vector_int.begin = new_instancemethod(_swig_std.vector_int_begin, None, vector_int)
vector_int.end = new_instancemethod(_swig_std.vector_int_end, None, vector_int)
vector_int.rbegin = new_instancemethod(_swig_std.vector_int_rbegin, None, vector_int)
vector_int.rend = new_instancemethod(_swig_std.vector_int_rend, None, vector_int)
vector_int.clear = new_instancemethod(_swig_std.vector_int_clear, None, vector_int)
vector_int.get_allocator = new_instancemethod(_swig_std.vector_int_get_allocator, None, vector_int)
vector_int.pop_back = new_instancemethod(_swig_std.vector_int_pop_back, None, vector_int)
vector_int.erase = new_instancemethod(_swig_std.vector_int_erase, None, vector_int)
vector_int.push_back = new_instancemethod(_swig_std.vector_int_push_back, None, vector_int)
vector_int.front = new_instancemethod(_swig_std.vector_int_front, None, vector_int)
vector_int.back = new_instancemethod(_swig_std.vector_int_back, None, vector_int)
vector_int.assign = new_instancemethod(_swig_std.vector_int_assign, None, vector_int)
vector_int.resize = new_instancemethod(_swig_std.vector_int_resize, None, vector_int)
vector_int.insert = new_instancemethod(_swig_std.vector_int_insert, None, vector_int)
vector_int.reserve = new_instancemethod(_swig_std.vector_int_reserve, None, vector_int)
vector_int.capacity = new_instancemethod(_swig_std.vector_int_capacity, None, vector_int)
vector_int_swigregister = _swig_std.vector_int_swigregister
vector_int_swigregister(vector_int)

class vector_float(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self):
        return self.iterator()

    def __init__(self, *args):
        _swig_std.vector_float_swiginit(self, _swig_std.new_vector_float(*args))
    __swig_destroy__ = _swig_std.delete_vector_float
vector_float.iterator = new_instancemethod(_swig_std.vector_float_iterator, None, vector_float)
vector_float.__nonzero__ = new_instancemethod(_swig_std.vector_float___nonzero__, None, vector_float)
vector_float.__bool__ = new_instancemethod(_swig_std.vector_float___bool__, None, vector_float)
vector_float.__len__ = new_instancemethod(_swig_std.vector_float___len__, None, vector_float)
vector_float.__getslice__ = new_instancemethod(_swig_std.vector_float___getslice__, None, vector_float)
vector_float.__setslice__ = new_instancemethod(_swig_std.vector_float___setslice__, None, vector_float)
vector_float.__delslice__ = new_instancemethod(_swig_std.vector_float___delslice__, None, vector_float)
vector_float.__delitem__ = new_instancemethod(_swig_std.vector_float___delitem__, None, vector_float)
vector_float.__getitem__ = new_instancemethod(_swig_std.vector_float___getitem__, None, vector_float)
vector_float.__setitem__ = new_instancemethod(_swig_std.vector_float___setitem__, None, vector_float)
vector_float.pop = new_instancemethod(_swig_std.vector_float_pop, None, vector_float)
vector_float.append = new_instancemethod(_swig_std.vector_float_append, None, vector_float)
vector_float.empty = new_instancemethod(_swig_std.vector_float_empty, None, vector_float)
vector_float.size = new_instancemethod(_swig_std.vector_float_size, None, vector_float)
vector_float.swap = new_instancemethod(_swig_std.vector_float_swap, None, vector_float)
vector_float.begin = new_instancemethod(_swig_std.vector_float_begin, None, vector_float)
vector_float.end = new_instancemethod(_swig_std.vector_float_end, None, vector_float)
vector_float.rbegin = new_instancemethod(_swig_std.vector_float_rbegin, None, vector_float)
vector_float.rend = new_instancemethod(_swig_std.vector_float_rend, None, vector_float)
vector_float.clear = new_instancemethod(_swig_std.vector_float_clear, None, vector_float)
vector_float.get_allocator = new_instancemethod(_swig_std.vector_float_get_allocator, None, vector_float)
vector_float.pop_back = new_instancemethod(_swig_std.vector_float_pop_back, None, vector_float)
vector_float.erase = new_instancemethod(_swig_std.vector_float_erase, None, vector_float)
vector_float.push_back = new_instancemethod(_swig_std.vector_float_push_back, None, vector_float)
vector_float.front = new_instancemethod(_swig_std.vector_float_front, None, vector_float)
vector_float.back = new_instancemethod(_swig_std.vector_float_back, None, vector_float)
vector_float.assign = new_instancemethod(_swig_std.vector_float_assign, None, vector_float)
vector_float.resize = new_instancemethod(_swig_std.vector_float_resize, None, vector_float)
vector_float.insert = new_instancemethod(_swig_std.vector_float_insert, None, vector_float)
vector_float.reserve = new_instancemethod(_swig_std.vector_float_reserve, None, vector_float)
vector_float.capacity = new_instancemethod(_swig_std.vector_float_capacity, None, vector_float)
vector_float_swigregister = _swig_std.vector_float_swigregister
vector_float_swigregister(vector_float)



