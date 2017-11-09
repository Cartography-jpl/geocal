from geocal_swig import Rsm, RsmId, RsmMultiSection, RsmRationalPolynomial, RsmGrid, RsmRpPlusGrid, GeodeticConverter
from .nitf_tre_rsmida import TreRSMIDA
from .nitf_tre_rsmgga import TreRSMGGA
from .nitf_tre_rsmgia import TreRSMGIA
from .nitf_tre_rsmpca import TreRSMPCA
from .nitf_tre_rsmpia import TreRSMPIA

# This provides a higher level interface to the RSM TREs, using a C++ Rsm object
# to read and write the full set of TREs.

def _rsm_add_rec(seg, v):
    if(isinstance(v, RsmRpPlusGrid)):
        _rsm_add_rec(seg, v.rational_polynomial)
        _rsm_add_rec(seg, v.correction_grid)
    elif(isinstance(v, RsmRationalPolynomial)):
        t = TreRSMPCA()
        t.rsm_rational_polynomial = v
        t.update_raw_field()
        seg.tre_list.append(t)
    elif(isinstance(v, RsmGrid)):
        t = TreRSMGGA()
        t.rsm_grid = v
        t.update_raw_field()
        seg.tre_list.append(t)
    elif(isinstance(v, RsmMultiSection)):
        if(isinstance(v.section(0,0), RsmRationalPolynomial)):
            t = TreRSMPIA()
            t.rsm_multi_section = v
            t.update_raw_field()
            seg.tre_list.append(t)
        elif(isinstance(v.section(0,0), RsmGrid)):
            t = TreRSMGIA()
            t.rsm_multi_section = v
            t.update_raw_field()
            seg.tre_list.append(t)
        else:
            raise RuntimeError("Unknown section in RsmMultiSection " + str(v.section(0,0)))
        for i in range(v.number_row_section):
            for j in range(v.number_col_section):
                _rsm_add_rec(seg, v.section(i, j))
    else:
        raise RuntimeError("Unknown Rsm element " + str(v))

rsm_tre_tag_list =  ['RSMIDA', 'RSMPIA','RSMPCA', 'RSMDCA', 'RSMDCB',
                     'RSMAPA', 'RSMAPB', 'RSMECA', 'RSMECB',
                     'RSMGIA', 'RSMGGA']
   
                
def rsm_prepare_tre_write(seg):
    '''Remove all the existing RSM TREs (if any), and add the TREs 
    to store seg.rsm'''
    # Remove any existing RSM Tres
    seg.tre_list = [t for t in seg.tre_list if t.tre_tag not in
                    rsm_tre_tag_list]
    # Currently only handle one RSM TRE set. We could extend this if needed.
    if(seg.rsm):
        t = TreRSMIDA()
        t.rsm_id = seg.rsm.rsm_id
        seg.tre_list.append(t)
        _rsm_add_rec(seg, seg.rsm.rsm_base)

def _rsm_find_tre_list(seg, tre_tag, edition, sec_row = None, sec_col = None):
    if(sec_row is None):
        return [i for i in seg.tre_list if i.tre_tag == tre_tag and
                (edition is None or i.edition == edition)]
    return [i for i in seg.tre_list if i.tre_tag == tre_tag and
            (edition is None or i.edition == edition) and
            i.row_section_number == sec_row and i.col_section_number == sec_col]

def _rsm_find_tre(seg, tre_tag, edition = None, sec_row = None, sec_col = None):
    t = _rsm_find_tre_list(seg, tre_tag, edition, sec_row, sec_col)
    if(len(t) > 1):
        raise RuntimeError("Found multiple TRE %s where only one expected" % tre_tag)
    return t[0] if len(t) == 1 else None;

def rsm_read_tre(seg):
    '''Read all the RSM TREs to fill in seg.rsm. If there are no RSM TREs,
    then set seg.rsm to None.'''
    # Currently only handle one RSM TRE set. We could extend this if needed.
    if _rsm_find_tre(seg, 'RSMIDA') is None:
       return
    t = _rsm_find_tre(seg, 'RSMIDA')
    r = Rsm(t.rsm_id)
    # Temp
    r.coordinate_converter = GeodeticConverter()
    edition = t.edition
    rsm_rp = None
    rsm_g = None
    if(_rsm_find_tre(seg, 'RSMPIA', edition) is not None):
        rsm_rp = _rsm_find_tre(seg, 'RSMPIA', edition).rsm_multi_section
        for i in range(rsm_rp.number_row_section):
            for j in range(rsm_rp.number_col_section):
                t = _rsm_find_tre(seg, 'RSMPCA', edition, sec_row = i + 1,
                                  sec_col = j + 1)
                if(t is None):
                    raise RuntimeError("Could find RSMPCA for (%d, %d)" % (i+1,j+1))
                rsm_rp.section(i, j, t.rsm_rational_polynomial)
    if(_rsm_find_tre(seg, 'RSMGIA', edition) is not None):
        rsm_g = _rsm_find_tre(seg, 'RSMGIA', edition).rsm_multi_section
        for i in range(rsm_g.number_row_section):
            for j in range(rsm_g.number_col_section):
                t = _rsm_find_tre(seg, 'RSMGGA', edition, sec_row = i + 1,
                                  sec_col = j + 1)
                if(t is None):
                    raise RuntimeError("Could find RSMGGA for (%d, %d)" % (i+1,j+1))
                rsm_g.section(i, j, t.rsm_grid)
    if(rsm_rp is None and _rsm_find_tre(seg, 'RSMPCA', edition) is not None):
        rsm_rp = _rsm_find_tre(seg, 'RSMPCA', edition).rsm_rational_polynomial
    if(rsm_g is None and _rsm_find_tre(seg, 'RSMGGA', edition) is not None):
        rsm_g = _rsm_find_tre(seg, 'RSMGGA', edition).rsm_grid
    if(rsm_rp is None and rsm_g is None):
        raise RuntimeError("Can't find either a RSMPCA or RSMGGA in TRE list")
    if(rsm_rp is not None):
        if(rsm_g is not None):
            r.rsm_base = RsmRpPlusGrid(rsm_rp, rsm_g)
        else:
            r.rsm_base = rsm_rp
    else:
        r.rsm_base = rsm_g
    seg.rsm = r
        
