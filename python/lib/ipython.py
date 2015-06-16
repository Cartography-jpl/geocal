# This contains ipython magic extensions. To load you can execute
# %load_ext geocal.ipython. You can add this to your ipython configuration
# file to always load by setting "c.InteractiveShellApp.extension =  ['geocal.ipython']"

from geocal import *
# The interact gives a warning everytime we load it. Kind of annoying, so
# turn off. 
import warnings
warnings.simplefilter(action = "ignore", category = FutureWarning)
from IPython.html.widgets import interact

def tie_point_display(tpcol, igccol, surface_image=None, tpind=0):
    '''This function is a simple wrapper around the tiepoint display
    routine, which allows some simple widget modifications.
    '''
    def tpdisp(scale = 3, size="50", tpind="0"):
        plt.rcParams["figure.figsize"] = (6 * scale, 4 * scale)
        tp = tpcol[int(tpind)]
        tp.display(igccol, int(size), surface_image = surface_image)
        plt.show()
        ind = []
        line_res = []
        sample_res = []
        lsigma = []
        ssigma = []
        for i, iloc in enumerate(tp.image_location):
            if(tp.image_location[i]):
                try:
                    icpred = igccol.image_coordinate(i, tp.ground_location)
                    ic, ls, ss = tp.image_location[i]
                    ind.append(i)
                    line_res.append(ic.line - icpred.line)
                    sample_res.append(ic.sample - icpred.sample)
                    lsigma = ls
                    ssigma = ss
                except ImageGroundConnectionFailed:
                    pass
        plt.rcParams["figure.figsize"] = (6, 4)
        plt.errorbar(ind, line_res, lsigma, fmt='o', label="Line residual")
        plt.errorbar(ind, sample_res, ssigma, fmt='o', label="Sample residual")
        plt.xlabel("Image index")
        plt.ylabel("Residual")
        if(tp.is_gcp):
            plt.title("GCP residual")
        else:
            plt.title("Tiepoint residual")
        plt.legend()
        plt.show()
    interact(tpdisp, scale = 3, size="50", tpind=str(tpind))

def load_ipython_extension(ipython):
    ipython.ex("from geocal.ipython import tie_point_display")

def unload_ipython_extension(ipython):
    # Nothing to do here
    pass



