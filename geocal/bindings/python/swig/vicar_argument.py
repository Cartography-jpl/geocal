# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _vicar_argument.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_vicar_argument', [dirname(__file__)])
        except ImportError:
            import _vicar_argument
            return _vicar_argument
        if fp is not None:
            try:
                _mod = imp.load_module('_vicar_argument', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _vicar_argument = swig_import_helper()
    del swig_import_helper
else:
    import _vicar_argument
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


SHARED_PTR_DISOWN = _vicar_argument.SHARED_PTR_DISOWN
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
class VicarArgument(geocal.generic_object.GenericObject):
    """
    This provides access to arguments passed by VICAR.

    Note an important limitation of VICAR, you can only initialize the
    argument handling once. This means that if you try to create multiple
    VicarArgument objects, you'll trigger a TAE error when you try to
    create the second object.

    VICAR uses a somewhat unusual mechanism to pass arguments to a program
    called from the TAE. Rather than putting the arguments on the command
    line to appear in Argv, the arguments get passed down a pipe (usually
    file logical 4) in a compressed format. I'm not sure why exactly it
    does this, probably some historical artifact from when it was
    developed on VMS. In any case, in order to fit into VICAR we need to
    parse arguments the same way.

    This class handles the arguments.

    C++ includes: vicar_argument.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        VicarArgument::VicarArgument(int Argc, char **Argv, bool copy_primary_input_label=false)
        Set up to parse VICAR arguments.

        VICAR arguments are accessed by keywords. This isn't a bad interface,
        but because we also want to support straight unix command line style
        arguments, we have the VicarArgument interface in terms of index
        number (first argument, second argument, etc.). Keyword_list gives the
        list of keywords in order that we want.

        VICAR has the convention that when creating a new file it
        automatically copies over all of the labels from the "primary input"
        - usually the first input file. The idea is that with old VICAR files
        most of the time the output image would have same size, map
        projection, etc. However, this doesn't fit well with the geocal usage
        at all. Much of the time the output file doesn't have any relationship
        to the input file. Also the label reading and writing happens in the
        background anyways, so there isn't a lot gained by the defaults. This
        can often cause problems, such as copying map projection data from an
        input file to an output file that isn't map projected.

        By default, we turn this copying off. You can get the old VICAR
        behavior by setting Copy_primary_input_label to true. 
        """
        _vicar_argument.VicarArgument_swiginit(self,_vicar_argument.new_VicarArgument(*args))
    def write_out(self, *args):
        """
        void VicarArgument::write_out(const std::string &Keyword, const std::string &Val)

        """
        return _vicar_argument.VicarArgument_write_out(self, *args)

    type_string = staticmethod(_vicar_argument.VicarArgument_type_string)
    type_count = staticmethod(_vicar_argument.VicarArgument_type_count)
    arg_str = staticmethod(_vicar_argument.VicarArgument_arg_str)
    arg_int = staticmethod(_vicar_argument.VicarArgument_arg_int)
    arg_real = staticmethod(_vicar_argument.VicarArgument_arg_real)
    def __getitem__(self, key):
      k = str(key).upper()
      t = self.type_string(k)
      if(t == "STRING"):
        r = self.arg_str(k)
      elif(t == "REAL"):
        r = self.arg_real(k)
      elif(t == "INT"):
        r = self.arg_int(k)
      if(self.type_count(k) == 1):
        r = r[0]
      return r

    def __setitem__(self, key, val):
      k = str(key).upper()
      t = self.type_string(k)
      if(t == "STRING"):
        self.write_out(k, str(val))
      elif(t == "REAL"):
        self.write_out(k, float(val))
      elif(t == "INT"):
        self.write_out(k, int(val))


    __swig_destroy__ = _vicar_argument.delete_VicarArgument
VicarArgument.write_out = new_instancemethod(_vicar_argument.VicarArgument_write_out,None,VicarArgument)
VicarArgument.__str__ = new_instancemethod(_vicar_argument.VicarArgument___str__,None,VicarArgument)
VicarArgument_swigregister = _vicar_argument.VicarArgument_swigregister
VicarArgument_swigregister(VicarArgument)

def VicarArgument_type_string(*args):
  return _vicar_argument.VicarArgument_type_string(*args)
VicarArgument_type_string = _vicar_argument.VicarArgument_type_string

def VicarArgument_type_count(*args):
  return _vicar_argument.VicarArgument_type_count(*args)
VicarArgument_type_count = _vicar_argument.VicarArgument_type_count

def VicarArgument_arg_str(*args):
  return _vicar_argument.VicarArgument_arg_str(*args)
VicarArgument_arg_str = _vicar_argument.VicarArgument_arg_str

def VicarArgument_arg_int(*args):
  return _vicar_argument.VicarArgument_arg_int(*args)
VicarArgument_arg_int = _vicar_argument.VicarArgument_arg_int

def VicarArgument_arg_real(*args):
  return _vicar_argument.VicarArgument_arg_real(*args)
VicarArgument_arg_real = _vicar_argument.VicarArgument_arg_real



