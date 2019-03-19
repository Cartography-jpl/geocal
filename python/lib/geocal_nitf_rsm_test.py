try:
    import pynitf
except ImportError:
    pass
from test_support import *
import geocal.geocal_nitf_rsm
from geocal.geocal_nitf_rsm import *
from geocal_swig import (GdalRasterImage, VicarRasterImage,
                         VicarLiteRasterImage, RsmId, RsmRationalPolynomial,
                         Rsm, ImageCoordinate, IgcMsp)
import geocal_swig
import six
import numpy as np
import os, subprocess

def create_image_seg(f):
    img = pynitf.NitfImageWriteNumpy(9, 10, np.uint8)
    img.image_subheader.idlvl = 1
    for i in range(9):
        for j in range(10):
            img[0,i,j] = i + j
    f.image_segment.append(pynitf.NitfImageSegment(img))

@require_pynitf
def test_rsm_rp(isolated_dir, rsm):
    '''Create a file, and write out a RSM. This RSM has just a single 
    rational polynomial in it'''    
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm
    f.write("nitf_rsm.ntf")
    f2 = pynitf.NitfFile("nitf_rsm.ntf")
    print(f2)

@require_msp
@require_pynitf
def test_rsm_generate_with_msp(isolated_dir, igc_rpc):
    '''This uses the MSP toolkit to generate a RSM file, and makes sure
    MSP is happy with it.

    The RSM generation has a API, but for this we are just using the sample
    code. 
    
    We could extend this, but we have our own RSM generation code and probably
    using MSP will be for occasional testing and/or comparison. We wrap this
    up in a test to document how we do this.
    '''

    # --------------------------------
    # 1. Start with an existing NITF file that we can create a sensor model for.
    #    In this case we use an RPC, but this could be anything that MSP
    #    supports (e.g, a GFM model)
    # --------------------------------
    
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rpc = igc_rpc.rpc
    f.write("nitf_input_rpc.ntf")
    msp_base = os.path.dirname(os.environ["MSP_DATA_DIR"])
    rsm_gen = msp_base + "/sampleCode/RsmGeneratorSample/testRsmGeneratorSample"

    # --------------------------------
    # 2. Run generation process.
    #
    #    Note the file extensions are required, it is how rsm_gen knows which
    #    each file is.
    # --------------------------------

    r = subprocess.run([rsm_gen,
                    os.environ["MSP_DATA_DIR"] + "/rsm/database_settings.strat",
                    "rsm_generate_report.rep",
                    "nitf_input_rpc.ntf",
                    "nitf_rsm_generate_out.tre"], stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE)
    with open("rsm_generate.log", "w") as fh:
        print(r.stdout, file=fh)


    # --------------------------------
    # 3. Tool doesn't directly create NITF, instead it write out a file
    #    with the TREs as strings. Parse this, and use to create a RSM.
    #    Note that we assume an order to the TREs, we could generalize
    #    this code if it proves useful.
    # --------------------------------

    with open("nitf_rsm_generate_out.tre") as fh:
        for ln in fh:
            if(ln == "RSM_TRE_DATA\n"):
                break
        # Could generalize this probably, but for now have an assumed form
        # for the RSM
        ln = fh.readline().rstrip("\n")
        if(ln[:6] != "RSMIDA"):
            raise RuntimeError("Didn't find the expected TRE")
        rsm_id = RsmId.read_tre_string(ln[11:])
        ln = fh.readline().rstrip("\n")
        if(ln[:6] != "RSMPCA"):
            raise RuntimeError("Didn't find the expected TRE")
        rsm_rp = RsmRationalPolynomial.read_tre_string(ln[11:])
    rsm = Rsm(rsm_id)
    rsm.rsm_base = rsm_rp

    # --------------------------------
    # 4. Generate a NITF file with the RSM in it.
    # --------------------------------

    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm
    f.write("nitf_rsm.ntf")
    
    # --------------------------------
    # 5. Run the MSP code to compare the RSM to our initial RPC.
    # --------------------------------

    igc_rsm = IgcMsp("nitf_rsm.ntf")
    for i in range(10):
        for j in range(10):
            ic = ImageCoordinate(i,j)
            p1 = igc_rsm.ground_coordinate(ic)
            p2 = igc_rpc.ground_coordinate(ic)
            p3 = rsm.ground_coordinate(ic, igc_rpc.dem)
            assert(geocal_swig.distance(p1, p2) < 0.01)
            assert(geocal_swig.distance(p1, p3) < 0.01)
    
@require_msp
@require_pynitf
def test_rsm_rp_with_msp(isolated_dir, rsm):
    '''Compare the RSM we write to a NITF file with what the MSP library 
    calculates. This verifies both the validity of our NITF and our RSM 
    code'''    
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm
    f.write("nitf_rsm.ntf")
    igc_msp = IgcMsp("nitf_rsm.ntf")
    for h in (rsm.rsm_id.ground_domain_vertex[0].height_reference_surface + 10.0,
              rsm.rsm_id.ground_domain_vertex[7].height_reference_surface - 10.0):
        for ln in np.linspace(0, 1000, 10):
            for smp in np.linspace(0, 1000, 10):
                ic = ImageCoordinate(ln,smp)
                p1 = igc_msp.ground_coordinate_approx_height(ic, h)
                p2 = rsm.ground_coordinate_approx_height(ic, h)
                assert(geocal_swig.distance(p1, p2) < 0.01)

@require_msp
@require_pynitf
def test_rsm_lc_rp_with_msp(isolated_dir, rsm_lc):
    '''Compare the RSM we write to a NITF file with what the MSP library 
    calculates. This verifies both the validity of our NITF and our RSM 
    code'''    
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm_lc
    f.write("nitf_rsm.ntf")
    igc_msp = IgcMsp("nitf_rsm.ntf")
    for h in (rsm_lc.rsm_id.ground_domain_vertex[0].height_reference_surface + 10.0,
              rsm_lc.rsm_id.ground_domain_vertex[7].height_reference_surface - 10.0):
        for ln in np.linspace(0, 1000, 10):
            for smp in np.linspace(0, 1000, 10):
                ic = ImageCoordinate(ln,smp)
                p1 = igc_msp.ground_coordinate_approx_height(ic, h)
                p2 = rsm_lc.ground_coordinate_approx_height(ic, h)
                p3 = rsm_lc.ground_coordinate_z(ic, -5294.140559317827)
                assert(geocal_swig.distance(p1, p2) < 0.01)
                
