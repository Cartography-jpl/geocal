from .nitf_tre import *
from .nitf_tre_rsmgga import *
from test_support import *
import io, six

def test_tre_rsmgga(rsm_grid):
    t = TreRSMGGA()
    t.rsm_grid = rsm_grid
    t.update_raw_field()
    fh = six.BytesIO()
    t.write_to_file(fh)
    # This is way too large to check, so skip this
    #assert fh.getvalue() == b'blah'
    fh2 = six.BytesIO(fh.getvalue())
    t2 = TreRSMGGA()
    t2.read_from_file(fh2)
    print(t2)
    # Check that we can't the field values.
    with pytest.raises(RuntimeError):
        t.iid = 'abc'
    with pytest.raises(RuntimeError):
        t.rcoord[0,0] = "1.0"

    assert t.iid is None
    assert t.deltaz == 1268.0

