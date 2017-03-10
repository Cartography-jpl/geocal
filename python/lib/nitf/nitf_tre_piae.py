from .nitf_tre import *

hlp = '''This is the PIAIMC TRE, Profile for Imagery Access Image

The field names can be pretty cryptic, but are documented in detail in 
the NITF TRE documentation (STDI-0002 V4.0, available at 
http://www.gwg.nga.mil/ntb/baseline/docs/stdi0002).

There is a table in the main body on page vii that gives the a pointer for 
where in the document a particular TRE is defined.

PIAIMC is documented at C-8.
'''
desc = ["PIAIMC",
        ["cloudcvr", 3, int, {"optional" : True}],
        ["srp", 1, str, {"optional" : True}],
        ["sensormode", 12, str, {"optional" : True}],
        ["sensname", 18, str, {"optional" : True}],
        ["source", 255, str, {"optional" : True}],
        ["comgen", 2, int, {"optional" : True}],
        ["subqual", 1, str, {"optional" : True}],
        ["piamsnnum", 7, str, {"optional" : True}],
        ["camspecs", 32, str, {"optional" : True}],
        ["projid", 2, str, {"optional" : True}],
        ["generation", 1, int, {"optional" : True}],
        ["esd", 1, str, {"optional" : True}],
        ["othercond", 2, str, {"optional" : True}],
        ["mean_gsd", 7, float, {"frmt" : "%07.1lf", "optional": True}],
        ["idatum", 3, str, {"optional" : True}],
        ["iellip", 3, str, {"optional" : True}],
        ["preproc", 2, str, {"optional" : True}],
        ["iproj", 2, str, {"optional" : True}],
        ["sattrack", 8, int, {"optional" : True}],
        ]

TrePIAIMC = create_nitf_tre_structure("TrePIAIMC", desc, hlp=hlp)
