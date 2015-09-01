from nose.tools import *
from geocal_swig import *
from tie_point_collect import *
from image_ground_connection import *
from simultaneous_bundle_adjustment import *
from sqlite_shelf import *
from lm_optimize import *
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
    chisq = np.inner(residual, residual) / (len(residual) - len(sba.parameter))
    print "Chisq", chisq
    j = sba.sba_jacobian(parm).tocsr()
    jtj = j.transpose() * j
    c = jtj + sp.spdiags(jtj.diagonal() * lam, 0, jtj.shape[0],
                                 jtj.shape[1], format="csr")
    jtres = j.transpose() * residual
    pnew = parm - sp.linalg.spsolve(c, jtres, use_umfpack=True)
    residual = sba.sba_eq(pnew)
    chisq = np.inner(residual, residual) / (len(residual) - len(sba.parameter))
    print "Chisq", chisq
    return pnew

def test_jac():
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
    # Use FD for now
    parameter_fd_step_size = np.zeros(igccol.parameter_subset.shape)
    parameter_fd_step_size[:] = 10
    parameter_fd_step_size[0:5]=0.1
    sba = SimultaneousBundleAdjustment(igccol, tpcol2, dem, gcp_sigma = 5,
                             ecr_fd_step_size = 10, 
                             parameter_fd_step_size = parameter_fd_step_size)
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
        jac_calc[:,i] = (y - y0) / eps[i]

    t = jac - jac_calc
    print "Surface constraint:"
    t2 = t[sba.surface_constraint_slice, :]
    print np.max(np.abs(t2))
    print np.unravel_index(np.argmax(np.abs(t2)), t2.shape)
    print "GCP constraint:"
    t2 = t[sba.gcp_constraint_slice, :]
    print np.max(np.abs(t2))
    print np.unravel_index(np.argmax(np.abs(t2)), t2.shape)
    print "Parameter constraint:"
    t2 = t[sba.parameter_constraint_slice, :]
    print np.max(np.abs(t2))
    print np.unravel_index(np.argmax(np.abs(t2)), t2.shape)
    print "Collinearity constraint, tp offset part:"
    t2 = t[sba.collinearity_constraint_slice, sba.tp_offset_slice]
    print jac.todense()[sba.collinearity_constraint_slice, sba.tp_offset_slice][228,49]
    print jac_calc[sba.collinearity_constraint_slice, sba.tp_offset_slice][228,49]
    print np.max(np.abs(jac.todense()[sba.collinearity_constraint_slice, sba.tp_offset_slice]))
    print np.max(np.abs(t2))
    print np.unravel_index(np.argmax(np.abs(t2)), t2.shape)
    print "Collineariy constraint, parm part:"
    t2 = t[sba.collinearity_constraint_slice, sba.igc_coll_param_slice]
    print jac.todense()[sba.collinearity_constraint_slice, sba.igc_coll_param_slice][420,0]
    print jac_calc[sba.collinearity_constraint_slice, sba.igc_coll_param_slice][420,0]
    print np.max(np.abs(t2))
    print np.unravel_index(np.argmax(np.abs(t2)), t2.shape)


