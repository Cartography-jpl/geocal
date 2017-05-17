from .nitf_field import *
from .nitf_tre import *

hlp = '''This is the STDIDC TRE, blah. 

The field names can be pretty cryptic, but are documented in detail in 
the NITF TRE documentation (STDI-0002 V4.0, available at 
http://www.gwg.nga.mil/ntb/baseline/docs/stdi0002).

There is a table in the main body on page vii that gives the a pointer for 
where in the document a particular TRE is defined.

STDIDC is documented at blah.

NOTE that "pass" is a python keyword. Therefore, the 3rd field is called "pass_"
'''
desc = ["STDIDC",
        ["acquisition_date", "ACQ DATE", 14, str],
        ["mission", "MISSION", 14, str],
        ["pass_", "PASS", 2, str],
        ["op_num", "OP NUM", 3, int],
        ["start_segment", "START SEGMENT", 2, str],
        ["repro_num", "REPRO NUM", 2, int],
        ["replay_regen", "REPLAY REGEN", 3, str],
        ["blank_fill", "BLANK FILL", 1, str],
        ["start_column", "START COLUMN", 3, int],
        ["start_row", "START ROW", 5, int],
        ["end_segment", "END SEGMENT", 2, str],
        ["end_column", "END COLUMN", 3, int],
        ["end_row", "END ROW", 5, int],
        ["country", "COUNTRY", 2, str],
        ["wac", "W A C", 4, int],
        ["location", "LOCATION", 11, str],
        ["reserv01", "Reserved 1", 5, str],
        ["reserv02", "Reserved 2", 8, str],
]

TreSTDIDC = create_nitf_tre_structure("TreSTDIDC",desc,hlp=hlp)

TreSTDIDC.blank_fill = hardcoded_value(" ")
TreSTDIDC.reserv01 = hardcoded_value(" "*5)
TreSTDIDC.reserv02 = hardcoded_value(" "*8)