import numpy as np
import pytest

from geocal import SpotReflectance


# Doesn't currently work
@pytest.mark.skip(reason="Doesn't currently work, SpotReflectance needs to be fixed")
def test_print(unit_test_data):
    """Test printing of metadata"""
    sr = SpotReflectance(
        str(unit_test_data / "spot6_multi_meta.xml"),
        str(unit_test_data / "spot6_pan_meta.xml"),
    )
    sr.printMetadata()


# Doesn't currently work
@pytest.mark.skip(reason="Doesn't currently work, SpotReflectance needs to be fixed")
def test_refl(unit_test_data):
    """Test reflectance calculation."""
    sr = SpotReflectance(
        str(unit_test_data / "spot6_multi_meta.xml"),
        str(unit_test_data / "spot6_pan_meta.xml"),
    )
    dn = np.array([[252, 199, 200], [241, 155, 142], [224, 142, 124]])
    ref_expect = np.array(
        [
            [0.1388, 0.1096, 0.1102],
            [0.1327, 0.08537, 0.07821],
            [0.1234, 0.07821, 0.06830],
        ]
    )
    np.testing.assert_allclose(sr.dn2TOAReflectance(dn, 3), ref_expect, atol=1e-3)
