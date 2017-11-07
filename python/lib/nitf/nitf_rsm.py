from geocal_swig import Rsm, RsmId, RsmMultiSection, RsmRationalPolynomial, RsmGrid
from .nitf_tre_rsmida import TreRSMIDA
from .nitf_tre_rsmgga import TreRSMGGA
from .nitf_tre_rsmgia import TreRSMGIA
from .nitf_tre_rsmpca import TreRSMPCA
from .nitf_tre_rsmpia import TreRSMPIA

# This provides a higher level interface to the RSM TREs, using a C++ Rsm object
# to read and write the full set of TREs.

def rsm_prepare_tre_write(seg):
    '''Remove all the existing RSM TREs (if any), and add the TREs 
    to store seg.rsm'''
    # Remove any existing RSM Tres
    seg.tre_list = [t for t in seg.tre_list if t.tre_tag not in
                    ('RSMIDA', 'RSMPIA','RSMPCA', 'RSMDCA', 'RSMDCB',
                     'RSMAPA', 'RSMAPB', 'RSMECA', 'RSMECB',
                     'RSMGIA', 'RSMGGA')]

def rsm_read_tre(seg):
    '''Read all the RSM TREs to fill in seg.rsm. If there are no RSM TREs,
    then set seg.rsm to None.'''
    pass
