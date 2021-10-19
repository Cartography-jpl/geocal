import textwrap
import numpy as np
import warnings
from geocal_swig import Time
try:
    import pynitf
    have_pynitf = True
except ImportError:
    # Ok if we don't have pynitf, we just can't execute this code
    have_pynitf = False
from functools import partial    

class GlasGfmNitfTimeWrapper:
    '''The GLAS stuff uses a particular format for the NITF time stamps.
    This converts a geocal time to and from a date and time such as 
    corr_ref_date and corr_ref_time. Not sure how general this class is, 
    it seems every TRE uses a different format. For now, we'll just have 
    this internal to this module but we could export it if this 
    proves useful.'''
    def __init__(self, field_base, cscsdb):
        self.cscsdb = cscsdb
        self.field_base = field_base
    def __getitem__(self, key):
        dt = self.cscsdb.get_raw_bytes(self.field_base +
                                       "_date", key).decode("utf-8")
        tm = self.cscsdb.get_raw_bytes(self.field_base + "_time",
                                       key).decode("utf-8")
        return Time.parse_time(
            f"{dt[0:4]}-{dt[4:6]}-{dt[6:8]}T{tm[0:2]}:{tm[2:4]}:{tm[4:]}Z")
    def __setitem__(self, key, val):
        # Split between date and time
        dt, tm = str(val).split("T")
        # Remove "-" in date, giving a string like 20210203, and assign
        getattr(self.cscsdb,
                self.field_base + "_date")[key] = dt.replace("-", "")
        # Put in HHMMSS.nnnnnnnnn format and assign
        getattr(self.cscsdb, self.field_base + "_time")[key] = \
        pynitf.NitfLiteral(("%016.9lf" % float(tm.replace("Z",
                                    "").replace(":",""))).encode('utf-8'))

class GlasGfmNitfArrayWrapper:
    '''We have a number of places where the is an 1d array over the last index,
    for example adj_parm_id. As a convenience, have a wrapper that translates
    this to and from an array.

    On assignment, the size field gets updated.'''
    def __init__(self, field, size_field, cscsdb):
        self.cscsdb = cscsdb
        self.field = field
        self.size_field = size_field
    def __getitem__(self, key):
        sz = getattr(self.cscsdb, self.size_field)[key]
        return [getattr(self.cscsdb, self.field)[(*key,i)] for i in range(sz)]
    def __setitem__(self, key, val):
        getattr(self.cscsdb, self.size_field)[key] = len(val)
        for i,v in enumerate(val):
            getattr(self.cscsdb, self.field)[(*key,i)] = v

class GlasGfmNitfCovWrapper:
    '''We have a number of places where the is an covariance over the last 
    index, for example errcov_c1. The data is stored row major, upper
    triangle only.

    On assignment, we check against the size field but don't update it.
    This is because the covariance goes against something already in the 
    file, so we just need to be consistent with it.'''
    def __init__(self, field, size_field, cscsdb):
        self.cscsdb = cscsdb
        self.field = field
        self.size_field = size_field
    def __getitem__(self, key):
        sz = getattr(self.cscsdb, self.size_field)[key]
        res = np.empty((sz,sz))
        k = 0
        for i in range(sz):
            for j in range(i, sz):
                res[i, j] = getattr(self.cscsdb, self.field)[(*key,k)]
                res[j, i] = res[i, j]
                k += 1
        return res
    def __setitem__(self, key, val):
        sz = getattr(self.cscsdb, self.size_field)[key]
        if(val.shape[0] != sz or val.shape[1] != sz):
            raise RuntimeError(f"Matrix needs to be {sz} x {sz}")
        k = 0
        for i in range(sz):
            for j in range(i, sz):
                getattr(self.cscsdb, self.field)[(*key,k)] = val[i, j]
                k += 1
            
if(have_pynitf):
    # Add functions to map to and from geocal Time
    pynitf.DesCSCSDB.corr_ref_geocal_time = property(partial(GlasGfmNitfTimeWrapper, "corr_ref"))
    pynitf.DesCSCSDB.adj_parm_id_array = property(partial(GlasGfmNitfArrayWrapper, "adj_parm_id", "num_adj_parm"))
    pynitf.DesCSCSDB.errcov_c1_array = property(partial(GlasGfmNitfCovWrapper, "errcov_c1", "num_adj_parm"))

