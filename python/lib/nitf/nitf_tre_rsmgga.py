from __future__ import print_function
from .nitf_field import *
from .nitf_tre import *

hlp = '''This is the RSMGGA TRE, blah. 

The field names can be pretty cryptic, but are documented in detail in 
the NITF TRE documentation (STDI-0002 V4.0, available at 
http://www.gwg.nga.mil/ntb/baseline/docs/stdi0002).

There is a table in the main body on page vii that gives the a pointer for 
where in the document a particular TRE is defined.

RSMGGA is documented at blah.
'''

_ggrfep_format = "%21.14E"

desc = ["RSMGGA",
        ["iid", "Image Identifier", 80, str, {'optional':True}],
        ["edition", "RSM Image Support Data Edition", 40, str],
        ["ggrsn", "Ground-to-image Grid Row Section Number", 3, int],
        ["ggcsn", "Ground-to-image Grid Column Section Number", 3, int],
        ["ggrfep", "Ground-to-image Grid Row Fitting Error", 21, float, {'frmt' : _ggrfep_format}],
        ["ggcfep", "Ground-to-image Grid Column Fitting Error", 21, float, {'frmt' : _ggrfep_format}],
        ["intord", "Ground-to-image Grid Interpolation", 1, int, {'optional':True}],
        ["npln", "Number of Grid Planes", 3, int],
        ["deltaz", "Delta Z between Grid Planes", 21, float, {'frmt': _ggrfep_format}],
        ["deltax", "Delta X between Grid Planes", 21, float, {'frmt': _ggrfep_format}],
        ["deltay", "Delta Y between Grid Planes", 21, float, {'frmt': _ggrfep_format}],
        ["zpln1", "Z Value of Plane 1", 21, float, {'frmt': _ggrfep_format}],
        ["xipln1", "X Value of Initial Point in Plane 1", 21, float, {'frmt': _ggrfep_format}],
        ["yipln1", "Y Value of Initial Point in Plane 1", 21, float, {'frmt': _ggrfep_format}],
        ["refrow", "Reference Image Row Coordinate Value", 9, int],
        ["refcol", "Reference Image Column Coordinate Value", 9, int],
        ["tnumrd", "Total Number of Image Row Coordinate Digits", 2, int],
        ["tnumcd", "Total Number of Image Column Coordinate Digits", 2, int],
        ["fnumrd", "Number of Image Row Coordinate Fractional Digits", 1, int],
        ["fnumcd", "Number of Image Column Coordinate Fractional Digits", 1, int],
        [["loop", "f.npln-1"],
         ["ixo", "Initial Grid Points X Offset", 4, int],
         ["iyo", "Initial Grid Points Y Offset", 4, int]
        ],
        [["loop", "f.npln"],
         ["nxpts", "Number of Grid Points in the X Direction", 3, int],
         ["nypts", "Number of Grid Points in the Y Direction", 3, int],
         [["loop", "f.nxpts[i1] * f.nypts[i1]"],
          ["rcoord", "Grid Point's Row Coordinate", "f.tnumrd", None, {'field_value_class' : StringFieldData, 'optional':True}],
          ["ccoord", "Grid Point's Row Coordinate", "f.tnumcd", None, {'field_value_class' : StringFieldData, 'optional':True}]
          #["rcoord", "Grid Point's Row Coordinate", "f.tnumrd", int, {'optional':True}],
          #["ccoord", "Grid Point's Row Coordinate", "f.tnumcd", int, {'optional':True}]
         ]
        ]
]

TreRSMGGA = create_nitf_tre_structure("TreRSMGGA",desc,hlp=hlp)

