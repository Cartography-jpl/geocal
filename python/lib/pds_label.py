from __future__ import print_function
from __future__ import division
from __future__ import absolute_import
import re

def pds_label_text(fname):
    '''While GDAL can read PDS format, it doesn't read most of the metadata.
    So this routine extracts out the front PDS metadata, which we could either
    process further, or store as a  "LBL" file (which is a detached header).

    This is PDS3 format, we don't yet handle PDS4.
    '''
    res = ''
    with open(fname, "r") as fh:
        while True:
            line = fh.readline()
            res += line
            if line.strip() == 'END' or line == '':
                break
    return res
        
def pds_label(fname):
    '''Read PDS data in and convert to a dict. Not sure how robust this is,
    we'll have to try this on a number of datasets and see what breaks.'''
    txt = pds_label_text(fname)
    res = dict()
    for ln in txt.split("\n"):
        m = re.match(r'\s*(\w+)\s*=(.+)*', ln)
        if(m):
            k = m.group(1)
            v = m.group(2).strip()
            m = re.match(r'"(.*)', v)
            if(m):
                v = m.group(1)
            res[k] = v
    return res
