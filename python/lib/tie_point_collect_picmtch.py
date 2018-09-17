from geocal.vicar_interface import VicarInterface
from geocal_swig import (IbisFile, ImageCoordinate, VicarImageCoordinate,
                         TiePoint,
                         TiePointCollection, VicarLiteRasterImage)
import time
import os
import numpy as np

class _tpcol(VicarInterface):
    '''Call picmtch5 like we do for gtpwarp, but don't actually do
    the warping. We then pull out the results to supply as tiepoints.
    '''
    def __init__(self, img1_fname, img2_fname, fftgrid=(42, 42),
                 fftsize=256, magnify=4.0, magmin=2.0, toler=1.5, redo=36,
                 ffthalf=2, seed=562, autofit=14, thr_res=10.0,
                 log_file=None, run_dir_name=None, quiet = True):
        VicarInterface.__init__(self)
        self.title = "tpcol"
        self.log_file = log_file
        self.run_dir_name = run_dir_name
        self.input = [img1_fname, img2_fname]
        self.print_output = not quiet
        # If there two image are significantly different in resolution, we get
        # better results by having the lower resolution image be the first
        # on in the match list. Controlled by ref_lowres flag.
        ref_lowres="n"
        img1_res = VicarLiteRasterImage(img1_fname).map_info.resolution_meter
        img2_res = VicarLiteRasterImage(img2_fname).map_info.resolution_meter
        if(1.1 * img1_res < img2_res):
            ref_lowres="y"
        self.before_body = '''
local toler real
local redo int
local magmin real
local magnify real
local bigtoler real
local fftgrid1 int
local fftgrid2 int
local ffthalf int
local seed int
local fftsize int
local autofit int
local inp string
local ref string
local ref_res string
local out string
local thr_res real
local l1 real
local s1 real
local l2 real
local s2 real
local l3 real
local s3 real
local l4 real
local s4 real

local magshrk string

local xqxqgrid1 string
local xqxqgrid2 string
local xqxqgrid3 string
local img1 string
local img2 string
local ref_lowres string
local ref_resf string
'''
        self.cmd = "let toler = %f\n" % toler
        self.cmd+= "let ref_lowres = \"%s\"\n" % ref_lowres
        self.cmd+= "let inp = \"%s\"\n" % os.path.basename(img1_fname)
        self.cmd+= "let ref = \"%s\"\n" % os.path.basename(img2_fname)
        # Not currently used. This is a parameter that appears in
        # gtpwarp, so we'll carry it. But for now this is just an empty
        # value.
        self.cmd+= "let ref_res = \"\"\n"
        self.cmd+= "let fftgrid1 = %d\n" % fftgrid[0]
        self.cmd+= "let fftgrid2 = %d\n" % fftgrid[1]
        self.cmd+= "let seed = %d\n" % seed
        self.cmd+= "let magmin = %f\n" % magmin
        self.cmd+= "let magnify = %f\n" % magnify
        self.cmd+= "let fftsize = %d\n" % fftsize
        self.cmd+= "let autofit = %d\n" % autofit
        self.cmd+= "let redo = %d\n" % redo
        self.cmd+= "let ffthalf = %d\n" % ffthalf
        self.cmd+= "let thr_res = %f\n" % thr_res
        self.cmd+= "let out = \"tpcol.ibis\"\n"
        self.cmd+= '''
let bigtoler = toler*4.0

let xqxqgrid1 = "xqxqgrid1qq"
let xqxqgrid2 = "xqxqgrid2qq"
let xqxqgrid3 = "xqxqgrid3qq"

!  do the picmtch5

if (ref_lowres="n")
   let img1="&inp"
   let img2="&ref"
else
   let img1="&ref"
   let img2="&inp"
end-if
if (ref_res="")
   let ref_resf=ref
else
   let ref_resf=ref_res
end-if

imcorner (&img1,&img2) line1=l1 samp1=s1 line2=l2 samp2=s2 +
      line3=l3 samp3=s3 line4=l4 samp4=s4 inside=0              !note, alz
gengrid2 out=&xqxqgrid1 ncol=21 nah=&fftgrid1 nav=&fftgrid2 +
     l1=&l1 s1=&s1 l2=&l2 s2=&s2 l3=&l3 s3=&s3 l4=&l4 s4=&s4
mf3 &xqxqgrid1 f="c11=@rand" seed=&seed
sort &xqxqgrid1 sortcol=11
if (magmin<magnify)
   let magshrk="y"
else
   let magshrk="n"
end-if

picmtch5 (&img1,&img2,&xqxqgrid1) fftsize=&fftsize search=&fftsize +
     minsrch=&fftsize magnify=(&magnify,&magnify) +
     pred=linear autofit=&autofit redo=&redo magmin=(&magmin,&magmin) +
     magshrk=&magshrk ffthalf=&ffthalf thr_res=&thr_res
! Swap if necessary so columns 3,4 are for inp and 6,7 are for ref
if (ref_lowres="y")
   mf3 &xqxqgrid1 func="c14=c3$c15=c4"
   mf3 &xqxqgrid1 func="c3=c6$c4=c7$c6=c14$c7=c15"
end-if
rowop2 &xqxqgrid1 &xqxqgrid2 keycol=9 range=(-100000,-1) 'delete

!  calculate the distortion (oldline,oldsamp)

gtproj2 (&xqxqgrid2,&inp,&ref_resf) incol=(3,4) outcol=(14,15) 'image

!  apply very tolerant ibislsq to remove very bad picmtchs

mf3 &xqxqgrid2 func="c5=1.0$c19=c3*c3$c20=c3*c4$c21=c4*c4"
ibislsq2 &xqxqgrid2 indcol=(3,4,5,19,20,21)  depcol=6 rescol=16
ibislsq2 &xqxqgrid2 indcol=(3,4,5,19,20,21)  depcol=7 rescol=17
mf3 &xqxqgrid2 func="c18=@sqrt(c16*c16+c17*c17)"
rowop2 &xqxqgrid2 &xqxqgrid3 keycol=18 range=(0.0,&bigtoler) 'select

!  apply stricter ibislsq to remove picmtchs above toler parameter

mf3 &xqxqgrid3 func="c5=1.0$c19=c3*c3$c20=c3*c4$c21=c4*c4"
ibislsq2 &xqxqgrid3 indcol=(3,4,5,19,20,21)  depcol=6 rescol=16
ibislsq2 &xqxqgrid3 indcol=(3,4,5,19,20,21)  depcol=7 rescol=17
mf3 &xqxqgrid3 func="c18=@sqrt(c16*c16+c17*c17)"
rowop2 &xqxqgrid3 &out keycol=18 range=(0.0,&toler) 'select

! If necessary, convert tiepoint from pixels in ref file to higher resolution
! ref_resf
if (ref_lowres="y")
  pixmap (&out,&ref) mapcols=(16,17) pixcols=(6,7) 'pixtomap
  pixmap (&out,&ref_resf) mapcols=(16,17) pixcols=(6,7) 'maptopix
end-if
'''
        self.timing = False
        if(self.run_dir_name is not None):
            self.keep_run_dir = True
        self.vicar_run()

    def post_run(self):
        f = IbisFile("tpcol.ibis")
        ln1 = f.column(2)[:]
        smp1 = f.column(3)[:]
        ln2 = f.column(5)[:]
        smp2 = f.column(6)[:]
        #z1 = f.column(4)[:]
        #z2 = f.column(7)[:]
        self.res = [(ImageCoordinate(VicarImageCoordinate(ln1[i],smp1[i])),
                     ImageCoordinate(VicarImageCoordinate(ln2[i],smp2[i])))
                    for i in range(f.number_row)]
        