class GlasGfmCovariance(object):
    '''This is the GLAS/GFM Covariance object. We map this to and
    from the CSCSDB DES. Right now this isn't a C++ implemenation
    (such as RsmIndirectCovarianceB), but rather than a class that
    can interpret and generate the CSCSDB data. We might rethink this,
    but there isn't really any strong reason now to need something that
    tightly coupled.'''
    def __init__(self):
        self.cov_version_date = "20000101"
        self.core_set = GlasGfmCoreSetList()
        self.spdcf = GlasGfmSpdcfList()
        self.io = GlasGfmIO()
        self.ts = GlasGfmTS()
        self.unmodeled_error = GlasGfmUnmodeledError()
        self.direct_covariance = GlasGfmDirectCovariance()
        self.id = None

    @classmethod
    def read_des(cls, cscsdb):
        '''Populate this object from the information in the supplied
        pynitf.DesCSCSDB object.'''
        res = cls()
        res.cov_version_date = cscsdb.cov_version_date
        res.core_set = GlasGfmCoreSetList.read_des(cscsdb)
        res.io = GlasGfmIO.read_des(cscsdb)
        res.ts = GlasGfmTS.read_des(cscsdb)
        res.unmodeled_error = GlasGfmUnmodeledError.read_des(cscsdb)
        res.spdcf = GlasGfmSpdcfList.read_des(cscsdb)
        res.direct_covariance = GlasGfmDirectCovariance.read_des(cscsdb)
        return res

    def write_des(self, cscsdb):
        '''Write to an existing DesCSCSDB.'''
        cscsdb.cov_version_date = self.cov_version_date
        self.core_set.write_des(cscsdb)
        self.io.write_des(cscsdb)
        self.ts.write_des(cscsdb)
        self.spdcf.write_des(cscsdb)
        self.direct_covariance.write_des(cscsdb)

    def create_des(self):
        '''Create a pynitf.DesCSCSDB from the information in this object'''
        res = pynitf.DesCSCSDB()
        self.write_des(res)
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

# TODO Implement GlasGfmIO    
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

# TODO Implement GlasGfmDirectCovariance
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
    
# Implement GlasGfmTS        
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

class GlasGfmGroupList(list):
    '''The list of GlasGfmGroup. Adds a few extra functions'''
    @classmethod
    def _read_des(cls, cscsdb, i):
        res = cls()
        for j in range(cscsdb.num_groups[i]):
            res.append(GlasGfmGroup._read_des(cscsdb, i, j))
        return res

    def _write_des(self, cscsdb, i):
        cscsdb.num_groups[i] = len(self)
        for j, s in enumerate(self):
            s._write_des(cscsdb, i, j)
            
    def __str__(self):
        if(len(self) == 0):
            return "Empty GlasGfmGroupList"
        res = ""
        for i, s in enumerate(self):
            res += f"Group {i}\n"
            res += textwrap.indent(str(s), "   ") + "\n"
        return res
    
class GlasGfmCoreSetList(list):
    '''The list of GlasGfmSpdcf in a GlasGfmCovariance. Adds a few extra
    functions.'''
    @classmethod
    def read_des(cls, cscsdb):
        res = cls()
        for i in range(cscsdb.core_sets):
            t = GlasGfmCoreSet._read_des(cscsdb, i)
            res.append(t)
        return res

    def write_des(self, cscsdb):
        cscsdb.core_sets = len(self)
        for i, s in enumerate(self):
            s._write_des(cscsdb, i)
            
    def __str__(self):
        if(len(self) == 0):
            return "Empty GlasGfmCoreSetList"
        res = f"GlasGfmCoreSetList of {len(self)} core sets\n"
        for i, s in enumerate(self):
            res += f"   Core set: {i}\n"
            res += textwrap.indent(str(s), "       ") + "\n"
        return res
    
