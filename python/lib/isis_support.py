import os
from geocal_swig import GdalRasterImage, SpiceKernelList
import json
import re
import subprocess

def setup_isis():
    '''Set up ISISROOT and ISISDATA environment variables with defaults, 
    if not already set'''
    if("ISISROOT" not in os.environ):
        if(os.path.exists("/opt/isis")):
            os.environ["ISISROOT"] = "/opt/isis"
        else:
            raise RuntimeError("Need to set ISISROOT environment variable")
    if("ISISDATA" not in os.environ):
        if(os.path.exists("/opt/afids/data/isisdata")):
            os.environ["ISISDATA"] = "/opt/afids/data/isisdata"
        else:
            raise RuntimeError("Need to set ISISDATA environment variable")

def read_kernel_from_isis(fname, Skip_load=True):
    '''This takes a ISIS cube file which has had spiceinit run on it,
    and determines the kernels listed in the file. Returned as a 
    SpiceKernelList, which has *not* been loaded yet.'''
    f = GdalRasterImage(fname)
    d = json.loads(f.metadata_list('json:ISIS3')[0])
    keylist = ['LeapSecond', 'TargetAttitudeShape', 'TargetPosition',
               'InstrumentPointing', 'SpacecraftClock', 'InstrumentPosition',
               'InstrumentAddendum', 'Instrument']
    kd = d['IsisCube']['Kernels']
    klist = []
    for k in keylist:
        t = kd[k]
        if t == 'Null':
            continue
        if isinstance(t, list):
            klist.extend(t[1:])
        else:
            klist.append(t)
    return(SpiceKernelList(klist, Skip_load))

def find_isis_kernel_file(f):
    '''This finds a single ISIS kernel file. See find_isis_kernel, which
    is probably the function you actually want to be using.'''
    setup_isis()
    if(os.path.exists(f)):
        return f
    spice_cache_dir = os.environ.get('SPICECACHE',
                                     os.path.expanduser("~/.spice_cache"))
    sublist = [('\$base',
                f"{os.environ['ISISDATA']}/base",
                f"{spice_cache_dir}/base",
                "isisdist.astrogeology.usgs.gov::isisdata/data/base",
                ),
               ('\$mro',
                f"{os.environ['ISISDATA']}/mro",
                f"{spice_cache_dir}/mro",
                "isisdist.astrogeology.usgs.gov::isisdata/data/mro"
                )
               ]
    for r,s,_,_ in sublist:
        t = re.sub(r,s,f)
        if(os.path.exists(t)):
            return t
    for r,_,s,s2 in sublist:
        t,c = re.subn(r,s,f)
        if(c == 0):
            continue
        if(os.path.exists(t)):
            return t
        t2 = re.sub(r,s2,f)
        subprocess.run(["mkdir","-p",os.path.dirname(t)],check=True)
        subprocess.run(["rsync", "-lptgoD", t2, t], check=True,
                       stdout=subprocess.DEVNULL)
        return t
    raise RuntimeError(f"Can't find kernel {f}")
    

def find_isis_kernel(klist, Skip_load=True):
    '''This takes a SpiceKernelList which might have various
    variables like $base or $mro that spiceinit uses. We then
    try to find that file on the system (in ISISDATA or SPICECACHE).
    If it isn't already on the system, we download it from the ISIS
    server.

    By default we don't actually load the kernels.'''
    return SpiceKernelList([find_isis_kernel_file(k)
                            for k in klist.kernel_list], Skip_load)

def pds_to_isis(pds_fname, isis_fname):
    '''Take a unprojected PDS file, and import it into an ISIS file - based 
    on the instrument listed the PDS file.'''
    f = GdalRasterImage(pds_fname)
    inst_id = f['INSTRUMENT_ID']
    if(inst_id == "CTX"):
        setup_isis()
        subprocess.run([f"{os.environ['ISISROOT']}/bin/mroctx2isis",
                        f"from={pds_fname}", f"to={isis_fname}"],
                       check=True)
        subprocess.run([f"{os.environ['ISISROOT']}/bin/spiceinit",
                        f"from={isis_fname}", "web=true"])
    elif(inst_id == "HIRISE"):
        raise RuntimeError("Not supported yet")
    else:
        raise RuntimeError(f"Unrecognized instrument ID '{inst_id}' found in file {pds_fname}")

__all__ = ["setup_isis", "read_kernel_from_isis", "find_isis_kernel_file",
           "find_isis_kernel", "pds_to_isis"]        
