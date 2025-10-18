"""
doc_inherit decorator

Usage:

class Foo(object):
    def foo(self):
        "Frobber"
        pass

class Bar(Foo):
    @doc_inherit
    def foo(self):
        pass

Now, Bar.foo.__doc__ == Bar().foo.__doc__ == Foo.foo.__doc__ == "Frobber"
"""

from builtins import object

from functools import wraps
from typing import Any

class DocInherit(object):
    """
    Docstring inheriting method descriptor

    The class itself is also used as a decorator
    """

    def __init__(self, mthd : Any) -> None:
        self.mthd = mthd
        self.name = mthd.__name__

    def __get__(self, obj : object, cls : Any) -> Any:
        if obj:
            return self.get_with_inst(obj, cls)
        else:
            return self.get_no_inst(cls)

    def get_with_inst(self, obj : object, cls : Any) -> Any:
        overridden = getattr(super(cls, obj), self.name, None)

        @wraps(self.mthd, assigned=("__name__", "__module__"))
        def f(*args, **kwargs): # type: ignore
            return self.mthd(obj, *args, **kwargs)

        return self.use_parent_doc(f, overridden)

    def get_no_inst(self, cls : Any) -> Any:
        for parent in cls.__mro__[1:]:
            overridden = getattr(parent, self.name, None)
            if overridden:
                break

        @wraps(self.mthd, assigned=("__name__", "__module__"))
        def f(*args, **kwargs): # type: ignore
            return self.mthd(*args, **kwargs)

        return self.use_parent_doc(f, overridden)

    def use_parent_doc(self, func : Any, source : Any) -> Any:
        if source is None:
            raise NameError("Can't find '%s' in parents" % self.name)
        func.__doc__ = source.__doc__
        return func


doc_inherit = DocInherit

__all__ = ["doc_inherit", "DocInherit"]
