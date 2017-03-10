from .nitf_tre import *
import copy
import re

def _tre_rpc_coeff_format(v):
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

hlp = '''This is the RPC TRE.

The field names can be pretty cryptic, but are documented in detail in 
the NITF TRE documentation (STDI-0002 V4.0, available at 
http://www.gwg.nga.mil/ntb/baseline/docs/stdi0002).

There is a table in the main body on page vii that gives the a pointer for 
where in the document a particular TRE is defined.

RPC00B is documented at E-62.
'''
desc = ["RPC00B",
        ["success", 1, int],
        ["err_bias", 7, float, {"frmt" : "%07.2lf"}],
        ["err_rand", 7, float, {"frmt" : "%07.2lf"}],
        ["line_off", 6, int],
        ["samp_off", 5, int],
        ["lat_off", 8, float, {"frmt" : "%+08.4lf"}],
        ["long_off", 9, float, {"frmt" : "%+09.4lf"}],
        ["height_off", 5, int],
        ["line_scale", 6, int],
        ["samp_scale", 5, int],
        ["lat_scale", 8, float, {"frmt" : "%+08.4lf"}],
        ["long_scale", 9, float, {"frmt": "%+09.4lf"}],
        ["height_scale", 5, int, {"frmt": "%+05d"}],
        [["loop", "20"],
         ["line_num_coeff", 12, float, {"frmt": _tre_rpc_coeff_format}]],
        [["loop", "20"],
         ["line_den_coeff", 12, float, {"frmt": _tre_rpc_coeff_format}]],
        [["loop", "20"],
         ["samp_num_coeff", 12, float, {"frmt": _tre_rpc_coeff_format}]],
        [["loop", "20"],
         ["samp_den_coeff", 12, float, {"frmt": _tre_rpc_coeff_format}]],
]

TreRPC00B = create_nitf_tre_structure("TreRPC00B",desc,hlp=hlp)

# RCP00A is the same format at the OOB, we just have the parameters in
# a different order (as already handled by the Rpc class). But this is
# a separate TRE

desc2 = copy.deepcopy(desc)
desc2[0] = "RPC00A"

TreRPC00A = create_nitf_tre_structure("TreRPC00A",desc2,hlp=hlp)
