from geocal_swig import (Rsm, RsmId, RsmMultiSection, RsmRationalPolynomial,
                         RsmGrid, RsmRpPlusGrid, RsmAdjustableParameterA,
                         have_msp_supported)
import re
try:
    from pynitf import (create_nitf_tre_structure, NitfSegmentHook, NitfFile,
                        TreRSMIDA, TreRSMGGA, TreRSMGIA, TreRSMPCA,
                        TreRSMPIA, TreRSMAPA, read_tre_data)
    have_pynitf = True
except ImportError:
    # Ok if we don't have pynitf, we just can't execute this code
    have_pynitf = False
import os

# Backdoor to suppress putting Rsm in place. Useful for example with
# nitfinfo where we might want to look at the raw nitf values.
suppress_rsm = False
if("NITF_USE_RAW_RSM" in os.environ):
    suppress_rsm = True
    
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
    if(TreRSMGGA.__doc__ is not None):
        hlp_rsmgga = TreRSMGGA.__doc__ + \
''' 
This TRE is mostly implemented by the RsmGrid available as
rsm_grid. This should be used to set the TRE values, and to use the
TRE values. This is handled mostly transparently, except that if you
update rsm_grid the raw fields in the TRE might not be updated. Call
update_raw_field() if you have modified rsm_grid and wish to access
the raw fields.'''
    else:
        hlp_rsmgga = None

    TreRSMGGA_geocal = create_nitf_tre_structure("TreRSMGGA",
                          TreRSMGGA._description,hlp=hlp_rsmgga,
                          tre_implementation_field="rsm_grid",
                          tre_implementation_class=RsmGrid)
    TreRSMGGA_geocal.row_section_number = TreRSMGGA.row_section_number
    TreRSMGGA_geocal.col_section_number = TreRSMGGA.col_section_number
    
    # This doesn't work in python 2.7, we can't write to the
    # doc. Rather than try to do something clever, just punt and
    # skip adding help for python 2.7. This works find with python 3
    if(TreRSMPCA.__doc__ is not None):
        hlp_rsmpca = TreRSMPCA.__doc__ + \
'''
This TRE is mostly implemented by the RsmRationalPolynomial available as
rsm_rational_polynomial. This should be used to set the TRE values, and to
use the TRE values. This is handled mostly transparently, except that if you
update rsm_rational_polynomial the raw fields in the TRE might not be
updated. Call update_raw_field() if you have modified rsm_rational_polynomial 
and wish to access the raw fields.
'''     
    else:
        hlp_rsmpca = None

    TreRSMPCA_geocal = create_nitf_tre_structure("TreRSMPCA",
                          TreRSMPCA._description,hlp=hlp_rsmpca,
                          tre_implementation_field="rsm_rational_polynomial",
                          tre_implementation_class=RsmRationalPolynomial)
    TreRSMPCA_geocal.row_section_number = TreRSMPCA.row_section_number
    TreRSMPCA_geocal.col_section_number = TreRSMPCA.col_section_number

    if(TreRSMGIA.__doc__ is not None):
        hlp_rsmgia = TreRSMGIA.__doc__ + \
'''
This TRE is mostly implemented by the RsmMultiSection available as
rsm_multi_section. This should be used to set the TRE values, and to
use the TRE values. This is handled mostly transparently, except that if you
update rsm_multi_section the raw fields in the TRE might not be
updated. Call update_raw_field() if you have modified rsm_multi_section
and wish to access the raw fields.
'''             
    else:
        hlp_rsmgia = None

    TreRSMGIA_geocal = create_nitf_tre_structure("TreRSMGIA",
                          TreRSMGIA._description,hlp=hlp_rsmgia,
                          tre_implementation_field="rsm_multi_section",
                          tre_implementation_class=RsmMultiSection)
    if(TreRSMPIA.__doc__ is not None):
        hlp_rsmpia = TreRSMPIA.__doc__ + \
'''
This TRE is mostly implemented by the RsmMultiSection available as
rsm_multi_section. This should be used to set the TRE values, and to
use the TRE values. This is handled mostly transparently, except that if you
update rsm_multi_section the raw fields in the TRE might not be
updated. Call update_raw_field() if you have modified rsm_multi_section
and wish to access the raw fields.
'''             
    else:
        hlp_rsmpia = None

    TreRSMPIA_geocal = create_nitf_tre_structure("TreRSMPIA",
                          TreRSMPIA._description,hlp=hlp_rsmpia,
                          tre_implementation_field="rsm_multi_section",
                          tre_implementation_class=RsmMultiSection)
    
    if(TreRSMIDA.__doc__ is not None):
        hlp_rsmida = TreRSMIDA.__doc__ + \
'''
This TRE is mostly implemented by the RsmId available as
rsm_id. This should be used to set the TRE values, and to
use the TRE values. This is handled mostly transparently, except that if you
update rsm_id the raw fields in the TRE might not be
updated. Call update_raw_field() if you have modified rsm_id
and wish to access the raw fields.
'''             
    else:
        hlp_rsmida = None

    TreRSMIDA_geocal = create_nitf_tre_structure("TreRSMIDA",
                          TreRSMIDA._description,hlp=hlp_rsmida,
                          tre_implementation_field="rsm_id",
                          tre_implementation_class=RsmId)

    if(TreRSMAPA.__doc__ is not None):
        hlp_rsmapa = TreRSMAPA.__doc__ + \
