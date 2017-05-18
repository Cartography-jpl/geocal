from geocal_swig import *

def burl_camera(nu, nv, xi, u0, v0, pld_q_c = Quaternion_double(1,0,0,0)):
    '''Mike Burl has matlab code that used a camera model very similar to
    our QuaternionCamera. However I'm constantly having to figure out how to
    translate between the two format, so this routine just handles the details.
    Mike uses u,v which is the same as our sample,line (so reversed). His 
    quaternions have the real part as element 0 just like we do  (as opposed
    to the other convention of putting this at the end as element 3).
    Instead of giving line and sample pitch, he tends to give xi which is the
    field angle as urad. He doesn't have the focal length separately, we just
    use a focal length of 1 to match his code.'''
    # Check, I might have pld_q_c backwards, we may need to invert
    return QuaternionCamera(pld_q_c, nv, nu, 1/xi, 1/xi, 1,
                            FrameCoordinate(v0, u0), QuaternionCamera.LINE_IS_Y)
