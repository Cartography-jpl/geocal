from .nitf_tre import *
from .nitf_tre_rsmgia import *
from test_support import *
import io, six

def test_tre_rsmpia(rsm_ms_polynomial):
    t = TreRSMGIA()
    t.rsm_multi_section = rsm_ms_polynomial
    t.update_raw_field()
    fh = six.BytesIO()
    t.write_to_file(fh)
    # This can vary depending on roundoff, so don't compare.
    #assert fh.getvalue() == b'Blah'
    fh2 = six.BytesIO(fh.getvalue())
    t2 = TreRSMGIA()
    t2.read_from_file(fh2)
    print(t2)
    assert t2.rsm_multi_section.number_row_section == 3
    assert t2.rsm_multi_section.number_col_section == 2
    assert t2.gcssiz == 13763.0
