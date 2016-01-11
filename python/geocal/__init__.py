# Just import any files we find in this directory, rather than listing
# everything

from __future__ import absolute_import
import os
import re
import glob
from geocal_swig import *

__path__.append(os.path.dirname(__file__) + "/../lib")

from .safe_matplotlib_import import *

for i in glob.glob(os.path.dirname(__file__) + "/../lib/*.py"):
    mname = os.path.basename(i).split('.')[0]
    # Don't load ipython, which is ipython magic extensions, or unit tests
    if(not mname == 'ipython' and
       not re.search('_test', mname)):
        exec("from .%s import *" % mname)
