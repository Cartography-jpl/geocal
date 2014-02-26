# Short script to generate map projected version of data.

from afids import *
demin = VicarLiteDem("nevada_elv_aoi.img", True)
igc1 = VicarImageGroundConnection("10MAY21-1.img", demin)
igc2 = VicarImageGroundConnection("10MAY21-2.img", demin)
# We are working with raw WV2 data which hasn't been corrected yet for 
# misregistration. We'll eventually code and test an SBA, but for the 
# purpose of these tests just correct the RPC to coregister the data.
igc2.rpc.line_offset -= -0.949821385786
igc2.rpc.sample_offset -= -0.97417620076

aoi = VicarLiteRasterImage("aoi.img")
mi = igc1.cover(aoi.map_info()).map_union(igc2.cover(aoi.map_info()))
igc1mp = IgcMapProjected(mi, igc1)
igc2mp = IgcMapProjected(mi, igc2)
GdalRasterImage.save_to_file("10MAY21-1_projected.tif", igc1mp, "gtiff")
GdalRasterImage.save_to_file("10MAY21-2_projected.tif", igc2mp, "gtiff")

