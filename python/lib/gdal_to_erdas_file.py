from __future__ import division
from builtins import str
from past.utils import old_div
from geocal_swig import *
import subprocess

def gdal_to_erdas_file(infname, outfname):
    '''This converts a GDAL file to ERDAS. We include the calculation of
    statistics and an image pyramid'''
    subprocess.check_call(["gdal_translate", 
                           "-of", "hfa",
                           "-co", "STATISTICS=YES",
                           "-co", "COMPRESSED=YES",
                           infname, outfname])
    # The default blocksize of ERDAS is 64x64. Since we just created the file,
    # we know this is the blocksize. 
    blocksize = 64
    cmd = ["gdaladdo", outfname, "2"]
    i = 2
    # We want to make image pyramids down to a single block
    infile = GdalRasterImage(infname)
    while(old_div(infile.number_line, i) > blocksize and
          old_div(infile.number_sample, i) > blocksize):
        i *= 2
        cmd.append(str(i))
    subprocess.check_call(cmd)

__all__ = ["gdal_to_erdas_file"]
