from __future__ import print_function
from .nitf_field import *
from .nitf_tre import *

hlp = '''This is the RSMIDA TRE, blah. 

The field names can be pretty cryptic, but are documented in detail in 
the NITF TRE documentation (STDI-0002 V4.0, available at 
http://www.gwg.nga.mil/ntb/baseline/docs/stdi0002).

There is a table in the main body on page vii that gives the a pointer for 
where in the document a particular TRE is defined.

RSMIDA is documented at blah.
'''

_second_format = "%02.6lf"

desc = ["RSMIDA",
        ["iid", "Image Identifier", 80, str],
        ["edition", "RSM Image Support Data Edition", 40, str],
        ["isid", "Image Sequence Identifier", 40, str],
        ["sid", "Sensor Identifier", 40, str],
        ["stid", "Sensor Type Identifier", 40, str],
        ["year", "Year of Image Acquisition", 4, int],
        ["month", "Month of Image Acquisition", 2, int],
        ["day", "Day of Image Acquisition", 2, int],
        ["hour", "Hour of Image Acquisition", 2, int],
        ["minute", "Minute of Image Acquisition", 2, int],
        ["second", "Second of Image Acquisition", 9, float, {"frmt" : _second_format}],
        ["nrg", "Number of Rows Acquired Simultaneously", 8, str],
        ["ncg", "Number of Columns Acquired Simultaneously", 8, str],
        ["trg", "Time Between Adjacent Row Groups", 21, str],
        ["tcg", "Time Between Adjacent Column Groups", 21, str],
        ["grndd", "Ground Domain Form", 1, str],
        ["xuor", "Regular Coordinate Origin (XUOR)", 21, str],
        ["yuor", "Regular Coordinate Origin (YUOR)", 21, str],
        ["zuor", "Regular Coordinate Origin (ZUOR)", 21, str],
        ["xuxr", "Rectangular Coord Unit Vector (XUXR)", 21, str],
        ["xuyr", "Rectangular Coord Unit Vector (XUYR)", 21, str],
        ["xuzr", "Rectangular Coord Unit Vector (XUZR)", 21, str],
        ["yuxr", "Rectangular Coord Unit Vector (YUXR)", 21, str],
        ["yuyr", "Rectangular Coord Unit Vector (YUYR)", 21, str],
        ["yuzr", "Rectangular Coord Unit Vector (YUZR)", 21, str],
        ["zuxr", "Rectangular Coord Unit Vector (ZUXR)", 21, str],
        ["zuyr", "Rectangular Coord Unit Vector (ZUYR)", 21, str],
        ["zuzr", "Rectangular Coord Unit Vector (ZUZR)", 21, str],
        ["v1x", "Vertex 1 - X coord of the RSM ground domain", 21, str],
        ["v1y", "Vertex 1 - Y coord of the RSM ground domain", 21, str],
        ["v1z", "Vertex 1 - Z coord of the RSM ground domain", 21, str],
        ["v2x", "Vertex 2 - X coord of the RSM ground domain", 21, str],
        ["v2y", "Vertex 2 - Y coord of the RSM ground domain", 21, str],
        ["v2z", "Vertex 2 - Z coord of the RSM ground domain", 21, str],
        ["v3x", "Vertex 3 - X coord of the RSM ground domain", 21, str],
        ["v3y", "Vertex 3 - Y coord of the RSM ground domain", 21, str],
        ["v3z", "Vertex 3 - Z coord of the RSM ground domain", 21, str],
        ["v4x", "Vertex 4 - X coord of the RSM ground domain", 21, str],
        ["v4y", "Vertex 4 - Y coord of the RSM ground domain", 21, str],
        ["v4z", "Vertex 4 - Z coord of the RSM ground domain", 21, str],
        ["v5x", "Vertex 5 - X coord of the RSM ground domain", 21, str],
        ["v5y", "Vertex 5 - Y coord of the RSM ground domain", 21, str],
        ["v5z", "Vertex 5 - Z coord of the RSM ground domain", 21, str],
        ["v6x", "Vertex 6 - X coord of the RSM ground domain", 21, str],
        ["v6y", "Vertex 6 - Y coord of the RSM ground domain", 21, str],
        ["v6z", "Vertex 6 - Z coord of the RSM ground domain", 21, str],
        ["v7x", "Vertex 7 - X coord of the RSM ground domain", 21, str],
        ["v7y", "Vertex 7 - Y coord of the RSM ground domain", 21, str],
        ["v7z", "Vertex 7 - Z coord of the RSM ground domain", 21, str],
        ["v8x", "Vertex 8 - X coord of the RSM ground domain", 21, str],
        ["v8y", "Vertex 8 - Y coord of the RSM ground domain", 21, str],
        ["v8z", "Vertex 8 - Z coord of the RSM ground domain", 21, str],
        ["grpx", "Ground Reference Point X", 21, str],
        ["grpy", "Ground Reference Point Y", 21, str],
        ["grpz", "Ground Reference Point Z", 21, str],
        ["fullr", "Number of Rows in Full Image", 8, str],
        ["fullc", "Number of Cols in Full Image", 8, str],
        ["minr", "Minimum Row", 8, str],
        ["maxr", "Maximum Row", 8, str],
        ["minc", "Minimum Col", 8, str],
        ["maxc", "Maximum Col", 8, str],
        ["ie0", "Illum Elevation Angle Const Coeff", 21, str],
        ["ier", "Illum Elevation Angle Coeff Per Row", 21, str],
        ["iec", "Illum Elevation Angle Coeff Per Col", 21, str],
        ["ierr", "Illum Elevation Angle Coeff Per Row^2", 21, str],
        ["ierc", "Illum Elevation Angle Coeff Per Row-Col", 21, str],
        ["iecc", "Illum Elevation Angle Coeff Per Col^2", 21, str],
        ["ie0", "Illum Azimuth Angle Const", 21, str],
        ["iar", "Illum Azimuth Angle Coeff Per Row", 21, str],
        ["iac", "Illum Azimuth Angle Coeff Per Col", 21, str],
        ["iarr", "Illum Azimuth Angle Coeff Per Row^2", 21, str],
        ["iarc", "Illum Azimuth Angle Coeff Per Row-Col", 21, str],
        ["iacc", "Illum Azimuth Angle Coeff Per Col^2", 21, str],
        ["spx", "Sensor x-position", 21, str],
        ["svx", "Sensor x-velocity", 21, str],
        ["sax", "Sensor x-acceleration", 21, str],
        ["spy", "Sensor y-position", 21, str],
        ["svy", "Sensor y-velocity", 21, str],
        ["say", "Sensor y-acceleration", 21, str],
        ["spz", "Sensor z-position", 21, str],
        ["svz", "Sensor z-velocity", 21, str],
        ["saz", "Sensor z-acceleration", 21, str],
]

TreRSMIDA = create_nitf_tre_structure("TreRSMIDA",desc,hlp=hlp)

