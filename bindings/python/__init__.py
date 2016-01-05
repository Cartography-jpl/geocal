# Just import any files we find in this directory, rather than listing
# everything

import os
import glob

from geocal_swig._swig_wrap import *
from sys import version_info

for i in glob.glob(os.path.dirname(__file__) + "/*.py"):
    # Use relative import if we are using python 3
    if version_info >= (3, 0, 0):
        exec('from .' + os.path.basename(i).split('.')[0] + ' import *')
    else:
        exec('from ' + os.path.basename(i).split('.')[0] + ' import *')
        
