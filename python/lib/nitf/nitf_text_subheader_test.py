from .nitf_text_subheader import *
from test_support import *
import io, six

def test_text_subheader_basic():

    t = NitfTextSubheader()

    t.textid = 'abcdefg'
    t.txtalvl = '001'
    t.txtitl = 'ABCDEFG'

    fh = six.BytesIO()
    t.write_to_file(fh)
    #assert fh.getvalue() == b'TEabcdefg001'
    fh2 = six.BytesIO(fh.getvalue())
    t2 = NitfTextSubheader()
    t2.read_from_file(fh2)

    assert t2.textid  ==  'abcdefg'
    assert t2.txtalvl == '001'
    assert t2.txtitl == 'ABCDEFG'
