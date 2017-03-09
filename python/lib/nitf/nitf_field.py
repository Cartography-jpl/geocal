# The code in this class is used to handle a NITF field structure, e.g., a file
# header or a TRE.
#
# The class structure is a little complicated, so you can look and the UML
# file doc/Nitf_file.xmi (e.g., use umbrello) to see the design.
#
# As an external user, the main thing that is exported is
# create_nitf_field_structure, The created class has functions to read
# and write each of the NITF fields,
# a write_to_file and read_from_file function, and a __str__() to give a
# printable description of the contents of the structure.
#
# A few support classes/routines are also exported. This includes
# NitfLiteral, FieldData (for things like TREs), hardcoded_value, and
# hardcoded_value1
#
# Note what might be slightly confusing using these classes, we do all this
# to setup a NitfField class, not a particular object. These means that all
# the variables internal to for instance _FieldValueAccess are at the
# class level of something like NitfFileHeader, so two objects ob1, ob2 will
# have the exact same _FieldValueAccess. This is accounted for in the
# existing code, but if you are modifying these classes you should be aware
# of this. There is a note in each class if it __dict__ is class or object
# level. Of course __dict__ is that object level for each class, the question
# is if we have one object for all _FieldStruct objects, or if the objects
# are per _FieldStruct.

from __future__ import print_function
import copy
import io,six
from collections import defaultdict
import sys

# Add a bunch of debugging if you are diagnosing a problem
DEBUG = False
class _FieldValueArrayAccess(object):
    '''Provides an array access to a _FieldValue that is part of a loop 
    structure'''
    # The __dict__ is at object level
    def __init__(self, fv, parent_obj):
        self.fv=fv
        self.parent_obj = parent_obj
    def __getitem__(self, ind):
        if(isinstance(ind, tuple)):
            return self.fv.get(self.parent_obj, ind)
        else:
            return self.fv.get(self.parent_obj, (ind,))
    def __setitem__(self,ind,v):
        if(isinstance(ind, tuple)):
            self.fv.set(self.parent_obj, ind,v)
        else:
            self.fv.set(self.parent_obj, (ind,),v)

class _FieldValueAccess(object):
    '''Python descriptor to provide access for getting and setting a
    single field value.'''
    # The __dict__ is at class level
    def __init__(self, fv):
        self.fv = fv
    def __get__(self, parent_obj, cls):
        if(self.fv.loop is not None):
            return _FieldValueArrayAccess(self.fv, parent_obj)
        return self.fv.get(parent_obj, ())
    def __set__(self, parent_obj, v):
        if(self.fv.loop is not None):
            raise RuntimeError("Need to give indexes to set value for field "
                               + self.fv.field_name)
        self.fv.set(parent_obj, (), v)
    
