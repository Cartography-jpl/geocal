from __future__ import print_function
from .nitf_field import *
from .nitf_tre import *

hlp = '''This is the RSMPIA TRE, blah. 

The field names can be pretty cryptic, but are documented in detail in 
the NITF TRE documentation (STDI-0002 V4.0, available at 
http://www.gwg.nga.mil/ntb/baseline/docs/stdi0002).

There is a table in the main body on page vii that gives the a pointer for 
where in the document a particular TRE is defined.

RSMPIA is documented at blah.
'''
desc = ["RSMPIA",
        ["iid", "Image Identifier", 80, str],
        ["edition", "RSM Image Support Data Edition", 40, str],
        ["r0", "Low Order Poly Const Coeff for Row", 21, str],
        ["rx", "Low Order Poly Coff of X for Row", 21, str],
        ["ry", "Low Order Poly Coff of Y for Row", 21, str],
        ["rz", "Low Order Poly Coff of Z for Row", 21, str],
        ["rxx", "Low Order Poly Coff of XX for Row", 21, str],
        ["rxy", "Low Order Poly Coff of XY for Row", 21, str],
        ["rxz", "Low Order Poly Coff of XZ for Row", 21, str],
        ["ryy", "Low Order Poly Coff of YY for Row", 21, str],
        ["ryz", "Low Order Poly Coff of YZ for Row", 21, str],
        ["rzz", "Low Order Poly Coff of ZZ for Row", 21, str],
        ["c0", "Low Order Poly Const Coeff for Col", 21, str],
        ["cx", "Low Order Poly Coff of X for Col", 21, str],
        ["cy", "Low Order Poly Coff of Y for Col", 21, str],
        ["cz", "Low Order Poly Coff of Z for Col", 21, str],
        ["cxx", "Low Order Poly Coff of XX for Col", 21, str],
        ["cxy", "Low Order Poly Coff of XY for Col", 21, str],
        ["cxz", "Low Order Poly Coff of XZ for Col", 21, str],
        ["cyy", "Low Order Poly Coff of YY for Col", 21, str],
        ["cyz", "Low Order Poly Coff of YZ for Col", 21, str],
        ["czz", "Low Order Poly Coff of ZZ for Col", 21, str],
        ["rnis", "Row Number of Image Sections", 3, int],
        ["cnis", "Column Number of Image Sections", 3, int],
        ["tnis", "Total Number of Image Sections", 3, int],
        ["rssiz", "Section Size in Rows", 21, str],
        ["cssiz", "Section Size in Cols", 21, str],
]

TreRSMPIA = create_nitf_tre_structure("TreRSMPIA",desc,hlp=hlp)