class TiePointCollectPicmtch(object):
    def __init__(self, igc_collection, surface_image_fname,
                 image_index1 = 0, image_index2 = -1,
                 ref_image_fname = None, ref_dem = None,
                 fftsize=256, magnify=4.0, magmin=2.0, toler=1.5, redo=36,
                 ffthalf=2, seed=562, autofit=14, thr_res=10.0,
                 log_file = None,
                 run_dir_name = None, quiet = True):
        '''This sets up for doing a tie point collection, using pictmtch5.
        This is similar to TiePointCollect.

        Note a limitation of pictmtch5 is that we can match exactly 2 images,
        and the images *must* be surface projected (unlike TiePointCollect where
        this is just optional). 
        
        Rather than passing in the surface and reference image as RasterImage
        objects, an explicit VICAR file name needs to be supplied.
        
        The second image can other be a second igc from the igc_collection, or
        a reference image. The difference is for the reference image that we 
        produce a Gcp.
        
        We match like gtpwarp, so you can pass in all the standard gtpwarp 
        options.

        fftsize - The size of the FFT window (default 256)
        magnify - Enlarge footprint of fft window by this factor.
        magmin - Minimum magnifier as correlation proceeds
        toler - Tolerance for accepting tiepoints
        redo - At end of matching, this number of points will be redone
        ffthalf - 0 = no halving of fftsize near edge, 
                  1 = single halving of fftsize near edge
                  2 = double halving of fftsize near edge
        seed - seed for randomizing picmtch5 grid

        If quiet is False, then we write the output to stdout as a we 
        process. This can be useful to view the status as we process.
        '''
        self.igc_collection = igc_collection
        self.surface_image_fname = surface_image_fname
        self.image_index1 = image_index1
        self.image_index2 = image_index2
        self.ref_image_fname = ref_image_fname
        self.quiet = quiet
        if(ref_dem is None):
            self.ref_dem = self.igc_collection.dem(image_index1)
        else:
            self.ref_dem = ref_dem
        self.fftsize = fftsize
        self.magnify = magnify
        self.magmin = magmin
        self.toler = toler
        self.redo = redo
        self.ffthalf = ffthalf
        self.seed = seed
        self.autofit = autofit
        self.thr_res = thr_res
        self.log_file = log_file
        self.run_dir_name = run_dir_name
        if((self.image_index2 >= 0 and self.ref_image_fname is not None) or
           (self.image_index2 < 0 and self.ref_image_fname is None)):
            raise RuntimeError("Need to either supply a ref_image_fname or an image_index2, but not both")

    @property
    def number_image(self):
        return self.igc_collection.number_image

    def tie_point_grid(self, num_x, num_y, diagnostic = None):
        '''Return a grid of tie points. Note that we may be missing results
        for particular grid points if matching isn\'t successful.'''
        tstart = time.time()
        img1_fname = self.surface_image_fname[self.image_index1]
        if(self.image_index2 >= 0):
            img2_fname = self.surface_image_fname[self.image_index2]
        else:
            img2_fname = self.ref_image_fname
        tpcollect = _tpcol(img1_fname, img2_fname, fftgrid=(num_y, num_x),
                           fftsize = self.fftsize, magnify = self.magnify,
                           magmin = self.magmin, toler = self.toler,
                           redo = self.redo, ffthalf = self.ffthalf,
                           seed = self.seed, autofit=self.autofit,
                           thr_res=self.thr_res,
                           log_file = self.log_file,
                           run_dir_name = self.run_dir_name,
                           quiet = self.quiet)
        tpcol = TiePointCollection()
        img1 = VicarLiteRasterImage(img1_fname)
        img2 = VicarLiteRasterImage(img2_fname)
        dem1 = self.igc_collection.dem(self.image_index1)
        if(self.image_index2 >= 0):
            dem2 = self.igc_collection.dem(self.image_index2)
        else:
            dem2 = self.ref_dem
        for sic1, sic2 in tpcollect.res:
            tp = TiePoint(self.igc_collection.number_image)
            gp1 = img1.ground_coordinate(sic1, dem1)
            ic1 = self.igc_collection.image_coordinate(self.image_index1, gp1)
            tp.image_coordinate(self.image_index1, ic1)
            if(self.ref_image_fname):
                tp.is_gcp = True
                tp.ground_location = img2.ground_coordinate(sic2, dem2).convert_to_cf()
            else:
                gp2 = img1.ground_coordinate(sic2, dem2)
                ic2 = self.igc_collection.image_coordinate(self.image_index2, gp2)
                tp.image_coordinate(self.image_index2, ic2)
                tp.ground_location = img2.ground_coordinate(sic2, dem2).convert_to_cf()
            tpcol.push_back(tp)
        return tpcol
        
        
__all__ = ["TiePointCollectPicmtch"]    
