from __future__ import print_function
from .nitf_field import *
from .nitf_tre import *

hlp = '''This is the RSMPCA TRE, blah. 

The field names can be pretty cryptic, but are documented in detail in 
the NITF TRE documentation (STDI-0002 V4.0, available at 
http://www.gwg.nga.mil/ntb/baseline/docs/stdi0002).

There is a table in the main body on page vii that gives the a pointer for 
where in the document a particular TRE is defined.

RSMPCA is documented at blah.
'''
desc = ["RSMPCA",
        ["iid", "Image Identifier", 80, str],
        ["edition", "RSM Image Support Data Edition", 40, str],
        ["rsn", "Row Section Number", 3, int],
        ["csn", "Column Section Number", 3, int],
        ["rfep", "Row Fitting Error", 21, str],
        ["cfep", "Col Fitting Error", 21, str],
        ["rnrmo", "Row Normalization Offset", 21, str],
        ["cnrmo", "Col Normalization Offset", 21, str],
        ["xnrmo", "X Normalization Offset", 21, str],
        ["ynrmo", "Y Normalization Offset", 21, str],
        ["znrmo", "Z Normalization Offset", 21, str],
        ["rnrmsf", "Row Normalization Scale Factor", 21, str],
        ["cnrmsf", "Col Normalization Scale Factor", 21, str],
        ["xnrmsf", "X Normalization Scale Factor", 21, str],
        ["ynrmsf", "Y Normalization Scale Factor", 21, str],
        ["znrmsf", "Z Normalization Scale Factor", 21, str],
        ["rnpwrx", "Row Numerator Poly Max Power of X", 1, int],
        ["rnpwry", "Row Numerator Poly Max Power of Y", 1, int],
        ["rnpwrz", "Row Numerator Poly Max Power of Z", 1, int],
        ["rntrms", "Row Numerator Poly Number of Poly Terms", 3, int],
        [["loop", "f.rntrms"],
        ["rnpcf", "Poly Coeff", 21, str],
        ],
        ["rdpwrx", "Row Denominator Poly Max Power of X", 1, int],
        ["rdpwry", "Row Denominator Poly Max Power of Y", 1, int],
        ["rdpwrz", "Row Denominator Poly Max Power of Z", 1, int],
        ["rdtrms", "Row Denominator Poly Number of Poly Terms", 3, int],
        [["loop", "f.rdtrms"],
        ["rdpcf", "Poly Coeff", 21, str],
        ],
        ["cnpwrx", "Row Numerator Poly Max Power of X", 1, int],
        ["cnpwry", "Row Numerator Poly Max Power of Y", 1, int],
        ["cnpwrz", "Row Numerator Poly Max Power of Z", 1, int],
        ["cntrms", "Row Numerator Poly Number of Poly Terms", 3, int],
        [["loop", "f.cntrms"],
        ["cnpcf", "Poly Coeff", 21, str],
        ],
        ["cdpwrx", "Row Denominator Poly Max Power of X", 1, int],
        ["cdpwry", "Row Denominator Poly Max Power of Y", 1, int],
        ["cdpwrz", "Row Denominator Poly Max Power of Z", 1, int],
        ["cdtrms", "Row Denominator Poly Number of Poly Terms", 3, int],
        [["loop", "f.cdtrms"],
        ["cdpcf", "Poly Coeff", 21, str],
        ],
]

TreRSMPCA = create_nitf_tre_structure("TreRSMPCA",desc,hlp=hlp)

