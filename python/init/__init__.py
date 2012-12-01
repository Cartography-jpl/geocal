# Just import any files we find in this directory, rather than listing
# everything

import os
import glob
from geocal import *

for i in glob.glob(os.path.dirname(__file__) + "/*.py"):
    exec 'from ' + os.path.basename(i).split('.')[0] + ' import *'