class _FieldValue(object):
    '''This handles a single NITF field. We consider a field in a looping 
    structure to be a single field presented as an array. So for example
    LISH0, LISH1,...LISH5 is one field 'lish' that is an array size 6.
    '''
    # The __dict__ is at class level
    def __init__(self, field_name, size, ty, loop, options):
        '''Give the size, type, loop structure, and options to to use. If
        default is given as None, we use a default default value of
        all spaces for type 'str' or 0 for type int or float.
        '''
        self.field_name = field_name
        self.size = size
        self.ty = ty
        self.loop = loop
        # The format string fstring is used to add the proper padding to give
        # the full size. For string is padded with spaces on the left. For
        # integers we pad on the right with 0.
        self.fstring = "{:%ds}" % self.size
        self.frmt = "%s"
        if(ty == int):
            self.fstring = "{:s}"
            self.frmt = "%%0%dd" % self.size
        if("frmt" in options):
            self.frmt = options["frmt"]
        self.default = options.get("default", None)
        self.condition = options.get("condition", None)

    def value(self,parent_obj):
        if(self.field_name is None):
            return ''
        if(self.field_name not in parent_obj.value):
            if(self.default is not None):
                parent_obj.value[self.field_name] = defaultdict(lambda : self.default)
            elif(self.ty == str):
                parent_obj.value[self.field_name] = defaultdict(lambda : " " * self.size)
            else:
                parent_obj.value[self.field_name] = defaultdict(lambda : 0)
        return parent_obj.value[self.field_name]
    def get_print(self, parent_obj, key):
        '''Return string suitable for printing. This is either the 
        value of this field, or "Not used" if the condition isn't met.'''
        t = self.get(parent_obj,key)
        if(t is None):
            t = "Not used"
        return str(t)
    def check_condition(self, parent_obj, key):
        '''Evaluate the condition (if present) and return False if it isn't
        met, True if it is or if there is no condition'''
        if(self.condition is None):
            return True
        f = parent_obj
        if(len(key) > 0):
            i1 = key[0]
        if(len(key) > 1):
            i2 = key[1]
        if(DEBUG):
            print("Condition: " + self.condition)
            print("eval: " + str(eval(self.condition)))
        return eval(self.condition)
    def get(self,parent_obj,key, no_type_conversion=False):
        if(self.field_name is None):
            return ''
        if(self.loop is not None):
            self.loop.check_index(parent_obj, key)
        if(not self.check_condition(parent_obj, key)):
            return None
        if(hasattr(parent_obj, self.field_name + "_value")):
            if(self.loop is None):
                return getattr(parent_obj, self.field_name + "_value")()
            else:
                return getattr(parent_obj, self.field_name + "_value")(*key)
        v = self.value(parent_obj)[key]
        if(no_type_conversion):
            return v
        if(isinstance(v, NitfLiteral)):
            v = v.value
        if(self.ty == str):
            return self.ty(v).rstrip()
        else:
            return self.ty(v)
    def set(self,parent_obj,key,v):
        if(self.field_name is None):
            raise RuntimeError("Can't set a reserved field")
        if(self.loop is not None):
            self.loop.check_index(parent_obj, key)
        if(not self.check_condition(parent_obj, key)):
            raise RuntimeError("Can't set value for field %s because the condition '%s' isn't met" % (self.field_name, self.condition))
        if(hasattr(parent_obj, self.field_name + "_value")):
            raise RuntimeError("Can't set value for field " + self.field_name)
        self.value(parent_obj)[key] = v
    def bytes(self, parent_obj, key=()):
        '''Return bytes version of this value, formatted and padded as
        NITF will store this.'''
        # If we have a NitfLiteral we assume some sort of funky formating
        # that is handled outside of this class. Pad, but otherwise don't
        # process this.
        t = self.get(parent_obj, key, no_type_conversion=True)
        if(isinstance(t, NitfLiteral)):
            t = ("{:%ds}" % self.size).format(t.value)
        else:
            # Otherwise, get the value and do the formatting that has been
            # supplied to us.
            v = self.get(parent_obj, key)
            # Don't format bytes if we have python 3
            if sys.version_info > (3,) and self.ty == bytes:
                t = v
            elif(isinstance(self.frmt, str)):
                t = self.fstring.format(self.frmt % v)
            else:
                t = self.fstring.format(self.frmt(v))
        if(len(t) != self.size):
            raise RuntimeError("Formatting error. String '%s' is not right length for NITF field %s" % (t, self.field_name))
        if(self.ty == bytes):
            return t
        else:
            return t.encode("utf-8")
    def write_to_file(self, parent_obj, key, fh):
        if(not self.check_condition(parent_obj, key)):
            return
        if(self.field_name is not None):
            if(DEBUG):
                print("Writing: ", self.field_name)
            parent_obj.fh_loc[self.field_name][key] = fh.tell()
        fh.write(self.bytes(parent_obj,key))
    def update_file(self, parent_obj, key, fh):
        '''Rewrite to a file after the value of this field has been updated'''
        # Not sure if updating a field that doesn't meet the condition should
        # just be a noop, or an error. For now treat as an error but we can
        # change this behavior is needed.
        if(not self.check_condition(parent_obj, key)):
            raise RuntimeError("Can't update value for field %s because the condition '%s' isn't met" % (self.field_name, self.condition))
        if(DEBUG):
            print("Updating: ", self.field_name)
        last_pos = fh.tell()
        fh.seek(parent_obj.fh_loc[self.field_name][key])
        fh.write(self.bytes(parent_obj,key))
        fh.seek(last_pos)           
    def read_from_file(self, parent_obj, key, fh,nitf_literal=False):
        if(not self.check_condition(parent_obj, key)):
            return
        if(DEBUG and self.field_name is not None):
            print("Reading: ", self.field_name)
        t = fh.read(self.size)
        if(len(t) != self.size):
            raise RuntimeError("Not enough bytes left to read %d bytes for field %s" % (self.size, self.field_name))
        if(self.field_name is not None):
            if(nitf_literal):
                self.value(parent_obj)[key] = NitfLiteral(t.rstrip().decode("utf-8"))
            elif(self.ty == str):
                self.value(parent_obj)[key] = t.rstrip().decode("utf-8")
            else:
                self.value(parent_obj)[key] = self.ty(t.rstrip())

