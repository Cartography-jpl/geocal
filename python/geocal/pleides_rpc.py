from __future__ import annotations

from xml.etree.ElementTree import Element, ElementTree, parse

from geocal_swig import Rpc


def _find(doc: ElementTree[Element], path: str) -> Element:
    t = doc.find(path)
    if t is None:
        raise RuntimeError(f"Didn't find {path} in the Pleides XML file")
    return t


def _float(t: Element, tag: str) -> float:
    v = t.findtext(tag)
    if v is None:
        raise RuntimeError(f"Didn't find {tag} in the Pleides XML file")
    return float(v)


def pleides_rpc(fname: str) -> Rpc:
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
    t = _find(doc, "Rational_Function_Model/Global_RFM/RFM_Validity")
    rpc.line_offset = _float(t, "LINE_OFF")
    rpc.line_scale = _float(t, "LINE_SCALE")
    rpc.sample_offset = _float(t, "SAMP_OFF")
    rpc.sample_scale = _float(t, "SAMP_SCALE")
    rpc.latitude_offset = _float(t, "LAT_OFF")
    rpc.latitude_scale = _float(t, "LAT_SCALE")
    rpc.longitude_offset = _float(t, "LONG_OFF")
    rpc.longitude_scale = _float(t, "LONG_SCALE")
    rpc.height_offset = _float(t, "HEIGHT_OFF")
    rpc.height_scale = _float(t, "HEIGHT_SCALE")
    t = _find(doc, "Rational_Function_Model/Global_RFM/Inverse_Model")
    line_num = []
    line_den = []
    samp_num = []
    samp_den = []
    for i in range(20):
        line_num.append(_float(t, f"LINE_NUM_COEFF_{i + 1:d}"))
        line_den.append(_float(t, f"LINE_DEN_COEFF_{i + 1:d}"))
        samp_num.append(_float(t, f"SAMP_NUM_COEFF_{i + 1:d}"))
        samp_den.append(_float(t, f"SAMP_DEN_COEFF_{i + 1:d}"))
    rpc.line_numerator = line_num
    rpc.line_denominator = line_den
    rpc.sample_numerator = samp_num
    rpc.sample_denominator = samp_den
    return rpc


__all__ = ["pleides_rpc"]
