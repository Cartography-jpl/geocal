# We import a number of "private" classes from nitf_field. This module really
# is part of nitf_field.py, we have just pulled it out into a separate file
# to keep things clean
from __future__ import print_function
from .nitf_field import _FieldStruct, _FieldLoopStruct, \
    _FieldValueArrayAccess, _create_nitf_field_structure
import copy
import io,six

class Tre(_FieldStruct):
    '''Add a little extra structure unique to Tres'''
    def cetag_value(self):
        return self.tre_tag
    def cel_value(self):
        return len(self.tre_bytes())
    def tre_bytes(self):
        '''All of the TRE expect for the front two cetag and cel fields'''
        fh = six.BytesIO()
        _FieldStruct.write_to_file(self, fh)
        return fh.getvalue()
    def read_from_file(self, fh):
        self.tre_tag = fh.read(6).rstrip().decode("utf-8")
        cel = int(fh.read(5))
        st = fh.tell()
        _FieldStruct.read_from_file(self,fh)
        sz = fh.tell() - st
        if(sz != cel):
            raise RuntimeError("TRE length was expected to be %d but was actually %d" % (cel, sz))
    def write_to_file(self, fh):
        fh.write("{:6s}".format(self.cetag_value()).encode("utf-8"))
        t = self.tre_bytes()
        v = len(t)
        if(v > 99999):
            raise RuntimeError("TRE string is too long")
        fh.write("{:0>5d}".format(v).encode("utf-8"))
        fh.write(t)
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
        print("TRE - %s" % self.tre_tag)
        for f in self.field_value_list:
            if(not isinstance(f, _FieldLoopStruct)):
                if(f.field_name is not None):
                    print(f.field_name.ljust(maxlen) + ": " + f.get_print(self,()),
                          file=res)
            else:
                print(f.desc(self), file=res, end='')
        return res.getvalue()

def create_nitf_tre_structure(name, description, hlp = None):
    '''This is like create_nitf_field_structure, but adds a little
    extra structure for TREs. The description should be almost like
    with create_nitf_field_structure, except for the addition of a
    TRE tag. By convention, we don't list the cetag and cel fields,
    since these are always present.'''
    t = _create_nitf_field_structure()
    desc = copy.deepcopy(description)
    tre_tag = desc.pop(0)
    # cetag and cel are really part of the field structure, but it is
    # convenient to treat the tre as all fields *except* these one. The
    # tre handles the rest these tags special.
    res = type(name, (Tre,), t.process(desc))
    res.tre_tag = tre_tag
    if(hlp is not None):
        try:
            # This doesn't work in python 2.7, we can't write to the
            # doc. Rather than try to do something clever, just punt and
            # skip adding help for python 2.7. This works find with python 3
            res.__doc__ = hlp
        except AttributeError:
            pass
    return res
    
