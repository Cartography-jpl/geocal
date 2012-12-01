import numpy as np
import scipy.sparse as sp
import scipy.sparse.linalg
import time

def lm_optimize(eq_func, x0, jac_func, min_chisqr = 0.1, 
                stopping_criteria = 0.001, max_iteration = 50, boost = 2, 
                drop = 3, lambda_initial =0.1, diagnostic = False):
    '''This is a simple implementation of Levenberg Marquardt for minimizing
    the residuals of a set of equations. scipy already has an optimizer 
    called scipy.optimize.leastsq. You should generally use this scipy 
    function in preference to this class, it is implemented in Fortran and
    is a more sophisticated algorithm. But unlike that code, this algorithm
    takes advantage of sparse matrixes.

    We use as a stopping criteria one of the following conditions:
    1. We exceed max_iteration. In that case, we quit and raise an exception
    2. The chisqr of the residuals is < min_chisqr,
    3. The change in chisqr from one iteration to the next is 
       < stopping_criteria
   '''
    start_time = time.clock()
    x = x0
    t1 = time.clock()
    res = eq_func(x)
    if(diagnostic):
        print "Done with residual."
        print "  Total time:", time.clock() - start_time
        print "  Delta time:", time.clock() - t1
    chisq = np.inner(res, res) / (len(res) - len(x0))
    lam = lambda_initial
    for i in range(max_iteration):
        t1 = time.clock()
        j = jac_func(x).tocsr()
        if(diagnostic):
            print "Done with jacobian."
            print "  Total time:", time.clock() - start_time
            print "  Delta time:", time.clock() - t1
        jtj = j.transpose() * j
        chisqold = chisq
        for k in range(max_iteration):
            c = jtj + sp.spdiags(jtj.diagonal() * lam, 0, jtj.shape[0],
                                 jtj.shape[1], format="csr")
            jtres = j.transpose() * res
            t1 = time.clock()
            # Note permc_spec has *no* effect on umfpack library, but we put 
            # this in place in case umf isn't available and we are using 
            # superlu. For superlu, this permutation significantly speeds
            # this up.
            xnew = x - sp.linalg.spsolve(c, jtres, use_umfpack=True,
                                         permc_spec="MMD_ATA")
            if(diagnostic):
                print "Done with spsolve."
                print "  Total time:", time.clock() - start_time
                print "  Delta time:", time.clock() - t1
            t1 = time.clock()
            resnew = eq_func(xnew)
            if(diagnostic):
                print "Done with residual."
                print "  Total time:", time.clock() - start_time
                print "  Delta time:", time.clock() - t1
            chisq = np.inner(resnew, resnew) / (len(resnew) - len(x0))
            if(chisq < chisqold):
                x = xnew
                lam /= drop
                res = resnew
                break
            else:
                lam *= boost
                if(diagnostic):
                    print "Redoing iteration with lambda boosted to", lam
        else:
            raise RuntimeError("Exceeded maximum number of iterators")
        if(chisq < min_chisqr or
           (chisqold - chisq) / chisq < stopping_criteria):
            break
        if(diagnostic):
            print "Done with iteration %d, chisq %f" % (i, chisq)
    else:
        raise RuntimeError("Exceeded maximum number of iterators")
    if(diagnostic):
        print "Done with optimize."
        print "  Total time:", time.clock() - start_time
    return x
