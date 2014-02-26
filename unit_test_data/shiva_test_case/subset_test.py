from afids import *
from geocal import *
import numpy as np

tile = (17700, 20900, 200, 300)

for f in (("../postroot_pan.img", "post_pan_sub.img"),
          ("../preroot_pan.img", "pre_pan_sub.img"),
          ("nxpostpan", "cvdnorm_sub.img"),
          ("pandif", "pandif_sub.img"),
          ("../postroot_b1.img", "post_b1.img"),
          ("../postroot_b2.img", "post_b2.img"),
          ("../postroot_b3.img", "post_b3.img"),
          ("../postroot_b4.img", "post_b4.img"),
          ("../postroot_b5.img", "post_b5.img"),
          ("../postroot_b6.img", "post_b6.img"),
          ("../postroot_b7.img", "post_b7.img"),
          ("../postroot_b8.img", "post_b8.img"),
          ("xxxim4", "material_sub.img"),
          ) :
    GdalRasterImage.save(f[1], "VICAR",
                         SubRasterImage(VicarRasterImage(f[0]), *tile),
                         GdalRasterImage.Int16)

