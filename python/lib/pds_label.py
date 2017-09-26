from __future__ import print_function
from __future__ import division
from __future__ import absolute_import
import re
import bisect
from geocal_swig import *

def pds_label_text(fname):
    '''While GDAL can read PDS format, it doesn't read most of the metadata.
    So this routine extracts out the front PDS metadata, which we could either
    process further, or store as a  "LBL" file (which is a detached header).

    This is PDS3 format, we don't yet handle PDS4.
    '''
    res = b''
    with open(fname, "rb") as fh:
        while True:
            line = fh.readline()
            # Strip off /r. Shows up in python 2, but not 3. Not sure why
            # the difference, but in any case we don't want them
            res += line.replace(b'\r',b'')
            if line.strip() == b'END' or line == b'':
                break
    return res
        
def pds_label(fname):
    '''Read PDS data in and convert to a dict. Not sure how robust this is,
    we'll have to try this on a number of datasets and see what breaks.'''
    txt = pds_label_text(fname)
    res = dict()
    for ln in txt.split(b"\n"):
        m = re.match(b'\s*(\^?\w+)\s*=(.+)*', ln)
        if(m):
            k = m.group(1)
            v = m.group(2).strip()
            m = re.match(b'"(.*)"', v)
            if(m):
                v = m.group(1)
            res[k.decode("utf-8")] = str(v.decode("utf-8"))
    return res

class SpiceKernelByTime(object):
    '''In some cases, we have a range of SPICE kernels with one for a range
    of times. These kernels have PDS labels associated with them that give
    the time ranges. So for example the mro spk kernels.

    This class handles parsing all the PDS labels, setting up the mapping,
    and then loading the right kernel for a particular time.

    This class can be saved (e.g., use json or pickling pickling through 
    write_shelve) so we don't need to parse the directory every time.
    '''
    def __init__(self, lbl_file_list):
        '''Parse each PDS label file. Note that we assume the spice kernel is
        in the same directory as each PDS label.

        We read the PDS labels START_TIME, STOP_TIME and PRODUCT_ID.

        Often the label list can be created by a glob, e.g. 
        glob.glob("/raid26/mars_kernels/mro_kernels/spk/mro_*_ssd_mro95a.lbl")
        '''
        self.data = []
        for f in lbl_file_list:
            d = pds_label(f)
            self.data.append((Time.parse_time(d["START_TIME"]).et,
                              Time.parse_time(d["STOP_TIME"]).et,
                              os.path.abspath(os.path.dirname(f) + "/" +
                                              d["PRODUCT_ID"])))
        # Sort by start time
        self.data.sort(key=lambda v : v[0])
        self.tstart_et = [v[0] for v in self.data]
        self.tend_et = [v[1] for v in self.data]
        self.kernel_file = [v[2] for v in self.data]

    def __getstate__(self):
        return { "data" : self.data }

    def __setstate__(self, d):
        self.data = d["data"]
        self.tstart_et = [v[0] for v in self.data]
        self.tend_et = [v[1] for v in self.data]
        self.kernel_file = [v[2] for v in self.data]

    def kernel(self, tm):
        '''Return the kernel covering the given time. If nothing 
        matches, we raise a ValueError.'''
        # Find largest value less than or equal to tm
        tm_et = tm.et
        i = bisect.bisect_left(self.tstart_et, tm_et)
        if not i:
            raise ValueError
        # Check that this matches the upper range (since there may be
        # gaps in the data)
        if(tm_et > self.tend_et[i-1]):
            raise ValueError
        return self.kernel_file[i-1]
    
        
