from .nitf_tre import *

hlp = '''This is the MAPLO TRE, Local Cartographic Coordinate System

The field names can be pretty cryptic, but are documented in detail in 
the NITF TRE documentation (STDI-0002 V4.0, available at 
http://www.gwg.nga.mil/ntb/baseline/docs/stdi0002).

There is a table in the main body on page vii that gives the a pointer for 
where in the document a particular TRE is defined.

This specific TRE is part of what is called "DIGEST", and is documented
in a separate document "The Digital Geographic Information Exchange Standard
(DIGEST) Part 2 - Annex D" (https://www.dgiwg.org/digest/html/DIGEST_2-1_D.pdf)

MAPLO is documented at table D1-7 starting on page D1-15
'''
desc = ["MAPLOB",
        ["uniloa", 3, str, {"default" : "M"}],
        ["lod", 5, int],
        ["lad", 5, int],
        ["lso", 15, float, {"frmt" : "%+015.1lf"}],
        ["pso", 15, float, {"frmt" : "%+015.1lf"}]
        ]
TreMAPLO = create_nitf_tre_structure("TreMAPLO", desc, hlp=hlp)

hlp = '''This is the GEOPS TRE, Local Cartographic Coordinate System

The field names can be pretty cryptic, but are documented in detail in 
the NITF TRE documentation (STDI-0002 V4.0, available at 
http://www.gwg.nga.mil/ntb/baseline/docs/stdi0002).

There is a table in the main body on page vii that gives the a pointer for 
where in the document a particular TRE is defined.

This specific TRE is part of what is called "DIGEST", and is documented
in a separate document "The Digital Geographic Information Exchange Standard
(DIGEST) Part 2 - Annex D" (https://www.dgiwg.org/digest/html/DIGEST_2-1_D.pdf)

GEOPS is documented at table D1-3 starting on page D1-11
'''
desc = ["GEOPSB",
        ["typ", 3, str, {"default" : "MAP"}],
        ["uni", 3, str, {"default" : "M"}],
        ["dag", 80, str, {"default": "World Geodetic System 1984"}],
        ["dcd", 4, str, {"default" : "WGE"}],
        ["ell", 80, str, {"default": "World Geodetic System 1984"}],
        ["elc", 3, str, {"default" : "WE"}],
        ["dvr", 80, str, {"default" : "Geodetic"}],
        ["vdcdvr", 4, str, {"default": "GEOD"}],
        ["sda", 80, str, {"default" : "Mean Sea"}],
        ["vdcsda" , 4, str, {"default" : "MSL"}],
        ["zor", 15, int],
        ["grd", 3, str],
        ["grn", 80, str],
        ["xna", 4, int]
]
TreGEOPS = create_nitf_tre_structure("TreGEOPS", desc, hlp=hlp)

hlp = '''This is the PRJPS TRE, Local Cartographic Coordinate System

The field names can be pretty cryptic, but are documented in detail in 
the NITF TRE documentation (STDI-0002 V4.0, available at 
http://www.gwg.nga.mil/ntb/baseline/docs/stdi0002).

There is a table in the main body on page vii that gives the a pointer for 
where in the document a particular TRE is defined.

This specific TRE is part of what is called "DIGEST", and is documented
in a separate document "The Digital Geographic Information Exchange Standard
(DIGEST) Part 2 - Annex D" (https://www.dgiwg.org/digest/html/DIGEST_2-1_D.pdf)

PRJPS is documented at table D1-4 starting on page D1-13
'''
desc = ["PRJPSB",
        ["prn", 80, str],
        ["pco", 2, str, {"default" : "TC"}],
        ["numprj", 1, int],
        [["loop", "f.numprj"],
         ["prj", 15, float]],
        ["xor", 15, int],
        ["yor", 15, int]
]
TrePRJPS = create_nitf_tre_structure("TrePRJPS", desc, hlp=hlp)

        
