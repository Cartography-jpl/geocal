from geocal import *
import re

class _GdalRasterImageHelper(object):
    def __init__(self, tre_name, tre_class):
        self.tre_name = tre_name
        self.tre_class = tre_class
    def __get__(self, d):
        return self.tre_class(d["TRE", self.tre_name])
    def __set__(self, d, val):
        d["TRE", self.tre_name] = val.string_value
    
class _TREVal(object):
    def __init__(self, sl, ty, frmt):
        '''This is an internally used object that handled a TRE value.
        We take a slice 'sl' which gives the area of the string we are part
        of. The type 'ty' is given to convert to. Finally we take the
        sprintf format 'frmt' string to use in setting the value.'''
        self.sl = sl
        self.ty = ty
        self.size = self.sl.stop - self.sl.start
        # This forces the string to be exactly the given size, padding with
        # spaces if needed.
        self.fstring = "{:%ds}" % self.size
        self.frmt = frmt
    def __get__(self, d, cls):
        if(d._data[self.sl] == ' ' * self.size):
            return None
        return self.ty(d._data[self.sl].rstrip())
    def __set__(self, d, v):
        if(isinstance(self.frmt, str)):
            t = self.fstring.format(self.frmt % v)
        else:
            t = self.fstring.format(self.frmt(v))
        if(len(t) > self.size):
            raise RuntimeError("Formatting error. String '%s' is too long" % t)
        d._data[self.sl] = t[0:self.size]

class _TREStruct(object):
    def __init__(self, value = None):
        '''Create a TRE structure. If there is an initial value (e.g.,
        this was read from an existing TRE) then the values are
        set. Otherwise all the strings are initialized to the empty
        string and numbers are initialized to 0.'''
        self._data = bytearray()
        for i in range(self.size): self._data.append(' ')
        if(value):
            self.string_value = value
    def __str__(self):
        '''Text description of structure, e.g., something you can print
        out.'''
        res = []
        maxlen = max([len(f) for f in self.field_list])
        for f in self.field_list:
            res.append(f.ljust(maxlen) + ": " + str(getattr(self, f)))
        return "\n".join(res)

    @property
    def string_value(self):
        '''The ASCII string representing the TRE. This is the raw data stored
        in the NITF file, this isn't something you would normally print
        out.'''
        return str(self._data)
    @string_value.setter
    def string_value(self, value):
        if(len(value) != len(self._data)):
            raise RuntimeError("Length of value must be exactly %d" %
                               len(self._data))
        self._data[:] = value

def create_tre(name, tre_name, raster_name, help_desc, description):
    '''This create a class to handle a specific NITF TRE (a extension of
    the file header). These are fixed field strings. We handle converting to
    and from the raw TRE string used by such classes as GdalRasterImage.

    In addition to creating this class, we also extend GdalRasterImage to
    add a function "raster_name" that read the metadata "tre_name" and
    return this class.

    This function takes a description of the TRE. This description
    describes each of field names, the length of the field, the type
    of the field, and the format string used to create the field.  In
    some cases, there may be reserved space in a TRE. This can be
    given a field name of None. This is given as an array. For
    example, the first part of the USEOOA is:

       [["angle_to_north", 3, int, "%03d"],
        ["mean_gsd", 5, float, "%05.1lf"],
        [None, 1, str, "%s"],
        ["dynamic_range", 5, int, "%05d"],
       ...]

    The class that is created, has properties for each of the fields allowing
    it to be read and written to. 

    Note that there can be certain odd formats (e.g., in the RPC) that
    can't be captured by a format string. In those cases, you can supply
    a function to call for the formatting instead.
    '''
    d = dict()
    start = 0
    d["field_list"] = []
    for field_name, len, ty, frmt in description:
        if(field_name):
            d[field_name] = _TREVal(slice(start, start + len), ty, frmt)
            d[field_name + "_string"] = _TREVal(slice(start, start + len), str,
                                                frmt)
            d["field_list"].append(field_name)
        start += len
    d['size'] = start
    res = type(name, (_TREStruct,), d)
    # This is a Descriptor, but for whatever reason when we use a general
    # Descriptor the python help function no longer works (in python 2.7.1
    # at least). Easy enough to wrap this as a property, which is handled
    # correctly by python help
    h = _GdalRasterImageHelper(tre_name, res)
    setattr(GdalRasterImage, raster_name, 
            property(h.__get__, h.__set__, None, help_desc))
    return res

TreUSE00A = create_tre("TreUSE00A",
                       "USE00A", "use00a", "USEOOA metadata",
           [["angle_to_north", 3, int, "%03d"],
            ["mean_gsd", 5, float, "%05.1lf"],
            [None, 1, str, "%s"],
            ["dynamic_range", 5, int, "%05d"],
            [None, 3, str, "%s"],
            [None, 1, str, "%s"],
            [None, 3, str, "%s"],
            ["obl_ang", 5, float, "%05.2lf"],
            ["roll_angle", 6, float, "%+04.2lf"],
            [None, 12, str, "%s"],
            [None, 15, str, "%s"],
            [None, 4, str, "%s"],
            [None, 1, str, "%s"],
            [None, 3, str, "%s"],
            [None, 1, str, "%s"],
            [None, 1, str, "%s"],
            ["n_ref", 2, int, "%02d"],
            ["rev_num", 5, int, "%05d"],
            ["n_seg", 3, int, "%03d"],
            ["max_lp_seg", 6, int, "%06d"],
            [None, 6, str, "%s"],
            [None, 6, str, "%s"],
            ["sun_el", 5, float, "%+05.1lf"],
            ["sun_az", 5, float, "%05.1lf"],
            ]
           )

