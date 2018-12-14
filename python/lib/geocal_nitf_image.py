from geocal_swig import (GdalMultiBand)
try:
    from pynitf import(register_image_class, NitfImageCannotHandle,
                       NitfImageWithSubset)
    have_pynitf = True
except ImportError:
    # Ok if we don't have pynitf, we just can't execute this code
    have_pynitf = False

# ---------------------------------------------------------
# Add classes to handles images
# ---------------------------------------------------------
if(have_pynitf):
    class NitfImageGdal(NitfImageWithSubset):
        '''Implementation that used GDAL to read the data.'''
        def __init__(self, image_subheader = None, header_size = None,
                     data_size = None):
            NitfImageWithSubset.__init__(self,image_subheader, header_size,
                                         data_size)
            self.data_start = None
        
        def __str__(self):
            return "Gdal image"

        def __getitem__(self, ind):
            '''Return data read from image segment. This takes band, line, and
            sample or line, sample. Because it is so common, we can take
            only 2 values, and assume the band number is held at 0. This
            is useful, because the 1 band case is common.Line, sample,
            band can be a slice or an integer.  This should return
            something like a numpy array
            '''
            # This should be more efficient, but for now just read everything
            # in.
            return self.data.read_all()[ind]
    
        def read_from_file(self, fh, segindex):
            '''Read an image from a file.'''
            if(segindex is None):
                raise NitfImageCannotHandle("Need segindex")
                print("failure 1")
            try:
                self.data = GdalMultiBand("NITF_IM:%d:%s" % (segindex, fh.name))
            except RuntimeError as e:
                print("failure 2")
                raise NitfImageCannotHandle("Can't read using GdalMultiBand: %s" % e)
            self.data_start = fh.tell()
            fh.seek(self.data_size, 1)

        def write_to_file(self, fh):
            '''Write an image to a file.'''
            raise NotImplementedError("Can't write a NitfImageGdal")

    # Try this after the classes in pynitf have been tried.
    register_image_class(NitfImageGdal, priority_order=1)

if(have_pynitf):
    __all__ = ["NitfImageGdal",]
else:
    __all__ = []
    
