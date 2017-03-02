from .nitf_file import *
from test_support import *
import subprocess

def test_basic_read():
    f = NitfFile(unit_test_data + "sample.ntf")
    assert len(f.image_segment) == 1
    img = f.image_segment[0].data
    assert img.data.shape == (10,10)
    for i in range(10):
        for j in range(10):
            assert img.data[i,j] == i + j
    if(False):
        print(f)

def test_basic_write(isolated_dir):
    f = NitfFile()
    img = NitfImageFromNumpy(nrow=10, ncol=10)
    for i in range(10):
        for j in range(10):
            img.data[i,j] = i + j
    f.image_segment.append(NitfImageSegment(img))
    f.write("z.ntf")
    f2 = NitfFile("z.ntf")
    assert len(f2.image_segment) == 1
    img = f2.image_segment[0].data
    assert img.data.shape == (10,10)
    for i in range(10):
        for j in range(10):
            assert img.data[i,j] == i + j
    
    # Print out diagnostic information, useful to make sure the file
    # we generate is valid.
    if(True):
        print(f)
        # Don't fail if the command does exist, but if we have it run
        # gdalinfo and show_nitf++ on the file
        #
        # gdalinfo is part of GDAL, show_nitf++ is part of Nitro
        if(cmd_exists("gdalinfo")):
            if sys.version_info > (3,):
                subprocess.run(["gdalinfo", "z.ntf"])
            else:
                subprocess.call(["gdalinfo", "z.ntf"])
        if(cmd_exists("show_nitf++")):
            if sys.version_info > (3,):
                subprocess.run(["show_nitf++", "z.ntf"])
            else:
                subprocess.call(["show_nitf++", "z.ntf"])
                
    