class _FieldLoopStruct(object):
    # The __dict__ is at class level
    def __init__(self, size):
        self.size = size
        # I think we only ever have 2 levels of nesting. We check for this,
        # in case we run into something else at some point. We could
        # extend this if needed, but we'll wait until we have to
        if(len(self.size) > 2):
            raise RuntimeError("We only support 2 levels of nesting now")
        self.field_value_list = []
    def shape(self, parent_object, lead=()):
        f = parent_object
        if(len(lead) >= 1):
            i1 = lead[0]
        return eval(self.size[len(lead)])
    def write_to_file(self, parent_object, key, fh):
        if(key is None):
            lead = ()
        else:
            lead = key
        maxi = self.shape(parent_object, lead=lead)
        for i in range(maxi):
            for f in self.field_value_list:
                f.write_to_file(parent_object, lead + (i,), fh)
    def read_from_file(self, parent_object, key, fh, nitf_literal=False):
        if(key is None):
            lead = ()
        else:
            lead = key
        maxi = self.shape(parent_object, lead=lead)
        for i in range(maxi):
            for f in self.field_value_list:
                f.read_from_file(parent_object, lead + (i,), fh, nitf_literal)
    def desc(self, parent_object):
        '''Text description of structure, e.g., something you can print
        out.'''
        try:
            maxlen = max(len(f.field_name) for f in self.field_value_list
                         if not isinstance(f, _FieldLoopStruct) and
                         f.field_name is not None)
        except ValueError:
            # We have no _FieldValue, so just set maxlen to a fixed value
            maxlen = 10
        maxi1 = self.shape(parent_object)
        maxlen += 2 + len(str(maxi1))
        res = six.StringIO()
        lead = "  " * (len(self.size) - 1)
        print(lead + "Loop - " + self.size[-1], file=res)
        for f in self.field_value_list:
            if(not isinstance(f, _FieldLoopStruct)):
                if(f.field_name is None):
                    pass
                elif(len(self.size) == 1):
                    for i1 in range(maxi1):
                        print(lead + "  " +
                              ("%s[%d]" % (f.field_name, i1)).ljust(maxlen) +
                              ": " + f.get_print(parent_object,(i1,)),
                              file=res)
                elif(len(self.size) == 2):
                    for i1 in range(maxi1):
                        maxi2 = self.shape(parent_object,lead=(i1,))
                        for i2 in range(maxi2):
                            print(lead + "  " +
                                  ("%s[%d, %d]" % (f.field_name, i1, i2)).ljust(maxlen) +
                                  ": " +  f.get_print(parent_object,(i1,i2)),
                                  file=res)
            else:
                print(f.desc(parent_object), file=res)
        return res.getvalue()
        
    def check_index(self, parent_object, key):
        '''Throw a IndexError exception if key is out of range'''
        if(len(key) != len(self.size)):
            raise IndexError()
        for i in range(len(key)):
            if(key[i] < 0 or key[i] >= self.shape(parent_object, lead=key[0:i])):
                raise IndexError()
        
