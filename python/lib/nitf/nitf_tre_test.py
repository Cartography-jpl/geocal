from .nitf_tre import *
from test_support import *
import io, six

def test_tre():
    TreUSE00A = create_nitf_tre_structure("TreUSE00A",
        ["USE00A",
         ["angle_to_north", 3, int],
         ["mean_gsd", 5, float, {"frmt" : "%05.1lf"}],
         [None, 1, str],
         ["dynamic_range", 5, int],
         ])
    t = TreUSE00A()
    t.angle_to_north = 270
    t.mean_gsd = 105.2
    t.dynamic_range = 2047
    fh = six.BytesIO()
    t.write_to_file(fh)
    assert fh.getvalue() == b'USE00A00014270105.2 02047'
    fh2 = six.BytesIO(fh.getvalue())
    t2 = TreUSE00A()
    t2.read_from_file(fh2)
    print(t2)
    
    
    
