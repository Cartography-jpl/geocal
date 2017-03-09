# This contains the top level object for reading and writing a NITF file.
#
# The class structure is a little complicated, so you can look and the UML
# file doc/Nitf_file.xmi (e.g., use umbrello) to see the design.

from __future__ import print_function
from .nitf_file_header import NitfFileHeader
from .nitf_image_subheader import NitfImageSubheader
from .nitf_image import NitfImageFromNumpy, NitfImagePlaceHolder, \
    NitfImageCannotHandle
from .nitf_tre import process_tre
from .nitf_tre_csde import *
import io,six

class NitfFile(object):
    def __init__(self, file_name = None):
        '''Create a NitfFile for reading or writing. Because it is common, if
        you give a file_name we read from that file to populate the Nitf 
        structure. Otherwise we start with a default file (a file header, but
        no segments) - which you can then populate before calling write'''
        self.file_header = NitfFileHeader()
        # This is the order things appear in the file
        self.image_segment = []
        self.graphic_segment = []
        self.text_segment = []
        self.des_segment = []
        self.res_segment = []
        # These are the file level TREs. There can also be TREs at the
        # image segment level
        self.tre_list = []
        if(file_name is not None):
            self.read(file_name)
    def __str__(self):
        '''Text description of structure, e.g., something you can print out'''
        res = six.StringIO()
        print("-------------------------------------------------------------",
              file=res)
        print("File Header:", file=res)
        print(self.file_header, file=res)
        print("-------------------------------------------------------------",
              file=res)
        if(len(self.tre_list) == 0):
            print("No file level TREs", file=res)
        else:
            print("File level TRES:", file=res)
            for t in self.tre_list:
                print(t)
            print("-------------------------------------------------------------",
                  file=res)
        for arr, name in [[self.image_segment, "Image"],
                          [self.graphic_segment, "Graphic"],
                          [self.text_segment, "Text"],
                          [self.des_segment, "Data Extension"],
                          [self.res_segment, "Reserved Extension"]]:
            if(len(arr) == 0):
                print("No %s segments" % name, file=res)
            else:
                print("-------------------------------------------------------------",
                      file=res)
            for i, seg in enumerate(arr):
                print("%s segment %d of %d" % (name, i+1, len(arr)),
                      file=res)
                print(seg,end='',file=res)
                print("-------------------------------------------------------------",
                      file=res)
        return res.getvalue()
    def read(self, file_name):
        '''Read the given file'''
        with open(file_name, 'rb') as fh:
            self.file_header.read_from_file(fh)
            self.tre_list = []
            if(self.file_header.udhdl > 0):
                if(self.file_header.udhofl > 0):
                    raise RuntimeError("Don't handle TRE overflow yet")
                self.tre_list.extend(process_tre(self.file_header.udhd))
            if(self.file_header.xhdl > 0):
                if(self.file_header.xhdlofl > 0):
                    raise RuntimeError("Don't handle TRE overflow yet")
                self.tre_list.extend(process_tre(self.file_header.xhd))
            self.image_segment = \
               [NitfImageSegment(header_size=self.file_header.lish[i],
                                 data_size=self.file_header.li[i]) for i in
                range(self.file_header.numi)]
            self.graphic_segment = \
               [NitfGraphicSegment(header_size=self.file_header.lssh[i],
                                   data_size=self.file_header.ls[i]) for i in
                range(self.file_header.nums)]
            self.text_segment = \
               [NitfTextSegment(header_size=self.file_header.ltsh[i],
                                data_size=self.file_header.lt[i]) for i in
                range(self.file_header.numt)]
            self.des_segment = \
               [NitfDesSegment(header_size=self.file_header.ldsh[i],
                               data_size=self.file_header.ld[i]) for i in
                range(self.file_header.numdes)]
            self.res_segment = \
               [NitfResSegment(header_size=self.file_header.lresh[i], 
                               data_size=self.file_header.lre[i]) for i in
                range(self.file_header.numres)]
            for seglist in [self.image_segment, self.graphic_segment, 
                            self.text_segment, self.des_segment, 
                            self.res_segment]:
                for seg in seglist:
                    seg.read_from_file(fh)
            for seg in self.image_segment:
                seg.process_tre()

    def prepare_tre_write(self):
        '''Go through the TRE list, and use this to fill in XHD, UDHD, and
        a TRE_OVERFLOW DES'''
        xhd_fh = six.BytesIO()
        xhd_size = 0
        udhd_fh = six.BytesIO()
        udhd_size = 0
        des_fh = six.BytesIO()
        des_size = 0
        for tre in self.tre_list:
            fht = six.BytesIO()
            tre.write_to_file(fht)
            t = fht.getvalue()
            if(xhd_size + len(t) < 99999-3):
                xhd_fh.write(t)
                xhd_size += len(t)
            elif(udhd_size + len(t) < 99999-3):
                udhd_fh.write(t)
                udhd_size += len(t)
            else:
                des_fh.write(t)
                des_size += len(t)
        if(xhd_size > 0):
            self.file_header.xhd = xhd_fh.getvalue()
        if(udhd_size > 0):
            self.file_header.udhd = udhd_fh.getvalue()
        if(des_size > 0):
            raise RuntimeError("Don't handle TRE overflow yet")
        
    def write(self, file_name):
        '''Write to the given file'''
        with open(file_name, 'wb') as fh:
            self.prepare_tre_write()
            h = self.file_header
            h.numi = len(self.image_segment)
            h.nums = len(self.graphic_segment)
            h.numt = len(self.text_segment)
            h.numdes = len(self.des_segment)
            h.numres = len(self.res_segment)
            h.write_to_file(fh)
            # Might be a cleaner way to do this, but for now we just "know"
            # we need to update the header length
            h.update_field(fh, "hl", fh.tell())
            # Write out each segment, updating the subheader and data sizes
            for seglist, fhs, fds in [[self.image_segment, "lish", "li"],
                                      [self.graphic_segment, "lssh", "ls"],
                                      [self.text_segment, "ltsh", "lt"],
                                      [self.des_segment, "ldsh", "ld"],
                                      [self.res_segment, "lresh", "lre"]]:
                for i, seg in enumerate(seglist):
                    hs, ds = seg.write_to_file(fh)
                    h.update_field(fh, fhs, hs, (i,))
                    h.update_field(fh, fds, ds, (i,))
            # Now we have to update the file length
            h.update_field(fh, "fl", fh.tell())

