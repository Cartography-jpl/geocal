# This collects together various cameras that come from SPICE data. They tend
# to each have their own quirks, so it makes sense for now at least to have
# separate functions for each type of camera. Perhaps we can consolidate these
# somehow at some point.

from geocal_swig import *
from .sqlite_shelf import read_shelve
import numpy as np
import os

isis_camera_dir = os.path.abspath(os.path.dirname(__file__)) + "/data/isis_camera/"
                                   
def hrsc_camera():
    '''Return the HRSC camera. Note you should have loaded the instrument
    kernel already (e.g., os.environ["MARS_KERNEL"] + "/mex_kernel/mex.ker").
    We purposely don't load this, so you can direct to a different kernel
    if desired.

    This is HRSC Nadir camera, not sure if we will need different ones in
    the future.'''

    focal_length = SpiceHelper.kernel_data_double("INS-41215_FOCAL_LENGTH")
    pitch_x, pitch_y = SpiceHelper.kernel_data_array_double("INS-41215_PIXEL_SIZE")
    pitch_x *= 1e-3
    pitch_y *= 1e-3
    
    nsamp = SpiceHelper.kernel_data_int("INS-41215_PIXEL_SAMPLES")

    # As far as I can tell, you just need to "know" that there are 4 shielded
    # pixels on each size - the data input is actually 8 pixels smaller than
    # nsamp. Note that these pixels don't even appear in the Level 1 product
    # (unlike with CTX1 camera), so we just completely chop these out of
    # the camera - the pixels in this camera only include pixels in Level 1.
    left_shield = 4
    right_shield = 4

    # We have the boresight from
    # the spice kernel (INS-41215_BORESIGHT). This is in ccd mm units,
    # so we translate to pixels.
    # Note that we would subtract here, except that we have
    # "INCREASE_IS_NEGATIVE", so this flips the sign
    center_x, center_y = SpiceHelper.kernel_data_array_double("INS-41215_CCD_CENTER")
    bore_x, bore_y, bore_z = SpiceHelper.kernel_data_array_double("INS-41215_BORESIGHT")
    principal_point = FrameCoordinate(center_y - 0.5 + bore_y / pitch_y,
                          center_x - 0.5 - left_shield + bore_x / pitch_x)
    hrsc_cam = QuaternionCamera(Quaternion_double(1,0,0,0), 1,
                                nsamp - left_shield - right_shield,
                                pitch_y, pitch_x, focal_length,
                                principal_point,
                                QuaternionCamera.LINE_IS_Y,
                                QuaternionCamera.INCREASE_IS_NEGATIVE,
                                QuaternionCamera.INCREASE_IS_NEGATIVE)
    return hrsc_cam

def hirise_camera(ccd_number, bin_mode = 1, tdi_mode = 128):
    '''Return the HIRSISE camera. Note you should have loaded the instrument
    kernel already (e.g., os.environ["MARS_KERNEL"] + "/mro_kernel/mro.ker").
    We purposely don't load this, so you can direct to a different kernel
    if desired.

    Note that there are multiple focal planes, indexed by CCD number. We 
    pass in the CCD number that we want the camera for.
    '''
    # focal_length and pitch are in mm
    focal_length = SpiceHelper.kernel_data_double("INS-74699_FOCAL_LENGTH")
    pitch = SpiceHelper.kernel_data_double("INS-74699_PIXEL_PITCH")
    ik_code = -74600 - ccd_number
    nsamp = SpiceHelper.kernel_data_double("INS%d_PIXEL_SAMPLES" % ik_code)
    od_k = SpiceHelper.kernel_data_array_double("INS-74699_OD_K");
    trans_x = SpiceHelper.kernel_data_array_double("INS%d_TRANSX" % ik_code)
    trans_y = SpiceHelper.kernel_data_array_double("INS%d_TRANSY" % ik_code)
    ccd_cen = SpiceHelper.kernel_data_array_double("INS%d_CCD_CENTER" % ik_code)
    # Extra 0.5 pixel is because the convention in the SPICE kernels is to
    # have (0,0) be the upper left hand corner of first pixel, while we
    # use (0,0) for the center of the pixel.
    ccd_off = [0.5,0.5 + tdi_mode / 2.0]
    t_off = np.array([trans_x[0],trans_y[0]])
    t_m = np.array([[trans_x[1], trans_x[2]],[trans_y[1],trans_y[2]]])
    # We calculate the inverse rather than reading it. Limitations on the
    # precision gives an inverse that round trips with an error of ~0.01
    # pixel. Doesn't matter in practice, but it better if our inverse really
    # reverses our forward calculation
    tinv_m = np.linalg.inv(t_m)
    tinv_off = np.dot(tinv_m, -t_off)
    return CameraRadialDistortionAndTransform(Quaternion_double(1,0,0,0), od_k,
        1, nsamp, pitch, pitch, focal_length, bin_mode, ccd_off, ccd_cen,
        t_off, t_m, tinv_off, tinv_m)
                                              
