from geocal_swig import Rsm, RsmId, RsmMultiSection, RsmRationalPolynomial, RsmGrid, RsmRpPlusGrid
from .nitf_tre_rsmida import TreRSMIDA
from .nitf_tre_rsmgga import TreRSMGGA
from .nitf_tre_rsmgia import TreRSMGIA
from .nitf_tre_rsmpca import TreRSMPCA
from .nitf_tre_rsmpia import TreRSMPIA

# This provides a higher level interface to the RSM TREs, using a C++ Rsm object
# to read and write the full set of TREs.

def _rsm_add_rec(seg, v):
    if(isinstance(v, RsmRpPlusGrid)):
        raise NotImplementedError()
    elif(isinstance(v, RsmRationalPolynomial)):
        t = TreRSMPCA()
        t.rsm_rational_polynomial = v
        seg.tre_list.append(t)
    elif(isinstance(v, RsmGrid)):
        t = TreRSMGGA()
        t.rsm_grid = v
        seg.tre_list.append(t)
    elif(isinstance(v, RsmMultiSection)):
        if(isinstance(v.section(0,0), RsmRationalPolynomial)):
            t = TreRSMPIA()
            t.rsm_multi_section = v
            seg.tre_list.append(t)
        elif(isinstance(v.section(0,0), RsmGrid)):
            t = TreRSMGIA()
            t.rsm_multi_section = v
            seg.tre_list.append(t)
        else:
            raise RuntimeError("Unknown section in RsmMultiSection " + str(v.section(0,0)))
        for i in range(v.number_row_section):
            for j in range(v.number_col_section):
                _rsm_add_rec(seg, v.section(i, j))
    else:
        raise RuntimeError("Unknown Rsm element " + str(v))
                
def rsm_prepare_tre_write(seg):
    '''Remove all the existing RSM TREs (if any), and add the TREs 
    to store seg.rsm'''
    # Remove any existing RSM Tres
    seg.tre_list = [t for t in seg.tre_list if t.tre_tag not in
                    ('RSMIDA', 'RSMPIA','RSMPCA', 'RSMDCA', 'RSMDCB',
                     'RSMAPA', 'RSMAPB', 'RSMECA', 'RSMECB',
                     'RSMGIA', 'RSMGGA')]
    if(seg.rsm):
        t = TreRSMIDA()
        t.rsm_id = seg.rsm.rsm_id
        seg.tre_list.append(t)
        _rsm_add_rec(seg, seg.rsm.rsm_base)
                
def rsm_read_tre(seg):
    '''Read all the RSM TREs to fill in seg.rsm. If there are no RSM TREs,
    then set seg.rsm to None.'''
    pass
