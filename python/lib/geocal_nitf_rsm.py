from geocal_swig import (Rsm, RsmId, RsmMultiSection, RsmRationalPolynomial,
                         RsmGrid, RsmRpPlusGrid, RsmAdjustableParameterA,
                         RsmAdjustableParameterB,
                         RsmDirectCovarianceA,
                         RsmIndirectCovarianceB,
                         RsmBSubgroup,
                         have_msp_supported)
import re
try:
    from pynitf import (NitfSegmentHook,
                        NitfSegmentHookSet,
                        TreRSMIDA, TreRSMGGA, TreRSMGIA, TreRSMPCA,
                        TreRSMPIA, TreRSMAPA, TreRSMDCA,
                        TreRSMAPB, TreRSMECB, read_tre_data)
    have_pynitf = True
except ImportError:
    # Ok if we don't have pynitf, we just can't execute this code
    have_pynitf = False
import os

# Backdoor to suppress putting Rsm in place. Useful mostly for testing.
suppress_rsm = False

# ****************************************************************************
# Note the RSM is a bit complicated, because it spans multiple TREs. 
# We handle the RSM as a special field, you can access it as iseg.rsm, or
# write to it by setting iseg.rsm to the Rsm. The lower level TREs are also
# available as any other TRE, but you wouldn't normally read or write these
# directly, instead go through self.rsm.  We have iseg.rsm set to None if the
# NitfImageSegment doesn't have an RSM.
#
# This code is a good example of extending the nitf module to support
# specialized objects.
# ****************************************************************************

# ---------------------------------------------------------
# Override various TREs to use the geocal objects instead
# ---------------------------------------------------------
if(have_pynitf and not suppress_rsm):
    TreRSMGGA.tre_implementation_field = "rsm_grid"
    TreRSMGGA.tre_implementation_class = RsmGrid
    TreRSMPCA.tre_implementation_field = "rsm_rational_polynomial"
    TreRSMPCA.tre_implementation_class = RsmRationalPolynomial
    TreRSMGIA.tre_implementation_field = "rsm_multi_section"
    TreRSMGIA.tre_implementation_class = RsmMultiSection
    TreRSMPIA.tre_implementation_field = "rsm_multi_section"
    TreRSMPIA.tre_implementation_class = RsmMultiSection
    TreRSMIDA.tre_implementation_field = "rsm_id"
    TreRSMIDA.tre_implementation_class = RsmId
    TreRSMAPA.tre_implementation_field = "rsm_adjustable_parameter"
    TreRSMAPA.tre_implementation_class = RsmAdjustableParameterA
    TreRSMAPB.tre_implementation_field = "rsm_adjustable_parameter"
    TreRSMAPB.tre_implementation_class = RsmAdjustableParameterB
    TreRSMDCA.tre_implementation_field = "rsm_direct_covariance"
    TreRSMDCA.tre_implementation_class = RsmDirectCovarianceA
    TreRSMECB.tre_implementation_field = "rsm_indirect_covariance"
    TreRSMECB.tre_implementation_class = RsmIndirectCovarianceB
    

# ---------------------------------------------------------
# This provides a higher level interface to the RSM TREs,
# using a C++ Rsm object
# to read and write the full set of TREs.
# ---------------------------------------------------------

