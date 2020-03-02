from .nitf_field_old import StringFieldDataOld
from .nitf_tre import *
import io

hlp = '''This is the MATESA TRE, Mates.

The field names can be pretty cryptic, but are documented in detail in
the NITF TRE documentation (STDI-0002 V4.0, available at
http://www.gwg.nga.mil/ntb/baseline/docs/stdi0002).

MATESA v1.0 final v4 2019-02-12.
'''

desc = ["MATESA",
        ["cur_source", "Current File/Segment Source", 42, str],
        ["cur_mate_type", "Current File/Segment Mate Type", 16, str],
        ["cur_file_id_len", "Length of CUR FILE ID field", 4, int],
        ["cur_file_id", "ID of the Current File/Segment", "f.cur_file_id_len", None,
          {'field_value_class' : StringFieldDataOld}],
        ["num_groups", "Number of Mate Relationship Types", 4, int],
        [["loop", "f.num_groups"],
         ["relationship", "Mate Relationship", 24, str],
         ["num_mates", "Number of Mates", 4, int],
         [["loop", "f.num_mates[i1]"],
          ["source", "Mate Source", 42, str],
          ["mate_type", "Identifier Type", 16, str],
          ["mate_id_len", "Length of the mth MATE IDnm field", 4, int],

          #TODO: This length doesn't seem to be enforced. Look into it later. It's working
          #TODO: correctly otherwise.
          ["mate_id", "Mate File Identifier","f.mate_id_len[i1, i2]", None,
          {'field_value_class' : StringFieldDataOld}],
         ],
        ],
]

TreMATESA = create_nitf_tre_structure("TreMATESA",desc,hlp=hlp)

def _summary(self):
    res = io.StringIO()
    print("MATESA num_groups:%d" % (self.num_groups), file=res)
    return res.getvalue()

TreMATESA.summary = _summary

__all__ = [ "TreMATESA" ]
