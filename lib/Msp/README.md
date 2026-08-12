This is SWIG wrappers to a subset of the MSP library. We have this pulled out
because 1) We don't want to geocal to directly depend on the MSP library and
2) Because the MSP library was built with gcc 4.8.5 we need special compiler
flags to link against it 