class _FieldStruct(object):
    # The __dict__ is at object level
    def __init__(self):
        '''Create a NITF field structure structure.'''
        self.value = {}
        self.fh_loc = defaultdict(lambda: dict())
    def write_to_file(self, fh):
        '''Write to a file stream.'''
        for f in self.field_value_list:
            f.write_to_file(self, (), fh)
    def read_from_file(self, fh, nitf_literal=False):
        '''Read from a file stream.  
        
        nitf_literal set to True is to handle odd formatting rules,
        where we want to read the values from a file as a string and
        not apply any additional formatting. The data is read as
        NitfLiteral objects. Normally you don't want this option, but
        it can be useful for cases hard to capture otherwise (e.g.,
        heritage systems that depend on specific formatting).
        '''
        for f in self.field_value_list:
            f.read_from_file(self, (), fh, nitf_literal)
    def update_field(self, fh, field_name, value, key = ()):
        '''Update a field name in an open file'''
        fv = self.field_map[field_name]
        fv.set(self, key, value)
        fv.update_file(self, key, fh)
    def __str__(self):
        '''Text description of structure, e.g., something you can print
        out.'''
        try:
            maxlen = max(len(f.field_name) for f in self.field_value_list
                         if not isinstance(f, _FieldLoopStruct) and
                         f.field_name is not None)
        except ValueError:
            # We have no _FieldValue, so just set maxlen to a fixed value
            maxlen = 10
        res = six.StringIO()
        for f in self.field_value_list:
            if(not isinstance(f, _FieldLoopStruct) and f.field_name is not None):
                print(f.field_name.ljust(maxlen) + ": " + f.get_print(self,()),
                      file=res)
            else:
                print(f.desc(self), file=res, end='')
        return res.getvalue()

class FieldData(object):
    '''Class to handle generic variable size data. We might add more
    specific classes, but for now just have this one generic one.'''
    def __init__(self, field_name, size_field, ty, loop, options):
        '''Note options can contains 'size_offset' to give the offset in
        the size variable.'''
        self.field_name = field_name
        self.loop = loop
        self.size_offset = options.get("size_offset", 0)
        # This isn't exactly the size. Rather it is the expression that
        # we either use to read or set the size, e.g., "ixshdl". Note
        # that in standard NITF bizarreness this isn't actually the size,
        # but rather size_offset + the size
        self.size_field = size_field
    def value(self, parent_obj):
        if(self.field_name not in parent_obj.value):
            parent_obj.value[self.field_name] = defaultdict(lambda : b'')
        return parent_obj.value[self.field_name]
    def get(self,parent_obj,key):
        return self.value(parent_obj)[key]
    def set(self,parent_obj,key,v):
        self.value(parent_obj)[key] = v
        if(len(key) > 0):
            raise RuntimeError("Need key support here")
        if(len(v) == 0):
            setattr(parent_obj, self.size_field, 0)
        else:
            setattr(parent_obj, self.size_field, len(v) +
                    self.size_offset)
    def write_to_file(self, parent_obj, key, fh):
        if(len(self.value(parent_obj)[key]) == 0):
            return
        parent_obj.fh_loc[self.field_name][key] = fh.tell()
        fh.write(self.value(parent_obj)[key])
    def read_from_file(self, parent_obj, key, fh,nitf_literal=False):
        if(len(key) > 0):
            raise RuntimeError("Need key support here")
        sz = getattr(parent_obj, self.size_field)
        if(sz == 0):
            self.value(parent_obj)[key] = b''
            return
        self.value(parent_obj)[key] = fh.read(sz - self.size_offset)
    def get_print(self, parent_obj,key):
        t = self.get(parent_obj,key)
        if(len(t) == 0):
            return "Not used"
        return "Data length %s" % len(t)
        