@require_pynitf
@require_vicar
@require_serialize
def test_vicar_rsm(isolated_dir, rsm):
    '''Test that we can read and write RSM attached to a VicarRasterImage
    and VicarLiteRasterImage. This requires everything to be installed, so
    we can't check this at the C++ level (which runs unit tests before 
    installing'''
    out = VicarRasterImage("vicar_rsm_nitf.img", "BYTE", 10, 11, 1)
    out.write(0,0,np.zeros((10,11),dtype=np.uint8))
    out.set_rsm(rsm)
    out = None
    fin = VicarRasterImage("vicar_rsm_nitf.img")
    print(fin.rsm)
    fin = VicarLiteRasterImage("vicar_rsm_nitf.img")
    print(fin.rsm)
    
    
@require_pynitf
def test_rsm_grid(isolated_dir, rsm_g):
    '''Create a file, and write out a RSM. This RSM has just a single 
    grid in it'''    
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm_g
    f.write("nitf_rsm.ntf")
    f2 = pynitf.NitfFile("nitf_rsm.ntf")
    print(f2)

@require_pynitf
def test_rsm_ms_rp(isolated_dir, rsm_ms_rp):
    '''Create a file, and write out a RSM. This has a multi section
    rational polynomial in it'''    
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm_ms_rp
    f.write("nitf_rsm.ntf")
    f2 = pynitf.NitfFile("nitf_rsm.ntf")
    print(f2)

@require_pynitf
def test_rsm_ms_g(isolated_dir, rsm_ms_g):
    '''Create a file, and write out a RSM. This has a multi section
    grid in it'''    
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].rsm = rsm_ms_g
    f.write("nitf_rsm.ntf")
    f2 = pynitf.NitfFile("nitf_rsm.ntf")
    print(f2)

# Test the raw TREs
@require_pynitf
def test_tre_rsmpca(rsm_rational_polynomial):
    t = geocal.geocal_nitf_rsm.TreRSMPCA_geocal()
    t.rsm_rational_polynomial = rsm_rational_polynomial
    t.update_raw_field()
    fh = six.BytesIO()
    t.write_to_file(fh)
    print(fh.getvalue())
    fh2 = six.BytesIO(fh.getvalue())
    t2 = geocal.geocal_nitf_rsm.TreRSMPCA_geocal()
    t2.read_from_file(fh2)
    print(t2)
    # Check that we can't the field values.
    with pytest.raises(RuntimeError):
        t.iid = 'abc'
    with pytest.raises(RuntimeError):
        t.rnpcf[0] = 1.0

    assert t.iid is None
    assert t2.iid is None
    assert t.rsn == 1
    assert t2.rsn == 1
    assert t.csn == 1
    assert t2.csn == 1
    assert t.rfep is None
    assert t2.rfep is None
    assert t.cfep is None
    assert t2.cfep is None
    assert t.rnrmo == 2881.0
    assert t2.rnrmo == 2881.0

@require_pynitf
def test_tre_rsmgga(rsm_grid):
    t = geocal.geocal_nitf_rsm.TreRSMGGA_geocal()
    t.rsm_grid = rsm_grid
    t.update_raw_field()
    fh = six.BytesIO()
    t.write_to_file(fh)
    # This is way too large to check, so skip this
    #assert fh.getvalue() == b'blah'
    fh2 = six.BytesIO(fh.getvalue())
    t2 = geocal.geocal_nitf_rsm.TreRSMGGA_geocal()
    t2.read_from_file(fh2)
    print(t2)
    # Check that we can't the field values.
    with pytest.raises(RuntimeError):
        t.iid = 'abc'
    with pytest.raises(RuntimeError):
        t.rcoord[0,0] = "1.0"

    assert t.iid is None
    assert t.deltaz == 1268.0
    
@require_pynitf
def test_tre_rsmpia(rsm_ms_polynomial):
    t = geocal.geocal_nitf_rsm.TreRSMGIA_geocal()
    t.rsm_multi_section = rsm_ms_polynomial
    t.update_raw_field()
    fh = six.BytesIO()
    t.write_to_file(fh)
    # This can vary depending on roundoff, so don't compare.
    #assert fh.getvalue() == b'Blah'
    fh2 = six.BytesIO(fh.getvalue())
    t2 = geocal.geocal_nitf_rsm.TreRSMGIA_geocal()
    t2.read_from_file(fh2)
    print(t2)
    assert t2.rsm_multi_section.number_row_section == 3
    assert t2.rsm_multi_section.number_col_section == 2
    assert t2.gcssiz == 13763.0

@require_pynitf
def test_tre_rsmpia(rsm_ms_polynomial):
    t = geocal.geocal_nitf_rsm.TreRSMPIA_geocal()
    t.rsm_multi_section = rsm_ms_polynomial
    t.update_raw_field()
    fh = six.BytesIO()
    t.write_to_file(fh)
    # This can vary depending on roundoff, so don't compare.
    #assert fh.getvalue() == b'Blah'
    fh2 = six.BytesIO(fh.getvalue())
    t2 = geocal.geocal_nitf_rsm.TreRSMPIA_geocal()
    t2.read_from_file(fh2)
    print(t2)
    assert t2.rsm_multi_section.number_row_section == 3
    assert t2.rsm_multi_section.number_col_section == 2
    assert t2.cssiz == 13763.0