class NitfSegment(object):
    def __init__(self, subheader, data):
        self.subheader = subheader
        self.data = data

    def __str__(self):
        '''Text description of structure, e.g., something you can print out'''
        fh = six.StringIO()
        print("Sub header:", file=fh)
        print(self.subheader, file=fh)
        print("Data", file=fh)
        print(self.data, file=fh)
        return fh.getvalue()

    def read_from_file(self, fh):
        '''Read from a file'''
        self.subheader.read_from_file(fh)
        self.data.read_from_file(fh)

    def write_to_file(self, fh):
        '''Write to a file. The returns (sz_header, sz_data), because this
        information is needed by NitfFile.'''
        start_pos = fh.tell()
        self.subheader.write_to_file(fh)
        header_pos = fh.tell()
        self.data.write_to_file(fh)
        return (header_pos - start_pos, fh.tell() - header_pos)

class NitfPlaceHolder(NitfSegment):
    '''Implementation of NitfSegment that just skips over the data.'''
    def __init__(self, header_size, data_size, type_name):
        NitfSegment.__init__(self, None, None)
        self.sz = header_size + data_size
        self.type_name = type_name
        self.seg_start = None
        
    def __str__(self):
        '''Text description of structure, e.g., something you can print out'''
        fh = six.StringIO()
        print("%s segment, size %d" % (self.type_name, self.sz), file=fh)
        return fh.getvalue()

    def read_from_file(self, fh):
        '''Read from a file'''
        # Just skip over the data
        self.seg_start = fh.tell()
        fh.seek(self.sz, 1)

    def write_to_file(self, fh):
        '''Write to a file. The returns (sz_header, sz_data), because this
        information is needed by NitfFile.'''
        raise NotImplementedError("write_to_file not implemented for %s" % self.type_name)
        

class NitfImageSegment(NitfSegment):
    '''Image segment (IS), supports the standard image type of data.'''
    def __init__(self, image = None,
                 header_size = None, data_size = None,
                 nitf_image_handle = [NitfImageFromNumpy,
                                      NitfImagePlaceHolder]):
        '''Initialize. You can pass a NitfImage class to use (i.e., you've
        created this for writing), or a list of classes to use to try
        to read an image. This list is tried in order, the first class
        that can handle an image is the one used.'''
        self.header_size = header_size
        self.data_size = data_size
        self.nitf_image_handle = nitf_image_handle
        if(image is None):
            h = NitfImageSubheader()
        else:
            h = image.image_subheader
        NitfSegment.__init__(self, h, image)
        self.tre_list = []
    def read_from_file(self, fh):
        '''Read from a file'''
        self.subheader.read_from_file(fh)
        for i,cls in enumerate(self.nitf_image_handle):
            t = cls(image_subheader=self.subheader,
                    header_size=self.header_size,
                    data_size=self.data_size)
            try:
                t.read_from_file(fh)
                break
            except NitfImageCannotHandle:
                if(i < len(self.nitf_image_handle) - 1):
                    pass
                else:
                    raise
        self.data = t
    def process_tre(self):
        if(self.subheader.udidl > 0):
            if(self.subheader.udofl > 0):
                raise RuntimeError("Don't handle TRE overflow yet")
            self.tre_list.extend(process_tre(self.udid))
        if(self.subheader.ixshdl > 0):
            if(self.subheader.ixofl > 0):
                raise RuntimeError("Don't handle TRE overflow yet")
            self.tre_list.extend(process_tre(self.subheader.ixshd))
    def __str__(self):
        '''Text description of structure, e.g., something you can print out'''
        fh = six.StringIO()
        print("Sub header:", file=fh)
        print(self.subheader, file=fh)
        print("TREs:", file=fh)
        if(len(self.tre_list) == 0):
            print("No image level TREs")
        else:
            for tre in self.tre_list:
                print(tre, file=fh)
        print("Data", file=fh)
        print(self.data, file=fh)
        return fh.getvalue()

class NitfGraphicSegment(NitfPlaceHolder):
    '''Graphic segment (GS), support the standard graphic type of data.'''
    def __init__(self, header_size=None, data_size=None):
        NitfPlaceHolder.__init__(self, header_size, data_size, "Graphic Segment")

class NitfTextSegment(NitfPlaceHolder):
    '''Text segment (TS), support the standard text type of data'''
    def __init__(self, header_size=None, data_size=None):
        NitfPlaceHolder.__init__(self, header_size, data_size, "Text Segment")
    
class NitfDes(NitfPlaceHolder):
    '''Data extension segment (DES), allows for the addition of different data 
    types with each type encapsulated in its own DES'''
    def __init__(self, header_size=None, data_size=None):
        NitfPlaceHolder.__init__(self, header_size, data_size, "DES")

class NitfReservedExtensionSegment(NitfPlaceHolder):
    '''Reserved extension segment (RES), non-standard data segment which is
    user-defined. A NITF file can support different user-defined types of 
    segments called RES.'''
    def __init__(self, header_size=None, data_size=None):
        NitfPlaceHolder.__init__(self, header_size, data_size, "RES")
    
