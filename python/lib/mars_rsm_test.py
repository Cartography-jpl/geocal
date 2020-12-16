from test_support import *
from .mars_igc import *
from .mars_rsm import *

@long_test
@require_spice
@require_mars_spice
def test_rsm_context(mars_test_data, isolated_dir):
    igc = igc_mro_context(mars_test_data + "P16_007388_2049_XI_24N020W.IMG")
    r = rsm_context(igc, diagnostic=True)
    rsm_plot_diff(r, igc, "rsm_diff.pdf")

@long_test    
@require_spice
@require_mars_spice
def test_rsm_hrsc(mars_test_data, isolated_dir):
    igc = igc_mex_hrsc(mars_test_data + "h1326_0000_nd2.img")
    r = rsm_hrsc(igc, diagnostic=True)
    rsm_plot_diff(r, igc, "rsm_diff.pdf")

@long_test    
@require_spice
@require_mars_spice
def test_rsm_hirise(mars_test_data, isolated_dir):
    igc = igc_mro_hirise(mars_test_data + "esp_025012_1745_red2.norm.cub")
    r = rsm_hirise(igc, diagnostic=True)
    rsm_plot_diff(r, igc, "rsm_diff.pdf")

