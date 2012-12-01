// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal

// This contains mappings from the underlying C types to the Ruby GTK
// library types.
#ifdef SWIGRUBY
%{
// Don't actually include this, so we don't have an explicit
// dependency. Instead we just copy the relevant lines here.
// #include "rbgobject.h"
#define GOBJ2RVAL(gobj) (rbgobj_ruby_object_from_instance(gobj))
extern "C" {
extern VALUE rbgobj_ruby_object_from_instance(void* instance);
}
%}

%typemap(out) GtkWidget* {
  $result = GOBJ2RVAL($1);
}
#endif
