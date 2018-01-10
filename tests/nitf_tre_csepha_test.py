from pynitf.nitf_tre import *
from pynitf.nitf_tre_csepha import *
from test_support import *
import io, six

#To Do: Figure out how to deal w floating point numbers
#and update the time component field unit test
def test_tre_csepha_basic():

    t = TreCSEPHA()

    r = 100
    offset1 = 1000
    offset2 = 2000

    # Set some values
    t.ephem_flag = "COLLECT_TIME"
    t.dt_ephem = 1.5
    t.date_ephem = 20170101
    t.t0_ephem = '235900.123456'
    t.num_ephem = 100
    for n in range(r):
        t.ephem_x[n] = n * n
        t.ephem_y[n] = n * n + offset1
        t.ephem_z[n] = n * n + offset2

    fh = six.BytesIO()
    t.write_to_file(fh)
    #print(fh.getvalue())
    assert fh.getvalue() == b'CSEPHA03641COLLECT_TIME1.5  20170101235900.123456100+00000000.00+00001000.00+00002000.00+00000001.00+00001001.00+00002001.00+00000004.00+00001004.00+00002004.00+00000009.00+00001009.00+00002009.00+00000016.00+00001016.00+00002016.00+00000025.00+00001025.00+00002025.00+00000036.00+00001036.00+00002036.00+00000049.00+00001049.00+00002049.00+00000064.00+00001064.00+00002064.00+00000081.00+00001081.00+00002081.00+00000100.00+00001100.00+00002100.00+00000121.00+00001121.00+00002121.00+00000144.00+00001144.00+00002144.00+00000169.00+00001169.00+00002169.00+00000196.00+00001196.00+00002196.00+00000225.00+00001225.00+00002225.00+00000256.00+00001256.00+00002256.00+00000289.00+00001289.00+00002289.00+00000324.00+00001324.00+00002324.00+00000361.00+00001361.00+00002361.00+00000400.00+00001400.00+00002400.00+00000441.00+00001441.00+00002441.00+00000484.00+00001484.00+00002484.00+00000529.00+00001529.00+00002529.00+00000576.00+00001576.00+00002576.00+00000625.00+00001625.00+00002625.00+00000676.00+00001676.00+00002676.00+00000729.00+00001729.00+00002729.00+00000784.00+00001784.00+00002784.00+00000841.00+00001841.00+00002841.00+00000900.00+00001900.00+00002900.00+00000961.00+00001961.00+00002961.00+00001024.00+00002024.00+00003024.00+00001089.00+00002089.00+00003089.00+00001156.00+00002156.00+00003156.00+00001225.00+00002225.00+00003225.00+00001296.00+00002296.00+00003296.00+00001369.00+00002369.00+00003369.00+00001444.00+00002444.00+00003444.00+00001521.00+00002521.00+00003521.00+00001600.00+00002600.00+00003600.00+00001681.00+00002681.00+00003681.00+00001764.00+00002764.00+00003764.00+00001849.00+00002849.00+00003849.00+00001936.00+00002936.00+00003936.00+00002025.00+00003025.00+00004025.00+00002116.00+00003116.00+00004116.00+00002209.00+00003209.00+00004209.00+00002304.00+00003304.00+00004304.00+00002401.00+00003401.00+00004401.00+00002500.00+00003500.00+00004500.00+00002601.00+00003601.00+00004601.00+00002704.00+00003704.00+00004704.00+00002809.00+00003809.00+00004809.00+00002916.00+00003916.00+00004916.00+00003025.00+00004025.00+00005025.00+00003136.00+00004136.00+00005136.00+00003249.00+00004249.00+00005249.00+00003364.00+00004364.00+00005364.00+00003481.00+00004481.00+00005481.00+00003600.00+00004600.00+00005600.00+00003721.00+00004721.00+00005721.00+00003844.00+00004844.00+00005844.00+00003969.00+00004969.00+00005969.00+00004096.00+00005096.00+00006096.00+00004225.00+00005225.00+00006225.00+00004356.00+00005356.00+00006356.00+00004489.00+00005489.00+00006489.00+00004624.00+00005624.00+00006624.00+00004761.00+00005761.00+00006761.00+00004900.00+00005900.00+00006900.00+00005041.00+00006041.00+00007041.00+00005184.00+00006184.00+00007184.00+00005329.00+00006329.00+00007329.00+00005476.00+00006476.00+00007476.00+00005625.00+00006625.00+00007625.00+00005776.00+00006776.00+00007776.00+00005929.00+00006929.00+00007929.00+00006084.00+00007084.00+00008084.00+00006241.00+00007241.00+00008241.00+00006400.00+00007400.00+00008400.00+00006561.00+00007561.00+00008561.00+00006724.00+00007724.00+00008724.00+00006889.00+00007889.00+00008889.00+00007056.00+00008056.00+00009056.00+00007225.00+00008225.00+00009225.00+00007396.00+00008396.00+00009396.00+00007569.00+00008569.00+00009569.00+00007744.00+00008744.00+00009744.00+00007921.00+00008921.00+00009921.00+00008100.00+00009100.00+00010100.00+00008281.00+00009281.00+00010281.00+00008464.00+00009464.00+00010464.00+00008649.00+00009649.00+00010649.00+00008836.00+00009836.00+00010836.00+00009025.00+00010025.00+00011025.00+00009216.00+00010216.00+00011216.00+00009409.00+00010409.00+00011409.00+00009604.00+00010604.00+00011604.00+00009801.00+00010801.00+00011801.00'

    fh2 = six.BytesIO(fh.getvalue())
    t2 = TreCSEPHA()
    t2.read_from_file(fh2)
    assert t2.ephem_flag == "COLLECT_TIME"
    assert t2.dt_ephem == 1.5
    assert t2.date_ephem == 20170101
    assert t2.t0_ephem == '235900.123456'
    assert t2.num_ephem == 100
    for n in range(r):
        assert t.ephem_x[n] == n * n
        assert t.ephem_y[n] == n * n + offset1
        assert t.ephem_z[n] == n * n + offset2

    print (t2.summary())