def ctx_camera(start_sample=0,nsamp=None):
    '''Return the CTX camera. Note you should have loaded the instrument
    kernel already (e.g., os.environ["MARS_KERNEL"] + "/mro_kernel/mro.ker").
    We purposely don't load this, so you can direct to a different kernel
    if desired.
    '''
    focal_length = SpiceHelper.kernel_data_double("INS-74021_FOCAL_LENGTH")
    pitch = SpiceHelper.kernel_data_double("INS-74021_PIXEL_PITCH")
    if(nsamp is None):
        nsamp = SpiceHelper.kernel_data_double("INS-74021_PIXEL_SAMPLES")
    principal_point = FrameCoordinate(\
        SpiceHelper.kernel_data_double("INS-74021_BORESIGHT_LINE"),
        SpiceHelper.kernel_data_double("INS-74021_BORESIGHT_SAMPLE") - 0.5)
    od_k = SpiceHelper.kernel_data_array_double("INS-74021_OD_K");
    ctx_cam = CameraRadialDistortion(Quaternion_double(1,0,0,0), od_k,
                                     1, nsamp+abs(start_sample),
                                     pitch, pitch, focal_length,
                                     principal_point)
    if(start_sample != 0):
        ctx_cam = SubCamera(ctx_cam, 0, start_sample, 1, nsamp)
    return ctx_cam

def lro_nac_camera_from_isis(typ="left"):
    '''Return the LRO NAC camera. Type should be "left" or "right".
    This gets this directly from a GLAS model saved from ISIS. This was
    done initially for testing, we've left this in place for now'''
    if(typ == "left"):
        return read_shelve(isis_camera_dir + "isis_lnac_cam.bin")
    elif(typ == "right"):
        return read_shelve(isis_camera_dir + "isis_rnac_cam.bin")
    else:
        raise RuntimeError("Typ should be 'right' or 'left'")

def lro_nac_camera(typ="left", spatial_summing=1):
    '''Return the LRO NAC camera. Type should be "left" or "right".'''
    if(typ == "left"):
        bname = f"INS-85600"
    elif(typ == "right"):
        bname = f"INS-85610"
    else:
        raise RuntimeError("Typ should be 'right' or 'left'")
    focal_length = SpiceHelper.kernel_data_double(f"{bname}_FOCAL_LENGTH")
    pitch = SpiceHelper.kernel_data_double(f"{bname}_PIXEL_PITCH")
    nsamp = SpiceHelper.kernel_data_int(f"{bname}_PIXEL_SAMPLES")
    nline = SpiceHelper.kernel_data_int(f"{bname}_PIXEL_LINES")
    ccd_cen = [SpiceHelper.kernel_data_double(f"{bname}_BORESIGHT_SAMPLE"),
               SpiceHelper.kernel_data_double(f"{bname}_BORESIGHT_LINE")]
    od_k = SpiceHelper.kernel_data_array_double(f"{bname}_OD_K");
    # For who knows what reason, the od_k correction has a different
    # form than the other spice cameras we have used. We can massage
    # the values to fit in the original form. The
    # code is x/(1+k * x^2), which we expand with a taylor series
    # to the form x * (1 - (k0 + k1 * x^2 + k2 * x^4 + k4 * x^6)).
    # This isn't exactly the same, but is pretty close
    od_k = [0, od_k[0], -(od_k[0] ** 2), od_k[0] ** 3]
    trans_x = SpiceHelper.kernel_data_array_double(f"{bname}_TRANSX")
    trans_y = SpiceHelper.kernel_data_array_double(f"{bname}_TRANSY")
    # Extra 0.5 pixel is because the convention in the SPICE kernels is to
    # have (0,0) be the upper left hand corner of first pixel, while we
    # use (0,0) for the center of the pixel.
    ccd_off = [0.5,0]
    t_off = np.array([trans_x[0],trans_y[0]])
    t_m = np.array([[trans_x[1], trans_x[2]],[trans_y[1],trans_y[2]]])
    # We calculate the inverse rather than reading it. Limitations on the
    # precision gives an inverse that round trips with an error of ~0.01
    # pixel. Doesn't matter in practice, but it better if our inverse really
    # reverses our forward calculation
    tinv_m = np.linalg.inv(t_m)
    tinv_off = np.dot(tinv_m, -t_off)
    bin_mode=spatial_summing
    # Adjust line, which doesn't actually change for spatial_summing
    # (our bin_mode knob changes both sample and line). We put in an offset
    # that just cancels this
    ccd_off[1] = (bin_mode / 2.0 - 0.5)
    nac_cam = CameraRadialDistortionAndTransform(Quaternion_double(1,0,0,0),
                                                 od_k,
        1, nsamp/bin_mode, pitch, pitch, focal_length, bin_mode,
        ccd_off, ccd_cen, t_off, t_m, tinv_off, tinv_m)
    return nac_cam
    
