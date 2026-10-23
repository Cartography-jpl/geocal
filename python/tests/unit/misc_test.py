from geocal import (
    makedirs_p,
    cib01_mapinfo,
    cib01_data,
    planet_mapinfo,
    comment_remover,
    PlanetConstant,
)
from fixtures.require_check import (
    require_serialize,
    require_afids_data,
    require_vicar,
)
import json
import os
import pytest


def test_makedirs_p(isolated_dir):
    """Test makedirs_p by creating a directory and checking that it actually
    gets created."""
    try:
        assert not os.path.exists("makedirs_test_dir_a/foo/bar")
        assert not os.path.exists("makedirs_test_dir_a/foo")
        assert not os.path.exists("makedirs_test_dir_a")
        makedirs_p("makedirs_test_dir_a/foo/bar")
        assert os.path.exists("makedirs_test_dir_a/foo/bar")
        assert os.path.exists("makedirs_test_dir_a/foo")
        assert os.path.exists("makedirs_test_dir_a")
        # Call a second time and make sure no errors occur even though
        # directory exists.
        makedirs_p("makedirs_test_dir_a/foo/bar")
        makedirs_p("makedirs_test_dir_a/foo")
        makedirs_p("makedirs_test_dir_a")
        assert os.path.exists("makedirs_test_dir_a/foo/bar")
        assert os.path.exists("makedirs_test_dir_a/foo")
        assert os.path.exists("makedirs_test_dir_a")
    finally:
        os.removedirs("makedirs_test_dir_a/foo/bar")


@require_afids_data
@require_vicar
# Doesn't currently work, we'll need to fix this
@pytest.mark.skip
def test_cib01_data():
    if not os.path.exists(os.environ["CIB1_ROOT"] + "/cib01_db.int"):
        pytest.skip("CIB1 data not found")
    cib01 = cib01_data()
    assert cib01.number_line == 8795732
    assert cib01.number_sample == 27109425


@require_serialize
def test_cib01_mapinfo():
    m = cib01_mapinfo()
    assert m.number_y_pixel == 8795732
    assert m.number_x_pixel == 27109425


@require_afids_data
def test_planet_mapinfo():
    mi = planet_mapinfo(PlanetConstant.MARS_NAIF_CODE, 35.0)
    assert mi.resolution_meter == pytest.approx(35.0, abs=1e-3)


def test_comment_remover():
    """Test parsing json with C and C++ style comments."""
    txt = """
{
    /* Here is a multiple line
       comment */
    "bar": 20,  // Single line comment
    "foo": 10,  # Python style comment
    "car" : "blah // foo",
    "dar" : "/*blah blah*/"
}
"""
    p = json.loads(comment_remover(txt))
    assert p["bar"] == 20
    assert p["car"] == "blah // foo"
    assert p["dar"] == "/*blah blah*/"
    assert p["foo"] == 10