@require_pynitf
def test_tre_rsmida(rsm):
    t = geocal.geocal_nitf_rsm.TreRSMIDA_geocal()
    t.rsm_id = rsm.rsm_id
    t.update_raw_field()
    fh = six.BytesIO()
    t.write_to_file(fh)
    # This can vary depending on roundoff, so don't compare.
    #assert fh.getvalue() == b'Blah'
    fh2 = six.BytesIO(fh.getvalue())
    t2 = geocal.geocal_nitf_rsm.TreRSMIDA_geocal()
    t2.read_from_file(fh2)
    print(t2)
    assert t2.rsm_id.rsm_suport_data_edition == "fake-1"
    
# The RSM sample data has expected results as a spreadsheet. We grab these
# values and put them here.
#
# Note, there seems to be clear problems with the spreadsheet. There are
# several places where we have what looks like duplicate data in the
# the latitude for different points, and where we have data that doesn't agree
# with its X,Y,Z calculation. I'm fairly certain the problem is with
# the test data spreadsheet as opposed to my code, if we convert to the X,Y,Z
# coordinates we can clearly see differences with the points with the duplicate
# data. I've commented out what looks like bad points.

# 1. Point calculation

# This is row, col, height, latitude, longitude
pcalc = dict()
pcalc["i_6130a.ntf"] = \
[
    [0, 0, -500, 33.1616263666, -117.0386236785],
    [1024, 0, -500, 33.1616140061, -117.0337581906],
    [2048, 1024, -500, 33.1657014400, -117.0288590590],
    [3072, 2048, -500, 33.1698074925, -117.0239367569],
    [4096, 3072, -500, 33.1739325945, -117.0189907556],
    [5120, 4096, -500, 33.1780771868, -117.0140205142],
    [6144, 5120, -500, 33.1822417202, -117.0090254794],
    [7168, 6144, -500, 33.1864266558, -117.0040050848],
    [8192, 7168, -500, 33.1906324659, -116.9989587503],
    [9216, 8192, -500, 33.1948596339, -116.9938858820],
    [10239, 9215, -500, 33.1991044947, -116.9887908653],
    [0, 0, 0, 33.1617298457, -117.0388169203],
    [1024, 0, 0, 33.1617198102, -117.0348583317],
    [2048, 1024, 0, 33.1650454017, -117.0308724867],
    [3072, 2048, 0, 33.1683861119, -117.0268679557],
    [4096, 3072, 0, 33.1717422449, -117.0228443664],
    [5120, 4096, 0, 33.1751141114, -117.0188013380],
    [6144, 5120, 0, 33.1785020290, -117.0147384814],
    [7168, 6144, 0, 33.1819063224, -117.0106553988],
    [8192, 7168, 0, 33.1853273233, -117.0065516833],
    [9216, 8192, 0, 33.1887653713, -117.0024269189],
    [10239, 9215, 0, 33.1922174304, -116.9982847393],
    [0, 0, 500, 33.1618333074, -117.0390101313],
    [1024, 0, 500, 33.1618255874, -117.0359582996],
    [2048, 1024, 500, 33.1643894424, -117.0328855435],
    [3072, 2048, 500, 33.1669649301, -117.0297985094],
    [4096, 3072, 500, 33.1695522492, -117.0266969546],
    [5120, 4096, 500, 33.1721516024, -117.0235806312],
    #[6144, 5120, 500, 33.1773872434, -117.0204492863],
    [7168, 6144, 500, 33.1773872434, -117.0173026615],
    [8192, 7168, 500, 33.1800239580, -117.0141404932],
    [9216, 8192, 500, 33.1826735607, -117.0109625121],
    [10239, 9215, 500, 33.1853336697, -117.0077715702],
    [2994, 2610, 0, 33.17020003, -117.027196],
    [1995, 7091, 0, 33.1842917, -117.031198],
    [8287, 4901, 0, 33.17798445, -117.005901],
    [8607, 5229, 0, 33.17908728, -117.004603],
    [887, 6410, 0, 33.18205237, -117.035415],
    [2936, 5149, 0, 33.17830643, -117.027508],
    [422, 449, 0, 33.16317745, -117.03719],
    [7218, 1676, 350, 33.1664582, -117.014765],
    [7817, 2259, 35, 33.1691211, -117.008082],
    [1269, 1443, -10, 33.16640833, -117.033891],
    [3508, 7208, -150, 33.18640237, -117.024367],
    [3109, 5259, 200, 33.17715898, -117.02801139],
#    [14, 2453, 100, 33.16924395, -117.038784],
    [4024, 398, -50, 33.16302301, -117.022615],
    [2084, 2918, -250, 33.17217737, -117.029753],
    [6127, 2385, -450, 33.17111665, -117.009412],
    [7742, 3966, -275, 33.17647492, -117.004083],
    [2394, 5686, 25, 33.17974551, -117.029745],
#    [128, 2590, 475, 33.16831576, -117.038602],
    [1603, 2123, 0, 33.16858343, -117.032629],
]
pcalc["i_6130b.ntf"] = \
[                       
    [0, 0, -500, 33.1616263667, -117.0386236785],
    [1024, 0, -500, 33.1616140061, -117.0337581906],
    [2048, 1024, -500, 33.16570143946534, -117.02885906007381],
    [3072, 2048, -500, 33.1698074925, -117.0239367569],
    [4096, 3072, -500, 33.1739325945, -117.0189907556],
    [5120, 4096, -500, 33.1780771868, -117.0140205143],
    [6144, 5120, -500, 33.1822417202, -117.0090254795],
    [7168, 6144, -500, 33.1864266558, -117.0040050848],
    [8192, 7168, -500, 33.1906324659, -116.9989587503],
    [9216, 8192, -500, 33.1948596338, -116.9938858821],
    [10239, 9215, -500, 33.1991044947, -116.9887908652],
    [0, 0, 0, 33.1617298457, -117.0388169203],
    [1024, 0, 0, 33.1617198102, -117.0348583319],
    [2048, 1024, 0, 33.1650454017, -117.0308724867],
    [3072, 2048, 0, 33.1683861119, -117.0268679557],
    [4096, 3072, 0, 33.1717422449, -117.0228443664],
    [5120, 4096, 0, 33.1751141114, -117.0188013380],
    [6144, 5120, 0, 33.1785020290, -117.0147384814],
    [7168, 6144, 0, 33.1819063224, -117.0106553988],
    [8192, 7168, 0, 33.1853273233, -117.0065516833],
    [9216, 8192, 0, 33.1887653713, -117.0024269189],
    [10239, 9215, 0, 33.1922174304, -116.9982847393],
    [0, 0, 500, 33.1618333074, -117.0390101313],
    [1024, 0, 500, 33.1618255874, -117.0359582996],
    [2048, 1024, 500, 33.164389442, -117.0328855435],
    [3072, 2048, 500, 33.1669649301, -117.0297985094],
    [4096, 3072, 500, 33.1695522492, -117.0266969546],
    [5120, 4096, 500, 33.1721516024, -117.0235806312],
    [6144, 5120, 500, 33.1747631968, -117.0204492862],
    [7168, 6144, 500, 33.1773872434, -117.0173026615],
    [8192, 7168, 500, 33.1800239580, -117.0141404932],
    [9216, 8192, 500, 33.1826735607, -117.0109625121],
    [10239, 9215, 500, 33.1853336697, -117.0077715703],
    [4294, 3329, 0, 33.1725837600, -117.0220670000],
    [6166, 5484, 0, 33.1796765300, -117.0146790000],
    [7808, 5517, 0, 33.1799469900, -117.0079590000],
    #[3128, 1589, 0, 33.1668975300, -117.0266300000],
    [5893, 4111, 0, 33.1752176100, -117.0156780000],
    [55, 4246, 0, 33.1752372200, -117.0385720000],
    [7761, 6880, 0, 33.1843452100, -117.0082960000],
    [436, 504, 0, 33.1633549400, -117.0371360000],
    [6304, 8055, 0, 33.1878888900, -117.0143290000],
    [1323, 8052, 0, 33.1871700400, -117.0337730000],
    [1387, 452, 0, 33.1631860100, -117.0334480000],
    [6734, 819, 425, 33.1639738100, -117.0173380000],
    [9100, 5297, 275, 33.1771935300, -117.0072230000],
    [431, 5711, 225, 33.1780172200, -117.0374020000],
    [244, 1824, 100, 33.1673408500, -117.0379510000],
    [5521, 4286, -125, 33.1765348000, -117.0159110000],
    [785, 318, -200, 33.1628054300, -117.0354320000],
    [1551, 7531, -300, 33.1888166400, -117.0319710000],
    [5605, 5103, -325, 33.1808135100, -117.0135290000],
    [6558, 0, -450, 33.1615528500, -117.0070140000],
]
pcalc["i_6130c.ntf"] = \
[
    [0, 0, -500, 33.1616263667, -117.0386236785],
    [1024, 0, -500, 33.1616140061, -117.0337581906],
    #[2048, 1024, -500, 33.1615701440, -117.0288590590],
    [3072, 2048, -500, 33.1698074925, -117.0239367569],
    [4096, 3072, -500, 33.1739325945, -117.0189907556],
    [5120, 4096, -500, 33.1780771868, -117.0140205143],
    [6144, 5120, -500, 33.1822417202, -117.0090254795],
    [7168, 6144, -500, 33.1864266558, -117.0040050848],
    [8192, 7168, -500, 33.1906324659, -116.9989587503],
    [9216, 8192, -500, 33.1948596338, -116.9938858821],
    [10239, 9215, -500, 33.1991044947, -116.9887908652],
    [0, 0, 0, 33.1617298457, -117.0388169203],
    [1024, 0, 0, 33.1617198102, -117.0348583319],
    [2048, 1024, 0, 33.1650454017, -117.0308724867],
    [3072, 2048, 0, 33.1683861119, -117.0268679557],
    [4096, 3072, 0, 33.1717422449, -117.0228443664],
    [5120, 4096, 0, 33.1751141114, -117.0188013380],
    [6144, 5120, 0, 33.1785020290, -117.0147384814],
    [7168, 6144, 0, 33.1819063224, -117.0106553988],
    [8192, 7168, 0, 33.1853273233, -117.0065516833],
    [9216, 8192, 0, 33.1887653713, -117.0024269189],
    [10239, 9215, 0, 33.1922174304, -116.9982847393],
    [0, 0, 500, 33.1618333074, -117.0390101313],
    [1024, 0, 500, 33.1618255874, -117.0359582996],
    #[2048, 1024, 500, 33.1643899442, -117.0328855435],
    [3072, 2048, 500, 33.1669649301, -117.0297985094],
    [4096, 3072, 500, 33.1695522492, -117.0266969546],
    [5120, 4096, 500, 33.1721516024, -117.0235806312],
    [6144, 5120, 500, 33.1747631968, -117.0204492862],
    [7168, 6144, 500, 33.1773872434, -117.0173026615],
    [8192, 7168, 500, 33.1800239580, -117.0141404932],
    [9216, 8192, 500, 33.1826735607, -117.0109625121],
    [10239, 9215, 500, 33.1853336697, -117.0077715703],
    [4870, 5367, 0, 33.1791764000, -117.0198840000],
    [12, 849, 0, 33.1644639800, -117.0387640000],
    [3613, 4533, 0, 33.1764053700, -117.0248280000],
    [8121, 2113, 0, 33.1687577100, -117.0062740000],
    #[4803, 4095, 0, 33.1750885500, -117.0220740000],
    [7880, 8081, 0, 33.1882059000, -117.0079340000],
    [6798, 3763, 0, 33.1741459100, -117.0119490000],
    [1370, 3855, 0, 33.1740931500, -117.0335510000],
    [8489, 3460, 0, 33.1732551800, -117.0048850000],
    [112, 3225, 500, 33.1697743000, -117.0386610000],
    [2077, 3947, 475, 33.1717646600, -117.0327440000],
    [6818, 2649, 300, 33.1693459300, -117.0155990000],
    [5105, 7454, 225, 33.1833867400, -117.0212000000],
    [2165, 1510, 150, 33.1663167800, -117.0310590000],
    [3387, 6169, 75, 33.1809036700, -117.0262600000],
    [9121, 4799, -25, 33.1779019500, -117.0019640000],
    [5744, 8108, -150, 33.1897469600, -117.0149890000],
    [898, 6941, -325, 33.1869001400, -117.0347370000],
    [4367, 5387, -450, 33.1827007400, -117.0182200000],
    [7800, 6978, -475, 33.1895605600, -117.0012810000],
    ]