class _create_nitf_field_structure(object):
    # The __dict__ is at class level
    def __init__(self):
        self.d = dict()
        self.d["field_value_list"] = []
        self.d["field_map"] = {}
    def process(self,description):
        desc = copy.deepcopy(description)
        while(len(desc) > 0):
            self.process_row(desc.pop(0), self.d["field_value_list"])
        return self.d
    def process_row(self,row, field_value_list, loop = None):
        # Determine if we have a looping construct, or a simple field.
        if(isinstance(row[0], list)):
            if(loop):
                sz = copy.copy(loop.size)
            else:
                sz = []
            sz.append(row.pop(0)[1])
            lp = _FieldLoopStruct(sz)
            while(len(row) > 0):
                self.process_row(row.pop(0), lp.field_value_list, lp)
            field_value_list.append(lp)
            return
        # If we are here, then this is a simple field
        #field_name, ln, ty, *rest = row
        # Write this so it also works in python 2.7
        field_name, ln, ty, rest = row[0],row[1],row[2],row[3:]
        options = {}
        if(len(rest) > 0):
            options = rest[0]
        if('field_value_class' in options):
            fv = options["field_value_class"](field_name, ln, ty, loop,
                                              options)
        else:
            fv = _FieldValue(field_name, ln, ty, loop, options)
        if(field_name is not None):
            self.d[field_name] = _FieldValueAccess(fv)
            self.d["field_map"][field_name] = fv
        field_value_list.append(fv)
        
def hardcoded_value(v):
    '''Create a function that returns a fixed value, useful for creating
    various blah_value functions'''
    def f(self, key=None):
        return v
    return f

class NitfLiteral(object):
    '''Sometimes we have a field with a particularly odd format, and it 
    is easier to just return a literal string to return as the TRE field 
    content. If this is passed, we return the exact string passed, plus
    any padding.'''
    def __init__(self, value, trunc_size = None):
        if(trunc_size is None):
            self.value = str(value)
        else:
            self.value = str(value)[0:(trunc_size-1)]

def create_nitf_field_structure(name, description, hlp = None):
    '''Create a nitf field structure and return a class for dealing with this. 
    The name is the class name, and the description should be a list of
    lists describing the field structure. You can optionally supply a help
    string to put in the classes __doc__, this is recommended but not
    required.

    The class provides a set of functions for each field_name that can be 
    used to return the value of that field (if we have read this from a file)
    or to set that field (if we are writing to a file). Some fields may
    have a looping structure, we present these fields as arrays.

    Take a look at the unit tests in nitf_field_test.py for examples of 
    using this. 

    The class returned can be further modified to add functions such as 
    foo_value. The change the field "foo" so it is no longer something that
    can be set, instead the class calculates the value that this should be.

    For the description, we take a field_name, size, type 'ty', and a
    set of optionally parameters.  The optional parameters are:

    frmt    - A format string or function
    default - The default value to use when writing. If not specified, the
              default is all spaces for a str and 0 for a number type.
    condition - An expression used to determine if the field is included
              or not.
    field_value_class - Most fields can be handled by our internal _FieldValue
              class. However there are some special cases (e.g., IXSHD used
              for image segment level TREs). If we need to change this,
              we can supply the class to use here. This class should supply
              a fieldname, get, set, loop, write_to_file, read_from_file, 
              and get_print function because these are the things we call. 
              Note that we have a generic FieldData class here, which might 
              be sufficient.

    The type might be something like 'int', 'float' or 'str'. We
    convert the NITF string to and from this type.

    The 'frmt' can be a format string (e.g., "%03d" for a 3 digit integer),
    or it can be a function that takes a value and returns a string - useful
    for more complicated formatting than can be captured with a format string
    (e.g., file date/time in the format CCYYMMDDhhmmss). The default format
    for str type is just "%s" and integer is "%d" - you don't need to specify
    this if you want the default (note that we already handling padding, so
    you don't need to specify something like "%03d" to get 0 filled padding).
    '''
    t = _create_nitf_field_structure()
    res = type(name, (_FieldStruct,), t.process(description))
    if(hlp is not None):
        try:
            # This doesn't work in python 2.7, we can't write to the
            # doc. Rather than try to do something clever, just punt and
            # skip adding help for python 2.7. This works find with python 3
            res.__doc__ = hlp
        except AttributeError:
            pass
    return res


    
