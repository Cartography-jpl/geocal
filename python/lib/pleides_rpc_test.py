from builtins import str
from builtins import range
from pleides_rpc import *
from numpy.testing import assert_almost_equal

test_data = os.path.dirname(__file__) + "/../../unit_test_data/"
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
    rpc_e.sample_numerator = [-0.006564240846587687,
                              1.004079504781189,
                              -0.0006821231253564823,
                              -0.0008370218170285255,
                              -0.001806765621156964,
                              7.100379397665977e-06,
                              -4.885437466979269e-06,
                              0.007272540633688348,
                              -0.001843009624739359,
                              -3.426203102616591e-08,
                              -1.283595453598615e-07,
                              4.466533941254061e-05,
                              3.521575821624373e-05,
                              3.535471525184789e-06,
                              3.697122872769391e-05,
                              0.0008394241478991341,
                              2.662029877416655e-10,
                              1.465541726751425e-07,
                              3.622186518388348e-07,
                              -2.995927581405171e-09]
    rpc_e.sample_denominator = [1,
                                -0.0007121899757103233,
                                0.001810231515380108,
                                -1.260522902800861e-05,
                                3.331922980295843e-05,
                                -9.325623673387547e-08,
                                3.561493215457294e-09,
                                5.678108285135689e-06,
                                -4.907354435202568e-05,
                                3.513057087737391e-06,
                                -6.261421747276997e-10,
                                -2.590627635667391e-07,
                                -1.904033691848051e-06,
                                -2.837095085970682e-08,
                                -7.72789814986541e-07,
                                -1.498145238671747e-05,
                                1.181902467391965e-08,
                                -2.335228897620424e-09,
                                -5.401855157539386e-09,
                                -9.363637668559989e-11]
    rpc_e.line_numerator = [-0.006424742888377009,
                            -0.04050329077621746,
                            -1.040263942736032,
                            -0.0001195459002490792,
                            -0.002932879494325628,
                            -3.07032731149465e-07,
                            1.690441603709594e-06,
                            -0.00061402441145284,
                            0.003657917308607424,
                            -2.777085978938428e-07,
                            -3.059964532480833e-08,
                            -2.992210396652361e-06,
                            3.48908984021711e-06,
                            -1.708453717879032e-06,
                            -3.720838917979654e-05,
                            -0.0001597911154305924,
                            -4.387590852819494e-05,
                            -5.184310926256514e-09,
                            -1.610292099777321e-08,
                            -5.047441180797062e-09]
    rpc_e.line_denominator = [1,
                              -0.003336003872158801,
                              -0.002996171852953001,
                              9.759984984312129e-07,
                              -1.558393365204776e-05,
                              4.223015470640286e-08,
                              -3.233161419002784e-08,
                              1.525922315082046e-05,
                              -8.10922392753457e-05,
                              4.216179226719246e-05,
                              6.257495949499623e-10,
                              -2.615446142828529e-07,
                              -1.862592625890856e-06,
                              -2.652885956998541e-07,
                              -4.262112677946345e-07,
                              -1.809109130204222e-06,
                              2.098526913651389e-08,
                              -1.129251816020838e-10,
                              5.863912965633309e-09,
                              9.240890112448958e-11]
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