pcalc["i_6130d.ntf"] = \
[
    [0, 0, -500, 33.1616263667, -117.0386236785],
    [1024, 0, -500, 33.1616140061, -117.0337581906],
    [2048, 1024, -500, 33.1615701440, -117.0288590590],
    [3072, 2048, -500, 33.1698074925, -117.0239367569],
    [4096, 3072, -500, 33.1739325945, -117.0189907556],
    [5120, 4096, -500, 33.1780771868, -117.0140205143],
    [6144, 5120, -500, 33.1822417202, -117.0090254795],
    [7168, 6144, -500, 33.1864266558, -117.0040050848],
    [8192, 7168, -500, 33.1906324659, -116.9989587503],
    [9216, 8192, -500, 33.1948596338, -116.9938858821],
    [10239, 9215, -500, 33.1991044947, -116.9887908652],
    [0, 0, 0, 33.1617298457, -117.0388169203],
    [1024, 0, 0, 33.1617198102, -117.0348583319],
    [2048, 1024, 0, 33.1650454017, -117.0308724867],
    [3072, 2048, 0, 33.1683861119, -117.0268679557],
    [4096, 3072, 0, 33.1717422449, -117.0228443664],
    [5120, 4096, 0, 33.1751141114, -117.0188013380],
    [6144, 5120, 0, 33.1785020290, -117.0147384814],
    [7168, 6144, 0, 33.1819063224, -117.0106553988],
    [8192, 7168, 0, 33.1853273233, -117.0065516833],
    [9216, 8192, 0, 33.1887653713, -117.0024269189],
    [10239, 9215, 0, 33.1922174304, -116.9982847393],
    [0, 0, 500, 33.1618333074, -117.0390101313],
    [1024, 0, 500, 33.1618255874, -117.0359582996],
    [2048, 1024, 500, 33.1643899442, -117.0328855435],
    [3072, 2048, 500, 33.1669649301, -117.0297985094],
    [4096, 3072, 500, 33.1695522492, -117.0266969546],
    [5120, 4096, 500, 33.1721516024, -117.0235806312],
    [6144, 5120, 500, 33.1747631968, -117.0204492862],
    [7168, 6144, 500, 33.1773872434, -117.0173026615],
    [8192, 7168, 500, 33.1800239580, -117.0141404932],
    [9216, 8192, 500, 33.1826735607, -117.0109625121],
    [10239, 9215, 500, 33.1853336697, -117.0077715703],
    [6674, 7506, 0, 33.1862019500, -117.0127900000],
    [5373, 4975, 0, 33.1779661900, -117.0178430000],
    [380, 359, 0, 33.1628869700, -117.0373520000],
    [2611, 6422, 0, 33.1822792000, -117.0288100000],
    [1857, 529, 0, 33.1634360200, -117.0316120000],
    [983, 7743, 0, 33.1861754600, -117.0350570000],
    [1315, 6059, 0, 33.1810074600, -117.0337840000],
    [8388, 4216, 0, 33.1757446700, -117.0053990000],
    [6766, 7058, 0, 33.1847875100, -117.0123780000],
    [2796, 6972, 0, 33.1840190300, -117.0281090000],
    [3272, 3551, 0, 33.1732404200, -117.0261340000],
    [7971, 557, 0, 33.1635297300, -117.0067270000],
    [3358, 646, 475, 33.1634637000, -117.0287210000],
    [52, 6715, 375, 33.1793414200, -117.0387530000],
    [846, 4205, 250, 33.1736762800, -117.0360330000],
    [9144, 8150, 75, 33.1877105200, -117.0039920000],
    [2078, 3901, -50, 33.1745614500, -117.0306120000],
    [7393, 6144, -175, 33.1835157000, -117.0073320000],
    [242, 148, -325, 33.1622096900, -117.0376210000],
    [5172, 841, -400, 33.1648898100, -117.0144700000],
]
pcalc["i_6130e.ntf"] = \
[
    [0, 0, -500, 33.1616263700, -117.0386240000],
    [1024, 0, -500, 33.1616140100, -117.0337580000],
    [2048, 1024, -500, 33.1657014400, -117.0288590000],
    [3072, 2048, -500, 33.1698074900, -117.0239370000],
    [4096, 3072, -500, 33.1739325900, -117.0189910000],
    [5120, 4096, -500, 33.1780771900, -117.0140210000],
    [6144, 5120, -500, 33.1822417200, -117.0090250000],
    [7168, 6144, -500, 33.1864266500, -117.0040050000],
    [8192, 7168, -500, 33.1906324700, -116.9989590000],
    [9216, 8192, -500, 33.1948596400, -116.9938860000],
    [10239, 9215, -500, 33.1991044900, -116.9887910000],
    [0, 0, 0, 33.1617298400, -117.0388170000],
    [1024, 0, 0, 33.1617198100, -117.0348580000],
    [2048, 1024, 0, 33.1650454100, -117.0308720000],
    [3072, 2048, 0, 33.1683861100, -117.0268680000],
    [4096, 3072, 0, 33.1717422400, -117.0228440000],
    [5120, 4096, 0, 33.1751141100, -117.0188010000],
    [6144, 5120, 0, 33.1785020300, -117.0147380000],
    [7168, 6144, 0, 33.1819063200, -117.0106550000],
    [8192, 7168, 0, 33.1853273300, -117.0065520000],
    [9216, 8192, 0, 33.1887653700, -117.0024270000],
    [10239, 9215, 0, 33.1922174200, -116.9982850000],
    [0, 0, 500, 33.1618333100, -117.0390100000],
    [1024, 0, 500, 33.1618255900, -117.0359580000],
    [2048, 1024, 500, 33.1643894400, -117.0328860000],
    [3072, 2048, 500, 33.1669649300, -117.0297990000],
    [4096, 3072, 500, 33.1695522500, -117.0266970000],
    [5120, 4096, 500, 33.1721516000, -117.0235810000],
    [6144, 5120, 500, 33.1747631900, -117.0204490000],
    [7168, 6144, 500, 33.1773872400, -117.0173030000],
    [8192, 7168, 500, 33.1800239600, -117.0141400000],
    [9216, 8192, 500, 33.1826735600, -117.0109630000],
    [10239, 9215, 500, 33.1853336700, -117.0077720000],
    [7982, 2431, 0, 33.1698129400, -117.0068950000],
    [2335, 6289, 0, 33.1818328900, -117.0298730000],
    [3397, 4655, 0, 33.1767772500, -117.0256840000],
    [5261, 7867, 0, 33.1871458800, -117.0184840000],
    [5500, 987, 0, 33.1649568300, -117.0170430000],
    [1672, 8549, 0, 33.1887443000, -117.0324540000],
    [672, 4840, 0, 33.1771431300, -117.0362280000],
    [496, 6251, 0, 33.1815181800, -117.0368950000],
    [8642, 464, 0, 33.1632142300, -117.0038670000],
    [3893, 7866, 0, 33.1869506800, -117.0238760000],
    [6584, 4526, 0, 33.1766173100, -117.0128960000],
    [6560, 1884, 225, 33.1673545600, -117.0155330000],
    [1587, 5856, 450, 33.1766452700, -117.0341680000],
    [713, 3378, 300, 33.1711208300, -117.0365640000],
    [3350, 1744, -25, 33.1674665100, -117.0255950000],
    [6847, 5825, -175, 33.1823405200, -117.0097130000],
    [522, 4365, -325, 33.1776479200, -117.0363730000],
    [9159, 3055, 0, 33.1719466700, -117.0020030000],
    [2404, 6527, 0, 33.1825842100, -117.0296120000],
    [9067, 4473, 0, 33.1766450600, -117.0025770000    ],
]
pcalc["i_6130f.ntf"] = \
[
    [0, 0, -500, 33.1616263600, -117.0386240000],
    [1024, 0, -500, 33.1616140000, -117.0337580000],
    [2048, 1024, -500, 33.1657014300, -117.0288590000],
    [3072, 2048, -500, 33.1698074900, -117.0239370000],
    [4096, 3072, -500, 33.1739325900, -117.0189910000],
    [5120, 4096, -500, 33.1780771800, -117.0140210000],
    [6144, 5120, -500, 33.1822417200, -117.0090250000],
    [7168, 6144, -500, 33.1864266500, -117.0040050000],
    [8192, 7168, -500, 33.1906324700, -116.9989590000],
    [9216, 8192, -500, 33.1948596400, -116.9938860000],
    [10239, 9215, -500, 33.1991045000, -116.9887910000],
    [0, 0, 0, 33.1617298400, -117.0388170000],
    [1024, 0, 0, 33.1617198000, -117.0348580000],
    [2048, 1024, 0, 33.1650453900, -117.0308720000],
    [3072, 2048, 0, 33.1683861000, -117.0268680000],
    [4096, 3072, 0, 33.1717422400, -117.0228440000],
    [5120, 4096, 0, 33.1751141100, -117.0188010000],
    [6144, 5120, 0, 33.1785020300, -117.0147380000],
    [7168, 6144, 0, 33.1819063200, -117.0106550000],
    [8192, 7168, 0, 33.1853273300, -117.0065520000],
    [9216, 8192, 0, 33.1887653800, -117.0024270000],
    [10239, 9215, 0, 33.1922174400, -116.9982850000],
    [0, 0, 500, 33.1618333000, -117.0390100000],
    [1024, 0, 500, 33.1618255800, -117.0359580000],
    [2048, 1024, 500, 33.1643894400, -117.0328860000],
    [3072, 2048, 500, 33.1669649200, -117.0297990000],
    [4096, 3072, 500, 33.1695522500, -117.0266970000],
    [5120, 4096, 500, 33.1721516000, -117.0235810000],
    [6144, 5120, 500, 33.1747632000, -117.0204490000],
    [7168, 6144, 500, 33.1773872500, -117.0173030000],
    [8192, 7168, 500, 33.1800239700, -117.0141400000],
    [9216, 8192, 500, 33.1826735700, -117.0109620000],
    [10239, 9215, 500, 33.1853336800, -117.0077720000],
    [6679, 3963, 0, 33.1747913600, -117.0124570000],
    [2072, 8071, 0, 33.1873324700, -117.0309220000],
    [7142, 5643, 0, 33.1802873200, -117.0107140000],
    [4264, 1516, 0, 33.1666822800, -117.0220900000],
    [1896, 6813, 0, 33.1834166800, -117.0315720000],
    [6507, 4171, 0, 33.1754570600, -117.0131800000],
    [7985, 6752, 0, 33.1839616800, -117.0073600000],
    [3951, 6582, 0, 33.1829336300, -117.0235890000],
    [9112, 3524, 0, 33.1735058800, -117.0022640000],
    [8373, 6120, 0, 33.1819650000, -117.0056830000],
    [7064, 1407, 0, 33.1663745100, -117.0106230000],
    [3166, 518, 0, 33.1634001200, -117.0264390000],
    [2513, 1006, 50, 33.1649270300, -117.0292860000],
    [2276, 2836, 425, 33.1691993200, -117.0318980000],
    [7129, 7112, 275, 33.1821305500, -117.0145320000],
    [2249, 6219, 150, 33.1802697400, -117.0308530000],
    [5221, 169, -200, 33.1622440900, -117.0161520000],
    [2004, 2957, -475, 33.1732494300, -117.0292170000],
    [595, 344, -75, 33.1628603900, -117.0364150000],
    [8844, 4729, -500, 33.1809724500, -116.9952680000    ],
]
pcalc["i_6130g.ntf"] = \
[
    [0, 0, -500, 33.1616259600, -117.0386240000],
    [1024, 0, -500, 33.1616129500, -117.0337590000],
    [2048, 1024, -500, 33.1656999900, -117.0288590000],
    [3072, 2048, -500, 33.1698063600, -117.0239360000],
    [4096, 3072, -500, 33.1739321900, -117.0189880000],
    [5120, 4096, -500, 33.1780776300, -117.0140170000],
    [6144, 5120, -500, 33.1822428400, -117.0090220000],
    [7168, 6144, -500, 33.1864279600, -117.0040020000],
    [8192, 7168, -500, 33.1906331300, -116.9989580000],
    [9216, 8192, -500, 33.1948585200, -116.9938900000],
    [10239, 9215, -500, 33.1991001100, -116.9888010000],
    [0, 0, 0, 33.1617300900, -117.0388170000],
    [1024, 0, 0, 33.1617194900, -117.0348590000],
    [2048, 1024, 0, 33.1650445900, -117.0308730000],
    [3072, 2048, 0, 33.1683854000, -117.0268670000],
    [4096, 3072, 0, 33.1717420400, -117.0228420000],
    [5120, 4096, 0, 33.1751146500, -117.0187980000],
    [6144, 5120, 0, 33.1785033300, -117.0147340000],
    [7168, 6144, 0, 33.1819082000, -117.0106510000],
    [8192, 7168, 0, 33.1853293900, -117.0065470000],
    [9216, 8192, 0, 33.1887670300, -117.0024240000],
    [10239, 9215, 0, 33.1922178400, -116.9982840000],
    [0, 0, 500, 33.1618342100, -117.0390100000],
    [1024, 0, 500, 33.1618260400, -117.0359590000],
    [2048, 1024, 500, 33.1643891900, -117.0328860000],
    [3072, 2048, 500, 33.1669644400, -117.0297990000],
    [4096, 3072, 500, 33.1695519000, -117.0266970000],
    [5120, 4096, 500, 33.1721516600, -117.0235790000],
    [6144, 5120, 500, 33.1747638100, -117.0204470000],
    [7168, 6144, 500, 33.1773884500, -117.0172990000],
    [8192, 7168, 500, 33.1800256600, -117.0141360000],
    [9216, 8192, 500, 33.1826755300, -117.0109580000],
    [10239, 9215, 500, 33.1853355800, -117.0077670000],
    [254, 1571, 0, 33.1667803500, -117.0378320000],
    [3017, 6739, 0, 33.1833196000, -117.0272440000],
    [7237, 8982, 0, 33.1909637100, -117.0106420000],
    [935, 1253, 0, 33.1657716300, -117.0352110000],
    [1118, 3202, 0, 33.1720086700, -117.0345160000],
    [9249, 6373, 0, 33.1828910600, -117.0020510000],
    [895, 542, 0, 33.1634774100, -117.0353620000],
    [6384, 144, 0, 33.1621451700, -117.0133300000],
    [3003, 2453, 0, 33.1696932400, -117.0271540000],
    [5986, 7367, 0, 33.1856700800, -117.0155500000],
    [172, 2823, 0, 33.1707622400, -117.0381380000],
    [5467, 2853, 50, 33.1708919800, -117.0178220000],
    [8326, 4316, 275, 33.1743195800, -117.0099500000],
    [5728, 5665, 325, 33.1775298400, -117.0199150000],
    [2506, 2925, 400, 33.1695405500, -117.0310540000],
    [888, 4536, -5, 33.1762397200, -117.0353950000],
    [8544, 2170, -175, 33.1695057200, -117.0016710000],
    [2887, 4732, -300, 33.1790166000, -117.0260390000],
    [6474, 450, -400, 33.1633509800, -117.0080920000],
    [7499, 1725, -425, 33.1684685000, -117.0028320000    ],
]

