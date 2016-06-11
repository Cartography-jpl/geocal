from __future__ import print_function
from __future__ import division
from builtins import range
from past.utils import old_div
from geocal_swig import *
from geocal.tie_point_collect import *
from geocal.image_ground_connection import *
from geocal.simultaneous_bundle_adjustment import *
from geocal.sqlite_shelf import *
from geocal.lm_optimize import *
import scipy.optimize
import scipy.sparse as sp
import scipy.sparse.linalg
from nose.plugins.skip import Skip, SkipTest
import matplotlib.pylab as plt
import matplotlib as mpl
import pandas as pd
import seaborn as sns
from matplotlib.backends.backend_pdf import PdfPages

import logging
import sys

console = logging.StreamHandler(stream=sys.stdout)
console.setLevel(logging.INFO)
logging.getLogger("geocal-python").addHandler(console)

test_data = "/data/smyth/MSPI-Ground/scripts_test/2013-01-31/sba/AirMSPI_ER2_CA-Mojave_SBA_20130131_211051-1_V010-test1-direct/"

def sol_iteration(sba, parm, lam):
    residual = sba.sba_eq(parm)
    chisq = old_div(np.inner(residual, residual), (len(residual) - len(sba.parameter)))
    print("Chisq", chisq)
    j = sba.sba_jacobian(parm).tocsr()
    jtj = j.transpose() * j
    # Think this is wrong
    c = jtj + lam * sp.eye(jtj.shape[0], jtj.shape[1], format="csr")
    jtres = j.transpose() * residual
    pnew = parm - sp.linalg.spsolve(c, jtres, use_umfpack=True)
    residual = sba.sba_eq(pnew)
    chisq = old_div(np.inner(residual, residual), (len(residual) - len(sba.parameter)))
    print("Chisq", chisq)
    print(pnew)
    print(sba.gcp_constraint())
    return pnew

def test_jac():
    '''Check the calculation of the jacobian against a finite difference
    calculation.'''
    raise SkipTest()
    igccol = read_shelve(test_data + "/igccol_initial.xml")
    x = []
    t = []
    for i in range(igccol.number_image):
        x.append(igccol.time_table(i).min_time)
        x.append(igccol.time_table(i).max_time)
        t.append(PiecewiseLinear.LINEAR)
        if(i < igccol.number_image - 1):
            t.append(PiecewiseLinear.CONSTANT)
    e_corr = PiecewiseLinear(x, t)
    n_corr = PiecewiseLinear(x, t)
    x = [igccol.time_table(0).min_time, 
         igccol.time_table(igccol.number_image-1).max_time]
    t = [PiecewiseLinear.CONSTANT]
    u_corr = PiecewiseLinear(x, t)
    orb = OrbitPiecewiseCorrection(igccol.orbit(0).orbit_uncorrected,
                                   e_corr, n_corr, u_corr)
    igccol.set_orbit(orb)
    tpcol = read_shelve(test_data + "/tp_data.db:tpcol")
    tpcol2 = TiePointCollection()
    for tp in tpcol:
        if(tp.is_gcp):
            tpcol2.append(tp)

    dem = igccol.dem(0)
    sba = SimultaneousBundleAdjustment(igccol, tpcol2, dem, gcp_sigma = 5)
    p0 = sba.parameter
    jac = sba.sba_jacobian(p0);
    eps = np.zeros((p0.shape[0]))
    eps[:] = 10
    eps[sba.igc_coll_param_slice.start:(sba.igc_coll_param_slice.start+6)] = 1e-4
    y0 = sba.sba_eq(p0)
    jac_calc = np.zeros((y0.shape[0], p0.shape[0]))
    for i in range(p0.shape[0]):
        p = p0.copy()
        p[i] += eps[i]
        y = sba.sba_eq(p)
        jac_calc[:,i] = old_div((y - y0), eps[i])

    t = jac - jac_calc
    assert np.max(np.abs(t)) < 0.02
    print("Surface constraint:")
    t2 = t[sba.surface_constraint_slice, :]
    print(np.max(np.abs(t2)))
    print(np.unravel_index(np.argmax(np.abs(t2)), t2.shape))
    print("GCP constraint:")
    t2 = t[sba.gcp_constraint_slice, :]
    print(np.max(np.abs(t2)))
    print(np.unravel_index(np.argmax(np.abs(t2)), t2.shape))
    print("Parameter constraint:")
    t2 = t[sba.parameter_constraint_slice, :]
    print(np.max(np.abs(t2)))
    print(np.unravel_index(np.argmax(np.abs(t2)), t2.shape))
    print("Collinearity constraint, tp offset part:")
    t2 = t[sba.collinearity_constraint_slice, sba.tp_offset_slice]
    # If we need to print a value out, do something like this:
    #print jac.todense()[sba.collinearity_constraint_slice, sba.tp_offset_slice][88,23]
    #print jac_calc[sba.collinearity_constraint_slice, sba.tp_offset_slice][88,23]
    print(np.max(np.abs(t2)))
    print(np.unravel_index(np.argmax(np.abs(t2)), t2.shape))
    print("Collineariy constraint, parm part:")
    t2 = t[sba.collinearity_constraint_slice, sba.igc_coll_param_slice]
    print(np.max(np.abs(t2)))
    print(np.unravel_index(np.argmax(np.abs(t2)), t2.shape))

