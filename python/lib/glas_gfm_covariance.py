import textwrap
import numpy as np
import warnings

class GlasGfmCovariance(object):
    '''This is the GLAS/GFM Covariance object. We map this to and
    from the CSCSDB DES. Right now this isn't a C++ implemenation
    (such as RsmIndirectCovarianceB), but rather than a class that
    can interpret and generate the CSCSDB data. We might rethink this,
    but there isn't really any strong reason now to need something that
    tightly coupled.'''
    def __init__(self):
        self.cov_version_date = "fakedate"
        self.core_set = GlasGfmCoreSet()
        self.spdcf = GlasGfmSpdcfList()
        self.io = GlasGfmIO()
        self.ts = GlasGfmTS()
        self.unmodeled_error = GlasGfmUnmodeledError()
        self.direct_covariance = GlasGfmDirectCovariance()

    @classmethod
    def read_des(cls, cscsdb):
        '''Populate this object from the information in the supplied
        pynitf.DesCSCSDB object.'''
        res = cls()
        res.cov_version_date = cscsdb.cov_version_date
        res.core_set = GlasGfmCoreSet.read_des(cscsdb)
        res.io = GlasGfmIO.read_des(cscsdb)
        res.ts = GlasGfmTS.read_des(cscsdb)
        res.unmodeled_error = GlasGfmUnmodeledError.read_des(cscsdb)
        res.spdcf = GlasGfmSpdcfList.read_des(cscsdb)
        res.direct_covariance = GlasGfmDirectCovariance.read_des(cscsdb)
        return res

    def create_des(self):
        '''Create a pynitf.DesCSCSDB from the information in this object'''
        res = pynitf.DesCSCSDB()
        res.cov_version_date = self.cov_version_date
        self.core_set.write_des(res)
        self.io.write_des(res)
        self.ts.write_des(res)
        self.spdcf.write_des(res)
        self.direct_covariance.write_des(res)
        return res

    def __str__(self):
        res = "GlasGfmCovariance:\n"
        res += f"   Covariance Version Date: {self.cov_version_date}\n"
        res += textwrap.indent(str(self.core_set), "   ") + "\n"
        res += textwrap.indent(str(self.io), "   ") + "\n"
        res += textwrap.indent(str(self.ts), "   ") + "\n"
        res += textwrap.indent(str(self.spdcf), "   ") + "\n"
        res += textwrap.indent(str(self.direct_covariance), "   ") + "\n"
        return res

class GlasGfmIO(object):
    '''Interior orientation for covariance

    Note we haven't actually implemented this yet.'''
    def __init__(self):
        pass

    @classmethod
    def read_des(cls, cscsdb):
        if(cscsdb.io_cal_ap != 0):
            warnings.warn("Skipping IO section of covariance, we don't support this yet")
        return cls()

    def write_des(self, cscsdb):
        # Not implemented, 
        cscsdb.io_cal_ap = 0

    def __str__(self):
        return "No IO"

class GlasGfmDirectCovariance(object):
    '''Direct covariance for covariance

    Note we haven't actually implemented this yet.'''
    def __init__(self):
        pass

    @classmethod
    def read_des(cls, cscsdb):
        if(cscsdb.direct_covariance_flag != 0):
            warnings.warn("Skipping Direct Covariance section of covariance, we don't support this yet")
        return cls()

    def write_des(self, cscsdb):
        # Not implemented, 
        cscsdb.direct_covariance_flag = 0

    def __str__(self):
        return "No Direct Covariance"
    
class GlasGfmTS(object):
    '''Time sync for covariance

    Note we haven't actually implemented this yet.'''
    def __init__(self):
        pass

    @classmethod
    def read_des(cls, cscsdb):
        if(cscsdb.ts_cal_ap != 0):
            warnings.warn("Skipping TS section of covariance, we don't support this yet")
        return cls()

    def write_des(self, cscsdb):
        # Not implemented, 
        cscsdb.ts_cal_ap = 0

    def __str__(self):
        return "No TS"

