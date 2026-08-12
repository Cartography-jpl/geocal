from geocal import (
    VicarLiteDem,
    VicarImageGroundConnection,
    IgcArray,
    VicarLiteRasterImage,
    TiePointCollect,
    TiePointCollectFM,
    read_shelve,
    have_cv2,
    ImageCoordinate,
    Geodetic,
    distance,
)
from multiprocessing import Pool
import pickle
import matplotlib.pyplot as plt
import pytest
from fixtures.require_check import require_serialize
from pathlib import Path

dr = Path(__file__).parent.parent.parent.parent.absolute() / "unit_test_data" / "Stereo"
demin = VicarLiteDem(str(dr / "nevada_elv_aoi.img"), True)
igc1 = VicarImageGroundConnection(str(dr / "10MAY21-1.img"), demin)
igc2 = VicarImageGroundConnection(str(dr / "10MAY21-2.img"), demin)
igc3 = VicarImageGroundConnection(str(dr / "10MAY21-3.img"), demin)
igc_coll = IgcArray([igc1, igc2, igc3])
gaoi = VicarLiteRasterImage(str(dr / "aoi.img")).map_info
ref_img = VicarLiteRasterImage(str(dr / "nevada_doq_aoi.img"))

tp_collect = TiePointCollect(igc_coll)


def test_tp():
    ic = igc1.image_coordinate(demin.surface_point(gaoi.ground_coordinate(550, 550)))
    ic.line = round(ic.line)
    ic.sample = round(ic.sample)
    tp, diag = tp_collect.tie_point(ic)
    assert (
        distance(
            tp.ground_location, Geodetic(36.7731575234, -116.116743146, 1276.36490196)
        )
        < 0.1
    )


@require_serialize
def test_tie_point_grid():
    pool = Pool()
    tpcol, diag = tp_collect.tie_point_grid(10, 10, aoi=gaoi, dem=demin, pool=pool)
    print(tpcol)
    diag.print_report()
    assert len(tpcol) == 95


@require_serialize
def test_pickle():
    _ = pickle.dumps(tp_collect)


@pytest.mark.skip
def test_show_image():
    tp, diag = tp_collect.tie_point(ImageCoordinate(500, 500))
    tp.display(igc_coll)
    plt.show()


# Data is way too big to check into source, so we put it here. This means
# we can only tests that depend on this on pistol. We may fold this into
# the afids data area at some point.
# This takes about 2 minutes to run. This isn't that long, but is a bit too
# long for a unit test. So normally skip this.
@pytest.mark.skip
@require_serialize
def test_fm(geocal_test_igc, geocal_test_data):
    """Test tiepoint generation using feature matching."""
    if not have_cv2:
        pytest.skip("Need opencv2")
    igccol = read_shelve(geocal_test_igc)
    ref_image = VicarLiteRasterImage(str(geocal_test_data / "ref.img"))
    tp_collect = TiePointCollectFM(
        igccol, ref_image=ref_image, max_ground_covariance=200**2
    )
    # Parallel doesn't work yet.
    pool = None
    tpcol = tp_collect.tie_point_list(pool=pool)
    print(tpcol)