def test_mspi_old_sba():
    '''This matches an old SBA run done using the old version of GeoCal.
    See if we can match this.'''
    raise SkipTest()
    # The old run used geocentric coordinates to define the attitude
    # axis. We manually edited this xml file to have vdef go from 0
    # (geodetic) to 1.
    igccol = read_shelve(test_data + "/igccol_initial_geocentric.xml")
    x = []
    t = []
    for i in range(igccol.number_image):
        x.append(igccol.time_table(i).min_time)
        x.append(igccol.time_table(i).max_time)
        t.append(PiecewiseLinear.LINEAR)
        if(i < igccol.number_image - 1):
            t.append(PiecewiseLinear.CONSTANT)
    e_corr = PiecewiseLinear(x, t)
    n_corr = PiecewiseLinear(x, t)
    x = [igccol.time_table(0).min_time, 
         igccol.time_table(igccol.number_image-1).max_time]
    t = [PiecewiseLinear.CONSTANT]
    u_corr = PiecewiseLinear(x, t)
    orb = OrbitPiecewiseCorrection(igccol.orbit(0).orbit_uncorrected,
                                   e_corr, n_corr, u_corr)
    igccol.set_orbit(orb)
    tpcol = TiePointCollection.read_old_mspi_format("/data/smyth/AirMSPISbaOldExample/podex/2013-01-31/tie_point/AirMSPI_ER2_CA-Mojave_TIE_POINTS_20130131_211051-1_L1B1_Vsba-1")
    dem = igccol.dem(0)
    sba = SimultaneousBundleAdjustment(igccol, tpcol, dem, gcp_sigma = 5)
    v = sba.sba_eq(sba.parameter)
    chisq = old_div(np.inner(v, v), (len(v) - len(sba.parameter)))
    print("Chisq", chisq)
    #sol_iteration(sba, sba.parameter, 0.1)
    #return
    parm = lm_optimize(sba.sba_eq, sba.parameter, sba.sba_jacobian)
    v = sba.sba_eq(sba.parameter)
    chisq = old_div(np.inner(v, v), (len(v) - len(sba.parameter)))
    print("Chisq", chisq)
    print(igccol.parameter_subset)
    print(igccol.orbit(0))
    print(igccol.camera(0))
    tpcol2 = TiePointCollection()
    for i in range(len(tpcol)):
        tp = TiePoint(tpcol[i].number_image)
        tp.is_gcp = tpcol[i].is_gcp
        tp.id = tpcol[i].id
        tp.image_location = tpcol[i].image_location
        tp.ground_location = sba.ground_location(i)
        tpcol2.append(tp)
    generate_diagnostic(igccol, tpcol, tpcol2)

    

def generate_diagnostic(igc, tpcol_original, tpcol_sba):
    '''Generate diagnostic plots.'''
    pdf = PdfPages("test_plots.pdf")
    cmap = mpl.colors.ListedColormap(sns.color_palette("RdBu_r", 256))
    gcp_diff_plot(pdf, tpcol_original, tpcol_sba)
    for i in range(igc.number_image):
        plot_cam(pdf, cmap, igc, tpcol_sba, i)
    pdf.close()

def gcp_diff_plot(pdf, tpcol_original, tpcol_sba):
    df = tpcol_sba.gcp_diff(tpcol_original)
    plt.plot(df.index, df.e_diff, 'o', label='E diff')
    plt.ylim((-100,100))
    plt.plot(df.index, df.n_diff, 'o', label='N diff')
    plt.plot(df.index, df.u_diff, 'o', label='U diff')
    plt.legend()
    plt.title("Difference GCP Location before/after SBA")
    plt.xlabel("TP ID")
    plt.ylabel("GCP Difference")
    pdf.savefig()

