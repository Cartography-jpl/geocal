from __future__ import division, print_function
from future import standard_library
standard_library.install_aliases()
from past.utils import old_div
import os
import errno
import sys
import re
import geocal_swig
import pickle
import subprocess
try:
    import osgeo.gdal as gdal
    have_osgeo = True
except ImportError:
    have_osgeo = False

# This contains miscellenous routines that don't really belong anywhere else.

def makedirs_p(dir):
    '''This is a version of makedirs that acts like mkdir -p, not complaining
    if directory already exists'''

    try:
        os.makedirs(dir)
    except OSError as exc:
        if exc.errno == errno.EEXIST: pass
        else: raise

def cib01_data():
    '''This return a raster image that can be used to read the CIB
    1 meter data.'''
    return geocal_swig.VicarMultiFile(os.environ["CIB1_ROOT"] + "/cib01_db.int",
                                      os.environ["CIB1_ROOT"], ".img")

def _write_cib01_mapinfo():
    '''Write out cib01 map info for when we don't have CIB 1 data available'''
    geocal_swig.serialize_write("cib01_mapinfo.xml", cib01_data().map_info)

# _write_cib01_mapinfo()

def cib01_mapinfo(desired_resolution = None):
    '''This gets the CIB 1 meter MapInfo. This is useful when we don't
    have the CIB 1 data available, but need a reasonable 1 meter
    MapInfo. You can optionally supply a desired resolution, to scale the 
    mapinfo. '''
    res = geocal_swig.serialize_read_generic(os.path.dirname(__file__) + 
                                             "/cib01_mapinfo.xml")
    if(desired_resolution):
        resbase = res.resolution_meter
        res = res.scale(old_div(desired_resolution, resbase),
                        old_div(desired_resolution, resbase))
    return res

def planet_mapinfo(naif_code, desired_resolution = 1):
    '''Like cib01_mapinfo, but for other planets. We have this as
    Planetocentric, sized to the desired resolution in meters'''
    cconv = geocal_swig.PlanetocentricConverter(naif_code)
    mi = geocal_swig.MapInfo(cconv,0,1,1,0,10,10)
    mi = mi.scale(desired_resolution/mi.resolution_meter,
                  desired_resolution/mi.resolution_meter)
    return mi

def pid_exists(pid):
    """Check whether pid exists in the current process table.
    UNIX only.
    """
    if pid < 0:
        return False
    if pid == 0:
        # According to "man 2 kill" PID 0 refers to every process
        # in the process group of the calling process.
        # On certain systems 0 is a valid PID but we have no way
        # to know that in a portable fashion.
        raise ValueError('invalid PID 0')
    try:
        os.kill(pid, 0)
    except OSError as err:
        if err.errno == errno.ESRCH:
            # ESRCH == No such process
            return False
        elif err.errno == errno.EPERM:
            # EPERM clearly means there's a process to deny access to
            return True
        else:
            # According to "man 2 kill" possible error values are
            # (EINVAL, EPERM, ESRCH)
            raise
    else:
        return True

def comment_remover(text):
    '''This removes C and C++ style comments (/* */ and //, C can be multiline)
    This is from https://gist.github.com/ChunMinChang/88bfa5842396c1fbbc5b.
    Fairly complicated regex, believe this originally came from 
    Jeffrey Friedl and later modified by Fred Curtis, from a perl FAQ.
    Extended this to also strip out python style comments starting with
    "#" (pretty much the same as // C++ style)
    '''
    def replacer(match):
        s = match.group(0)
        if s.startswith('/') or s.startswith('#'):
            return " " # note: a space and not an empty string
        else:
            return s
    pattern = re.compile(
        r'//.*?$|#.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
        re.DOTALL | re.MULTILINE
    )
    return re.sub(pattern, replacer, text)

