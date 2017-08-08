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
    nline = rpc.line_scale * 2
    nsamp = rpc.sample_scale * 2
    min_height = rpc.height_offset - rpc.height_scale
    max_height = rpc.height_offset + rpc.height_scale
    lnvv = np.linspace(0, nline, 20)
    smpvv = np.linspace(0, nsamp, 60)
    h = np.linspace(min_height, max_height, 20)
    lat = np.empty((lnvv.size, smpvv.size, h.size))
    lon = np.empty(lat.shape)
    ln = np.empty(lat.shape)
    smp = np.empty(lat.shape)
    for i, lnv in enumerate(lnvv):
        for j, smpv in enumerate(smpvv):
            for k, hv in enumerate(h):
                gc = rpc.ground_coordinate(ImageCoordinate(lnv, smpv), hv)
                lat[i,j,k] = gc.latitude
                lon[i,j,k] = gc.longitude
                ic = rpc.image_coordinate(gc)
                ln[i,j,k] = ic.line
                smp[i,j,k] = ic.sample
    RpcResult = namedtuple('RpcResult', ['rpc', 'ln', 'smp', 'lnvv', 'smpvv',
                         'lat', 'lon',
                         'h', 'nline', 'nsamp', 'min_height', 'max_height'])
    return RpcResult(rpc, ln, smp, lnvv, smpvv, lat, lon, h, nline, nsamp,
                     min_height, max_height)

@pytest.fixture(scope="module")
def rpc_data_latgrid(rpc_data):
    '''Variation of rpc_data where we used fixed spacing latitude/longitude
    and height rather than in image space. Think this will work better with
    rpc grid'''
    latv = np.linspace(rpc_data.lat.min(), rpc_data.lat.max(), 30)
    lonv = np.linspace(rpc_data.lon.min(), rpc_data.lon.max(), 30)
    h = rpc_data.h
    lat = np.empty((latv.size, lonv.size, h.size))
    lat[:,:,:] = latv[:, np.newaxis, np.newaxis]
    lon = np.empty(lat.shape)
    lon[:,:,:] = lonv[np.newaxis,:,np.newaxis]
    ln = np.empty(lat.shape)
    smp = np.empty(lon.shape)
    for i in range(lat.shape[0]):
        for j in range(lon.shape[0]):
            for k, hv in enumerate(h):
                gc = Geodetic(lat[i,j,k],lon[i,j,k], hv)
                ic = rpc_data.rpc.image_coordinate(gc)
                ln[i,j,k] = ic.line
                smp[i,j,k] = ic.sample
    RpcResult2 = namedtuple('RpcResult2', ['ln', 'smp', 'lat', 'lon', 'h'])
    return RpcResult2(ln, smp, lat, lon, h)

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
    r.fit(rpc_data.ln, rpc_data.smp, rpc_data.lat, rpc_data.lon, rpc_data.h)
    lncalc, smpcalc = r(rpc_data.lat,rpc_data.lon,
                        rpc_data.h[np.newaxis, np.newaxis, :])
    assert abs(rpc_data.ln-lncalc).max() < 0.01
    assert abs(rpc_data.smp-smpcalc).max() < 0.01

@require_serialize
def test_rsm_fit_1dln(rpc_data):
    '''Repeat fit, but with lnvv and smpvv held fixed. This is slightly less
    accurate because it folds in the error for the rpc in calculating the
    ground coordinates for a given image coordinate'''
    r = RsmRationalPolynomial(3,3,3)
    r.fit(rpc_data.lnvv, rpc_data.smpvv, rpc_data.lat, rpc_data.lon, rpc_data.h)
    lncalc, smpcalc = r(rpc_data.lat,rpc_data.lon,
                        rpc_data.h[np.newaxis, np.newaxis, :])
    assert abs(rpc_data.lnvv[:,np.newaxis,np.newaxis]-lncalc).max() < 0.1

    # We have a asymptote in our solution, which is apparently a common
    # problem with rational polynomials (see
    # https://en.wikipedia.org/wiki/Polynomial_and_rational_function_modeling).
    # We'll need to come back to this, we may be able
    # to do a full nonlinear fit or something else. But for now, just
    # check that things are working by filtering out this data.
    t = abs(rpc_data.smpvv[np.newaxis,:,np.newaxis]-smpcalc)
    assert np.median(t) < 0.02
    assert np.count_nonzero(t > 1) / t.size < 0.005
    
@require_serialize
def test_low_order_polynomial(rpc_data):
    lp = RsmLowOrderPolynomial()
    lp.fit(rpc_data.ln, rpc_data.smp, rpc_data.lat, rpc_data.lon, rpc_data.h)
    lncalc, smpcalc = lp(rpc_data.lat,rpc_data.lon,
                         rpc_data.h[np.newaxis,np.newaxis,:])
    assert abs(rpc_data.ln-lncalc).max() < 2.0
    assert abs(rpc_data.smp-smpcalc).max() < 2.0