def plot_cam(pdf, cmap, igc, tpcol, i):
    vn = "View %d" %i
    df = tpcol.data_frame(igc, i)
    df_gcp = df[df.is_gcp == True]
    df_tp = df[df.is_gcp != True]

    if(df_gcp.shape[0] > 0):
        plt.clf()
        plt.scatter(df_gcp.line, df_gcp.samp, c = df_gcp.line_residual, 
                    cmap = cmap, vmin=-10, vmax=10)
        plt.title("%s after SBA Line residual, GCP only" % vn)
        plt.xlabel("Sample")
        plt.ylabel("Line")
        plt.colorbar()
        pdf.savefig()

        plt.clf()
        plt.scatter(df_gcp.line, df_gcp.samp, c = df_gcp.samp_residual, 
                    cmap = cmap, vmin=-10, vmax=10)
        plt.title("%s after SBA Sample residual, GCP only" % vn)
        plt.xlabel("Sample")
        plt.ylabel("Line")
        plt.colorbar()
        pdf.savefig()

    if(df_tp.shape[0] > 0):
        plt.clf()
        plt.scatter(df_tp.line, df_tp.samp, c = df_tp.line_residual, 
                    cmap = cmap, vmin=-10, vmax=10)
        plt.title("%s after SBA Line residual, TP only" % vn)
        plt.xlabel("Sample")
        plt.ylabel("Line")
        plt.colorbar()
        pdf.savefig()

        plt.clf()
        plt.scatter(df_tp.line, df_tp.samp, c = df_tp.samp_residual, 
                    cmap = cmap, vmin=-10, vmax=10)
        plt.title("%s after SBA Sample residual, TP only" % vn)
        plt.xlabel("Sample")
        plt.ylabel("Line")
        plt.colorbar()
        pdf.savefig()

    plt.clf()
    plt.plot(df_gcp.samp, df_gcp.line_residual, 'o', label='GCP')
    plt.ylim((-10,10))
    plt.plot(df_tp.samp, df_tp.line_residual, 'o', label="TP")
    plt.legend()
    plt.title("%s after SBA Line residual" % vn)
    plt.xlabel("Sample")
    plt.ylabel("Line residual")
    pdf.savefig()

    plt.clf()
    plt.plot(df_gcp.samp, df_gcp.samp_residual, 'o', label='GCP')
    plt.ylim((-10,10))
    plt.plot(df_tp.samp, df_tp.samp_residual, 'o', label="TP")
    plt.legend()
    plt.title("%s after SBA Sample residual" % vn)
    plt.xlabel("Sample")
    plt.ylabel("Sample residual")
    pdf.savefig()

def test_mspi_sba():
    '''Directly run MSPI SBA case, and generate diagnostic output.'''
    raise SkipTest
    igccol = read_shelve(test_data + "/igccol_initial.xml")
    x = []
    t = []
    for i in range(igccol.number_image):
        x.append(igccol.time_table(i).min_time)
        x.append(igccol.time_table(i).max_time)
        t.append(PiecewiseLinear.LINEAR)
        if(i < igccol.number_image - 1):
            t.append(PiecewiseLinear.CONSTANT)
    e_corr = PiecewiseLinear(x, t)
    n_corr = PiecewiseLinear(x, t)
    x = [igccol.time_table(0).min_time, 
         igccol.time_table(igccol.number_image-1).max_time]
    t = [PiecewiseLinear.CONSTANT]
    u_corr = PiecewiseLinear(x, t)
    orb = OrbitPiecewiseCorrection(igccol.orbit(0).orbit_uncorrected,
                                   e_corr, n_corr, u_corr)
    igccol.set_orbit(orb)
    tpcol_original = read_shelve(test_data + "/tp_data.db:tpcol")
    if False:
        tpcol_old = TiePointCollection.read_old_mspi_format("/data/smyth/AirMSPISbaOldExample/podex/2013-01-31/tie_point/AirMSPI_ER2_CA-Mojave_TIE_POINTS_20130131_211051-1_L1B1_Vsba-1")
        tpcol = TiePointCollection([tp for tp in tpcol_original if not tp.is_gcp])
        tpcol.extend([tp for tp in tpcol_old if tp.is_gcp])
    if False:
        tpcol = read_shelve("play_data.db:tpsim")
    tpcol = TiePointCollection([tp for tp in tpcol_original if not tp.id in (12, 13)])
    #tpcol_original.write_old_mspi_format("old_mspi_tp")
    #tpcol = TiePointCollection([tp for tp in tpcol_original if tp.is_gcp])
    #tpcol = tpcol_original
    dem = igccol.dem(0)
    sba = SimultaneousBundleAdjustment(igccol, tpcol, dem, gcp_sigma = 5)
    print(sba.parameter)
    v = sba.sba_eq(sba.parameter)
    chisq = old_div(np.inner(v, v), (len(v) - len(sba.parameter)))
    print("Chisq", chisq)
    print("Change position: ", distance(tpcol[10].ground_location,
                                        sba.ground_location(10)))
    print(igccol.parameter_subset)
    #sol_iteration(sba, sba.parameter, 0.1)
    #return
    parm = lm_optimize(sba.sba_eq, sba.parameter, sba.sba_jacobian)
    v = sba.sba_eq(sba.parameter)
    chisq = old_div(np.inner(v, v), (len(v) - len(sba.parameter)))
    print("Chisq", chisq)
    print("Change position: ", distance(tpcol[10].ground_location,
                                        sba.ground_location(10)))
    print(igccol.parameter_subset)
    print(orb)
    print(sba.parameter)
    tpcol2 = TiePointCollection()
    for i in range(len(tpcol)):
        tp = TiePoint(tpcol[i].number_image)
        tp.is_gcp = tpcol[i].is_gcp
        tp.id = tpcol[i].id
        tp.image_location = tpcol[i].image_location
        tp.ground_location = sba.ground_location(i)
        tpcol2.append(tp)
    generate_diagnostic(igccol, tpcol, tpcol2)
    if(False):
        tpsim = TiePointCollection()
        for tp in tpcol_original:
            if(not tp.is_gcp):
                tpsim.append(tp)
            else:
                tp.ground_location = igccol.ground_coordinate(4, tp.image_location[4][0])
                tpsim.append(tp)
        write_shelve("play_data.db:tpsim", tpsim)

