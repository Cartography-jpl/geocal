KPL/FK

This adds in the galileo scan platform as a frame. For some
reason, the Galileo project doesn't have this already. Some
I've written this (Mike Smyth).

I believe the this is the same frame as needed by the solid
state imaging (SSI) subsystem. According to the instrument kernel
(gll36001.ti), there doesn't seem to be any orientation between
the 2 frame that we need to worry about (other than the twist
offset angle, which we consider as part of the camera model in
geocal).

\begindata 
      FRAME_GLL_SCAN_PLANE   =  -77001
      FRAME_-77001_NAME     = 'GLL_SCAN_PLANE'
      FRAME_-77001_CLASS    =  3
      FRAME_-77001_CENTER   = -77
      FRAME_-77001_CLASS_ID = -77001