def tre_rpc_coeff_format(v):
    '''Convert to string for RPC coefficient for a TRE'''
    # If too small to represent, replace with zero
    if(abs(v) < 1e-9):
        return '+0.000000E+0'
    # Error if too big
    if(abs(v) >= 1e10):
        raise RuntimeError("Value is to large to represent in a TRE")
    t = "%+010.6lE" % v
    # Replace 2 digit exponent with 1 digit
    t = re.sub(r'E([+-])0', r'E\1', t)
    return t

TreRPC00B = create_tre("TreTPC00B",
                       "RPC00B", "rpc00b", "RPC B metadata",
           [["success", 1, int, "%01d"],
            ["err_bias", 7, float, "%07.2lf"],
            ["err_rand", 7, float, "%07.2lf"],
            ["line_off", 6, int, "%06d"],
            ["samp_off", 5, int, "%05d"],
            ["lat_off", 8, float, "%+08.4lf"],
            ["long_off", 9, float, "%+09.4lf"],
            ["height_off", 5, int, "%+05d"],
            ["line_scale", 6, int, "%06d"],
            ["samp_scale", 5, int, "%05d"],
            ["lat_scale", 8, float, "%+08.4lf"],
            ["long_scale", 9, float, "%+09.4lf"],
            ["height_scale", 5, int, "%+05d"],
            ["line_num_coeff_1", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_2", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_3", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_4", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_5", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_6", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_7", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_8", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_9", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_10", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_11", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_12", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_13", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_14", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_15", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_16", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_17", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_18", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_19", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_20", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_1", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_2", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_3", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_4", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_5", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_6", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_7", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_8", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_9", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_10", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_11", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_12", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_13", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_14", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_15", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_16", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_17", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_18", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_19", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_20", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_1", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_2", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_3", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_4", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_5", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_6", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_7", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_8", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_9", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_10", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_11", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_12", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_13", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_14", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_15", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_16", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_17", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_18", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_19", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_20", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_1", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_2", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_3", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_4", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_5", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_6", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_7", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_8", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_9", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_10", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_11", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_12", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_13", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_14", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_15", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_16", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_17", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_18", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_19", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_20", 12, float, tre_rpc_coeff_format],
            ]
           )

# RCP00A is the same format at the OOB, we just have the parameters in
# a different order (as already handled by the Rpc class). But this is
# a separate TRE
TreRPC00A = create_tre("TreTPC00A",
                       "RPC00A", "rpc00a", "RPC A metadata",
           [["success", 1, int, "%01d"],
            ["err_bias", 7, float, "%07.2lf"],
            ["err_rand", 7, float, "%07.2lf"],
            ["line_off", 6, int, "%06d"],
            ["sample_off", 5, int, "%05d"],
            ["lat_off", 8, float, "%+08.4lf"],
            ["long_off", 9, float, "%+09.4lf"],
            ["height_off", 5, int, "%+05d"],
            ["line_scale", 6, int, "%06d"],
            ["samp_scale", 5, int, "%05d"],
            ["lat_scale", 8, float, "%+08.4lf"],
            ["long_scale", 9, float, "%+09.4lf"],
            ["height_scale", 5, int, "%+05d"],
            ["line_num_coeff_1", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_2", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_3", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_4", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_5", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_6", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_7", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_8", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_9", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_10", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_11", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_12", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_13", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_14", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_15", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_16", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_17", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_18", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_19", 12, float, tre_rpc_coeff_format],
            ["line_num_coeff_20", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_1", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_2", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_3", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_4", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_5", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_6", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_7", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_8", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_9", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_10", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_11", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_12", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_13", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_14", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_15", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_16", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_17", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_18", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_19", 12, float, tre_rpc_coeff_format],
            ["line_den_coeff_20", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_1", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_2", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_3", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_4", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_5", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_6", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_7", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_8", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_9", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_10", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_11", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_12", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_13", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_14", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_15", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_16", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_17", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_18", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_19", 12, float, tre_rpc_coeff_format],
            ["samp_num_coeff_20", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_1", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_2", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_3", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_4", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_5", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_6", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_7", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_8", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_9", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_10", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_11", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_12", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_13", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_14", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_15", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_16", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_17", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_18", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_19", 12, float, tre_rpc_coeff_format],
            ["samp_den_coeff_20", 12, float, tre_rpc_coeff_format],
            ]
           )

def tre_is_rpc_a(self):
    return True

def tre_isnot_rpc_a(self):
    return False

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
    rpc.fit_line_numerator.set([False] * 20)
    rpc.fit_sample_numerator.set([False] * 20)
    for i in range(20):
        rpc.line_denominator[i] = getattr(self, "line_den_coeff_%d" % (i+1))
        rpc.line_numerator[i] = getattr(self, "line_num_coeff_%d" % (i+1))
        rpc.sample_denominator[i] = getattr(self, "samp_den_coeff_%d" % (i+1))
        rpc.sample_numerator[i] = getattr(self, "samp_num_coeff_%d" % (i+1))
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
        setattr(self, "line_den_coeff_%d" % (i+1), rpc.line_denominator[i])
        setattr(self, "line_num_coeff_%d" % (i+1), rpc.line_numerator[i])
        setattr(self, "samp_den_coeff_%d" % (i+1), rpc.sample_denominator[i])
        setattr(self, "samp_num_coeff_%d" % (i+1), rpc.sample_numerator[i])

setattr(TreRPC00B, "rpc", 
        property(tre_get_rpc, tre_set_from_rpc, None, 
                 "TRE RPC structure as a GeoCal RPC object"))

setattr(TreRPC00A, "rpc", 
        property(tre_get_rpc, tre_set_from_rpc, None, 
                 "TRE RPC structure as a GeoCal RPC object"))