# Doesn't work yet, so stub out test. We'll come back to this
#pcalc["i_6130a.ntf"] = []
#pcalc["i_6130b.ntf"] = []
#pcalc["i_6130c.ntf"] = []
#pcalc["i_6130f.ntf"] = []
#pcalc["i_6130g.ntf"] = []
# 2. Partial derivative

# 3. Partial derivative

# 4. Direct error covariance

# 5. Indirect error covariance

# 6. Error covariance

# 7. Illumination

# 8. Trajectory

# 9. Original full image ID

# 10. Request sensor ID

# 11. Request sension type

# 12. Request RSM image domain

# 13. Requstion edition ID

# 15. Request tringulation ID

# 16. Request RSM domains height range

# 17. Requestion time of image

# 18. Request identity and defnition of RSM coordinate system

# 19. Request fit error
@require_pynitf
@require_rsm_sample_data    
def test_rsm_sample(isolated_dir):
    '''Check if we can read the various RSM sample data files.
    This comes from http://www.gwg.nga.mil/ntb/baseline/software/testfile/rsm/samples.htm'''
    for fname in ('i_6130a.ntf', 'i_6130b.ntf', 'i_6130c.ntf',
                  # File looks like it is corrupt at the web site, the unzip
                  # reports a CRC error
                  #'i_6130d.ntf',
                  # Can't process this file, limit it nitf code
                  #'i_6130e.ntf',
                  'i_6130f.ntf', 'i_6130g.ntf'):
        print(fname)
        f = pynitf.NitfFile(rsm_sample_data + fname)
        print(f.image_segment[0].rsm)
        r = f.image_segment[0].rsm
        for ln, smp, h, lat, lon in pcalc[fname]:
            ic_expect = ImageCoordinate(ln, smp)
            ic_calc = r.image_coordinate(Geodetic(lat, lon, h))
            if(abs(ic_expect.line - ic_calc.line) >= 0.2 or
               abs(ic_expect.sample - ic_calc.sample) >= 0.2):
                print(fname)
                print(Geodetic(lat, lon, h))
                print(ic_expect)
                print(ic_calc)
            # According to sample file tutorial, difference should be
            # no greater than 0.05. But it appears the lat/lon is lower
            # resolution than some of the other data, so this gives closer to
            # 0.2 pixel
            assert(abs(ic_expect.line - ic_calc.line) < 0.2)
            assert(abs(ic_expect.sample - ic_calc.sample) < 0.2)
        # Should add tests to check against the expected value spreadsheet
    
