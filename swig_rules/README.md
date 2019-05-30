This contains generic rules for wrapping code with SWIG.

This is not standalone code, it is meant to be included in a larger piece
of code. But this contains the SWIG specific part of this code.

Look for example at the [swig-rules-skeleton](https://github.jpl.nasa.gov/Cartography/swig-rules-skeleton) repository for a skeleton of using
this. A more full example can be found at 
[geocal](https://github.jpl.nasa.gov/Cartography/geocal).

Classes needed in code that uses this
-------------------------------------
The shared_ptr_type_mapper.i code depends on having a GenericObject 
available as generic_object.h. All the class should be derived from
this base class in order to be able to be cast automatically to the 
right type.

The code swig_type_mapper_base.cc should be compiled and included in
the parent library. This should have -DSWIG_MAPPER_NAMESPACE=GeoCal
or similar defined when compiled.
