from geocal import vicar_to_numpy
from fixtures.require_check import require_vicar, require_vicarb


@require_vicarb
@require_vicar
def test_vicar_to_numpy(isolated_dir):
    """Test vicar_to_numpy."""
    out = vicar_to_numpy("gen out NL=15 NS=15 IVAL=90")
    assert out[1, 2] == 93