def test_mspi_sba2():
    raise SkipTest()
    # Try fitting the camera parameter only using nadir, followed by more
    # full fit.
    #igccol = read_shelve(test_data + "/igccol_initial.xml")
    igccol = read_shelve(test_data + "/igccol_initial_geocentric.xml")
    t = vector_int()
    t.push_back(4)
    igccol2 = igccol.subset(t)

    x = []
    t = []
    for i in range(igccol2.number_image):
        x.append(igccol2.time_table(i).min_time)
        x.append(igccol2.time_table(i).max_time)
        t.append(PiecewiseLinear.LINEAR)
        if(i < igccol2.number_image - 1):
            t.append(PiecewiseLinear.CONSTANT)
    e_corr = PiecewiseLinear(x, t)
    n_corr = PiecewiseLinear(x, t)
    x = [igccol.time_table(0).min_time, 
         igccol.time_table(igccol.number_image-1).max_time]
    t = [PiecewiseLinear.CONSTANT]
    u_corr = PiecewiseLinear(x, t)
    orb = OrbitPiecewiseCorrection(igccol.orbit(0).orbit_uncorrected,
                                   e_corr, n_corr, u_corr)

    #igccol2.set_orbit(igccol2.orbit(0).orbit_uncorrected)
    igccol2.set_orbit(orb)
    igccol2.gimbal(0).parameter_mask = [True, False, True]
    tpcol = read_shelve(test_data + "/tp_data.db:tpcol")
    #tpcol = read_shelve(test_data + "/tp_data_new.db:tpcol")
    tpcol2 = TiePointCollection()
    for tp in tpcol:
        if(tp.is_gcp and tp.image_location[4] is not None):
            tp2 = TiePoint(1)
            tp2.ground_location = tp.ground_location
            tp2.is_gcp = True
            tp2.id = tp.id
            tp2.image_location[0] = tp.image_location[4]
            tpcol2.append(tp2)
    dem = igccol2.dem(0)
    # Use FD for now
    parameter_fd_step_size = np.zeros(igccol2.parameter_subset.shape)
    parameter_fd_step_size[:] = 10
    parameter_fd_step_size[0:5]=0.1
    sba = SimultaneousBundleAdjustment(igccol2, tpcol2, dem, gcp_sigma = 5,
                             ecr_fd_step_size = 10,
                             parameter_fd_step_size = parameter_fd_step_size)
    print igccol2.parameter_subset
    v = sba.sba_eq(sba.parameter)
    chisq = np.inner(v, v) / (len(v) - len(sba.parameter))
    print "Chisq", chisq
    print "Change position: ", distance(tpcol2[10].ground_location,
                                        sba.ground_location(10))
    parm = lm_optimize(sba.sba_eq, sba.parameter, sba.sba_jacobian)
    v = sba.sba_eq(sba.parameter)
    chisq = np.inner(v, v) / (len(v) - len(sba.parameter))
    print "Chisq", chisq
    print "Change position: ", distance(tpcol2[10].ground_location,
                                        sba.ground_location(10))
    print igccol2.parameter_subset

    #sba = SimultaneousBundleAdjustment(igccol2, tpcol2, dem, gcp_sigma = 5)
    print igccol2.parameter_subset
    v = sba.sba_eq(sba.parameter)
    chisq = np.inner(v, v) / (len(v) - len(sba.parameter))
    print "Chisq", chisq
    print "Change position: ", distance(tpcol2[10].ground_location,
                                        sba.ground_location(10))
    #parm = lm_optimize(sba.sba_eq, sba.parameter, sba.sba_jacobian)
    tpcol3 = TiePointCollection()
    for i in range(len(tpcol2)):
        tp = TiePoint(tpcol2[i].number_image)
        tp.is_gcp = tpcol2[i].is_gcp
        tp.id = tpcol2[i].id
        tp.image_location = tpcol2[i].image_location
        tp.ground_location = sba.ground_location(i)
        tpcol3.append(tp)
    v = sba.sba_eq(sba.parameter)
    chisq = np.inner(v, v) / (len(v) - len(sba.parameter))
    print "Chisq", chisq
    print "Change position: ", distance(tpcol2[10].ground_location,
                                        sba.ground_location(10))
    print igccol2.parameter_subset
    generate_diagnostic(igccol2, tpcol2, tpcol3)

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
    for tp in tpcol:
        for i,ic in enumerate(tp.image_location):
            if(ic is not None):
                if(ic[1] < 0.3):
                    ic = (ic[0], 0.3, ic[2])
                if(ic[2] < 0.3):
                    ic = (ic[0], ic[1], 0.3)
                tp.image_location[i] = ic

    tpcol2 = TiePointCollection()
    for tp in tpcol:
        if(tp.is_gcp):
            tpcol2.append(tp)
    tpcol2 = tpcol
    dem = igccol.dem(0)
    # Use FD for now
    parameter_fd_step_size = np.zeros(igccol.parameter_subset.shape)
    parameter_fd_step_size[:] = 10
    parameter_fd_step_size[0:5]=0.1
    #sba = SimultaneousBundleAdjustment(igccol, tpcol2, dem, gcp_sigma = 5)
    sba = SimultaneousBundleAdjustment(igccol, tpcol2, dem, gcp_sigma = 0.1,
                             ecr_fd_step_size = 10,
                             parameter_fd_step_size = parameter_fd_step_size)
    t = sba.psigma.copy()
    t[0:5] = 1.0
    t[5:] = 1e8
    sba.psigma = t
    v = sba.sba_eq(sba.parameter)
    chisq = np.inner(v, v) / (len(v) - len(sba.parameter))
    print "Chisq", chisq
    print "Change position: ", distance(tpcol2[10].ground_location,
                                        sba.ground_location(10))
    print igccol.parameter_subset
    parm = lm_optimize(sba.sba_eq, sba.parameter, sba.sba_jacobian)
    v = sba.sba_eq(sba.parameter)
    chisq = np.inner(v, v) / (len(v) - len(sba.parameter))
    print "Chisq", chisq
    print "Change position: ", distance(tpcol2[10].ground_location,
                                        sba.ground_location(10))
    print igccol.parameter_subset
    print orb
    tpcol3 = TiePointCollection()
    for i in range(len(tpcol2)):
        tp = TiePoint(tpcol2[i].number_image)
        tp.is_gcp = tpcol2[i].is_gcp
        tp.id = tpcol2[i].id
        tp.image_location = tpcol2[i].image_location
        tp.ground_location = sba.ground_location(i)
        tpcol3.append(tp)
    generate_diagnostic(igccol, tpcol2, tpcol3)
    
    if(False):
        pstart = sba.parameter
        sba = SimultaneousBundleAdjustment(igccol, tpcol2, dem, gcp_sigma = 5)
        print "Chisq", chisq
        print "Change position: ", distance(tpcol2[10].ground_location,
                                            sba.ground_location(10))
        parm = lm_optimize(sba.sba_eq, pstart, sba.sba_jacobian)
        v = sba.sba_eq(sba.parameter)
        chisq = np.inner(v, v) / (len(v) - len(sba.parameter))
        print "Chisq", chisq
        print "Change position: ", distance(tpcol2[10].ground_location,
                                            sba.ground_location(10))

    if(False):
        for i,tp in enumerate(tpcol):
            if(tp.is_gcp):
                print i
#    lam = 0.1
#    print igccol.parameter_subset
#    sol_iteration(sba, sba.parameter, lam)
#    print "Change position: ", distance(tpcol[10].ground_location,
#                                        sba.ground_location(10))
#    print igccol.parameter_subset


