import os
from geocal_swig import GdalRasterImage, SpiceKernelList, gdal_driver_name
from .priority_handle_set import GeoCalPriorityHandleSet
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
               'InstrumentPointing', 'Instrument', 'SpacecraftClock',
               'InstrumentPosition', 'InstrumentAddendum']
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
    sublist = [(r'\$base',
                f"{os.environ['ISISDATA']}/base",
                f"{spice_cache_dir}/base",
                "isisdist.astrogeology.usgs.gov::isisdata/data/base",
                ),
               (r'\$mro',
                f"{os.environ['ISISDATA']}/mro",
                f"{spice_cache_dir}/mro",
                "isisdist.astrogeology.usgs.gov::isisdata/data/mro"
                ),
               (r'\$lro',
                f"{os.environ['ISISDATA']}/lro",
                f"{spice_cache_dir}/lro",
                "isisdist.astrogeology.usgs.gov::isisdata/data/lro"
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

class PdsToIsisHandleSet(GeoCalPriorityHandleSet):
    '''Handle set for pds to isis.

    Note this is a bit of overkill, we have little more than a 
    switch on INSTRUMENT_ID. But since we already have the machinery
    here, go ahead and put this in place. If nothing else, it allows 
    downstream programs to add handles for new instruments.
    '''
    def handle_h(self, h, fin, pds_fname, isis_fname, pds_fname2):
        return h.pds_to_isis(fin, pds_fname, isis_fname, pds_fname2 = pds_fname2)

class CtxPdsToIsis:
    '''The data format is described at https://pds-imaging.jpl.nasa.gov/data/mro/mars_reconnaissance_orbiter/ctx/mrox_0001/document/ctxsis.pdf if you
    need information about the labels.'''
    def pds_to_isis(self, fin, pds_fname, isis_fname, pds_fname2 = None):
        if(fin['INSTRUMENT_ID'] != "CTX"):
            return (False, None)
        setup_isis()
        subprocess.run([f"{os.environ['ISISROOT']}/bin/mroctx2isis",
                        f"from={pds_fname}", f"to={isis_fname}"],
                       check=True)
        subprocess.run([f"{os.environ['ISISROOT']}/bin/spiceinit",
                        f"from={isis_fname}", "web=true"],
                       check=True, stdout=subprocess.DEVNULL)
        return (True, isis_fname)

PdsToIsisHandleSet.add_default_handle(CtxPdsToIsis())

class HrscPdsToIsis:
    def pds_to_isis(self, fin, pds_fname, isis_fname, pds_fname2 = None):
        if(fin['INSTRUMENT_ID'] != "HRSC"):
            return (False, None)
        setup_isis()
        subprocess.run([f"{os.environ['ISISROOT']}/bin/hrsc2isis",
                        f"from={pds_fname}", f"to={isis_fname}"],
                       check=True)
        subprocess.run([f"{os.environ['ISISROOT']}/bin/spiceinit",
                        f"from={isis_fname}", "web=true"],
                       check=True, stdout=subprocess.DEVNULL)
        return (True, isis_fname)

PdsToIsisHandleSet.add_default_handle(HrscPdsToIsis())

class LroPdsToIsis:
    '''The data format is described at http://pds.lroc.asu.edu/data/LRO-L-LROC-2-EDR-V1.0/LROLRC_0001/DOCUMENT/LROCSIS.PDF if you need information about the labels.'''
    def pds_to_isis(self, fin, pds_fname, isis_fname, pds_fname2 = None):
        if(fin['INSTRUMENT_ID'] != "LROC"):
            return (False, None)

        # Interestingly there isn't any direct indication of NAC vs
        # WAC. You would think that there would be a basic instrument
        # id or something like that, but there isn't. Instead we look for
        # the field 'INSTRUMENT_MODE_ID', it is present for WAC but
        # not for NAC
        d = json.loads(fin.metadata_list('json:PDS')[0])
        setup_isis()
        if("INSTRUMENT_MODE_ID" in d):
            self.pds_wac_to_isis(fin, pds_fname, isis_fname)
        else:
            if(fin["PRODUCT_TYPE"] == "EDR"):
                self.pds_edr_nac_to_isis(fin, pds_fname, isis_fname)
            else:
                self.pds_cdr_nac_to_isis(fin, pds_fname, isis_fname)
                
        # Think we only need spiceinit on one file, we'll see if that
        # is enough
        subprocess.run([f"{os.environ['ISISROOT']}/bin/spiceinit",
                        f"from={isis_fname}", "web=true"],
                       check=True, stdout=subprocess.DEVNULL)
        return (True, isis_fname)

    def pds_wac_to_isis(self, fin, pds_fname, isis_fname):
        subprocess.run([f"{os.environ['ISISROOT']}/bin/lrowac2isis",
                        f"from={pds_fname}", f"to={isis_fname}"],
                       check=True)
        # Not sure exactly how to handle this. lrowac2isis produces 4
        # output files, not just one. For now leave as 4 and we'll
        # possibly figure out how to combine this. We return
        # create a symbolic link for one of the files, just so we
        # have a pointer to is.
        t = os.path.splitext(isis_fname)[0]
        try:
            os.remove(isis_fname)
        except:
            pass
        os.symlink(f"{t}.vis.even.cub", isis_fname)
        # For now, run spiceinit on the other cubes. Not sure if
        # we need this long term, but do for now
        subprocess.run([f"{os.environ['ISISROOT']}/bin/spiceinit",
                        f"from={t}.vis.odd.cub", "web=true"],
                       check=True, stdout=subprocess.DEVNULL)
        subprocess.run([f"{os.environ['ISISROOT']}/bin/spiceinit",
                        f"from={t}.uv.even.cub", "web=true"],
                       check=True, stdout=subprocess.DEVNULL)
        subprocess.run([f"{os.environ['ISISROOT']}/bin/spiceinit",
                        f"from={t}.uv.odd.cub", "web=true"],
                       check=True, stdout=subprocess.DEVNULL)

    def pds_edr_nac_to_isis(self, fin, pds_fname, isis_fname):
        subprocess.run([f"{os.environ['ISISROOT']}/bin/lronac2isis",
                        f"from={pds_fname}", f"to={isis_fname}"],
                       check=True)

    def pds_cdr_nac_to_isis(self, fin, pds_fname, isis_fname):
        raise RuntimeError("We don't currently support CDR format for NAC. Not sure if ISIS supports this, we'll need to dig a bit to see.")
        subprocess.run([f"{os.environ['ISISROOT']}/bin/pds2isis",
                        f"from={pds_fname}", f"to={isis_fname}"],
                       check=True)
        
PdsToIsisHandleSet.add_default_handle(LroPdsToIsis())

class DummyPdsToIsis:
    '''Degenerate case of file already being a isis file'''
    def pds_to_isis(self, fin, pds_fname, isis_fname, pds_fname2 = None):
        if(gdal_driver_name(pds_fname) != "ISIS3"):
            return (False, None)
        # Degenerate case of output file being the same as input
        if(not os.path.exists(isis_fname)):
            os.symlink(pds_fname, isis_fname)
        elif(not os.path.samefile(pds_fname, isis_fname)):
            raise RuntimeError(f"File f{isis_fname} already exists when trying to import f{pds_fname}")
        return (True, isis_fname)

PdsToIsisHandleSet.add_default_handle(DummyPdsToIsis(), priority_order=1000)

class HirisePdsToIsis:
    '''Documentation in HiRISE format is at https://hirise-pds.lpl.arizona.edu/PDS/DOCUMENT/HIRISE_EDR_SIS.PDF'''
    def pds_to_isis(self, fin, pds_fname, isis_fname, pds_fname2 = None):
        if((fin['INSTRUMENT_ID'] != "HIRISE" and
            fin['INSTRUMENT_ID'] != '"HIRISE"') or
           pds_fname2 is None):
            return (False, None)
        setup_isis()
        fbase, _ = os.path.splitext(isis_fname)
        subprocess.run([f"{os.environ['ISISROOT']}/bin/hi2isis",
                        f"from={pds_fname}", f"to={fbase}_f1.cub"],
                       check=True, stdout=subprocess.DEVNULL)
        subprocess.run([f"{os.environ['ISISROOT']}/bin/hi2isis",
                        f"from={pds_fname2}", f"to={fbase}_f2.cub"],
                       check=True, stdout=subprocess.DEVNULL)
        subprocess.run([f"{os.environ['ISISROOT']}/bin/spiceinit",
                        f"from={fbase}_f1.cub", "web=true"], check=True,
                       stdout=subprocess.DEVNULL)
        subprocess.run([f"{os.environ['ISISROOT']}/bin/spiceinit",
                        f"from={fbase}_f2.cub", "web=true"], check=True,
                       stdout=subprocess.DEVNULL)
        subprocess.run([f"{os.environ['ISISROOT']}/bin/hical",
                        f"from={fbase}_f1.cub", f"to={fbase}_f1_cal.cub"],
                       check=True, stdout=subprocess.DEVNULL)
        subprocess.run([f"{os.environ['ISISROOT']}/bin/hical",
                        f"from={fbase}_f2.cub", f"to={fbase}_f2_cal.cub"],
                       check=True, stdout=subprocess.DEVNULL)
        subprocess.run([f"{os.environ['ISISROOT']}/bin/histitch",
                        f"from1={fbase}_f1_cal.cub",
                        f"from2={fbase}_f2_cal.cub",
                        f"to={isis_fname}"], check=True,
                       stdout=subprocess.DEVNULL)
        # TODO We could possibly do cubenorm if desired. Perhaps as an option?
        return (True, isis_fname)

PdsToIsisHandleSet.add_default_handle(HirisePdsToIsis())
    
def pds_to_isis(pds_fname, isis_fname, pds_fname2 = None):
    '''Take a unprojected PDS file, and import it into an ISIS file - based 
    on the instrument listed the PDS file.

    Note just to simplify processing, the file passed in can already be a ISIS
    file. In that case, we just create a symbolic link from the input to the
    output file.'''
    f = GdalRasterImage(pds_fname)
    return PdsToIsisHandleSet.default_handle_set().handle(f,pds_fname,isis_fname,pds_fname2)

__all__ = ["setup_isis", "read_kernel_from_isis", "find_isis_kernel_file",
           "find_isis_kernel", "pds_to_isis", "PdsToIsisHandleSet"]        