class GlasGfmUnmodeledError(object):
    '''Unmodeled error for covariance

    Note we haven't actually implemented this yet.'''
    def __init__(self):
        pass

    @classmethod
    def read_des(cls, cscsdb):
        if(cscsdb.ue_flag != 0):
            warnings.warn("Skipping UE section of covariance, we don't support this yet")
        return cls()

    def write_des(self, cscsdb):
        # Not implemented, 
        cscsdb.ue_flag = 0

    def __str__(self):
        return "No Unmodeled Error"

class GlasGfmCoreSet(object):
    '''Core set for covariance

    Note we haven't actually implemented this yet.'''
    def __init__(self):
        pass

    @classmethod
    def read_des(cls, cscsdb):
        if(cscsdb.core_sets != 0):
            warnings.warn("Skipping Core Sets section of covariance, we don't support this yet")
        return cls()

    def write_des(self, cscsdb):
        # Not implemented, 
        cscsdb.core_sets = 0

    def __str__(self):
        return "No Core Sets"
    
    
class GlasGfmSpdcfList(list):
    '''The list of GlasGfmSpdcf in a GlasGfmCovariance. Adds a few extra
    functions.'''
    @classmethod
    def read_des(cls, cscsdb):
        res = cls()
        if(cscsdb.spdcf_flag == 1):
            for i in range(cscsdb.num_spdcf):
                t = GlasGfmSpdcf._read_des(cscsdb, i)
                res.append(t)
        return res

    def write_des(self, cscsdb):
        if(len(self) == 0):
            cscsdb.spdcf_flag = 0
            return
        cscsdb.spdcf_flag = 1
        cscsdb.num_spdcf = len(self)
        for i, s in enumerate(self):
            s._write_des(cscsdb, i)
            
    def __str__(self):
        if(len(self) == 0):
            return "Empty GlasGfmSpdcfList"
        res = f"GlasGfmSpdcfList of {len(self)} SPDCFs\n"
        for i, s in enumerate(self):
            res += f"   SPDCF Id: {s.id_number}\n"
            res += textwrap.indent(str(s), "       ") + "\n"
        return res
    
class GlasGfmSpdcf(object):
    '''This is a strictly positive definite correlation function. This
    describes the time behavior of the correlation between parameters
    at different times.'''
    def __init__(self, weight=1.0, id_number=None):
        self.weight = weight
        self.id_number = id_number
        # Filled in by derived classes
        self.family_id = None

    def number_constituent_spdcf(self):
        return 1

    def scalar_value(self, delta_t):
        '''The scalar value p(delta_t) for the SPDCF.'''
        raise NotImplementedError()

    @classmethod
    def _read_des(cls, cscsdb, n, j=None):
        '''Try to read the information for the n,j SDPCF in the DesCSCSDB.
        return object if successful, None otherwise'''
        id_number = None
        if(j is None):
            id_number = cscsdb.spdcf_id[n]
            if(cscsdb.spdcf_p[n] > 1):
                return GlasGfmSpdcfComposite._read_des(cscsdb, n)
            else:
                j = 0
        for cls_try in (GlasGfmSpdcfCsm, GlasGfmSpdcfPiecwiseLinear,
                        GlasGfmSpdcfDampedCosine):
            res = cls_try._read_des(cscsdb, n, j)
            if(res is not None):
                res.id_number = id_number
                return res
        return None

    def _write_des(self, cscsdb, n, j=None):
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
    def _read_des(cls, cscsdb, n, j):
        '''Read the information for the n,j SDPCF in the DesCSCSDB'''
        if cscsdb.spdcf_fam[n,j] != 0:
            return None
        return cls(cscsdb.fp_a[n,j], cscsdb.fp_alpha[n,j],
                   cscsdb.fp_beta[n,j], cscsdb.fp_t[n,j],
                   weight=cscsdb.spdcf_weight[n,j])

    def _write_des(self, cscsdb, n, j=None):
        '''Write information for the n,j SDPCF in the DesCSCSDB'''
        if(j is None):
            cscsdb.spdcf_id[n] = self.id_number
            cscsdb.spdcf_p[n] = 1
            j = 0
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
    def _read_des(cls, cscsdb, n, j):
        '''Read the information for the n,j SDPCF in the DesCSCSDB'''
        if cscsdb.spdcf_fam[n,j] != 1:
            return None
        nseg = cscsdb.num_segs[n,j]
        return cls([cscsdb.pl_max_cor[n,j,k] for k in range(nseg)],
                   [cscsdb.pl_tau_max_cor[n,j,k] for k in range(nseg)],
                   weight=cscsdb.spdcf_weight[n,j])

    def _write_des(self, cscsdb, n, j=None):
        '''Write information for the n,j SDPCF in the DesCSCSDB'''
        if(j is None):
            cscsdb.spdcf_id[n] = self.id_number
            cscsdb.spdcf_p[n] = 1
            j = 0
        cscsdb.spdcf_fam[n,j] = 1
        cscsdb.spdcf_weight[n,j] = self.weight
        cscsdb.num_segs[n,j] = len(self.max_correlation)
        for k in range(len(self.max_correlation)):
            cscsdb.pl_max_cor[n,j,k] = self.max_correlation[k]
            cscsdb.pl_tau_max_cor[n,j,k] = self.tau_max_correlation[k]
        