@require_pynitf
@require_rsm_sample_data    
def test_rsm_sample_file_a(isolated_dir):
    '''Test that we can generate the same TREs as we read.'''
    fname = rsm_sample_data + "i_6130a.ntf"
    f = pynitf.NitfFile(fname)
    rsm = f.image_segment[0].rsm
    # GdalRasterImage has much more limited reading of TRE, it just gives
    # the data as a string. However that is exactly what we want to get the
    # raw uninterpreted data.
    fgdal = GdalRasterImage(fname)
    texpect = fgdal["TRE", "RSMPCA"]
    assert rsm.rsm_base.tre_string() == texpect
    texpect = fgdal["TRE", "RSMIDA"]
    assert rsm.rsm_id.tre_string() == texpect
    # This also as RSMDCA and RSMECA (the direct and indirect
    # covariance). We don't currently read these

@require_pynitf
@require_rsm_sample_data    
def test_rsm_sample_file_b(isolated_dir):
    '''Test that we can generate the same TREs as we read.'''
    fname = rsm_sample_data + "i_6130b.ntf"
    f = pynitf.NitfFile(fname)
    rsm = f.image_segment[0].rsm
    fgdal = GdalRasterImage(fname)
    texpect = fgdal["TRE", "RSMPCA"]
    assert rsm.rsm_base.tre_string() == texpect
    texpect = fgdal["TRE", "RSMIDA"]
    assert rsm.rsm_id.tre_string() == texpect
    # This also as RSMDCA and RSMECA (the direct and indirect
    # covariance). We don't currently read these