class GlasGfmCoreSet(object):
    '''Core set for covariance'''
    # Reference frames
    ECF = 1
    SENSOR_FRAME = 2
    ORBITAL_FRAME = 3
    TARGET_CENTERED_EARTH_FIXED = 4
    ECI = 5
    SENSOR_CENTERED_EARTH_FIXED = 6
    def __init__(self, ref_frame_position = ECF,
                 ref_frame_attitude = ECF,
                 group = GlasGfmGroupList()):
        self.ref_frame_position = ref_frame_position
        self.ref_frame_attitude = ref_frame_attitude
        self.sensor_error_parameter_group = group
    
    def _frame_id_to_name(self, i):
        if(i == self.ECF):
            return "ECF"
        if(i == self.SENSOR_FRAME):
            return "SENSOR_FRAME"
        if(i == self.ORBITAL_FRAME):
            return "ORBITAL_FRAME"
        if(i == self.TARGET_CENTERED_EARTH_FIXED):
            return "TARGET_CENTERED_EARTH_FIXED"
        if(i == self.ECI):
            return "ECI"
        if(i == self.SENSOR_CENTERED_EARTH_FIXED):
            return "SENSOR_CENTERED_EARTH_FIXED"
        return f"Unknown, ID={i}"
    
    @classmethod
    def _read_des(cls, cscsdb, i):
        res = cls(cscsdb.ref_frame_position[i],
                  cscsdb.ref_frame_attitude[i],
                  GlasGfmGroupList._read_des(cscsdb, i))
        return res

    def _write_des(self, cscsdb, i):
        cscsdb.ref_frame_position[i] = self.ref_frame_position
        cscsdb.ref_frame_attitude[i] = self.ref_frame_attitude
        self.sensor_error_parameter_group._write_des(cscsdb, i)

    def __str__(self):
        res = "GlasGfmCoreSet:\n"
        res += f"   Reference frame position: {self._frame_id_to_name(self.ref_frame_position)}\n"
        res += f"   Reference frame attitude: {self._frame_id_to_name(self.ref_frame_attitude)}\n"
        res +=  "   Independent Sensor Error Parameter Group:\n"
        res += textwrap.indent(str(self.sensor_error_parameter_group), "      ")
        return res

# TODO Implement PF handling
# TODO Implement PL handling    
class GlasGfmGroup(list):
    '''A Independent Sensor Error Parameter Group'''
    def __init__(self, corr_ref_t=Time.parse_time("2000-01-01T00:00:00Z"),
                 adjustable_parameter=[],
                 basic_sub_alloc = True,
                 post_sub_alloc = False,
                 errcov_c1 = None,
                 sr_spdcf_id = None):
        self.corr_ref_t = corr_ref_t
        self.adjustable_parameter = adjustable_parameter
        self.basic_sub_alloc = bool(basic_sub_alloc)
        self.post_sub_alloc = bool(post_sub_alloc)
        self.errcov_c1 = errcov_c1
        self.sr_spdcf_id = sr_spdcf_id
                 
    @classmethod
    def _read_des(cls, cscsdb, i, j):