def test_mspi_sba_subset():
    '''Run SBA on a subset of views, to see if results are improved'''
    # Try fitting the camera parameter only using nadir, followed by more
    # full fit.
    raise SkipTest
    igccol = read_shelve(test_data + "/igccol_initial.xml")
    t = vector_int()
    t.push_back(3)
    t.push_back(4)
    t.push_back(5)
    igccol = igccol.subset(t)

    x = []
    t = []
    for i in range(igccol.number_image):
        x.append(igccol.time_table(i).min_time)
        x.append(igccol.time_table(i).max_time)
        t.append(PiecewiseLinear.LINEAR)
        if(i < igccol.number_image - 1):
            t.append(PiecewiseLinear.CONSTANT)
    e_corr = PiecewiseLinear(x, t)
    n_corr = PiecewiseLinear(x, t)
    x = [igccol.time_table(0).min_time, 
         igccol.time_table(igccol.number_image-1).max_time]
    t = [PiecewiseLinear.CONSTANT]
    u_corr = PiecewiseLinear(x, t)
    orb = OrbitPiecewiseCorrection(igccol.orbit(0).orbit_uncorrected,
                                   e_corr, n_corr, u_corr)

    #igccol.set_orbit(igccol.orbit(0).orbit_uncorrected)
    igccol.set_orbit(orb)
    tpcol_initial = read_shelve(test_data + "/tp_data.db:tpcol")
    tpcol = TiePointCollection()
    for tp in tpcol_initial:
        tp2 = TiePoint(3)
        tp2.ground_location = tp.ground_location
        tp2.is_gcp = tp.is_gcp
        tp2.id = tp.id
        tp2.image_location[0] = tp.image_location[3]
        tp2.image_location[1] = tp.image_location[4]
        tp2.image_location[2] = tp.image_location[5]
        if(tp2.number_image_location > 1):
            tpcol.append(tp2)
    dem = igccol.dem(0)
    sba = SimultaneousBundleAdjustment(igccol, tpcol, dem, gcp_sigma = 5)
    print(igccol.parameter_subset)
    v = sba.sba_eq(sba.parameter)
    chisq = old_div(np.inner(v, v), (len(v) - len(sba.parameter)))
    print("Chisq", chisq)
    print("Change position: ", distance(tpcol[10].ground_location,
                                        sba.ground_location(10)))
    parm = lm_optimize(sba.sba_eq, sba.parameter, sba.sba_jacobian)
    v = sba.sba_eq(sba.parameter)
    chisq = old_div(np.inner(v, v), (len(v) - len(sba.parameter)))
    print("Chisq", chisq)
    print("Change position: ", distance(tpcol[10].ground_location,
                                        sba.ground_location(10)))
    print(igccol.parameter_subset)

    tpcol2 = TiePointCollection()
    for i in range(len(tpcol)):
        tp = TiePoint(tpcol[i].number_image)
        tp.is_gcp = tpcol[i].is_gcp
        tp.id = tpcol[i].id
        tp.image_location = tpcol[i].image_location
        tp.ground_location = sba.ground_location(i)
        tpcol2.append(tp)
    generate_diagnostic(igccol, tpcol, tpcol2)


