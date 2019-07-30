from geocal.vicar_interface import VicarInterface
from geocal.sqlite_shelf import write_shelve
from geocal_swig import (IbisFile, ImageCoordinate, VicarImageCoordinate,
                         TiePoint,
                         TiePointCollection, VicarLiteRasterImage)
import os
import numpy as np
import shutil

class _warp(VicarInterface):
    '''Call gtwarpxd'''
    def __init__(self, img_in, img_out, ref_img, tpcol, image_index=0,
                 nah=100, nav=100, polyfit=None,
                 tpgrid_out=None,
                 interp="bilin",
                 log_file=None, run_dir_name=None, quiet = True):
        VicarInterface.__init__(self)
        self.title = "warp"
        self.log_file = log_file
        self.run_dir_name = run_dir_name
        self.tpcol = tpcol
        self.image_index= image_index
        if(tpgrid_out):
            self.tpgrid_out = os.path.abspath(tpgrid_out)
        else:
            self.tpgrid_out = None
        self.ref_img = os.path.abspath(ref_img)
        self.input = [img_in, ref_img]
        self.output = [img_out,]
        self.img_out = os.path.abspath(img_out)
        self.print_output = not quiet
        self.before_body = '''
local img_in string
local img_out string
local ref_img string
local nah int
local nav int
local polyfit string
local interp string
local distort string
local gridout string
'''
        self.cmd = "let img_in=\"%s\"\n" % os.path.basename(img_in)
        self.cmd += "let ref_img=\"%s\"\n" % os.path.basename(ref_img)
        self.cmd += "let img_out=\"%s\"\n" % os.path.basename(img_out)
        self.cmd += "let distort=\"distort.ibis\"\n"
        self.cmd += "let nah=%d\n" % nah
        self.cmd += "let nav=%d\n" % nav
        self.cmd += "let polyfit=\"%s\"\n" % (polyfit if polyfit else "")
        self.cmd += "let interp=\"%s\"\n" % interp
        self.cmd += "let gridout=\"gridout\"\n"
        self.cmd += '''
gtwarpxd inp=&img_in out=&img_out ref=&ref_img 'coverref interp=&interp +
        nah=&nah nav=&nav gridout=&gridout +
        distort=(&distort,,,,) dcols=(1,2,3,4) polyfit=("&polyfit",,,,) +
        echo="yes"
'''
        self.timing = False
        if(self.run_dir_name is not None):
            self.keep_run_dir = True
        self.vicar_run()

    def post_run(self):
        if(self.tpgrid_out):
            tpcol = TiePointCollection()
            f = VicarLiteRasterImage(self.img_out)
            tpcol.add_ibis_file("gridout1", f, None)
            write_shelve(self.tpgrid_out, tpcol)

    def pre_run(self):
        npts = np.count_nonzero([tp.image_coordinate(self.image_index) for tp in self.tpcol])
        data = np.empty((npts,4))
        i = 0
        ref = VicarLiteRasterImage(self.ref_img)
        for tp in self.tpcol:
            ic = tp.image_coordinate(self.image_index)
            if(ic):
                ic = VicarImageCoordinate(ic)
                data[i,2] = ic.line
                data[i,3] = ic.sample
                ic2 = VicarImageCoordinate(ref.coordinate(tp.ground_location))
                data[i,0] = ic2.line
                data[i,1] = ic2.sample
                i += 1
        IbisFile.create("distort.ibis", data)
        
def warp_image(img_in, img_out, ref_img, tpcol, image_index=0,
               nah=100, nav=100, polyfit=None,
               tpgrid_out=None,
               interp="bilin",
               log_file=None, run_dir_name=None, quiet = True):
               
    '''This is a wrapper around gtwarpxd. We warp the vicar file input
    img_in, writing out as img_out. This matches the given ref_img. This
    take the set of tiepoint in tpcol (e.g., generated by 
    TiePointCollectPicmtch).

    nah and nav are the number of grid cells in the horizontal and vertical
    direction used by gtwarpxd.

    Interpolation should be noin, bilin, cubcon or cubsplin. 

    Polyfit can override the triangulation surface fit with a
    polynomial fit. It should be empty, linear, keystone, quad or
    cubic.
    '''
    w = _warp(img_in, img_out, ref_img, tpcol, image_index=image_index,
              nah=nah, nav=nav,polyfit=polyfit,interp=interp,
              tpgrid_out=tpgrid_out,
              log_file=log_file,run_dir_name=run_dir_name,quiet=quiet)
        
__all__ = ["warp_image"]    
    
