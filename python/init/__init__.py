# Just import any files we find in this directory, rather than listing
# everything

import os
import glob
from geocal import *

# Make sure we can safely import matplotlib without getting an error
# (see this module for details on this)
import safe_matplotlib_import

for i in glob.glob(os.path.dirname(__file__) + "/*.py"):
    exec 'from ' + os.path.basename(i).split('.')[0] + ' import *'
