from __future__ import print_function
from .nitf_field import *
from .nitf_tre import *
from geocal_swig import RsmGrid

class BaseCoord(FieldData):
    '''The row/column coordinate is a scaled integer. This class handles 
    presenting this as an actual value.'''
    def conv_float(self, t, parent_obj):
        raise NotImplemented
    def get(self,parent_obj,key):
        if(self.loop is not None):
            self.loop.check_index(parent_obj, key)
        if(not self.check_condition(parent_obj, key)):
            return None
        t = self.value(parent_obj)[key]
        if(t.rstrip() == ""):
            return None
        return self.conv_float(t, parent_obj)
    def get_print(self, parent_obj,key):
        t = self.get(parent_obj,key)
        if(t is None):
            return "None"
        return str(t)

class RCoord(BaseCoord):
    def conv_float(self, t, parent_obj):
        return int(t) / pow(10.0, parent_obj.fnumrd) + parent_obj.refrow

class CCoord(BaseCoord):
    def conv_float(self, t, parent_obj):
        return int(t) / pow(10.0, parent_obj.fnumcd) + parent_obj.refcol    
    
hlp = '''This is the RSMGGA TRE, Replacement Sensor Model Ground-to-Image Grid 

This TRE is mostly implemented by the RsmGrid available as
rsm_grid. This should be used to set the TRE values, and to use the
TRE values. This is handled mostly transparently, except that if you
update rsm_grid the raw fields in the TRE might not be updated. Call
update_raw_field() if you have modified rsm_grid and wish to access
the raw fields.

The field names can be pretty cryptic, but are documented in detail in 
the NITF TRE documentation (STDI-0002 V4.0, available at 
http://www.gwg.nga.mil/ntb/baseline/docs/stdi0002).

There is a table in the main body on page vii that gives the a pointer for 
where in the document a particular TRE is defined.

RSMGGA is documented at U-6, which points to other documentation, such as
"Replacement Sensor Model Tagged Record Extensions Specification for NITF
2.1" (http://www.gwg.nga.mil/ntb/baseline/docs/RSM/RSM_NITF_TREs_v1.0_.pdf)
'''

_ggrfep_format = "%+21.14E"

desc = ["RSMGGA",
        ["iid", "Image Identifier", 80, str, {'optional':True}],
        ["edition", "RSM Image Support Data Edition", 40, str],
        ["ggrsn", "Ground-to-image Grid Row Section Number", 3, int],
        ["ggcsn", "Ground-to-image Grid Column Section Number", 3, int],
        ["ggrfep", "Ground-to-image Grid Row Fitting Error", 21, float, {'frmt' : _ggrfep_format, 'optional':True}],
        ["ggcfep", "Ground-to-image Grid Column Fitting Error", 21, float, {'frmt' : _ggrfep_format, 'optional':True}],
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
          ["rcoord", "Grid Point's Row Coordinate", "f.tnumrd", None, {'field_value_class' : RCoord}],
          ["ccoord", "Grid Point's Row Coordinate", "f.tnumcd", None, {'field_value_class' : CCoord}]
         ]
        ]
]

TreRSMGGA = create_nitf_tre_structure("TreRSMGGA",desc,hlp=hlp,
                        tre_implementation_field="rsm_grid",
                        tre_implementation_class=RsmGrid)

