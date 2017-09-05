from test_support import *
import pickle
from geocal.rsm_polynomial import *
import matplotlib.pylab as plt
import matplotlib as mpl
import seaborn as sns
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
                         'lon', 'lat',
                  'h', 'nline', 'nsamp', 'min_height', 'max_height', 'igc'])
    igc = RpcImageGroundConnection(rpc, SimpleDem(),
                                   MemoryRasterImage(int(nline), int(nsamp)))
    return RpcResult(rpc, ln, smp, lnvv, smpvv, lon, lat, h, nline, nsamp,
                     min_height, max_height, igc)

def plot_diff(lncalc, smpcalc, ln, smp):
    cmap = mpl.colors.ListedColormap(sns.color_palette("RdBu_r", 256))
    t = np.unravel_index(abs(ln-lncalc).argmax(),ln.shape)
    print("Maximum Line diff:")
    print("  Index:      ", t)
    print("  Line:       ", ln[t])
    print("  Line calc:  ", lncalc[t])
    t = np.unravel_index(abs(smp-smpcalc).argmax(),ln.shape)
    print("Maximum Sample diff:")
    print("  Index:      ", t)
    print("  Sample:     ", smp[t])
    print("  Sample calc:", smpcalc[t])
    plt.clf()
    plt.imshow(lncalc[:,:,0] - ln[:,:,0], cmap=cmap, vmin=-5, vmax=5)
    plt.title("Line diff")
    plt.xlabel("Sample")
    plt.ylabel("Line")
    plt.colorbar()
    plt.show()
    plt.clf()
    plt.imshow(smpcalc[:,:,0] - smp[:,:,0], cmap=cmap, vmin=-5, vmax=5)
    plt.title("Sample diff")
    plt.xlabel("Sample")
    plt.ylabel("Line")
    plt.colorbar()
    plt.show()

def plot_diff2(lncalc, smpcalc, ln, smp):
    cmap = mpl.colors.ListedColormap(sns.color_palette("RdBu_r", 256))
    plt.clf()
    for i,s in enumerate(smp):
        plt.plot(ln[:,i,0], lncalc[:,i,0] - ln[:,i,0])
    plt.title("Line diff")
    plt.xlabel("Line")
    plt.ylabel("Diff")
    plt.show()
    plt.clf()
    for i,s in enumerate(smp):
        plt.plot(ln[:,i,0], smpcalc[:,i,0] - smp[:,i,0])
    plt.title("Sample diff")
    plt.xlabel("Line")
    plt.ylabel("Diff")
    plt.show()
    plt.clf()

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
    RpcResult2 = namedtuple('RpcResult2', ['ln', 'smp', 'lon', 'lat', 'h'])
    return RpcResult2(ln, smp, lon, lat, h)

@require_serialize
def test_rsm_rational_polynomial():
    r = RsmRationalPolynomial(3,3,3,3,3,3)
    with open(unit_test_data + "rpc_example.pkl", "rb") as f:
        rpc = pickle.load(f)
    r.set_rpc_coeff(rpc)
    g = rpc.ground_coordinate(ImageCoordinate(10, 20), 1000)
    ic_rpc = rpc.image_coordinate(g.latitude, g.longitude,
                                  g.height_reference_surface)
    ic_rsm = r.image_coordinate(g.longitude, g.latitude,
                                g.height_reference_surface)
    assert abs(ic_rpc.line - ic_rsm.line) < 0.01
    assert abs(ic_rpc.sample - ic_rsm.sample) < 0.01
    
@require_serialize
def test_rsm_fit2(rpc_data):
    r = RsmRationalPolynomial(3,3,3,3,3,3, 3, 3)
    r.fit(rpc_data.igc, GeodeticConverter(), rpc_data.h.min(),
          rpc_data.h.max(), 0, int(rpc_data.ln.max()), 0,
          int(rpc_data.smp.max()))
    h = np.empty(rpc_data.lat.shape)
    h[:,:,:] = rpc_data.h[np.newaxis,np.newaxis,:]
    lncalc, smpcalc = r.image_coordinate(rpc_data.lon,rpc_data.lat,h)
    assert abs(rpc_data.ln-lncalc).max() < 0.1
    assert abs(rpc_data.smp-smpcalc).max() < 0.1

@require_serialize
def test_low_order_polynomial(rpc_data):
    lp = RsmLowOrderPolynomial()
    h = np.empty(rpc_data.lat.shape)
    h[:,:,:] = rpc_data.h[np.newaxis,np.newaxis,:]
    lp.fit_data(rpc_data.ln.flatten(), rpc_data.smp.flatten(),
                rpc_data.lon.flatten(), rpc_data.lat.flatten(),
                h.flatten())
    lncalc, smpcalc = lp.image_coordinate(rpc_data.lon,rpc_data.lat,h)
    assert abs(rpc_data.ln-lncalc).max() < 2.0
    assert abs(rpc_data.smp-smpcalc).max() < 2.0