class GlasGfmSpdcfDampedCosine(GlasGfmSpdcf):
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
    def _read_des(cls, cscsdb, n, j):
        '''Read the information for the n,j SDPCF in the DesCSCSDB'''
        if cscsdb.spdcf_fam[n,j] != 2:
            return None
        return cls(cscsdb.dc_a[n,j], cscsdb.dc_t[n,j],
                   cscsdb.dc_p[n,j], 
                   weight=cscsdb.spdcf_weight[n,j])

    def _write_des(self, cscsdb, n, j=None):
        '''Write information for the n,j SDPCF in the DesCSCSDB'''
        if(j is None):
            cscsdb.spdcf_id[n] = self.id_number
            cscsdb.spdcf_p[n] = 1
            j = 0
        cscsdb.spdcf_fam[n,j] = 2
        cscsdb.spdcf_weight[n,j] = self.weight
        cscsdb.dc_a[n,j] = self.A
        cscsdb.dc_t[n,j] = self.T
        cscsdb.dc_p[n,j] = self.P

class GlasGfmSpdcfComposite(GlasGfmSpdcf):
    '''A composite SPDCF, made up of a number of SPDCFs with weights'''
    def __init__(self, spdcf_list, id_number=None):
        super().__init__(id_number=id_number)
        self.spdcf_list = spdcf_list

    def __str__(self):
        res = "GlasGfmSpdcfComposite:\n"
        for s in self.spdcf_list:
            res += f"   Weight {s.weight}\n"
            res += textwrap.indent(str(s), "      ") + "\n"
        return res

    @classmethod
    def _read_des(cls, cscsdb, n, j=None):
        '''Read the information for the n,j SDPCF in the DesCSCSDB'''
        if(j is not None):
            return None
        return cls([GlasGfmSpdcf._read_des(cscsdb, n, i)
                    for i in range(cscsdb.spdcf_p[n])],
                   id_number=cscsdb.spdcf_id[n])

    def _write_des(self, cscsdb, n, j=None):
        '''Write information for the n,j SDPCF in the DesCSCSDB'''
        if(j is not None):
            raise RuntimeError("This shouldn't happen")
        cscsdb.spdcf_id[n] = self.id_number
        cscsdb.spdcf_p[n] = len(self.spdcf_list)
        for j, s in enumerate(self.spdcf_list):
            s._write_des(cscsdb, n, j)
    
__all__ = ["GlasGfmCovariance", "GlasGfmSpdcfList",
           "GlasGfmSpdcf", "GlasGfmSpdcfCsm",
           "GlasGfmSpdcfPiecwiseLinear", "GlasGfmSpdcfDampedCosine",
           "GlasGfmSpdcfComposite"]
