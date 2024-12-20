from builtins import str
from builtins import range
# This contains ipython magic extensions. To load you can execute
# %load_ext geocal.ipython. 
# You may well want
# %matplotlib inline 
# also
# You can add this to your ipython configuration
# file to always load by setting "c.InteractiveShellApp.extension =  ['geocal.ipython']"

import geocal
# The interact gives a warning everytime we load it. Kind of annoying, so
# turn off. 
import warnings
warnings.simplefilter(action = "ignore", category = FutureWarning)
from ipywidgets import interact, interact_manual
import matplotlib.pyplot as plt
try:
    # Use prettier plots, if available
    import seaborn as sns
    sns.set(style='ticks', palette='Set2')
except ImportError:
    pass

def tie_point_display(tpcol, igccol, surface_image=None, ref_image=None, 
                      tpind=0):
    '''This function is a simple wrapper around the tiepoint display
    routine, which allows some simple widget modifications.
    '''
    def tpdisp(scale = 3, size="50", tpind="0"):
        plt.rcParams["figure.figsize"] = (6 * scale, 4 * scale)
        tp = tpcol[int(tpind)]
        tp.display(igccol, int(size), surface_image = surface_image,
                   ref_image = ref_image)
        plt.show()
        ind = list(range(tp.number_image))
        residual = tp.ic_diff(igccol)
        sigma = tp.ic_sigma
        plt.rcParams["figure.figsize"] = (6, 4)
        plt.xlim(0, igccol.number_image)
        plt.errorbar(ind, residual[0,:], sigma[0,:], fmt='o', 
                     label="Line residual")
        plt.errorbar(ind, residual[1,:], sigma[1,:], fmt='o', 
                     label="Sample residual")
        plt.xlabel("Image index")
        plt.ylabel("Residual")
        if(tp.is_gcp):
            plt.title("GCP ID %d residual" % tp.id)
        else:
            plt.title("Tiepoint ID %d residual" % tp.id)
        plt.legend()
        plt.show()
    interact(tpdisp, scale = 3, size="50", tpind=str(tpind))

def load_ipython_extension(ipython):
    ipython.ex("from geocal.ipython import tie_point_display")

def unload_ipython_extension(ipython):
    # Nothing to do here
    pass



