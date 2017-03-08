from .nitf_image_subheader import NitfImageSubheader
import abc, six
import numpy as np

@six.add_metaclass(abc.ABCMeta)
class NitfImage():
    '''This contains a image that we want to read or write from NITF.

    This class supplies a basic interface, a specific type of image can
    derive from this class and supply some of the missing functionality.

    We take in the image subheader (a NitfImageSubheader object), derived
    classes should fill in details of the subheader.

    A image doesn't actually have to derive from NitfImage if for some
    reason that is inconvenient, we use the standard duck typing and
    any class that supplies the right functions can be used. But this
    base class supplies what the interface should be.
    '''
    def __init__(self, image_subheader):
        self.image_subheader = image_subheader
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
        pass

    @abc.abstractmethod
    def write_to_file(self, fh):
        '''Write an image to a file.'''
        pass
    
class NitfImageFromNumpy(NitfImage):
    '''This is a simple implementation of a NitfImage where we just use 
    a numpy array to hold the data, and write that out. This is really just
    meant as a simple test case/example.

    This example is a byte, single band.'''
    def __init__(self, image_subheader = None, nrow = None, ncol = None):
        if image_subheader is None:
            image_subheader = NitfImageSubheader()
        NitfImage.__init__(self,image_subheader)
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
            raise RuntimeError("Can only handle 1 band images")
        if(ih.ic != "NC"):
            raise RuntimeError("Can only handle uncompressed images")
        if(ih.nbpr != 1 or ih.nbpc != 1):
            raise RuntimeError("Cannot handle blocked data")
        if(ih.nbpp == 8):
            self.data = np.fromfile(fh, dtype=np.uint8, count=ih.nrows*ih.ncols)
        elif(ih.nbpp ==16):
            self.data = np.fromfile(fh, dtype=np.uint16,
                                    count=ih.nrows*ih.ncols)
        elif(ih.nbpp ==32):
            self.data = np.fromfile(fh, dtype=np.uint32,
                                    count=ih.nrows*ih.ncols)
        else:
            raise RuntimeError("Unrecognized nbpp %d" % ih.npp)
        self.data = np.reshape(self.data, (ih.nrows,ih.ncols))

    def write_to_file(self, fh):
        '''Write to a file'''
        # Note that data is a single byte, so endianness isn't something
        # we need to worry about
        fh.write(self.data.tobytes())
        
