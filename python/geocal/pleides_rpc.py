from xml.etree.ElementTree import parse

from geocal_swig import Rpc


def pleides_rpc(fname):
    """This read a Pleides XML file to get the RPC. Note that the RPC
    for Pleides doesn't follow the normal NITF constraints (e.g.,
    line_offset being an int). This should only matter if we actually try
    to save this as a NITF.
    """
    rpc = Rpc()
    doc = parse(fname)

    # The data is stored in a few different places in the XML file. We go
    # through and grab each piece.

    rpc.rpc_type = Rpc.RPC_B
    rpc.error_bias = 0
    rpc.error_random = 0
    t = doc.find("Rational_Function_Model/Global_RFM/RFM_Validity")
    rpc.line_offset = float(t.findtext("LINE_OFF"))
    rpc.line_scale = float(t.findtext("LINE_SCALE"))
    rpc.sample_offset = float(t.findtext("SAMP_OFF"))
    rpc.sample_scale = float(t.findtext("SAMP_SCALE"))
    rpc.latitude_offset = float(t.findtext("LAT_OFF"))
    rpc.latitude_scale = float(t.findtext("LAT_SCALE"))
    rpc.longitude_offset = float(t.findtext("LONG_OFF"))
    rpc.longitude_scale = float(t.findtext("LONG_SCALE"))
    rpc.height_offset = float(t.findtext("HEIGHT_OFF"))
    rpc.height_scale = float(t.findtext("HEIGHT_SCALE"))
    t = doc.find("Rational_Function_Model/Global_RFM/Inverse_Model")
    line_num = []
    line_den = []
    samp_num = []
    samp_den = []
    for i in range(20):
        line_num.append(float(t.findtext(f"LINE_NUM_COEFF_{i + 1:d}")))
        line_den.append(float(t.findtext(f"LINE_DEN_COEFF_{i + 1:d}")))
        samp_num.append(float(t.findtext(f"SAMP_NUM_COEFF_{i + 1:d}")))
        samp_den.append(float(t.findtext(f"SAMP_DEN_COEFF_{i + 1:d}")))
    rpc.line_numerator = line_num
    rpc.line_denominator = line_den
    rpc.sample_numerator = samp_num
    rpc.sample_denominator = samp_den
    return rpc


__all__ = ["pleides_rpc"]