# This doesn't work yet. We'll need to reorganize how the sections work
# with polynomials.
@require_serialize
def test_multi_section_polynomial(rpc_data):
    r = RsmMultiSection(rpc_data.nline, rpc_data.nsamp, 3, 2,
                           lambda : RsmRationalPolynomial(3,3,3,3,3,3,3,3))
    r.fit(rpc_data.igc, GeodeticConverter(), rpc_data.h.min(),
          rpc_data.h.max(), 0, int(rpc_data.ln.max()), 0,
          int(rpc_data.smp.max()))

    h = np.empty(rpc_data.lat.shape)
    h[:,:,:] = rpc_data.h[np.newaxis,np.newaxis,:]
    lncalc, smpcalc = r.image_coordinate(rpc_data.lon,rpc_data.lat,h)
    #plot_diff(lncalc, smpcalc, rpc_data.ln, rpc_data.smp)
    #print(lncalc)
    #print(rpc_data.ln)
    # We have a few points that have fairly large errors. I think this
    # is just because of the particular data we have, so we'll check
    # the 90 error rather than the max
    t = abs(rpc_data.ln-lncalc).flatten()
    t.sort()
    assert t[int(t.size * 0.90)] < 0.06
    t = abs(rpc_data.smp-smpcalc).flatten()
    t.sort()
    assert t[int(t.size * 0.90)] < 0.06
    
    def f(ln,smp):
        gc = rpc_data.rpc.ground_coordinate(ImageCoordinate(ln,smp),0)
        return r.image_coordinate(gc.longitude, gc.latitude,
                                  gc.height_reference_surface)
    #print(f(0,0))
    #print(f(0,14237))
    #print(f(0,14239))
    #print(f(0,15000))

@require_serialize
def test_grid_fit(rpc_data_latgrid):
    r = RsmGrid(60,60,20)
    r.fit(rpc_data_latgrid.ln, rpc_data_latgrid.smp, 
          rpc_data_latgrid.lon, rpc_data_latgrid.lat, rpc_data_latgrid.h)
    h = np.empty(rpc_data_latgrid.lat.shape)
    h[:,:,:] = rpc_data_latgrid.h[np.newaxis,np.newaxis,:]
    lncalc, smpcalc = r.image_coordinate(rpc_data_latgrid.lon,
                                         rpc_data_latgrid.lat,h)
    assert abs(rpc_data_latgrid.ln-lncalc).max() < 0.05
    assert abs(rpc_data_latgrid.smp-smpcalc).max() < 0.3

# This doesn't work yet    
@skip    
@require_serialize
def test_multi_section_grid(rpc_data_latgrid, rpc_data):
    r = RsmMultiSection(rpc_data.nline, rpc_data.nsamp, 3, 2,
                           lambda : RsmGrid(60,60,20))
    r.fit(rpc_data_latgrid.ln, rpc_data_latgrid.smp, 
          rpc_data_latgrid.lon, rpc_data_latgrid.lat,rpc_data_latgrid.h)
    h = np.empty(rpc_data_latgrid.lat.shape)
    h[:,:,:] = rpc_data_latgrid.h[np.newaxis,np.newaxis,:]
    lncalc, smpcalc = r.image_coordinate(rpc_data_latgrid.lon,
                                         rpc_data_latgrid.lat,h)
    assert np.nanmax(abs(rpc_data_latgrid.ln-lncalc)) < 0.3
    assert np.nanmax(abs(rpc_data_latgrid.smp-smpcalc)) < 5
    
@require_serialize
def test_polynomial_plus_grid_fit(rpc_data_latgrid):
    # The 2,2,2 will by test design be pretty crappy, so we give the
    # correction grid something to do.
    r = RsmRationalPolynomialPlusGrid(RsmRationalPolynomial(2,2,2,2,2,2),
                                      RsmGrid(60,60,20))
    r.fit(rpc_data_latgrid.ln, rpc_data_latgrid.smp, 
          rpc_data_latgrid.lon, rpc_data_latgrid.lat, rpc_data_latgrid.h)
    h = np.empty(rpc_data_latgrid.lat.shape)
    h[:,:,:] = rpc_data_latgrid.h[np.newaxis,np.newaxis,:]
    lncalc, smpcalc = r.image_coordinate(rpc_data_latgrid.lon,
                                         rpc_data_latgrid.lat,h)
    lcorr, scorr = r.corr_grid.image_coordinate(rpc_data_latgrid.lon,
                                                rpc_data_latgrid.lat,h)
    assert abs(rpc_data_latgrid.ln-lncalc).max() < 0.1
    assert np.median(abs(rpc_data_latgrid.smp-smpcalc)) < 0.1