@require_pynitf
@require_rsm_sample_data    
def test_rsm_sample_file_c(isolated_dir):
    '''Test that we can generate the same TREs as we read.'''
    fname = rsm_sample_data + "i_6130c.ntf"
    f = pynitf.NitfFile(fname)
    rsm = f.image_segment[0].rsm
    fgdal = GdalRasterImage(fname)
    texpect = fgdal["TRE", "RSMPCA"]
    assert rsm.rsm_base.section(0,0).tre_string() == texpect
    texpect = fgdal["TRE", "RSMPCA_2"]
    assert rsm.rsm_base.section(0,1).tre_string() == texpect
    texpect = fgdal["TRE", "RSMPCA_3"]
    assert rsm.rsm_base.section(1,0).tre_string() == texpect
    texpect = fgdal["TRE", "RSMPCA_4"]
    assert rsm.rsm_base.section(1,1).tre_string() == texpect
    texpect = fgdal["TRE", "RSMPIA"]
    assert rsm.rsm_base.tre_string() == texpect
    texpect = fgdal["TRE", "RSMIDA"]
    assert rsm.rsm_id.tre_string() == texpect
    # This also as RSMDCA and RSMECA (the direct and indirect
    # covariance). We don't currently read these

# Doesn't work yet    
@skip
@require_pynitf
@require_rsm_sample_data    
def test_rsm_sample_file_e(isolated_dir):
    '''Test that we can generate the same TREs as we read.'''
    fname = rsm_sample_data + "i_6130e.ntf"
    f = pynitf.NitfFile(fname)
    rsm = f.image_segment[0].rsm
    fgdal = GdalRasterImage(fname)
    texpect = fgdal["TRE", "RSMPCA"]
    assert rsm.rsm_base.tre_string() == texpect
    texpect = fgdal["TRE", "RSMIDA"]
    assert rsm.rsm_id.tre_string() == texpect
    # This also as RSMDCA and RSMECA (the direct and indirect
    # covariance). We don't currently read these

