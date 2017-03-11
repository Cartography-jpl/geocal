from .nitf_image_subheader import NitfImageSubheader
import abc, six
import numpy as np

class NitfImageCannotHandle(RuntimeError):
    '''Exception that indicates we can't read a particular image. Note that
    this does *not* mean an error occured - e.g., a corrupt image. Rather this
    means that the image is a type we don't handle, e.g., JPEG-2000.'''
    def __init__(self, msg = "Can't handle this type of image"):
        RuntimeError.__init__(self, msg)
        
@six.add_metaclass(abc.ABCMeta)
class NitfImage(object):
    '''This contains a image that we want to read or write from NITF.

    This class supplies a basic interface, a specific type of image can
    derive from this class and supply some of the missing functionality.

    We take in the image subheader (a NitfImageSubheader object), derived
    classes should fill in details of the subheader.

    A image doesn't actually have to derive from NitfImage if for some
    reason that is inconvenient, we use the standard duck typing and
    any class that supplies the right functions can be used. But this
    base class supplies what the interface should be.

    Note that a NitfImage class doesn't need to handle all types of images
    (e.g., supporting reading JPEG-2000). If it can't handle reading 
    a particular image, it should throw a NitfImageCannotHandle exception. 
    The NitfImageSegment class will then just move on to next possible
    handler class.
    '''
    def __init__(self, image_subheader=None, header_size=None, data_size=None):
        self.image_subheader = image_subheader
        self.header_size = header_size
        self.data_size = data_size
        # Derived classes should fill in information

    # Derived classes may want to override this to give a more detailed
    # description of what kind of image this is.
    def __str__(self):
        return 'NitfImage'

    @abc.abstractmethod
    def read_from_file(self, fh):
        '''Read an image from a file. For larger images a derived class might
        want to not actually read in the data (e.g., you might memory
        map the data or otherwise generate a 'read on demand'), but at
        the end of the read fh should point past the end of the image data
        (e.g., do a fh.seek(start_pos + size of image) or something like 
        that)'''
        raise NotImplementedError()

    @abc.abstractmethod
    def write_to_file(self, fh):
        '''Write an image to a file.'''
        raise NotImplementedError()

class NitfImagePlaceHolder(NitfImage):
    '''Implementation that doesn't actually read any data, useful as a
    final place holder if none of our other NitfImage classes can handle
    a particular image.'''
    def __init__(self, image_subheader = None, header_size = None,
                 data_size = None):
        NitfImage.__init__(self,image_subheader, header_size, data_size)
        self.data_start = None
        
    def __str__(self):
        return "NitfImagePlaceHolder %d bytes of data" % (self.data_size)

    def read_from_file(self, fh):
        '''Read an image from a file. For larger images a derived class might
        want to not actually read in the data (e.g., you might memory
        map the data or otherwise generate a 'read on demand'), but at
        the end of the read fh should point past the end of the image data
        (e.g., do a fh.seek(start_pos + size of image) or something like 
        that)'''
        self.data_start = fh.tell()
        fh.seek(self.data_size, 1)

    def write_to_file(self, fh):
        '''Write an image to a file.'''
        raise NotImplementedError("Can't write a NitfImagePlaceHolder")

class NitfImageFromNumpy(NitfImage):
    '''This is a simple implementation of a NitfImage where we just use 
    a numpy array to hold the data, and write that out. This is really just
    meant as a simple test case/example.

    This example is a byte, single band.'''
    def __init__(self, image_subheader = None, header_size = None,
                 data_size = None, nrow = None, ncol = None):
        if image_subheader is None:
            image_subheader = NitfImageSubheader()
        NitfImage.__init__(self,image_subheader, header_size, data_size)
        # Only fill in data if we give a size. Otherwise we assume
        # that we are doing to just do a read
        if(nrow is None):
            return
        self.data = np.zeros((nrow, ncol), dtype = np.uint8)
        ih = self.image_subheader
        ih.iid1 = "Test data"
        ih.iid2 = "This is from a NitfImageFromNumpy, used as sample data."
        ih.idatim = "20160101120000"
        ih.nrows = nrow
        ih.ncols = ncol
        ih.nbpr = 1
        ih.nbpc = 1
        ih.npphb = ncol
        ih.npphv = nrow
        ih.pvtype = "INT"
        ih.irep = "MONO"
        ih.icat = "VIS"
        ih.abpp = 8
        ih.nbpp = 8
        ih.ic = "NC"
        ih.nbands = 1
        ih.irepband[0] = 'M'

    def __str__(self):
        return "NitfImageFromNumpy %d x %d uint8 image" % (self.data.shape[0], self.data.shape[1])

    def read_from_file(self, fh):
        '''Read from a file'''
        # Sanity check that we can access this data
        ih = self.image_subheader
        if(ih.nbands != 1):
            raise NitfImageCannotHandle("Can only handle 1 band images")
        if(ih.ic != "NC"):
            raise NitfImageCannotHandle("Can only handle uncompressed images")
        if(ih.nbpr != 1 or ih.nbpc != 1):
            raise NitfImageCannotHandle("Cannot handle blocked data")
        if(ih.nbpp == 8):
            self.data = np.fromfile(fh, dtype=np.uint8, count=ih.nrows*ih.ncols)
        elif(ih.nbpp ==16):
            self.data = np.fromfile(fh, dtype=np.uint16,
                                    count=ih.nrows*ih.ncols)
        elif(ih.nbpp ==32):
            self.data = np.fromfile(fh, dtype=np.uint32,
                                    count=ih.nrows*ih.ncols)
        else:
            raise NitfImageCannotHandle("Unrecognized nbpp %d" % ih.npp)
        self.data = np.reshape(self.data, (ih.nrows,ih.ncols))

    def write_to_file(self, fh):
        '''Write to a file'''
        # Note that data is a single byte, so endianness isn't something
        # we need to worry about
        fh.write(self.data.tobytes())
        