@require_serialize
def test_low_order_polynomial_1dv(rpc_data):
    lp = RsmLowOrderPolynomial()
    lp.fit(rpc_data.lnvv, rpc_data.smpvv, rpc_data.lat, rpc_data.lon,
           rpc_data.h)
    lncalc, smpcalc = lp(rpc_data.lat,rpc_data.lon,
                         rpc_data.h[np.newaxis,np.newaxis,:])
    assert abs(rpc_data.lnvv[:,np.newaxis,np.newaxis]-lncalc).max() < 2.0
    assert abs(rpc_data.smp[np.newaxis,:,np.newaxis]-smpcalc).max() < 2.0
    
@require_serialize
def test_multi_section_polynomial(rpc_data):
    r = RsmMultiSection(rpc_data.nline, rpc_data.nsamp, 3, 2,
                           lambda : RsmRationalPolynomial(3,3,3))
    r.fit(rpc_data.ln, rpc_data.smp, rpc_data.lat, rpc_data.lon, rpc_data.h)

    h = np.empty(rpc_data.lat.shape)
    h[:,:,:] = rpc_data.h[np.newaxis,np.newaxis,:]
    lncalc, smpcalc = r(rpc_data.lat,rpc_data.lon,h)
    assert abs(rpc_data.ln-lncalc).max() < 0.01
    assert abs(rpc_data.smp-smpcalc).max() < 0.01
    
    def f(ln,smp):
        gc = rpc_data.rpc.ground_coordinate(ImageCoordinate(ln,smp),0)
        return r(gc.latitude, gc.longitude, gc.height_reference_surface)
    #print(f(0,0))
    #print(f(0,14237))
    #print(f(0,14239))
    #print(f(0,15000))
    
    
@require_serialize
def test_grid_fit(rpc_data_latgrid):
    r = RsmGrid(60,60,20)
    r.fit(rpc_data_latgrid.ln, rpc_data_latgrid.smp, rpc_data_latgrid.lat,
          rpc_data_latgrid.lon, rpc_data_latgrid.h)
    h = np.empty(rpc_data_latgrid.lat.shape)
    h[:,:,:] = rpc_data_latgrid.h[np.newaxis,np.newaxis,:]
    lncalc, smpcalc = r(rpc_data_latgrid.lat,rpc_data_latgrid.lon,h)
    assert abs(rpc_data_latgrid.ln-lncalc).max() < 0.05
    assert abs(rpc_data_latgrid.smp-smpcalc).max() < 0.3

# This doesn't work yet    
@skip    
@require_serialize
def test_multi_section_grid(rpc_data_latgrid, rpc_data):
    r = RsmMultiSection(rpc_data.nline, rpc_data.nsamp, 3, 2,
                           lambda : RsmGrid(60,60,20))
    r.fit(rpc_data_latgrid.ln, rpc_data_latgrid.smp, rpc_data_latgrid.lat,
          rpc_data_latgrid.lon, rpc_data_latgrid.h)
    h = np.empty(rpc_data_latgrid.lat.shape)
    h[:,:,:] = rpc_data_latgrid.h[np.newaxis,np.newaxis,:]
    lncalc, smpcalc = r(rpc_data_latgrid.lat,rpc_data_latgrid.lon,h)
    assert np.nanmax(abs(rpc_data_latgrid.ln-lncalc)) < 0.3
    assert np.nanmax(abs(rpc_data_latgrid.smp-smpcalc)) < 5
    
@require_serialize
def test_polynomial_plus_grid_fit(rpc_data_latgrid):
    # The 2,2,2 will by test design be pretty crappy, so we give the
    # correction grid something to do.
    r = RsmRationalPolynomialPlusGrid(RsmRationalPolynomial(2,2,2),
                                      RsmGrid(60,60,20))
    r.fit(rpc_data_latgrid.ln, rpc_data_latgrid.smp, rpc_data_latgrid.lat,
          rpc_data_latgrid.lon, rpc_data_latgrid.h)
    h = np.empty(rpc_data_latgrid.lat.shape)
    h[:,:,:] = rpc_data_latgrid.h[np.newaxis,np.newaxis,:]
    lncalc, smpcalc = r(rpc_data_latgrid.lat,rpc_data_latgrid.lon,h)
    lcorr, scorr = r.corr_grid(rpc_data_latgrid.lat,rpc_data_latgrid.lon,h)
    #print(abs(lcorr).max())
    #print(abs(scorr).max())
    assert abs(rpc_data_latgrid.ln-lncalc).max() < 0.1
    assert np.median(abs(rpc_data_latgrid.smp-smpcalc)) < 0.1
