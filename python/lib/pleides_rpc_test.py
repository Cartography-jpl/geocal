from nose.tools import *
from pleides_rpc import *

test_data = os.path.dirname(__file__) + "/../unit_test_data/"
xml_fname = test_data + "RPC_PHR1B_P_201307191703469_SEN_646065101-001.XML"
def test_pleides_rpc():
    rpc = pleides_rpc(xml_fname)
    # Expected value here comes from directly copying from the XML file
    rpc_e = Rpc()
    rpc_e.rpc_type= Rpc.RPC_B
    rpc_e.error_bias = 0
    rpc_e.error_random = 0
    rpc_e.line_offset = 19416.5
    rpc_e.line_scale = 19415.5
    rpc_e.sample_offset = 20000.5
    rpc_e.sample_scale = 19999.5
    rpc_e.latitude_offset = 25.90428433205637
    rpc_e.latitude_scale = 0.1004903167268196
    rpc_e.longitude_offset = -97.38111913704327
    rpc_e.longitude_scale = 0.1261000066282989
    rpc_e.height_offset = -10
    rpc_e.height_scale = 20
    rpc_e.sample_numerator = [0.006533064626706719,
                              0.9957940323802271,
                              -0.0006303714751038053,
                              0.0008334454497558985,
                              -0.001635858714058743,
                              -1.881751175117233e-05,
                              -4.194596517194051e-06,
                              -0.007282346576080248,
                              0.001709536893299357,
                              1.463688210575885e-08,
                              3.89640100119625e-07,
                              6.226417658422115e-05,
                              2.863560861935821e-05,
                              8.896268349781061e-07,
                              8.306975526850318e-05,
                              0.0007404795287822898,
                              1.382029361341874e-10,
                              1.192129231162493e-07,
                              1.35529592360303e-08,
                              7.44839337745008e-10]
    rpc_e.sample_denominator = [1,
                                0.000745928491632828,
                                0.001675473888645732,
                                1.35827144140661e-05,
                                1.235202057782227e-05,
                                -5.02158257597116e-08,
                                3.524648742856159e-07,
                                -1.506487645629298e-05,
                                6.127039748416494e-05,
                                8.936809455858683e-07,
                                4.055182886783897e-09,
                                8.000010353667942e-07,
                                -3.411307877235737e-08,
                                7.198108083813748e-09,
                                1.19781979963821e-06,
                                1.090133915577399e-05,
                                3.07925382667808e-09,
                                1.298041915576127e-09,
                                2.707097864140982e-09,
                                2.55309704013703e-11]
    rpc_e.line_numerator = [ -0.006430194927694196,
                            -0.03877812567478926,
                            -0.9611869108419688,
                            -0.0001473689361894729,
                            0.003396822417439105,
                            -2.233996697114469e-09,
                            2.371239529022261e-06,
                            -0.0002748833623049806,
                            0.003236977707072896,
                            2.212515841270927e-07,
                            -1.054346852008697e-07,
                            5.292462235986268e-06,
                            -4.238835847048009e-05,
                            1.306101130466613e-06,
                            8.887751641740891e-06,
                            0.0001122815951847657,
                            3.234510477327235e-05,
                            3.463133781790626e-08,
                            -2.517492812562697e-08,
                            4.955762971320874e-09]
    rpc_e.line_denominator = [ 1,
                               0.003005400685308182,
                               -0.002937954845833481,
                               1.334171085529705e-06,
                               -1.242726727688847e-05,
                               -7.014973894037535e-08,
                               -1.016356922637066e-08,
                               -3.657035276545347e-05,
                               7.749981698014053e-05,
                               -3.366238029177599e-05,
                               2.217270632349841e-09,
                               -1.701741100471856e-10,
                               -1.959852115365173e-06,
                               -2.151250951312738e-07,
                               6.138595397293603e-07,
                               1.492857126166691e-06,
                               -1.391475334732865e-08,
                               1.343043806945056e-11,
                               -6.521145829515509e-09,
                               -1.093095348720787e-10]
    assert str(rpc) == str(rpc_e)
    assert_almost_equal(rpc.height_offset, rpc_e.height_offset)
    assert_almost_equal(rpc.height_scale, rpc_e.height_scale)
    assert_almost_equal(rpc.latitude_offset, rpc_e.latitude_offset)
    assert_almost_equal(rpc.latitude_scale, rpc_e.latitude_scale)
    assert_almost_equal(rpc.longitude_offset, rpc_e.longitude_offset)
    assert_almost_equal(rpc.longitude_scale, rpc_e.longitude_scale)
    assert_almost_equal(rpc.line_offset, rpc_e.line_offset)
    assert_almost_equal(rpc.line_scale, rpc_e.line_scale)
    assert_almost_equal(rpc.sample_offset, rpc_e.sample_offset)
    assert_almost_equal(rpc.sample_scale, rpc_e.sample_scale)
    for i in range(20):
        assert_almost_equal(rpc.line_denominator[i], rpc_e.line_denominator[i])
        assert_almost_equal(rpc.line_numerator[i], rpc_e.line_numerator[i])
        assert_almost_equal(rpc.sample_denominator[i], 
                            rpc_e.sample_denominator[i])
        assert_almost_equal(rpc.sample_numerator[i], 
                            rpc_e.sample_numerator[i])
        assert_almost_equal(rpc.fit_line_numerator[i], 
                            rpc_e.fit_line_numerator[i])
        assert_almost_equal(rpc.fit_sample_numerator[i],
                            rpc_e.fit_sample_numerator[i])