def lro_wac_camera(band=3, mode="COLOR"):
    '''Return the LRO WAC camera. Note that this varies depending on the
    band. By convention the first two bands are the UV, the next 5 are
    the visible'''
    if(band in (1, 2)):
        raise RuntimeError("We don't support UV1 or UV2 yet. Use channels 3-7")
        # LRO_LROCWAC_UV_FILTER_1
        # LRO_LROCWAC_UV_FILTER_2
        bname = f"INS-8564{band}"
        sub_nline = 16
    elif(band in (3,4,5,6,7)):
        # LRO_LROCWAC_VIS_FILTER_1 - 5
        bname = f"INS-8563{band-2}"
        sub_nline = 14
    else:
        raise RuntimeError(f"Band {band} is unknown, should be 1-7")
 
    focal_length = SpiceHelper.kernel_data_double(f"{bname}_FOCAL_LENGTH")
    pitch = SpiceHelper.kernel_data_double(f"{bname}_PIXEL_PITCH")
    nsamp = SpiceHelper.kernel_data_int(f"{bname}_PIXEL_SAMPLES")
    nline = SpiceHelper.kernel_data_int(f"{bname}_PIXEL_LINES")
    ccd_cen = [SpiceHelper.kernel_data_double(f"{bname}_BORESIGHT_SAMPLE"),
               SpiceHelper.kernel_data_double(f"{bname}_BORESIGHT_LINE")]
    od_k = SpiceHelper.kernel_data_array_double(f"{bname}_OD_K");
    # For who knows what reason, the od_k correction has a different
    # form than the other spice cameras we have used. We can massage
    # the values to fit in the original form
    od_k = [0,-od_k[0],-od_k[1],-od_k[2]]
    trans_x = SpiceHelper.kernel_data_array_double(f"{bname}_TRANSX")
    trans_y = SpiceHelper.kernel_data_array_double(f"{bname}_TRANSY")
    # Extra 0.5 pixel is because the convention in the SPICE kernels is to
    # have (0,0) be the upper left hand corner of first pixel, while we
    # use (0,0) for the center of the pixel.
    loff = SpiceHelper.kernel_data_int(f"{bname}_FILTER_OFFSET")
    soff = SpiceHelper.kernel_data_int(f"{bname}_{mode}_SAMPLE_OFFSET")
    # TODO - should this be +0.5?
    ccd_off = [-0.5+soff,-0.5+loff]
    t_off = np.array([trans_x[0],trans_y[0]])
    t_m = np.array([[trans_x[1], trans_x[2]],[trans_y[1],trans_y[2]]])
    # We calculate the inverse rather than reading it. Limitations on the
    # precision gives an inverse that round trips with an error of ~0.01
    # pixel. Doesn't matter in practice, but it better if our inverse really
    # reverses our forward calculation
    tinv_m = np.linalg.inv(t_m)
    tinv_off = np.dot(tinv_m, -t_off)
    bin_mode=1
    wac_cam = CameraRadialDistortionAndTransform(Quaternion_double(1,0,0,0),
                                                 od_k,
        sub_nline, nsamp - 2 * soff, pitch, pitch, focal_length, bin_mode,
        ccd_off, ccd_cen, t_off, t_m, tinv_off, tinv_m)
    return wac_cam




__all__ = ["hrsc_camera", "ctx_camera", "hirise_camera", "lro_wac_camera",
           "lro_nac_camera", "lro_nac_camera_from_isis"]
