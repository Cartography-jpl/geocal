from geocal import Geoeye1Reflectance
import numpy as np
import numpy.testing as npt


def test_print(unit_test_data):
    """Test printing of metadata"""
    ge1 = Geoeye1Reflectance(
        unit_test_data / "5V090727M0003172424A222000100202M_001535172.pvl"
    )
    ge1.printMetadata()


def test_refl(unit_test_data):
    """Test reflectance calculation."""
    ge1 = Geoeye1Reflectance(
        unit_test_data / "5V090727M0003172424A222000100202M_001535172.pvl"
    )
    dn = np.array([[416, 398, 378], [435, 403, 369], [438, 409, 375]])
    ref_expect = np.array(
        [
            [0.10461536, 0.10008873, 0.09505915],
            [0.10939347, 0.10134613, 0.09279584],
            [0.1101479, 0.10285501, 0.09430471],
        ]
    )
    npt.assert_almost_equal(ge1.dn2TOAReflectance(dn, 4), ref_expect, 8)
