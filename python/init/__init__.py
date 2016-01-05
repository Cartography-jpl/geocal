# Just import any files we find in this directory, rather than listing
# everything

import os
import glob
from geocal_swig import *

# Make sure we can safely import matplotlib without getting an error
# (see this module for details on this)
from sys import version_info
# Use relative import if we are using python 3
if version_info >= (3, 0, 0):
    from .safe_matplotlib_import import *
else:
    import safe_matplotlib_import


for i in glob.glob(os.path.dirname(__file__) + "/*.py"):
    mname = os.path.basename(i).split('.')[0]
    # Don't load ipython, which is ipython magic extensions
    if(not mname == 'ipython'):
        # Use relative import if we are using python 3
        if version_info >= (3, 0, 0):
            exec("from .%s import *" % mname)
        else:
            exec("from %s import *" % mname)