# Warn about pieces we don't handle. Note that we don't need special handling
# for basic_sub_alloc being False, both 0 and None map to False
        if(cscsdb.basic_pf_flag[i,j]):
            warnings.warn("Skipping PF section of group, we don't support this yet")
        if(cscsdb.basic_pl_flag[i,j]):
            warnings.warn("Skipping PL section of group, we don't support this yet")
        if(cscsdb.post_pf_flag[i,j]):
            warnings.warn("Skipping PF section of group, we don't support this yet")
        if(cscsdb.post_pl_flag[i,j]):
            warnings.warn("Skipping PL section of group, we don't support this yet")
        res = GlasGfmGroup(corr_ref_t = cscsdb.corr_ref_geocal_time[i,j],
                           adjustable_parameter = cscsdb.adj_parm_id_array[i,j],
                           basic_sub_alloc = cscsdb.basic_sub_alloc[i,j],
                           post_sub_alloc = cscsdb.post_sub_alloc[i,j],
                           sr_spdcf_id = cscsdb.basic_sr_spdcf[i,j]
                           )
        if(cscsdb.basic_sub_alloc[i,j]):
            res.errcov_c1 = cscsdb.errcov_c1_array[i,j]
        return res
    
    def _write_des(self, cscsdb, i, j):
        cscsdb.corr_ref_geocal_time[i,j] = self.corr_ref_t
        cscsdb.adj_parm_id_array[i,j] = self.adjustable_parameter
        cscsdb.basic_sub_alloc[i,j] = int(self.basic_sub_alloc)
        if(self.basic_sub_alloc):
            # Right now PF and PL aren't handled, so always turn off
            cscsdb.basic_pf_flag[i,j] = 0
            cscsdb.basic_pl_flag[i,j] = 0
            cscsdb.errcov_c1_array[i,j] = self.errcov_c1
            if(self.sr_spdcf_id):
                cscsdb.basic_sr_flag[i,j] = 1
                cscsdb.basic_sr_spdcf[i, j] = self.sr_spdcf_id
            else:
                cscsdb.basic_sr_flag[i,j] = 0
        cscsdb.post_sub_alloc[i,j] = int(self.post_sub_alloc)
        if(self.post_sub_alloc):
            # Right now PF and PL aren't handled, so always turn off
            cscsdb.post_pf_flag[i,j] = 0
            cscsdb.post_pl_flag[i,j] = 0
            
    def __str__(self):
        res = f"Date of last De-Correlation event: {str(self.corr_ref_t)}\n"
        res += "Adjustable parameter id:\n"
        res += textwrap.indent(str(self.adjustable_parameter), "   ") + "\n"
        if(self.basic_sub_alloc):
            res += "Individual Error Covariance:\n"
            res += textwrap.indent(str(self.errcov_c1), "   ") + "\n"
            res += "No Platform type SPDCF\n"
            res += "No Payload type SPDCF\n"
            if(self.sr_spdcf_id):
                res += f"Sensor type SPDCF id: {self.sr_spdcf_id}\n"
            else:
                res += "No Sensor type SPDCF\n"
        return res
    
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

def _glas_gfm_covariance(iseg):
    '''Search for GlasGfmCovariance that goes with a image segment'''
    lv = iseg.idlvl
    f = iseg.nitf_file
    if f is None:
        raise RuntimeError("Need to have nitf_file set for image segment level %d" % lv)
    possible = [d for d in f.des_segment if
                d.subheader.desid == "CSCSDB" and
                (d.des.user_subheader.numais == "ALL" or
                 lv in d.des.user_subheader.aisdlvl)]
    if(len(possible) == 0):
        return None
    if(len(possible) > 1):
        raise RuntimeError("Found more than one possible covariance_cscsdb for image esegment level %d" % lv)
    return GlasGfmCovariance().read_des(possible[0].des)

def _glas_gfm_covariance_set(iseg, glas_gfm_cov):
    f = iseg.nitf_file
    if(f is None):
        raise RuntimeError("Need to add NitfImageSegment to a NitfFile before we can add a pos_csephb to it")
    d = f.find_des_by_uuid(glas_gfm_cov.id)
    if(d is None):
        d = glas_gfm_cov.create_des()
        d.generate_uuid_if_needed()
        glas_gfm_cov.id = d.id
        f.des_segment.append(pynitf.NitfDesSegment(d))
    d.add_display_level(iseg.idlvl)

if(have_pynitf):    
    pynitf.NitfImageSegment.glas_gfm_covariance = property(_glas_gfm_covariance, _glas_gfm_covariance_set)
    
__all__ = ["GlasGfmCovariance",
           "GlasGfmGroupList",
           "GlasGfmGroup",
           "GlasGfmCoreSet",
           "GlasGfmCoreSetList",
           "GlasGfmSpdcfList",
           "GlasGfmSpdcf", "GlasGfmSpdcfCsm",
           "GlasGfmSpdcfPiecwiseLinear", "GlasGfmSpdcfDampedCosine",
           "GlasGfmSpdcfComposite"]
