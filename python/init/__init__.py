# Just import any files we find in this directory, rather than listing
# everything

from __future__ import absolute_import
import os
import glob
from geocal_swig import *

from .safe_matplotlib_import import *

for i in glob.glob(os.path.dirname(__file__) + "/*.py"):
    mname = os.path.basename(i).split('.')[0]
    # Don't load ipython, which is ipython magic extensions
    if(not mname == 'ipython'):
        exec("from .%s import *" % mname)
