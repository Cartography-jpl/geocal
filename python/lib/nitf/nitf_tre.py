# We import a number of "private" classes from nitf_field. This module really
# is part of nitf_field.py, we have just pulled it out into a separate file
# to keep things clean
#
# Note when importing TREs that much of the documentation is in PDF tables.
# You can't easily paste this directly to emacs. But you can import to Excel.
# To do this, cute and paste the table into *Word*, and then cut and paste
# from word to Excel. For some reason, you can't go directly to Excel. You
# can then cut and paste from excel to emacs
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
    def read_from_tre_bytes(self, bt, nitf_literal = False):
        fh = six.BytesIO(bt)
        _FieldStruct.read_from_file(self,fh, nitf_literal)
    def read_from_file(self, fh, nitf_literal = False):
        tag = fh.read(6).rstrip().decode("utf-8")
        if(tag != self.tre_tag):
            raise RuntimeError("Expected TRE %s but got %s" % (self.tre_tag, tag))
        cel = int(fh.read(5))
        st = fh.tell()
        _FieldStruct.read_from_file(self,fh, nitf_literal)
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
        print("TRE - %s" % self.tre_tag, file=res)
        for f in self.field_value_list:
            if(not isinstance(f, _FieldLoopStruct)):
                if(f.field_name is not None):
                    print(f.field_name.ljust(maxlen) + ": " + f.get_print(self,()),
                          file=res)
            else:
                print(f.desc(self), file=res, end='')
        return res.getvalue()

class TreUnknown(Tre):
    '''The is a general class to handle TREs that we don't have another 
    handler for. It just reports the tre string.'''
    def __init__(self, tre_tag):
        self.tre_tag = tre_tag
        self.tre_bytes = b''
    def cetag_value(self):
        return self.tre_tag
    def cel_value(self):
        return len(self.tre_bytes())
    def read_from_file(self, fh, nitf_literal=False):
        self.tre_tag = fh.read(6).rstrip().decode("utf-8")
        cel = int(fh.read(5))
        self.tre_bytes = fh.read(cel)
    def write_to_file(self, fh):
        fh.write("{:6s}".format(self.cetag_value()).encode("utf-8"))
        v = len(self.tre_bytes)
        if(v > 99999):
            raise RuntimeError("TRE string is too long")
        fh.write("{:0>5d}".format(v).encode("utf-8"))
        fh.write(t)
    def __str__(self):
        '''Text description of structure, e.g., something you can print
        out.'''
        res = six.StringIO()
        print("TRE - %s" % self.tre_tag, file=res)
        print( "   String: %s" % self.tre_bytes, file=res)
        return res.getvalue()

_tre_class = {}

def tre_object(tre_name):
    '''Return a TRE object that can be used to read or write the given tre
    name, or a TreUnknown if we don't have that registered.'''
    if(tre_name in _tre_class):
        return _tre_class[tre_name]()
    return TreUnknown(tre_name)

def read_tre(header, des_list, field_list = []):
    '''This reads a TRE for a particular type of header (e.g., NitfFileHeader,
    NitfImageSubheader). The reading is complicated. There are one or
    more base field names to check, each has three fields,
    e.g. udhdl, udhofl, udhd or the file header.
    Each of these fields may or may not have TRE data. In addition, there
    is an "overflow" indicator which points to a TRE_OVERFLOW DES to read 
    additional TREs. This function processes through this logic and 
    reads all the TREs, returning a (possibly empty) list of TREs.'''
    tre_list = []
    for h_len, h_ofl, h_data in field_list:
        if(getattr(header, h_len) > 0):
            des_index = getattr(header, h_ofl)
            if(des_index > 0):
                # des_index is 1 based, so subtract 1 to get the des
                des = des_list[getattr(header, h_ofl)-1]
                tre_list.extend(read_tre_data(des.data))
            tre_list.extend(read_tre_data(getattr(header, h_data)))
    return tre_list

def prepare_tre_write(tre_list, header, des_list, field_list = [],
                      seg_index = 0):
    '''This prepares TREs for writing, placing them in the right place
    in a header and/or creating TRE_OVERFLOW DES. This is the reverse
    of read_tre, the field_list should be the same as for that.'''
    head_fh = [six.BytesIO() for i in range(len(field_list))]
    des_fh = six.BytesIO()
    for tre in tre_list:
        fht = six.BytesIO()
        tre.write_to_file(fht)
        t = fht.getvalue()
        wrote = False
        for fh in head_fh:
            if(len(fh.getvalue()) + len(t) < 99999-3):
                fh.write(t)
                wrote = True
                break
        if(not wrote):
            des_fh.write(t)
    for i in range(len(field_list)):
        h_len, h_offl, h_data = field_list[i]
        if(len(head_fh[i].getvalue()) > 0):
            setattr(header, h_data, head_fh[i].getvalue())
    if(len(des_fh.getvalue()) > 0):
        # We have a circular dependency. It is actually real, and isn't
        # something we particularly need to break. Instead, work around by
        # delaying the import
        from .nitf_file import NitfDesSegment
        des = NitfDesSegment()
        h_len, h_offl, h_data = field_list[i]
        h = des.subheader
        h.desid = "TRE_OVERFLOW"
        h.dsver = 1
        h.dsclas = "U"
        h.desoflw = str.upper(h_data)
        h.desitem = seg_index
        des.data = des_fh.getvalue()
        des_list.append(des)
        setattr(header, h_offl, len(des_list))
    
def read_tre_data(data):
    '''Read a blob of data, and translate into a series of TREs'''
    fh = six.BytesIO(data)
    res = []
    while True:
        st = fh.tell()
        tre_name = fh.read(6)
        if(len(tre_name) == 0):
            break
        if(len(tre_name) != 6):
            raise RuntimeError("Not enough data to get TRE name.")
        fh.seek(st)
        t = tre_object(tre_name)
        t.read_from_file(fh)
        res.append(t)
    return res
    
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
    _tre_class[tre_tag.encode("utf-8")] = res
    return res
    