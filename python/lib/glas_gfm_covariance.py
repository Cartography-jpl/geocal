import textwrap

class GlasGfmCovariance(object):
    '''This is the GLAS/GFM Covariance object. We map this to and
    from the CSCSDB DES. Right now this isn't a C++ implemenation
    (such as RsmIndirectCovarianceB), but rather than a class that
    can interpret and generate the CSCSDB data. We might rethink this,
    but there isn't really any strong reason now to need something that
    tightly coupled.'''
    def __init__(self):
        pass

    def read_des(self, cscsdb):
        '''Populate this object from the information in the supplied
        pynitf.DesCSCSDB object.'''
        pass

    def write_des(self):
        '''Create a pynitf.DesCSCSDB from the information in this object'''
        pass

class GlasGfmSpdcf(object):
    '''This is a strictly positive definite correlation function. This
    describes the time behavior of the correlation between parameters
    at different times.'''
    def __init__(self, weight=1.0, id_number=None):
        self.weight = weight
        self.id_number = None
        # Filled in by derived classes
        self.family_id = None

    def number_constituent_spdcf(self):
        return 1

    def scalar_value(self, delta_t):
        '''The scalar value p(delta_t) for the SPDCF.'''
        raise NotImplementedError()

    @classmethod
    def _read_tre(cls, cscsdb, n, j):
        '''Try to read the information for the n,j SDPCF in the DesCSCSDB.
        return object if successful, None otherwise'''
        for cls_try in (GlasGfmSpdcfCsm, GlasGfmSpdcfPiecwiseLinear,
                        GlasGfmSpdcDampedCosine):
            res = clt_try._read_tre(cscsdb, n, j)
            if(res is not None):
                return res
        return None

    def _write_tre(self, cscsdb, n, j):
        '''Write information for the n,j SDPCF in the DesCSCSDB'''
        raise NotImplementedError()

    def __str__(self):
        return "GlasGfmSpdcf"
    

class GlasGfmSpdcfCsm(GlasGfmSpdcf):
    '''This is a CSM four parameter SPDCF'''
    def __init__(self, A, alpha, beta, T, weight=1.0, id_number=None):
        super().__init__(weight, id_number)
        # Note this breaks the normal python naming convention, but
        # because this is how the parameters are called in the NITF
        # documentation this trumps the python convention. A and T
        # aren't constants, even though they are capital letters.
        self.A = A
        self.alpha = alpha
        self.beta = beta
        self.T = T

    def __str__(self):
        return f"CSM four parameter SPDCF ({self.A}, {self.alpha}, {self.beta}, {self.T})"
    
    @classmethod
    def _read_tre(cls, cscsdb, n, j):
        '''Read the information for the n,j SDPCF in the DesCSCSDB'''
        if cscsdb.spdcf_fam[n,j] != 0:
            return None
        return cls(cscsdb.fp_a[n,j], cscsdb.fp_alpha[n,j],
                   cscsdb.fp_beta[n,j], cscsdb.fp_t[n,j],
                   weight=cscsdb.spdcf_weight[n,j])

    def _write_tre(self, cscsdb, n, j):
        '''Write information for the n,j SDPCF in the DesCSCSDB'''
        cscsdb.spdcf_fam[n,j] = 0
        cscsdb.spdcf_weight[n,j] = self.weight
        cscsdb.fp_a[n,j] = self.A
        cscsdb.fp_alpha[n,j] = self.alpha
        cscsdb.fp_beta[n,j] = self.beta
        cscsdb.fp_t[n,j] = self.T

class GlasGfmSpdcfPiecwiseLinear(GlasGfmSpdcf):
    '''This is a piecewise linear SPDCF'''
    def __init__(self, max_correlation, tau_max_correlation,
                 weight=1.0, id_number=None):
        super().__init__(weight, id_number)
        self.max_correlation = max_correlation
        self.tau_max_correlation = tau_max_correlation
        
    def __str__(self):
        return f'''Piecewise linear SPDCF
   {self.max_correlation}   
   {self.tau_max_correlation}'''
    
    @classmethod
    def _read_tre(cls, cscsdb, n, j):
        '''Read the information for the n,j SDPCF in the DesCSCSDB'''
        if cscsdb.spdcf_fam[n,j] != 1:
            return None
        nseg = cscsdb.num_seg[n,j]
        return cls(np.array(cscsdb.pl_max_cor[n,j,k] for k in range(nseg)),
                   np.array(cscsdb.pl_tau_max_cor[n,j,k] for k in range(nseg)),
                   weight=cscsdb.spdcf_weight[n,j])

    def _write_tre(self, cscsdb, n, j):
        '''Write information for the n,j SDPCF in the DesCSCSDB'''
        cscsdb.spdcf_fam[n,j] = 1
        cscsdb.spdcf_weight[n,j] = self.weight
        cscsdb.num_seg[n,j] = len(cscsdb.pl_max_cor)
        for k in range(len(cscsdb.pl_max_cor)):
            cscsdb.pl_max_cor[n,j,k] = self.max_correlation[k]
            cscsdb.pl_tau_max_cor[n,j,k] = self.tau_max_correlation[k]
        
class GlasGfmSpdcDampedCosine(GlasGfmSpdcf):
    '''This is a damped cosine SPDCF'''
    def __init__(self, A, T, P, weight=1.0, id_number=None):
        super().__init__(weight, id_number)
        # Note this breaks the normal python naming convention, but
        # because this is how the parameters are called in the NITF
        # documentation this trumps the python convention. A and T
        # aren't constants, even though they are capital letters.
        self.A = A
        self.T = T
        self.P = P

    def __str__(self):
        return f"Damped cosine SPDCF ({self.A}, {self.T}, {self.P})"
    
    @classmethod
    def _read_tre(cls, cscsdb, n, j):
        '''Read the information for the n,j SDPCF in the DesCSCSDB'''
        if cscsdb.spdcf_fam[n,j] != 2:
            return None
        return cls(cscsdb.dc_a[n,j], cscsdb.dc_t[n,j],
                   cscsdb.dc_p[n,j], 
                   weight=cscsdb.spdcf_weight[n,j])

    def _write_tre(self, cscsdb, n, j):
        '''Write information for the n,j SDPCF in the DesCSCSDB'''
        cscsdb.spdcf_fam[n,j] = 2
        cscsdb.spdcf_weight[n,j] = self.weight
        cscsdb.dc_a[n,j] = self.A
        cscsdb.dc_t[n,j] = self.T
        cscsdb.dc_p[n,j] = self.P

class GlasGfmSpdcfComposite(GlasGfmSpdcf):
    '''A composite SPDCF, made up of a number of SPDCFs with weights'''
    def __init__(self, spdcf_list):
        super().__init__()
        self.spdcf_list = spdcf_list

    def __str__(self):
        return f"Damped cosine SPDCF ({self.A}, {self.T}, {self.P})"
        
    
