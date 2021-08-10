import geocal_swig

# Add some useful functions to IgcMsp

def _check_plugin(cls, f, iid1, plugin, check_corner=True,
                  corner_tol=1.0):
    '''This does a basic check of using the MSP library to read the
    data for a file. This is meant for diagnostic purposes, there are
    all kinds of reasons why a file we generate doesn't work with MSP.
    We can optionally check the corners stored in CSCRNA TRE.
    
    f should be a pynitf.NitfFile.

    We simple print out information about using this plugin, and then return
    a generic True or False to indicate if the plugin worked. We may want
    to change this over time, but right now this is the interface that makes
    the most sense'''
    desc = "file '%s' image id '%s' plugin '%s'" % (f.file_name, iid1, plugin)
    try:
        iseg = f.iseg_by_iid1(iid1)
        if(len(iseg) == 0):
            print("Image id not found in " + desc)
            return False
        if(len(iseg) > 1):
            print("More than one image id found in " + desc)
            return False
        iseg = iseg[0]
        segindx = [t for t, isg in enumerate(f.image_segment) if isg is iseg][0]
        try:
            igc = geocal_swig.IgcMsp(f.file_name, geocal_swig.SimpleDem(),
                                     segindx, plugin, plugin)
        except RuntimeError as e:
            print("MSP failed to read " + desc)
            return False
        if(check_corner):
            t = iseg.find_tre("CSCRNA")
            if(len(t) == 0):
                print("Could not find CSCRNA " + desc)
                return False
            if(len(t) > 1):
                print("Found more than one CSCRNA " + desc)
                return False
            t = t[0]
            gpt1 = geocal_swig.Geodetic(t.ulcnr_lat, t.ulcnr_long, t.ulcnr_ht)
            gpt2 = geocal_swig.Geodetic(t.urcnr_lat, t.urcnr_long, t.urcnr_ht)
            gpt3 = geocal_swig.Geodetic(t.lrcnr_lat, t.lrcnr_long, t.lrcnr_ht)
            gpt4 = geocal_swig.Geodetic(t.llcnr_lat, t.llcnr_long, t.llcnr_ht)
            ic1 = igc.image_coordinate(gpt1)
            ic2 = igc.image_coordinate(gpt2)
            ic3 = igc.image_coordinate(gpt3)
            ic4 = igc.image_coordinate(gpt4)
            print("ic1: ", ic1)
            print("ic2: ", ic2)
            print("ic3: ", ic3)
            print("ic4: ", ic4)
            nline = iseg.subheader.nrows
            nsamp = iseg.subheader.ncols
            print("Image %d x %d" % (nline,nsamp))
            if(abs(ic1.line - 0) > corner_tol or
               abs(ic1.sample - 0) > corner_tol or
               abs(ic2.line - 0) > corner_tol or
               abs(ic2.sample - (nsamp-1)) > corner_tol or
               abs(ic3.line - (nline-1)) > corner_tol or
               abs(ic3.sample - (nsamp-1)) > corner_tol or
               abs(ic4.line - (nline-1)) > corner_tol or
               abs(ic4.sample - 0) > corner_tol):
                print("Corner tolerance not met " + desc)
                return False
            print("Corner tolerance met " + desc)
        print("MSP succeeded to read " + desc)
        return True
    except RuntimeError as e:
        print("General failure for " + desc)
        print(e)
        return False
                                                               
geocal_swig.IgcMsp.check_plugin = classmethod(_check_plugin)
