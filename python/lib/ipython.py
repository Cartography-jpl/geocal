# This contains ipython magic extensions. To load you can execute
# %load_ext geocal.ipython. You can add this to your ipython configuration
# file to always load by setting "c.InteractiveShellApp.extension =  ['geocal.ipython']"

from geocal import *
# The interact gives a warning everytime we load it. Kind of annoying, so
# turn off. 
import warnings
warnings.simplefilter(action = "ignore", category = FutureWarning)
from IPython.html.widgets import interact

def tie_point_display(tpcol, igccol, surface_image=None):
    '''This function is a simple wrapper around the tiepoint display
    routine, which allows some simple widget modifications.
    '''
    def tpdisp(scale = 3, size="50", tpind="0"):
        plt.rcParams["figure.figsize"] = (6 * scale, 4 * scale)
        tpcol[int(tpind)].display(igccol, int(size), 
                                  surface_image = surface_image)
    interact(tpdisp, scale = 3, size="50", tpind="0")

def load_ipython_extension(ipython):
    ipython.ex("from geocal.ipython import tie_point_display")

def unload_ipython_extension(ipython):
    # Nothing to do here
    pass