'''
This TRE is mostly implemented by the RsmAdjustableParameterA available as
rsm_adjustable_parameter. This should be used to set the TRE values, and to
use the TRE values. This is handled mostly transparently, except that if you
update rsm_rsm_adjustable_parameter the raw fields in the TRE might not be
updated. Call update_raw_field() if you have modified 
rsm_rsm_adjustable_parameter and wish to access the raw fields.
'''             
    else:
        hlp_rsmapa = None

    TreRSMAPA_geocal = create_nitf_tre_structure("TreRSMAPA",
                          TreRSMAPA._description,hlp=hlp_rsmapa,
                          tre_implementation_field="rsm_adjustable_parameter",
                          tre_implementation_class=RsmAdjustableParameterA)
    

# ---------------------------------------------------------
# This provides a higher level interface to the RSM TREs,
# using a C++ Rsm object
# to read and write the full set of TREs.
# ---------------------------------------------------------

if(have_pynitf and not suppress_rsm):
    class RsmImageSegementHook(NitfSegmentHook):
        def __init__(self):
            # Temporarily take out the direct and indirect error covariances
            #self.rsm_tre_tag_list =  ['RSMIDA', 'RSMPIA','RSMPCA', 'RSMDCA',
            #                          'RSMDCB',
            #                          'RSMAPA', 'RSMAPB', 'RSMECA', 'RSMECB',
            #                          'RSMGIA', 'RSMGGA']
            self.rsm_tre_tag_list =  ['RSMIDA', 'RSMPIA','RSMPCA', 
                                      'RSMAPA', 'RSMAPB', 
                                      'RSMGIA', 'RSMGGA']
        def init_hook(self, seg):
            seg.rsm = None
        def _rsm_add_rec(self, seg, v):
            if(isinstance(v, RsmRpPlusGrid)):
                self._rsm_add_rec(seg, v.rational_polynomial)
                self._rsm_add_rec(seg, v.correction_grid)
            elif(isinstance(v, RsmRationalPolynomial)):
                t = TreRSMPCA_geocal()
                t.rsm_rational_polynomial = v
                t.update_raw_field()
                seg.tre_list.append(t)
            elif(isinstance(v, RsmGrid)):
                t = TreRSMGGA_geocal()
                t.rsm_grid = v
                t.update_raw_field()
                seg.tre_list.append(t)
            elif(isinstance(v, RsmMultiSection)):
                if(isinstance(v.section(0,0), RsmRationalPolynomial)):
                    t = TreRSMPIA_geocal()
                    t.rsm_multi_section = v
                    t.update_raw_field()
                    seg.tre_list.append(t)
                elif(isinstance(v.section(0,0), RsmGrid)):
                    t = TreRSMGIA_geocal()
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
                    
        def prepare_tre_write_hook(self, seg, des_list, seg_list):
            '''Remove all the existing RSM TREs (if any), and add the TREs 
            to store seg.rsm'''
            # Remove any existing RSM Tres
            seg.tre_list = [t for t in seg.tre_list if t.tre_tag not in
                            self.rsm_tre_tag_list]
            # Currently only handle one RSM TRE set. We could extend this if needed.
            if(seg.rsm):
                t = TreRSMIDA_geocal()
                t.rsm_id = seg.rsm.rsm_id
                seg.tre_list.append(t)
                if(seg.rsm.rsm_adjustable_parameter):
                    t = TreRSMAPA_geocal()
                    t.rsm_adjustable_parameter = seg.rsm_adjustable_parameter
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
    
        def read_tre_hook(self, seg, des_list):
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
    
        def str_hook(self, seg, fh):
            '''Called at the start of NitfSegment.__str__'''
            if(seg.rsm):
                print(seg.rsm, file=fh)
            else:
                print("Rsm: None", file=fh)
    
        def str_tre_handle_hook(self, seg, tre, fh):
            '''Called before printing a TRE. If this returns true we assume
            that this class has handled the TRE printing. Otherwise, we
            call print on the tre'''
            if(tre.tre_tag in self.rsm_tre_tag_list):
                print("%s: See RSM above" % tre.tre_tag, file=fh)
                return True
            return False
            
    NitfFile.image_segment_hook_obj.append(RsmImageSegementHook())

    def tre_str_to_rsm(t):
        '''MSP dumps a RSM it generates out as a string that is a set of TREs.
        This function interprets this string.'''
        # Wrap up as an image segment, just to reuse code we already have. This
        # just provides a tre_list and a place to stick rsm.
        class FakeSeg:
            pass
        fseg = FakeSeg()
        fseg.tre_list = read_tre_data(re.sub(r'\n','',t).encode('utf8'))
        isegh = RsmImageSegementHook()
        isegh.read_tre_hook(fseg, None)
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
