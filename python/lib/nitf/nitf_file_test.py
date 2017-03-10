from .nitf_file import *
from .nitf_tre_csde import *
from .nitf_tre_piae import *
from .nitf_tre_rpc import *
from .nitf_tre_geosde import *
from test_support import *
import subprocess

# Do these in a few places, so collect in one spot.
def create_image_seg(f):
    img = NitfImageFromNumpy(nrow=10, ncol=10)
    for i in range(10):
        for j in range(10):
            img.data[i,j] = i + j
    f.image_segment.append(NitfImageSegment(img))

def create_tre(f):
    t = TreUSE00A()
    t.angle_to_north = 270
    t.mean_gsd = 105.2
    t.dynamic_range = 2047
    t.obl_ang = 34.12
    t.roll_ang = -21.15
    t.n_ref = 0
    t.rev_num = 3317
    t.n_seg = 1
    t.max_lp_seg = 6287
    t.sun_el = 68.5
    t.sun_az = 131.3
    f.tre_list.append(t)

def create_tre2(f):
    t = TreUSE00A()
    t.angle_to_north = 290
    t.mean_gsd = 105.2
    t.dynamic_range = 2047
    t.obl_ang = 34.12
    t.roll_ang = -21.15
    t.n_ref = 0
    t.rev_num = 3317
    t.n_seg = 1
    t.max_lp_seg = 6287
    t.sun_el = 68.5
    t.sun_az = 131.3
    f.tre_list.append(t)
    
def check_tre(t):    
    assert t.tre_tag == "USE00A"
    assert t.angle_to_north == 270
    assert_almost_equal(t.mean_gsd, 105.2)
    assert t.dynamic_range == 2047
    assert_almost_equal(t.obl_ang, 34.12)
    assert_almost_equal(t.roll_ang, -21.15)
    assert t.n_ref == 0
    assert t.rev_num == 3317
    assert t.n_seg == 1
    assert t.max_lp_seg == 6287
    assert_almost_equal(t.sun_el, 68.5)
    assert_almost_equal(t.sun_az, 131.3)

def check_tre2(t):    
    assert t.tre_tag == "USE00A"
    assert t.angle_to_north == 290
    assert_almost_equal(t.mean_gsd, 105.2)
    assert t.dynamic_range == 2047
    assert_almost_equal(t.obl_ang, 34.12)
    assert_almost_equal(t.roll_ang, -21.15)
    assert t.n_ref == 0
    assert t.rev_num == 3317
    assert t.n_seg == 1
    assert t.max_lp_seg == 6287
    assert_almost_equal(t.sun_el, 68.5)
    assert_almost_equal(t.sun_az, 131.3)
    
def print_diag(f):
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
                subprocess.run(["gdalinfo", "-mdd", "xml:TRE", "z.ntf"])
            else:
                subprocess.call(["gdalinfo", "-mdd", "xml:TRE", "z.ntf"])
        if(cmd_exists("show_nitf++")):
            if sys.version_info > (3,):
                subprocess.run(["show_nitf++", "z.ntf"])
            else:
                subprocess.call(["show_nitf++", "z.ntf"])
    
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

def test_read_tre():
    f = NitfFile(unit_test_data + "test_use00a.ntf")
    assert len(f.image_segment) == 1
    assert len(f.image_segment[0].tre_list) == 1
    check_tre(f.image_segment[0].tre_list[0])
    if(False):
        print(f)
        
def test_basic_write(isolated_dir):
    f = NitfFile()
    create_image_seg(f)
    create_tre(f)
    create_tre2(f.image_segment[0])
    f.write("z.ntf")
    f2 = NitfFile("z.ntf")
    assert len(f2.image_segment) == 1
    img = f2.image_segment[0].data
    assert img.data.shape == (10,10)
    for i in range(10):
        for j in range(10):
            assert img.data[i,j] == i + j
    assert len(f2.tre_list) == 1
    assert len(f2.image_segment[0].tre_list) == 1
    check_tre(f2.tre_list[0])
    check_tre2(f2.image_segment[0].tre_list[0])
    print_diag(f2)

def test_large_tre_write(isolated_dir):
    '''Repeat of test_basic_write, but also include a really big TRE that
    forces the use of the second place in the header for TREs'''
    desc = ["BIGTRE",
            ["big_field", 99999-20, str]
            ]
    TreBig = create_nitf_tre_structure("TreBig", desc)
    f = NitfFile()
    create_image_seg(f)
    f.tre_list.append(TreBig())
    f.image_segment[0].tre_list.append(TreBig())
    create_tre(f)
    create_tre2(f.image_segment[0])
    f.write("z.ntf")
    f2 = NitfFile("z.ntf")
    assert len(f2.image_segment) == 1
    img = f2.image_segment[0].data
    assert img.data.shape == (10,10)
    for i in range(10):
        for j in range(10):
            assert img.data[i,j] == i + j
    assert len(f2.tre_list) == 2
    assert len(f2.image_segment[0].tre_list) == 2
    check_tre([tre for tre in f2.tre_list if tre.tre_tag == "USE00A"][0])
    check_tre2([tre for tre in f2.image_segment[0].tre_list if tre.tre_tag == "USE00A"][0])
    print_diag(f2)

def test_tre_overflow_write(isolated_dir):
    '''Repeat of test_basic_write, but also include two really big TREs that
    forces the use of the DES TRE overflow for TREs'''
    desc = ["BIGTRE",
            ["big_field", 99999-20, str]
            ]
    TreBig = create_nitf_tre_structure("TreBig", desc)
    f = NitfFile()
    create_image_seg(f)
    f.tre_list.append(TreBig())
    f.tre_list.append(TreBig())
    f.image_segment[0].tre_list.append(TreBig())
    f.image_segment[0].tre_list.append(TreBig())
    create_tre(f)
    create_tre2(f.image_segment[0])
    f.write("z.ntf")
    f2 = NitfFile("z.ntf")
    assert len(f2.image_segment) == 1
    img = f2.image_segment[0].data
    assert img.data.shape == (10,10)
    for i in range(10):
        for j in range(10):
            assert img.data[i,j] == i + j
    assert len(f2.tre_list) == 3
    assert len(f2.image_segment[0].tre_list) == 3
    check_tre([tre for tre in f2.tre_list if tre.tre_tag == "USE00A"][0])
    check_tre2([tre for tre in f2.image_segment[0].tre_list if tre.tre_tag == "USE00A"][0])
    print_diag(f2)
    
def test_read_quickbird():
    '''Test reading a quickbird NITF file.'''
    # This is a large file, which we don't want to depend on. Run this
    # test if we happen to find the data, but skip otherwise.
    fname = "/raid21/quickbird/05NOV23034644-P1BS-005545406180_01_P001.NTF"
    try: 
        f = NitfFile(fname)
        print(f)
    except FileNotFoundError:
        pass

def test_read_worldview():
    '''Test reading a worldview NITF file.'''
    # This is a large file, which we don't want to depend on. Run this
    # test if we happen to find the data, but skip otherwise.
    fname = "/raid23/worldview/nagoya/12JAN23015358-P1BS-052654848010_01_P003.NTF"
    try: 
        f = NitfFile(fname)
        print(f)
    except FileNotFoundError:
        pass
    

def test_read_ikonos():
    '''Test reading a ikonos NITF file.'''
    # This is a large file, which we don't want to depend on. Run this
    # test if we happen to find the data, but skip otherwise.
    fname = "/raid20/11DEC11IK0101000po_755166_pan_0000000.ntf"
    try: 
        f = NitfFile(fname)
        print(f)
    except FileNotFoundError:
        pass
    
    