@require_pynitf
@require_rsm_sample_data    
def test_rsm_sample_file_f(isolated_dir):
    '''Test that we can generate the same TREs as we read.'''
    fname = rsm_sample_data + "i_6130f.ntf"
    f = pynitf.NitfFile(fname)
    rsm = f.image_segment[0].rsm
    fgdal = GdalRasterImage(fname)
    texpect = fgdal["TRE", "RSMPCA"]
    assert rsm.rsm_base.tre_string() == texpect
    texpect = fgdal["TRE", "RSMIDA"]
    assert rsm.rsm_id.tre_string() == texpect
    # This also as RSMDCA and RSMECA (the direct and indirect
    # covariance). We don't currently read these
    # This also as RSMDCA and RSMAPA (the direct covariance,
    # adjustment). We don't currently read these.

@require_pynitf
@require_rsm_sample_data    
def test_rsm_sample_file_g(isolated_dir):
    '''Test that we can generate the same TREs as we read.'''
    fname = rsm_sample_data + "i_6130g.ntf"
    f = NitfFile(fname)
    rsm = f.image_segment[0].rsm
    fgdal = GdalRasterImage(fname)
    texpect = fgdal["TRE", "RSMPCA"]
    assert rsm.rsm_base.section(0,0).tre_string() == texpect
    texpect = fgdal["TRE", "RSMPIA"]
    assert rsm.rsm_base.tre_string() == texpect
    texpect = fgdal["TRE", "RSMIDA"]
    assert rsm.rsm_id.tre_string() == texpect
    # This also as RSMECA (the indirect
    # covariance). We don't currently read these
    
