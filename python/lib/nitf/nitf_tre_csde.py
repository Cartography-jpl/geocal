from .nitf_tre import *

hlp = '''This is the STDIDC TRE, Standard ID

The field names can be pretty cryptic, but are documented in detail in 
the NITF TRE documentation (STDI-0002 V4.0, available at 
http://www.gwg.nga.mil/ntb/baseline/docs/stdi0002).

There is a table in the main body on page vii that gives the a pointer for 
where in the document a particular TRE is defined.

STDIDC is documented at D-8.
'''
desc = ["STDIDC",
        ["acquisition_date", 14, str],
        ["mission", 14, str],
        ["pass", 2, str],
        ["op_num", 3, int],
        ["start_segment", 2, str],
        ["repro_num", 2, int],
        ["replay_regen", 3, str],
        ["blank_fill", 1, str],
        ["start_column", 3, int],
        ["start_row", 5, int],
        ["end_segment", 2, str],
        ["end_column", 3, int],
        ["end_row", 5, int],
        ["country", 2, str, {"optional" : True}],
        ["wac", 4, int, {"optional" : True}],
        ["location", 11, str],
	[None, 5, None],
	[None, 8, None]
]

TreSTDIDC = create_nitf_tre_structure("TreSTDIDC", desc, hlp=hlp)

hlp = '''This is the USE00A TRE, Exploitation Usability TRE. 

The field names can be pretty cryptic, but are documented in detail in 
the NITF TRE documentation (STDI-0002 V4.0, available at 
http://www.gwg.nga.mil/ntb/baseline/docs/stdi0002).

There is a table in the main body on page vii that gives the a pointer for 
where in the document a particular TRE is defined.

USE00A is documented at D-11.
'''
desc = ["USE00A",
        ["angle_to_north", 3, int],
        ["mean_gsd", 5, float, {"frmt" : "%05.1lf"}],
        [None, 1, str],
        ["dynamic_range", 5, int],
        [None, 3, str],
        [None, 1, str],
        [None, 3, str],
        ["obl_ang", 5, float, {"frmt" : "%05.2lf"}],
        ["roll_ang", 6, float, {"frmt" : "%+04.2lf"}],
        [None, 12, str],
        [None, 15, str],
        [None, 4, str],
        [None, 1, str],
        [None, 3, str],
        [None, 1, str],
        [None, 1, str],
        ["n_ref", 2, int],
        ["rev_num", 5, int],
        ["n_seg", 3, int],
        ["max_lp_seg", 6, int],
        [None, 6, str],
        [None, 6, str],
        ["sun_el", 5, float, {"frmt" : "%+05.1lf"}],
        ["sun_az", 5, float, {"frmt" : "%05.1lf"}],
]

TreUSE00A = create_nitf_tre_structure("TreUSE00A",desc,hlp=hlp)

