from geocal import IkonosReflectance
import numpy as np
import numpy.testing as npt


def test_print(unit_test_data):
    """Test printing of metadata"""
    ik = IkonosReflectance(unit_test_data / "418524_meta.txt")
    ik.printMetadata()


def test_refl(unit_test_data):
    """Test reflectance calculation."""
    ik = IkonosReflectance(unit_test_data / "418524_meta.txt")
    dn = np.array([[416, 398, 378], [435, 403, 369], [438, 409, 375]])
    ref_expect = np.array(
        [
            [0.15923692, 0.15234687, 0.14469124],
            [0.16650977, 0.15426077, 0.14124621],
            [0.16765811, 0.15655746, 0.1435429],
        ]
    )
    npt.assert_almost_equal(ik.dn2TOAReflectance(dn, 4), ref_expect, 8)