def run_tee(exec_cmd, out_fh = None, quiet = False):
    '''This is like subprocess.run, but allowing a unix like 'tee' where
    we write the output to a log file and/or stdout.

    The command (which should be a standard array) is run, and the output
    is always returned. In addition, the output is written to the given
    out_fh is supplied, and to stdout if "quiet" is not True.
    '''
    process = subprocess.Popen(exec_cmd, stdout=subprocess.PIPE,
                               stderr=subprocess.STDOUT)
    stdout = b''
    while(True):
        output = process.stdout.readline()
        if output == b'' and process.poll() is not None:
            break
        if output:
            stdout = stdout + output
            if(not quiet):
                print(output.strip().decode('utf-8'))
                sys.stdout.flush()
            if(out_fh):
                print(output.strip().decode('utf-8'), file=out_fh)
                out_fh.flush()
    if(process.poll() != 0):
        raise subprocess.CalledProcessError(process.poll(), exec_cmd,
                                            output=stdout)
    return stdout

def mars_fix_projection(fin, fout, band, hirise_correction=False):
    '''There are potential issues with some of the map projections with 
    Mars, errors or missing pieces. This create a VRT file that fixes any 
    of these problems.

    fin should be a file name with any readable GDAL file (e.g., PDS, 
    HiRISE JPEG2000). fout will be a ".vrt" file with the projection 
    information corrected.

    There is a HiRISE problem that I'm not sure is always there (see code 
    comment for details). Since this could potentially corrupt good data,
    we only include this correction of hirise_correction is set to True.
    '''
    if(not have_osgeo):
        raise RuntimeError("mars_fix_projection requires that the GDAL python library osgeo.gdal be installed")
    cmd = ["gdal_translate", "-of", "VRT", "-b", str(band), fin, fout]
    subprocess.run(cmd, check=True)
    f = gdal.Open(fout, gdal.GA_Update)
    p = str(f.GetProjectionRef())
    # The PolarSterographic can have the wrong scale factor of 0. This
    # appears to be an error in ISIS, they don't actually set the scale
    # factor and assume it defaults to 1, when actually it defaults to 0. 
    # See https://trac.osgeo.org/gdal/ticket/4499. Fix this if this is the 
    # projection. 
    if(re.search("Polar_Stereographic", p)):
        p = re.sub('PARAMETER\["scale_factor",0\]', 
                   'PARAMETER["scale_factor",1]', p)
        f.SetProjection(p)
    # The standard_parallel_1 and latitude_of_origin are sometimes swapped for
    # HiRISE. Examples I've seen have standard_parallel_1 0, so also possible
    # this is just a mislabeling of standard_parallel_1 as latitude_of_origin,
    # combined with gdal or some other tool filling in the missing
    # standard_parallel_1 with default value of 0. In any case, we swap
    # both values since this doesn't hurt even if standard_parallel_1 is
    # always zero.
    #
    # Not sure if this swap/misname is always the case, or just with some data.
    # The ticket https://trac.osgeo.org/gdal/ticket/2706 seems to talk about
    # this to some level. We have a heuristic here which checks to a name
    # "Equirectangular MARS" or "Equirectangular_MARS", and if found we swap
    # the standard_parallel_1 and latitude_of_origin. Since I'm not sure that
    # this won't corrupt some actually good data, we only do this if the
    # user selected the --hirise-correction option
    if(hirise_correction and
       re.search("Equirectangular[_ ]MARS", p)):
        m1 = re.search('PARAMETER\["latitude_of_origin",([\\d\\.]+)\]', p)
        m2 = re.search('PARAMETER\["standard_parallel_1",([\\d\\.]+)\]', p)
        if(m1 and m2 and m1.group(1) != m2.group(1)):
            print("MSG:----------------------------------------------------\n")
            print("MSG: Doing HiRISE correction of swapping latitude_of_origin and standard_parallel_1 for the Equirectangular projection")
            sys.stdout.flush()
            p = re.sub('PARAMETER\["latitude_of_origin",[\\d\\.]+\]',
                       'PARAMETER["latitude_of_origin",' + m2.group(1) + ']',
                       p)
            p = re.sub('PARAMETER\["standard_parallel_1",[\\d\\.]+\]',
                       'PARAMETER["standard_parallel_1",' + m1.group(1) + ']',
                       p)
            f.SetProjection(p)
            
    # Force close/flush
    f = None
    
    

__all__ = ["makedirs_p", "cib01_data", "cib01_mapinfo", "planet_mapinfo",
           "pid_exists", "comment_remover", "run_tee", "mars_fix_projection"]
