# This contains the top level object for reading and writing a NITF file.
#
# The class structure is a little complicated, so you can look and the UML
# file doc/Nitf_file.xmi (e.g., use umbrello) to see the design.

from __future__ import print_function
from .nitf_file_header import NitfFileHeader
from .nitf_image_subheader import NitfImageSubheader
from .nitf_image import NitfImageFromNumpy
from .nitf_tre import process_tre
from .nitf_tre_use00a import *
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
        print("-------------------------------------------------------------",
              file=res)
        print("File level TRES:", file=res)
        if(len(self.tre_list) == 0):
            print("No file level TREs", file=res)
        else:
            print("File level TRES:", file=res)
            for t in self.tre_list:
                print(t)
        for arr, name in [[self.image_segment, "Image"],
                          [self.graphic_segment, "Graphic"],
                          [self.text_segment, "Text"],
                          [self.des_segment, "Data Extension"],
                          [self.res_segment, "Reserved Extension"]]:
            if(len(arr) == 0):
                print("No %s segments" % name, file=res)
            for i, seg in enumerate(arr):
                print("%s segment %d of %d" % (name, i+1, len(arr)),
                      file=res)
                print(seg,file=res)
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
                self.tre_list.extend(process_tre(self.udhd))
            if(self.file_header.xhdl > 0):
                if(self.file_header.xhdlofl > 0):
                    raise RuntimeError("Don't handle TRE overflow yet")
                self.tre_list.extend(process_tre(self.xhd))
            self.image_segment = [NitfImageSegment() for i in
                                  range(self.file_header.numi)]
            self.graphic_segment = [NitfGraphicSegment() for i in
                                    range(self.file_header.nums)]
            self.text_segment = [NitfTextSegment() for i in
                                 range(self.file_header.numt)]
            self.des_segment = [NitfDesSegment() for i in
                                 range(self.file_header.numdes)]
            self.res_segment = [NitfResSegment() for i in
                                 range(self.file_header.numres)]
            for seglist in [self.image_segment, self.graphic_segment, 
                            self.text_segment, self.des_segment, 
                            self.res_segment]:
                for seg in seglist:
                    seg.read_from_file(fh)
            for seg in self.image_segment:
                seg.process_tre()
            
    def write(self, file_name):
        '''Write to the given file'''
        with open(file_name, 'wb') as fh:
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

class NitfTre(object):
    '''Tagged Record Extension (TRE)'''
    def __init__(self):
        pass

class NitfImageSegment(NitfSegment):
    '''Image segment (IS), supports the standard image type of data.'''
    def __init__(self, image = None):
        # We probably will want to add some kind of logic on how to
        # pick the image handler. But for now, just hardcode this.
        if(image is None):
            image = NitfImageFromNumpy()
        NitfSegment.__init__(self,image.image_subheader, image)
        self.tre_list = []
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
            

class NitfGraphicSegment(NitfSegment):
    '''Graphic segment (GS), support the standard graphic type of data.'''
    def __init__(self):
        pass

class NitfTextSegment(NitfSegment):
    '''Text segment (TS), support the standard text type of data'''
    def __init__(self):
        pass
    
class NitfDes(NitfSegment):
    '''Data extension segment (DES), allows for the addition of different data 
    types with each type encapsulated in its own DES'''
    def __init__(self):
        pass

class NitfReservedExtensionSegment(NitfSegment):
    '''Reserved extension segment (RES), non-standard data segment which is
    user-defined. A NITF file can support different user-defined types of 
    segments called RES.'''
    def __init__(self):
        pass
    