if(have_pynitf and not suppress_rsm):
    class RsmImageSegmentHook(NitfSegmentHook):
        def __init__(self):
            # Temporarily take out the direct and indirect error covariances
            #self.rsm_tre_tag_list =  ['RSMIDA', 'RSMPIA','RSMPCA', 'RSMDCA',
            #                          'RSMDCB',
            #                          'RSMAPA', 'RSMAPB', 'RSMECA', 'RSMECB',
            #                          'RSMGIA', 'RSMGGA']
            self.rsm_tre_tag_list =  ['RSMIDA', 'RSMPIA','RSMPCA', 'RSMDCA',
                                      "RSMECB",
                                      'RSMAPA', 'RSMAPB', 
                                      'RSMGIA', 'RSMGGA']
        def after_init_hook(self, seg, nitf_file):
            seg.rsm = None
        def after_append_hook(self, seg, nitf_file):
            if(not hasattr(seg, "rsm")):
                seg.rsm = None
        def _rsm_add_rec(self, seg, v):
            if(isinstance(v, RsmRpPlusGrid)):
                self._rsm_add_rec(seg, v.rational_polynomial)
                self._rsm_add_rec(seg, v.correction_grid)
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
                        self._rsm_add_rec(seg, v.section(i, j))
            else:
                raise RuntimeError("Unknown Rsm element " + str(v))
                    
        def before_write_hook(self, seg, nitf_file):
            '''Remove all the existing RSM TREs (if any), and add the TREs 
            to store seg.rsm'''
            # Remove any existing RSM Tres
            seg.tre_list = [t for t in seg.tre_list if t.tre_tag not in
                            self.rsm_tre_tag_list]
            # Currently only handle one RSM TRE set. We could extend this if needed.
            if(seg.rsm):
                t = TreRSMIDA()
                t.rsm_id = seg.rsm.rsm_id
                seg.tre_list.append(t)
                if(seg.rsm.rsm_adjustable_parameter):
                    if(isinstance(seg.rsm.rsm_adjustable_parameter,
                                  RsmAdjustableParameterA)):
                        t = TreRSMAPA()
                    else:
                        t = TreRSMAPB()
                    t.rsm_adjustable_parameter = seg.rsm.rsm_adjustable_parameter
                    seg.tre_list.append(t)
                if(seg.rsm.rsm_direct_covariance):
                    t = TreRSMDCA()
                    t.rsm_direct_covariance = seg.rsm.rsm_direct_covariance
                    seg.tre_list.append(t)
                if(seg.rsm.rsm_indirect_covariance):
                    t = TreRSMECB()
                    t.rsm_indirect_covariance = seg.rsm.rsm_indirect_covariance
                    seg.tre_list.append(t)
                self._rsm_add_rec(seg, seg.rsm.rsm_base)
    
        def _rsm_find_tre_list(self, seg, tre_tag, edition,
                               sec_row = None, sec_col = None):
            if(sec_row is None):
                return [i for i in seg.tre_list if i.tre_tag == tre_tag and
                        (edition is None or i.edition == edition)]
            return [i for i in seg.tre_list if i.tre_tag == tre_tag and
                    (edition is None or i.edition == edition) and
                    i.row_section_number == sec_row and
                    i.col_section_number == sec_col]
    
        def _rsm_find_tre(self, seg, tre_tag, edition = None, sec_row = None,
                          sec_col = None):
            t = self._rsm_find_tre_list(seg, tre_tag, edition, sec_row, sec_col)
            if(len(t) > 1):
                raise RuntimeError("Found multiple TRE %s where only one expected" % tre_tag)
            return t[0] if len(t) == 1 else None;
    
        def after_read_hook(self, seg, nitf_file):
            '''Read all the RSM TREs to fill in seg.rsm. If there are no RSM TREs,
            then set seg.rsm to None.'''
            # Currently only handle one RSM TRE set. We could extend this if needed.
            if self._rsm_find_tre(seg, 'RSMIDA') is None:
                return
            t = self._rsm_find_tre(seg, 'RSMIDA')
            r = Rsm(t.rsm_id)
            edition = t.edition
            t = self._rsm_find_tre(seg, 'RSMAPA')
            if(t is not None):
                r.rsm_adjustable_parameter = t.rsm_adjustable_parameter
            t = self._rsm_find_tre(seg, 'RSMAPB')
            if(t is not None):
                r.rsm_adjustable_parameter = t.rsm_adjustable_parameter
            t = self._rsm_find_tre(seg, 'RSMDCA')
            if(t is not None):
                r.rsm_direct_covariance = t.rsm_direct_covariance
            t = self._rsm_find_tre(seg, 'RSMECB')
            if(t is not None):
                r.rsm_indirect_covariance = t.rsm_indirect_covariance
            rsm_rp = None
            rsm_g = None
            if(self._rsm_find_tre(seg, 'RSMPIA', edition) is not None):
                rsm_rp = self._rsm_find_tre(seg, 'RSMPIA',
                                            edition).rsm_multi_section
                for i in range(rsm_rp.number_row_section):
                    for j in range(rsm_rp.number_col_section):
                        t = self._rsm_find_tre(seg, 'RSMPCA', edition,
                                               sec_row = i + 1, sec_col = j + 1)
                        if(t is None):
                            raise RuntimeError("Could find RSMPCA for (%d, %d)" % (i+1,j+1))
                        rsm_rp.section(i, j, t.rsm_rational_polynomial)
            if(self._rsm_find_tre(seg, 'RSMGIA', edition) is not None):
                rsm_g = self._rsm_find_tre(seg, 'RSMGIA', edition).rsm_multi_section
                for i in range(rsm_g.number_row_section):
                    for j in range(rsm_g.number_col_section):
                        t = self._rsm_find_tre(seg, 'RSMGGA', edition,
                                               sec_row = i + 1,
                                               sec_col = j + 1)
                        if(t is None):
                            raise RuntimeError("Could find RSMGGA for (%d, %d)" % (i+1,j+1))
                        rsm_g.section(i, j, t.rsm_grid)
            if(rsm_rp is None and self._rsm_find_tre(seg, 'RSMPCA', edition)
               is not None):
                rsm_rp = self._rsm_find_tre(seg, 'RSMPCA',
                                            edition).rsm_rational_polynomial
            if(rsm_g is None and self._rsm_find_tre(seg, 'RSMGGA', edition)
               is not None):
                rsm_g = self._rsm_find_tre(seg, 'RSMGGA', edition).rsm_grid
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
    
        def before_str_hook(self, seg, nitf_file, fh):
            '''Called at the start of NitfSegment.__str__'''
            if(seg.rsm):
                print(seg.rsm, file=fh)
            else:
                print("Rsm: None", file=fh)
    
        def before_str_tre_hook(self, seg, tre, nitf_file, fh):
            '''Called before printing a TRE. If this returns true we assume
            that this class has handled the TRE printing. Otherwise, we
            call print on the tre'''
            if(tre.tre_tag in self.rsm_tre_tag_list):
                print("%s: See RSM above" % tre.tre_tag, file=fh)
                return True
            return False
            
    NitfSegmentHookSet.add_default_hook(RsmImageSegmentHook())

    def tre_str_to_rsm(t):
        '''MSP dumps a RSM it generates out as a string that is a set of TREs.
        This function interprets this string.'''
        # Wrap up as an image segment, just to reuse code we already have. This
        # just provides a tre_list and a place to stick rsm.
        class FakeSeg:
            pass
        fseg = FakeSeg()
        fseg.tre_list = read_tre_data(re.sub(r'\n','',t).encode('utf8'))
        isegh = RsmImageSegmentHook()
        isegh.after_read_hook(fseg, None)
        return fseg.rsm

    if(have_msp_supported()):
        from geocal_swig import IgcMsp

        def _generate_rsm(self, report = "", rsm_config = ""):
            t = self.generate_rsm_tre(report, rsm_config)
            t = re.split(r'^RSM_TRE_DATA$', t, flags=re.MULTILINE)[1]
            return tre_str_to_rsm(t)

        IgcMsp.generate_rsm = _generate_rsm
            

if(have_pynitf and not suppress_rsm):
    __all__ = ["tre_str_to_rsm", ]
else:
    __all__ = []
