import numpy as np

from geocal import QuickBirdReflectance


def test_print(unit_test_data):
    """Test printing of metadata"""
    qb = QuickBirdReflectance(
        str(unit_test_data / "07JUN08184839-M1BS-005616921010_01_P001.IMD"),
        str(unit_test_data / "07JUN08184839-P1BS-005616921010_01_P001.IMD"),
    )
    qb.printMetadata()


def test_refl(unit_test_data):
    """Test reflectance calculation."""
    qb = QuickBirdReflectance(
        str(unit_test_data / "07JUN08184839-M1BS-005616921010_01_P001.IMD"),
        str(unit_test_data / "07JUN08184839-P1BS-005616921010_01_P001.IMD"),
    )
    dn = np.array([[416, 398, 378], [435, 403, 369], [438, 409, 375]])
    ref_expect = np.array(
        [
            [0.21578284, 0.20644608, 0.1960719],
            [0.2256383, 0.20903962, 0.19140353],
            [0.22719443, 0.21215187, 0.19451578],
        ]
    )
    np.testing.assert_allclose(qb.dn2TOAReflectance(dn, 4), ref_expect, atol=1e-8)
