from geocal_swig import (GdalMultiBand, GdalRasterImage)
try:
    from pynitf import(NitfSegmentDataHandleSet, 
                       NitfImageWithSubset, NitfImageSegment, NitfDiffHandle,
                       NitfDiffHandleSet)
    have_pynitf = True
except ImportError:
    # Ok if we don't have pynitf, we just can't execute this code
    have_pynitf = False
import logging
import numpy as np

# ---------------------------------------------------------
# Add classes to handles images
# ---------------------------------------------------------
if(have_pynitf):
    class NitfImageGdal(NitfImageWithSubset):
        '''Implementation that used GDAL to read the data.'''
        def __init__(self, seg=None):
            super().__init__(seg)
            self.data_start = None
            self.fh_in_name = None
            
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
                return False
            try:
                self.data = GdalMultiBand("NITF_IM:%d:%s" % (segindex, fh.name))
            except RuntimeError as e:
                return False
            self.data_start = fh.tell()
            self.fh_in_name = fh.name
            fh.seek(self._seg().data_size, 1)
            return True

        def write_to_file(self, fh):
            '''Write an image to a file.'''
            # Useful to be able to copy from an input file to a output file.
            # So, for the special case of having read an input file
            # we allow writing. Otherwise, we don't currently support
            # writing.
            if(self.fh_in_name is None):
                raise NotImplementedError("Can't write a NitfImageGdal")
            bytes_left = self.data_size
            buffer_size = 1024*1024
            with open(self.fh_in_name, 'rb') as fh_in:
                fh_in.seek(self.data_start)
                while(bytes_left > 0):
                    if bytes_left < buffer_size:
                        d = fh_in.read(bytes_left)
                    else:
                        d = fh_in.read(buffer_size)
                    fh.write(d)

                    #This may go negative on the last loop but that's fine
                    bytes_left = bytes_left - buffer_size
            

    # Try this after the classes in pynitf have been tried.
    # Guard against adding multiple times
    if 'NitfImageGdal' not in [h[0].__name__ for
                    h in NitfSegmentDataHandleSet.default_handle_set()]:
        NitfSegmentDataHandleSet.add_default_handle(NitfImageGdal,
                                                    priority_order=-1)

    logger = logging.getLogger('nitf_diff')
    class ImageGdalDiff(NitfDiffHandle):
        def handle_diff(self, d1, d2, nitf_diff):
            if(not isinstance(d1, NitfImageGdal) or
               not isinstance(d2, NitfImageGdal)):
                return (False, None)
            is_same = True
            t1 = d1[:,:,:]
            t2 = d2[:,:,:]
            # TODO Expose tolerance as configuration parameters
            if(not np.allclose(t1, t2)):
                logger.difference("Image '%s is different",
                                  d1.subheader.iid1)
                is_same = False
            return (True, is_same)

    # Guard against adding multiple times
    if 'ImageGdalDiff' not in [h[0].__class__.__name__ for
                    h in NitfDiffHandleSet.default_handle_set()]:
        NitfDiffHandleSet.add_default_handle(ImageGdalDiff())
    
    # Alternative interface, access a nitf image segment as a GdalRasterImage
    def _gdal_multi_band(self):
        '''Access a NITF image as a GdalMultiBand.'''
        if self.nitf_file is None:
            raise RuntimeError("Need nitf_file to get gdal_multi_band")
        if self.nitf_file.file_name is None:
            raise RuntimeError("Need nitf_file.file_name to get gdal_multi_band")
        segindex = [t for t, iseg in enumerate(self.nitf_file.image_segment)
                    if iseg is self][0]
        return GdalMultiBand("NITF_IM:%d:%s" % (segindex, self.nitf_file.file_name))

    def _gdal_raster_image(self):
        '''Because it is pretty common to have a 1 band image, return
        the data as a raster image.'''
        return _gdal_multi_band(self).gdal_raster_image(0)

    NitfImageSegment.raster_image_multi_band = property(_gdal_multi_band)
    NitfImageSegment.raster_image = property(_gdal_raster_image)

if(have_pynitf):
    __all__ = ["NitfImageGdal",]
else:
    __all__ = []
    
