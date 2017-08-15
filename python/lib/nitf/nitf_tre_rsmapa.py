from __future__ import print_function
from .nitf_field import *
from .nitf_tre import *

hlp = '''This is the RSMAPA TRE, blah. 

The field names can be pretty cryptic, but are documented in detail in 
the NITF TRE documentation (STDI-0002 V4.0, available at 
http://www.gwg.nga.mil/ntb/baseline/docs/stdi0002).

There is a table in the main body on page vii that gives the a pointer for 
where in the document a particular TRE is defined.

RSMAPA is documented at blah.
'''
desc = ["RSMAPA",
        ["iid", "Image Identifier", 80, str],
        ["edition", "RSM Image Support Data Edition", 40, str],
        ["tid", "Triangulation ID", 40, str],
        ["npar", "Number of Parameters", 2, int],
        ["xuol", "Local Coord Origin (XUOL)", 21, str],
        ["yuol", "Local Coord Origin (YUOL)", 21, str],
        ["zuol", "Local Coord Origin (ZUOL)", 21, str],
        ["xuxl", "Local Coord Unit Vector (XUXL)", 21, str],
        ["xuyl", "Local Coord Unit Vector (XUYL)", 21, str],
        ["xuzl", "Local Coord Unit Vector (XUZL)", 21, str],
        ["yuxl", "Local Coord Unit Vector (YUXL)", 21, str],
        ["yuyl", "Local Coord Unit Vector (YUYL)", 21, str],
        ["yuzl", "Local Coord Unit Vector (YUZL)", 21, str],
        ["zuxl", "Local Coord Unit Vector (ZUXL)", 21, str],
        ["zuyl", "Local Coord Unit Vector (ZUYL)", 21, str],
        ["zuzl", "Local Coord Unit Vector (ZUZL)", 21, str],
        ["ir0", "Image Row Constant Index", 2, str],
        ["irx", "Image Row X Index", 2, str],
        ["iry", "Image Row Y Index", 2, str],
        ["irz", "Image Row Z Index", 2, str],
        ["irxx", "Image Row X^2 Index", 2, str],
        ["irxy", "Image Row XY Index", 2, str],
        ["irxz", "Image Row XZ Index", 2, str],
        ["iryy", "Image Row Y^2 Index", 2, str],
        ["iryz", "Image Row YZ Index", 2, str],
        ["irzz", "Image Row Z^2 Index", 2, str],
        ["ic0", "Image Col Constant Index", 2, str],
        ["icx", "Image Col X Index", 2, str],
        ["icy", "Image Col Y Index", 2, str],
        ["icz", "Image Col Z Index", 2, str],
        ["icxx", "Image Col X^2 Index", 2, str],
        ["icxy", "Image Col XY Index", 2, str],
        ["icxz", "Image Col XZ Index", 2, str],
        ["icyy", "Image Col Y^2 Index", 2, str],
        ["icyz", "Image Col YZ Index", 2, str],
        ["iczz", "Image Col Z^2 Index", 2, str],
        ["gx0", "Ground X Constant Index", 2, str],
        ["gy0", "Ground Y Constant Index", 2, str],
        ["gz0", "Ground Z Constant Index", 2, str],
        ["gxr", "Ground Rotation X", 2, str],
        ["gyr", "Ground Rotation Y", 2, str],
        ["gzr", "Ground Rotation Z", 2, str],
        ["gs", "Ground Scale", 2, str],
        ["gxx", "Ground X Adj Proportional to X index", 2, str],
        ["gxy", "Ground X Adj Proportional to Y index", 2, str],
        ["gxz", "Ground X Adj Proportional to Z index", 2, str],
        ["gyx", "Ground Y Adj Proportional to X index", 2, str],
        ["gyy", "Ground Y Adj Proportional to Y index", 2, str],
        ["gyz", "Ground Y Adj Proportional to Z index", 2, str],
        ["gzx", "Ground Z Adj Proportional to X index", 2, str],
        ["gzy", "Ground Z Adj Proportional to Y index", 2, str],
        ["gzz", "Ground Z Adj Proportional to Z index", 2, str],
        [["loop", "f.npar"],
        ["parval", "Component Value", 21, str],
        ],
]

TreRSMAPA = create_nitf_tre_structure("TreRSMAPA",desc,hlp=hlp)

