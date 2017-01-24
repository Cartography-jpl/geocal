from test_support import *
import pickle
from geocal.rsm_polynomial import *

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
    
def test_rsm_fit():
    r = RsmRationalPolynomial(3,3,3)
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
    r.fit_offset_and_scale(nline, nsamp, min_height, max_height, lat, lon)
    r.fit(ln, smp, lat, lon, h)
    lncalc, smpcalc = r(lat,lon,h)
    assert abs(ln-lncalc).max() < 0.01
    assert abs(smp-smpcalc).max() < 0.01
    
        
    
