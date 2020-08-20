from geocal_swig import *
from geocal.mmap_file import *
import numpy as np

# TODO Add support for mmap
# TODO Add support for parallel runs
def ray_caster_project(out_fname, igc, map_info, dtype,
                       mmap_scratch=False):
    '''This puts together everything to generate map projected data.
    We write the output to the given file, as a VICAR file of given
    dtype.

    This uses scratch data which can either be kept all in memory, or
    for larger images we can optionally use temporary files for the 
    scratch data. This is controlled by mmap_scratch, if True we use
    scratch file otherwise we just use memory.
    '''
    pass
