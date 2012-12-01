// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

//--------------------------------------------------------------
// This defines the function %shared_ptr_dynamic_list(base, type1, type2...).
// This is used in conjunction with %shared_ptr, and it allows a
// function returning a boost::shared_ptr<base> to determine
// the underlying type to pass to python. The list is searched i
// order, so you should have more general types later.
//
// Nothing bad happens if a particular type Foo is not in the list, we
// just return the most general type base in that case. This means
// any functions specific to Foo won't be able to be used by python,
// but all the function found in Base will be able to.
//
// See for example datum.i for an example of using
// this.
//--------------------------------------------------------------

%define %_shared_ptr_dispatch(TYPE)
if (!dcast) {
  boost::shared_ptr< TYPE > dobj = boost::dynamic_pointer_cast< TYPE >($1);
  if (dobj) {
    dcast = 1;
    boost::shared_ptr< TYPE > *smartresult = new boost::shared_ptr< TYPE >(dobj);
    %set_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(boost::shared_ptr< TYPE > *), SWIG_POINTER_OWN));
  }   
}%enddef

%define %_shared_ptr_dispatch2(TYPE)
if (!dcast) {
  boost::shared_ptr< TYPE > dobj = boost::dynamic_pointer_cast< TYPE >(*$1);
  if (dobj) {
    dcast = 1;
    boost::shared_ptr< TYPE > *smartresult = new boost::shared_ptr< TYPE >(dobj);
    %set_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(boost::shared_ptr< TYPE > *), SWIG_POINTER_OWN));
  }   
}%enddef

%define %shared_ptr_dynamic_list(BaseType, Types...)
%typemap(out) boost::shared_ptr<BaseType> {
  int dcast = 0;
  %formacro(%_shared_ptr_dispatch, Types)
  if (!dcast) {
    boost::shared_ptr< BaseType > *smartresult = new SWIG_SHARED_PTR_QNAMESPACE::shared_ptr< BaseType >($1);
    %set_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(boost::shared_ptr< BaseType > *), SWIG_POINTER_OWN));
  }
}

%typemap(out) const boost::shared_ptr<BaseType>& {
  int dcast = 0;
  %formacro(%_shared_ptr_dispatch2, Types)
  if (!dcast) {
    boost::shared_ptr< BaseType > *smartresult = new SWIG_SHARED_PTR_QNAMESPACE::shared_ptr< BaseType >(*$1);
    %set_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(boost::shared_ptr< BaseType > *), SWIG_POINTER_OWN));
  }
}

%typemap(out) boost::shared_ptr<BaseType>& {
  int dcast = 0;
  %formacro(%_shared_ptr_dispatch2, Types)
  if (!dcast) {
    boost::shared_ptr< BaseType > *smartresult = new SWIG_SHARED_PTR_QNAMESPACE::shared_ptr< BaseType >(*$1);
    %set_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(boost::shared_ptr< BaseType > *), SWIG_POINTER_OWN));
  }
}

%enddef

