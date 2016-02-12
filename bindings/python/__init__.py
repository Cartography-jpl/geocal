# Just import any files we find in this directory, rather than listing
# everything

from __future__ import absolute_import
import os
import glob
import importlib

from geocal_swig._swig_wrap import *

for i in glob.glob(os.path.dirname(__file__) + "/*.py"):
    mod = importlib.import_module(".%s" % os.path.basename(i).split('.')[0], "geocal_swig")
    locals().update(mod.__dict__)
