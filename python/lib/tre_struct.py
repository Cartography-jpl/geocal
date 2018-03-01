from __future__ import print_function
from builtins import range
from builtins import object
from geocal_swig import *
try:
    from pynitf import TreUSE00A, TreRPC00B, TreRPC00A
    have_pynitf = True
except ImportError:
    have_pynitf = False
import re
import six
import sys

# This bit of code may go away. It was an older way of handling TREs before
# we wrote geocal.nitf. It attaches the TRE to existing GdalRasterImage class.
# For now keep this, it isn't clear what will break by removing this. This
# code gets used in gdal_to_nitf and nitf_to_vicar, perhaps we will rewrite
# to use the full geocal.nitf library in the future. But for now, leave this
# in place. For new code, you most likely don't want to use this.

class _GdalRasterImageHelper(object):
    def __init__(self, tre_name, tre_class, nitf_literal=False):
        self.tre_name = tre_name
        self.tre_class = tre_class
        self.nitf_literal = nitf_literal
    def __get__(self, d):
        t = self.tre_class()
        t.read_from_tre_bytes(d["TRE", self.tre_name].encode('utf-8'),
                              self.nitf_literal)
        return t
    def exists(self, d):
        return d.has_metadata(self.tre_name, "TRE")
    def __set__(self, d, val):
        if sys.version_info > (3,):
            d["TRE", self.tre_name] = val.tre_bytes().decode('utf-8')
        else:
            d["TRE", self.tre_name] = val.tre_bytes()

def _add_tre_to_gdal(tre, raster_name, help_desc, nitf_literal=False):
    h = _GdalRasterImageHelper(tre.tre_tag, tre, nitf_literal=nitf_literal)
    setattr(GdalRasterImage, raster_name, 
            property(h.__get__, h.__set__, None, help_desc))
    setattr(GdalRasterImage, "has_" + raster_name, 
            property(h.exists, None, None, help_desc))

if(have_pynitf):
    _add_tre_to_gdal(TreUSE00A, "use00a", "USEOOA metadata")    

def _use00a_from_gdal(self, f):
    '''Fill in TRE based on GDAL parameters. These are the metadata field
    starting with "NITF_USE00A", for example "NITF_USE00A_SUN_EL"'''
    for field in self.field_map.keys():
        # The use00a stuff can appear either as NITF_USE00A_<blah>
        # or as NITF_<blah>. Support both ways.
        # We might not have all the tags in an particular VICAR file.
        # If not, then default to a blank value.
        if(f.has_metadata("NITF_USE00A_" + field.upper())):
            self.field_map[field].set(self,(),self.field_map[field].ty(f["NITF_USE00A_" + field.upper()]))
        elif(f.has_metadata("NITF_" + field.upper())):
            self.field_map[field].set(self,(),self.field_map[field].ty(f["NITF_" + field.upper()]))

def _use00a_to_vicar(self, f):
    '''Fill in VICAR metadata based on TRE. These are the metadata field
    starting with "NITF_USE00A_"'''
    for field in self.field_map.keys():
        if sys.version_info > (3,):
            f["GEOTIFF", "NITF_" + field.upper()] = self.field_map[field].bytes(self).decode('utf-8')
        else:
            f["GEOTIFF", "NITF_" + field.upper()] = self.field_map[field].bytes(self)

def _use00a_to_gdal(self, f):
    '''Fill in VICAR metadata based on TRE. These are the metadata field
    starting with "NITF_USE00A_"'''
    for field in self.field_map.keys():
        if sys.version_info > (3,):
            f["TRE_NITF_" + field.upper()] =  self.field_map[field].bytes(self).decode('utf-8')
        else:
            f["TRE_NITF_" + field.upper()] =  self.field_map[field].bytes(self)

if(have_pynitf):
    TreUSE00A.from_gdal = _use00a_from_gdal
    TreUSE00A.to_vicar = _use00a_to_vicar
    TreUSE00A.to_gdal = _use00a_to_gdal

def tre_use00a_to_gdal(fin, fout):
    '''Function to copy use00a structure from TRE in input to output vicar file.'''
    if(fin.has_use00a):
        fin.use00a.to_gdal(fout)

def tre_use00a_pretty_print(f):
    '''Pretty print of TRE'''
    if(f.has_use00a):
        print("Use00A tre:")
        s = re.sub(r"^", "  ",str(f.use00a),flags=re.M)
        print(s)
    
