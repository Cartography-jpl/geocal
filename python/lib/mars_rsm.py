# This generates an RSM form various instruments. There is already general
# RSM code, but this has the various choices appropriate to each instrument.

from geocal_swig import *
import matplotlib.pylab as plt
import matplotlib as mpl
import seaborn as sns
from matplotlib.backends.backend_pdf import PdfPages
import numpy as np

def rsm_plot_diff(r, igc, fname=None, min_height = -5000,
                  max_height = -1500):
    '''This provides a quick plot/summary of the differences between the RSM
    we calculate and the underlying igc.

    If a file is supplied, we also write out the cross section plots.'''
    
    dem_nominal = (max_height + min_height) / 2.0
    ln, smp, lncalc, smpcalc = r.compare_igc(igc, igc.number_line, 5,
                                             dem_nominal)
    print("Max line diff:   ", np.nanmax(abs(ln-lncalc)), " at ",
          np.unravel_index(np.nanargmax(abs(ln-lncalc)), ln.shape))
    print("Max sample diff: ", np.nanmax(abs(smp-smpcalc)), " at ",
          np.unravel_index(np.nanargmax(abs(smp-smpcalc)), smp.shape))
    if(fname is None):
        return
    pdf = PdfPages(fname)
    cmap = mpl.colors.ListedColormap(sns.color_palette("RdBu_r", 256))
    plt.clf()
    plt.ylim(-2,2)
    for i in range(smp.shape[1]):
        plt.plot(ln[:,i], lncalc[:,i] - ln[:,i], label="Sample %d" % smp[0,i])
    plt.legend()
    plt.title("Line diff")
    plt.xlabel("Line")
    plt.ylabel("Diff")
    pdf.savefig()
    plt.clf()
    plt.ylim(-2,2)
    for i in range(smp.shape[1]):
        plt.plot(ln[:,i], smpcalc[:,i] - smp[:,i], label="Sample %d" % smp[0,i])
    plt.legend()
    plt.title("Sample diff")
    plt.xlabel("Line")
    plt.ylabel("Diff")
    pdf.savefig()
    plt.clf()
    
    pdf.close()

def rsm_context(igc, diagnostic=False, min_height = -5000,
                    max_height = -1500):
    '''Generate a RSM for MRO CTX. The igc should come from something
    like igc_mro_context.

    There are a few checks we can do to make sure that various assumptions
    we make are true.
    '''
    if(diagnostic):
        # In order for LocalRcParameter to be useful, the look,
        # sample, and line directions need to be mostly orthogonal. So
        # dot product of unit vectors should be close to 0.
        LocalRcParameter.check_rc_assumption(igc)

        # Verify that we calculate nearly the same boresight at SPICE.
        ic = ImageCoordinate(200, igc.number_sample / 2)
        pts = SpiceHelper.boresight_and_footprint(igc.pixel_time(ic),
                                         PlanetConstant.MARS_NAIF_CODE,
                                         "MRO", "MRO_CTX")
        print("Distance spice boresight and IGC: ",
              distance(pts[0], igc.ground_coordinate(ic)))
        print(igc.image_coordinate(pts[0]))
        
    r = Rsm(RsmRpPlusGrid(RsmRationalPolynomial(5,5,3,1,1,1,5,1,
                                                40, 40, 20, 40),
                          RsmGrid(3, 1000, 50)),
            LocalRcConverter(LocalRcParameter(igc)))
    r.fit(igc, min_height, max_height)
    return r

def rsm_hrsc(igc, diagnostic=False, min_height = -5000,
                    max_height = -1500):
    '''Generate a RSM for MEX HRSC. The igc should come from something
    like igc_mex_hrsc.

    There are a few checks we can do to make sure that various assumptions
    we make are true.
    '''
    if(diagnostic):
        # In order for LocalRcParameter to be useful, the look,
        # sample, and line directions need to be mostly orthogonal. So
        # dot product of unit vectors should be close to 0.
        LocalRcParameter.check_rc_assumption(igc)

        # Verify that we calculate nearly the same boresight at SPICE.
        ic = ImageCoordinate(200, igc.number_sample / 2)
        pts = SpiceHelper.boresight_and_footprint(igc.pixel_time(ic),
                                         PlanetConstant.MARS_NAIF_CODE,
                                         "MEX", "MEX_HRSC_NADIR")
        print("Distance spice boresight and IGC: ",
              distance(pts[0], igc.ground_coordinate(ic)))
        print(igc.image_coordinate(pts[0]))
        
    # Really big image, so break up into a few sections. We should perhaps have
    # a dynamic way to determine this.
    r = Rsm(RsmRpPlusGrid(RsmMultiSection(igc.number_line,
                                          igc.number_sample, 3, 1,
                                RsmRationalPolynomial(5,5,3,1,1,1,5,1,
                                                      40, 40, 20, 40, False)),
                          RsmGrid(3, 5000, 50, False)),
            LocalRcConverter(LocalRcParameter(igc)))
    r.fit(igc, min_height, max_height)
    return r

def rsm_hirise(igc, diagnostic=False, min_height = -5000,
                    max_height = -1500):
    '''Generate a RSM for MRO HiRISE. The igc should come from something
    like igc_mro_hirise.

    There are a few checks we can do to make sure that various assumptions
    we make are true.
    '''
    if(diagnostic):
        # In order for LocalRcParameter to be useful, the look,
        # sample, and line directions need to be mostly orthogonal. So
        # dot product of unit vectors should be close to 0.
        LocalRcParameter.check_rc_assumption(igc)
        
    # Really big image, so break up into a few sections. We should perhaps have
    # a dynamic way to determine this.
    r = Rsm(RsmRpPlusGrid(RsmMultiSection(igc.number_line,
                                          igc.number_sample, 3, 1,
                                RsmRationalPolynomial(5,5,3,1,1,1,5,1,
                                                      40, 40, 20, 40, False)),
                          RsmGrid(3, 5000, 50, False)),
            LocalRcConverter(LocalRcParameter(igc)))
    r.fit(igc, min_height, max_height)
    return r

__all__ = ["rsm_plot_diff", "rsm_context", "rsm_hrsc", "rsm_hirise"]
