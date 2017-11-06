from .nitf_tre import *
from .nitf_tre_rsmida import *
from test_support import *
import io, six

def test_tre_rsmida(rsm):
    t = TreRSMIDA()
    t.rsm_id = rsm.rsm_id
    t.update_raw_field()
    fh = six.BytesIO()
    t.write_to_file(fh)
    # This can vary depending on roundoff, so don't compare.
    #assert fh.getvalue() == b'Blah'
    fh2 = six.BytesIO(fh.getvalue())
    t2 = TreRSMIDA()
    t2.read_from_file(fh2)
    print(t2)
    assert t2.rsm_id.rsm_suport_data_edition == "fake-1"

    #t.iid = 'abc'
    #t.edition = 'abc'
    #t.isid = 'abc'
    #t.sid = 'abc'
    #t.stid = 'abc'
    #t.year = 2017
    #t.month=12
    #t.day = 1
    #t.hour = 10
    #t.minute = 59
    #t.second = 12.5
    #t.grndd = 'G'
    #t.xuor = 1234.567890
    #t.yuor = 1234.567890
    #t.zuor = 1234.567890
    #t.xuxr = 0.1234567890
    #t.xuyr = 0.1234567890
    #t.xuzr = 0.1234567890
    #t.yuxr = 0.1234567890
    #t.yuyr = 0.1234567890
    #t.yuzr = 0.1234567890
    #t.zuxr = 0.1234567890
    #t.zuyr = 0.1234567890
    #t.zuzr = 0.1234567890
    #t.v1x = 0.1234567890
    #t.v1y = 0.1234567890
    #t.v1z = 0.1234567890
    #t.v2x = 0.1234567890
    #t.v2y = 0.1234567890
    #t.v2z = 0.1234567890
    #t.v3x = 0.1234567890
    #t.v3y = 0.1234567890
    #t.v3z = 0.1234567890
    #t.v4x = 0.1234567890
    #t.v4y = 0.1234567890
    #t.v4z = 0.1234567890
    #t.v5x = 0.1234567890
    #t.v5y = 0.1234567890
    #t.v5z = 0.1234567890
    #t.v6x = 0.1234567890
    #t.v6y = 0.1234567890
    #t.v6z = 0.1234567890
    #t.v7x = 0.1234567890
    #t.v7y = 0.1234567890
    #t.v7z = 0.1234567890
    #t.v8x = 0.1234567890
    #t.v8y = 0.1234567890
    #t.v8z = 0.1234567890
    #t.grpx = 0.1234567890
    #t.grpy = 0.1234567890
    #t.grpz = 0.1234567890
    #t.fullr = 1234567
    #t.fullc = 1234567
    #t.minr = 1234567
    #t.maxr = 1234567
    #t.minc = 1234567
    #t.maxc = 1234567
    #t.ie0 = 0.1234567890
    #t.ier = 0.1234567890
    #t.iec = 0.1234567890
    #t.ierr = 0.1234567890
    #t.ierc = 0.1234567890
    #t.iecc = 0.1234567890
    #t.ia0 = 0.1234567890
    #t.iar = 0.1234567890
    #t.iac = 0.1234567890
    #t.iarr = 0.1234567890
    #t.iarc = 0.1234567890
    #t.iacc = 0.1234567890
    #t.spx = 0.1234567890
    #t.svx = 0.1234567890
    #t.sax = 0.1234567890
    #t.spy = 0.1234567890
    #t.svy = 0.1234567890
    #t.say = 0.1234567890
    #t.spz = 0.1234567890
    #t.svz = 0.1234567890
    #t.saz = 0.1234567890
