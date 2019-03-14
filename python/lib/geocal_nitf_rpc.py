from __future__ import print_function
from geocal_swig import Rpc
from .tre_struct import *
try:
    from pynitf import(NitfSegmentHook, NitfFile, TreRPC00B, TreRPC00A)
    have_pynitf = True
except ImportError:
    # Ok if we don't have pynitf, we just can't execute this code
    have_pynitf = False

# ****************************************************************************
# As a convenience, we give a mapping from the underlying TREs and our
# geocal Rpc class. A NitfImageSegment will have a iseg.rpc that can be used
# to read and write a RPC. It will be set to None if the NitfImageSegment
# doesn't have a RPC.
# ****************************************************************************

if(have_pynitf):
    class RpcImageSegementHook(NitfSegmentHook):
        def __init__(self):
            self.rpc_tre_tag_list = ['RPC00A', 'RPC00B']

        def init_hook(self, seg):
            seg.rpc = None
            
        def prepare_tre_write_hook(self, seg, des_list, seg_list):
            '''Remove all the existing RPC TREs (if any), and add the TREs 
            to store seg.rpc'''
            seg.tre_list = [t for t in seg.tre_list if t.tre_tag not in
                            self.rpc_tre_tag_list]
            if(seg.rpc):
                # Leverage off the code in tre_struct. This was geared more
                # around GDAL files, and we may decide to have it go away.
                # But for now, leave the code in that one place and just add
                # a "new" interface to it so it looks like we do with the the
                # Rsm
                if(seg.rpc.rpc_type == Rpc.RPC_A):
                    tre = TreRPC00A()
                else:
                    tre = TreRPC00B()
                tre.rpc = seg.rpc
                seg.tre_list.append(tre)
                
        def read_tre_hook(self, seg, des_list):
            '''Read all the RPC TREs to fill in seg.rpc. If there are 
            no RPC TREs, then set seg.rpc to None.'''
            # Currently only handle one RPC TRE.
            t1 = seg.find_one_tre('RPC00A')
            t2 = seg.find_one_tre('RPC00B')
            if(t1 and t2):
                raise RuntimeError("Found more than one RPC TRE")
            t = t1 or t2
            if(t):
                seg.rpc = t.rpc
            else:
                seg.rpc = None
        def str_hook(self, seg, fh):
            '''Called at the start of NitfSegment.__str__'''
            if(seg.rpc):
                print(seg.rpc, file=fh)
            else:
                print("Rpc: None", file=fh)
    
        def str_tre_handle_hook(self, seg, tre, fh):
            '''Called before printing a TRE. If this returns true we assume
            that this class has handled the TRE printing. Otherwise, we
            call print on the tre'''
            if(tre.tre_tag in self.rpc_tre_tag_list):
                print("%s: See RPC above" % tre.tre_tag, file=fh)
                return True
            return False
            
    NitfFile.image_segment_hook_obj.append(RpcImageSegementHook())

__all__ = []
    
