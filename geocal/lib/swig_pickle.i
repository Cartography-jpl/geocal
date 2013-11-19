// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

//--------------------------------------------------------------
// Support for writing the __reduce__ function needed for pickling.
//--------------------------------------------------------------

//--------------------------------------------------------------
// Code to support the python side.
//--------------------------------------------------------------

%pythoncode {
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
}

%define %pickle_init(VER, ARG...)
  %pythoncode {
@classmethod
def pickle_format_version(cls):
  return VER

def __reduce__(self):
  return _new_from_init, (self.__class__, VER, ARG)
}
%enddef

%define %pickle_vector()
  %pythoncode {
@classmethod
def pickle_format_version(cls):
  return 1

def to_list(self):
   res = []
   for i in range(self.size()):
      res.append(self[i])
   return res

def __reduce__(self):
  return _new_vector, (self.__class__, 1, self.to_list())
}
%enddef
