# Just import any files we find in this directory, rather than listing
# everything.

import glob as _glob
import os as _os
import re as _re

from geocal_swig import *  # type: ignore

for _i in _glob.glob(_os.path.dirname(__file__) + "/*.py"):
    mname = _os.path.basename(_i).split(".")[0]
    # Don't load ipython, which is ipython magic extensions, or unit tests
    if mname != "ipython" and mname != "cython_try" and not _re.search("_test", mname):
        exec(f"from .{mname} import *")  # noqa: S102

# mypy can't follow the exec above, so __init__.pyi lists the modules
# explicitly for type checkers.

del _i
del _re
del _os
del _glob
