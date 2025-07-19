from builtins import object
from .docopt import *
from geocal_swig import VicarArgument
import sys
import re
import os


class DocOptSimple(object):
    """The package docopt (http://docopt.org) is a nice package,
    but it has the disadvantage that getting the options etc. from it
    uses a somewhat unnatural interface (e.g., --my-opt=n comes as
    int(d["--my-opt"]) rather than OptionParser cleaner sort of
    options.my_opt). This class tries to give a simple interface that
    is sufficient for many purposes. If you try to access the attribute
    "my_val", we first look for the argument "my_val", then "<my_val>",
    then "--my_val", then "--my-val". If the value looks like a integer,
    we return it to an integer. If it looks like a float, we return it to
    a float. If we don't otherwise recognize it, we return this as a string."""

    def __init__(self, doc, argv=None, help=True, version=None, options_first=False):
        self.args = docopt(
            doc, argv=argv, help=help, version=version, options_first=options_first
        )

    def __getstate__(self):
        return {"args": self.args}

    def __setstate__(self, dict):
        self.args = dict["args"]

    def __contains__(self, name):
        for key in (name, "<" + name + ">", "--" + name, "--" + name.replace("_", "-")):
            if key in self.args:
                return True
        return False

    def __getattr__(self, name):
        # Don't normally get called with "args", but can before object is
        # fully initialized. So catch this an return an empty dict, without
        # this handling we can enter an infinite recursion
        if name == "args":
            self.args = {}
            return self.args
        for key in (name, "<" + name + ">", "--" + name, "--" + name.replace("_", "-")):
            if key in self.args:
                return self.__find_type(key)
        raise AttributeError(name)

    def __find_type(self, key):
        """Find the type of the value, and return in"""
        v = self.args[key]
        if isinstance(v, str):
            if re.match(r"[+-]?\d+$", v):
                return int(v)
            if re.match(r"[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?$", v):
                return float(v)
            if re.match(r"[-+]?[0-9]+\.([eE][-+]?[0-9]+)?$", v):
                return float(v)
        return v


class DocOptSimpleVicar(object):
    """Make VicarArgument look like DocOptSimple."""

    def __init__(self, argv, long_name_to_vicar):
        if argv is None:
            argv = sys.argv
        self.argv = argv
        self.long_name_to_vicar = long_name_to_vicar
        self.va = VicarArgument(argv)

    def __getstate__(self):
        return {"argv": self.argv, "long_name_to_vicar": self.long_name_to_vicar}

    def __setstate__(self, dict):
        self.argv = dict["argv"]
        self.long_name_to_vicar = dict["long_name_to_vicar"]
        self.va = VicarArgument(self.argv)

    def __contains__(self, name):
        if self.long_name_to_vicar and name in self.long_name_to_vicar:
            name = self.long_name_to_vicar[name]
        try:
            t = self.va[name]
            return True
        except RuntimeError as e:
            if e.args[0] == "Call to zvpstat failed":
                return False
            raise

    def __getattr__(self, name):
        if self.long_name_to_vicar and name in self.long_name_to_vicar:
            name = self.long_name_to_vicar[name]
        t = self.va[name]
        # Treat "" or "n" as false
        if t == "" or t == "n":
            return False
        return t


def docopt_simple(
    doc,
    argv=None,
    help=True,
    version=None,
    options_first=False,
    long_name_to_vicar=None,
):
    """The package docopt (http://docopt.org) is a nice package,
    but it has the disadvantage that getting the options etc. from it
    uses a somewhat unnatural interface. This gives a simpler interface
    sufficient for many programs. See DocOptSimple class for details
    on the interface.

    This can work with either a VICAR call or a normal unix call, if we
    have a VICAR .pdf file supplied. Because VICAR has a much shorter name
    constraint (default 15 characters), you can supply a mapping between the
    longer more readable name and the VICAR name used in the PDF file. This
    allows programs to be agnostic about if we get arguments from VICAR
    or from unix"""
    # Test for being called as a VICAR proc. We assume we have been if:
    #
    # 1. We get only one command line argument (the name of the script)
    # 2. There is a .pdf file next to the script
    #
    # Note this isn't full proof, but I'm not sure what else to check
    if argv is None and len(sys.argv) == 1 and os.path.exists(sys.argv[0] + ".pdf"):
        return DocOptSimpleVicar(argv, long_name_to_vicar)
    # Otherwise, we use the normal docopt parsing of the command line
    return DocOptSimple(
        doc, argv=argv, help=help, version=version, options_first=options_first
    )


__all__ = ["docopt_simple"]
