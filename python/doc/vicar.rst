*************
Calling VICAR
*************

We have code to wrap calls to VICAR. 

To run VICAR, we create a temporary directory and run in that directory.
This is because VICAR tends to vomit lots of temporary files that we want
to clean up. Also, different PDFs tend to clash in the file names used, so
if we want to allow parallel execution of commands we need to isolate them.

The base class for working with VICAR is 
:class:`VicarInterface <afids.vicar_interface.VicarInterface>`.

vicar_to_numpy
==============

A very useful function defined in this module is 
:func:`vicar_to_numpy <afids.vicar_interface.vicar_to_numpy>`. This
handle the common case of running a piece of VICAR code, and then reading
the resulting output into a numpy array for some further processing.

# .. ipython::

   In [1]: from afids import *

   In [3]: a = vicar_to_numpy("gen out NL=15 NS=15 IVAL=90")

   In [5]: print a[1,2]

