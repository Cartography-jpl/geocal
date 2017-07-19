from test_support import *
import pickle
from geocal.rsm_polynomial import *
from collections import namedtuple

@pytest.fixture(scope="module")
def rpc_data():
    '''Use this is in a couple of places, so go ahead and put this in a 
    fixture'''
    with open(unit_test_data + "rpc_example.pkl", "rb") as f:
        rpc = pickle.load(f)
    ln = []
    smp = []
    lat = []
    lon = []
    h = []
    nline = rpc.line_scale * 2
    nsamp = rpc.sample_scale * 2
    min_height = rpc.height_offset - rpc.height_scale
    max_height = rpc.height_offset + rpc.height_scale
    for lnv in np.linspace(0, nline, 20):
        for smpv in np.linspace(0, nsamp, 20):
            for hv in np.linspace(min_height, max_height, 20):
                gc = rpc.ground_coordinate(ImageCoordinate(lnv, smpv), hv)
                lat.append(gc.latitude)
                lon.append(gc.longitude)
                h.append(gc.height_reference_surface)
                ic = rpc.image_coordinate(gc)
                ln.append(ic.line)
                smp.append(ic.sample)
    ln = np.array(ln)
    smp = np.array(smp)
    lat = np.array(lat)
    lon = np.array(lon)
    h = np.array(h)
    RpcResult = namedtuple('RpcResult', ['rpc', 'ln', 'smp', 'lat', 'lon',
                         'h', 'nline', 'nsamp', 'min_height', 'max_height'])
    return RpcResult(rpc, ln, smp, lat, lon, h, nline, nsamp, min_height, max_height)

def test_rsm_polynomial():
    r = RsmPolynomial(3,3,3)
    r.coefficient[0,0,0] = 10.0
    r.coefficient[1,1,1] = 1.0
    r.coefficient[2,1,1] = 2.0
    r.coefficient[3,1,1] = 3.0
    r.coefficient[2,2,1] = 4.0
    r.coefficient[2,2,3] = 5.0
    assert r(2, 3, 4) == 10 + 1 * 2 * 3 * 4 + 2 * 2**2 * 3 * 4 + \
        3 * 2**3 * 3 * 4 +  4 * 2**2 * 3**2 * 4 + 5 * 2**2 * 3**2 * 4**3
    
@require_serialize
def test_rsm_rational_polynomial():
    r = RsmRationalPolynomial(3,3,3)
    with open(unit_test_data + "rpc_example.pkl", "rb") as f:
        rpc = pickle.load(f)
    r.rpc_set(rpc)
    g = rpc.ground_coordinate(ImageCoordinate(10, 20), 1000)
    ic_rpc = rpc.image_coordinate(g.latitude, g.longitude,
                                  g.height_reference_surface)
    ic_rsm = r(g.latitude, g.longitude, g.height_reference_surface)
    assert abs(ic_rpc.line - ic_rsm[0]) < 0.01
    assert abs(ic_rpc.sample - ic_rsm[1]) < 0.01
    
@require_serialize
def test_rsm_fit(rpc_data):
    r = RsmRationalPolynomial(3,3,3)
    r.fit_offset_and_scale(rpc_data.nline, rpc_data.nsamp, rpc_data.min_height,
                           rpc_data.max_height, rpc_data.lat, rpc_data.lon)
    r.fit(rpc_data.ln, rpc_data.smp, rpc_data.lat, rpc_data.lon, rpc_data.h)
    lncalc, smpcalc = r(rpc_data.lat,rpc_data.lon,rpc_data.h)
    assert abs(rpc_data.ln-lncalc).max() < 0.01
    assert abs(rpc_data.smp-smpcalc).max() < 0.01
    
@require_serialize
def test_low_order_polynomial(rpc_data):
    lp = LowOrderPolynomial()
    lp.fit(rpc_data.ln, rpc_data.smp, rpc_data.lat, rpc_data.lon, rpc_data.h)
    lncalc, smpcalc = lp(rpc_data.lat,rpc_data.lon,rpc_data.h)
    assert abs(rpc_data.ln-lncalc).max() < 2.0
    assert abs(rpc_data.smp-smpcalc).max() < 2.0
    
    
    