def tre_use00a(fin, fout, creation_option):
    '''Function that copies the use00a structure from fin to a TRE in the
    output file. Both of these should be GdalRasterImage objects. Note 
    that some of the data needs to be passed as creation options (pretty much
    anything in the file or image header). We fill in an array creation_option
    with anything we need to add to this.'''
    if("NITF_USE00A_ANGLE_TO_NORTH" in fin.raster_image(0) or
       "NITF_ANGLE_TO_NORTH" in fin.raster_image(0)):
        tre = TreUSE00A()
        tre.from_gdal(fin.raster_image(0))
        fout.use00a = tre
if(have_pynitf):
    _add_tre_to_gdal(TreRPC00A, "rpc00a", "RPC A metadata")    
    _add_tre_to_gdal(TreRPC00B, "rpc00b", "RPC B metadata")    
        
def tre_is_rpc_a(self):
    return True

def tre_isnot_rpc_a(self):
    return False

if(have_pynitf):
    setattr(TreRPC00B, "is_rpc_a", 
            property(tre_isnot_rpc_a, None, None, 
                     "Return true if this is type A"))

    setattr(TreRPC00A, "is_rpc_a", 
            property(tre_is_rpc_a, None, None, 
                     "Return true if this is type A"))

def tre_get_rpc(self):
    '''Get a Rpc object directly from a TRE object'''
    rpc = Rpc()
    rpc.rpc_type = Rpc.RPC_A if(self.is_rpc_a) else Rpc.RPC_B
    rpc.error_bias = self.err_bias
    rpc.error_random = self.err_rand
    rpc.height_offset = self.height_off
    rpc.height_scale = self.height_scale
    rpc.latitude_offset = self.lat_off
    rpc.latitude_scale = self.lat_scale
    rpc.longitude_offset = self.long_off
    rpc.longitude_scale = self.long_scale
    rpc.line_offset = self.line_off
    rpc.line_scale = self.line_scale
    rpc.sample_offset = self.samp_off
    rpc.sample_scale = self.samp_scale
    rpc.fit_line_numerator = [False] * 20
    rpc.fit_sample_numerator = [False] * 20
    rpc.line_denominator = list(self.line_den_coeff)
    rpc.line_numerator = list(self.line_num_coeff)
    rpc.sample_denominator = list(self.samp_den_coeff)
    rpc.sample_numerator = list(self.samp_num_coeff)
    return rpc

def tre_set_from_rpc(self, rpc):
    '''Set a TRE object object directly from a geocal RPC object'''
    if((self.is_rpc_a and not rpc.rpc_type == Rpc.RPC_A)):
        raise RuntimeError("Attempt to set a TRE of RPC type A with a RPC of type B")
    if(not self.is_rpc_a and not rpc.rpc_type == Rpc.RPC_B):
        raise RuntimeError("Attempt to set a TRE of RPC type B with a RPC of type A")
    self.success = 1
    self.err_bias = rpc.error_bias
    self.err_rand = rpc.error_random
    self.height_off = rpc.height_offset
    self.height_scale = rpc.height_scale 
    self.lat_off = rpc.latitude_offset 
    self.lat_scale = rpc.latitude_scale 
    self.long_off = rpc.longitude_offset 
    self.long_scale = rpc.longitude_scale 
    self.line_off = rpc.line_offset 
    self.line_scale = rpc.line_scale 
    self.samp_off = rpc.sample_offset 
    self.samp_scale = rpc.sample_scale 
    for i in range(20):
        self.line_den_coeff[i] = rpc.line_denominator[i]
        self.line_num_coeff[i] = rpc.line_numerator[i]
        self.samp_den_coeff[i] = rpc.sample_denominator[i]
        self.samp_num_coeff[i] = rpc.sample_numerator[i]

if(have_pynitf):
    setattr(TreRPC00B, "rpc", 
            property(tre_get_rpc, tre_set_from_rpc, None, 
                     "TRE RPC structure as a GeoCal RPC object"))

    setattr(TreRPC00A, "rpc", 
            property(tre_get_rpc, tre_set_from_rpc, None, 
                     "TRE RPC structure as a GeoCal RPC object"))

if(have_pynitf):
    __all__ = ["tre_use00a_to_gdal", "tre_use00a_pretty_print", "tre_use00a",
               "TreRPC00A", "TreRPC00B", "TreUSE00A"]
else:
    __all__ = ["tre_use00a_to_gdal", "tre_use00a_pretty_print", "tre_use00a"]
