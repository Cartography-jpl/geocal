from __future__ import annotations

import typing

import scipy.optimize
from geocal_swig import (
    ImageCoordinate,
    ImageGroundConnection,
    PlanetSimpleDem,
    distance,
)

if typing.TYPE_CHECKING:
    from geocal_swig import (
        Camera,
        Dem,
        GroundCoordinate,
        RasterImage,
        SpicePlanetOrbit,
        Time,
        TimeTable,
    )


class SpiceIgc(ImageGroundConnection):
    """This is a IGC that is directly implemented by SPICE subroutine
    class. This isn't very efficient, but it can be useful for verifying
    that our other ImageGroundConnection's are implemented correctly.

    In particular, this can be used to look at the effect of light transfer
    time, which at least currently we don't model in our ImageGroundConnection.
    For anything in a low to mid range orbit, this effect tends to be
    very small and something we can ignore. But it can be good to verify
    this.
    """

    def __init__(
        self,
        spice_planet_orbit: SpicePlanetOrbit,
        camera: Camera,
        time_table: TimeTable,
        img: RasterImage | None = None,
        corr_type: str = "CN+S",
        surface_intersect_method: str = "Ellipsoid",
    ) -> None:
        self._img = img
        self.time_table = time_table
        self.camera = camera
        self.orbit = spice_planet_orbit
        dem = PlanetSimpleDem(self.orbit.naif_id)
        self.corr_type = corr_type
        self.surface_intersect_method = surface_intersect_method
        super().__init__(dem, self._img, None, "SPICE IGC")

    def pixel_time(self, ic: ImageCoordinate) -> Time:
        tm, _fc = self.time_table.time(ic)
        return tm

    def ground_coordinate_dem(self, ic: ImageCoordinate, dem: Dem) -> GroundCoordinate:
        # We ignore the DEM. Could perhaps add some kind of check
        # for this, but we are only using this for comparison with SPICE
        # so not worth trying to get that sorted out.
        tm, fc = self.time_table.time(ic)
        gc, _ = self.orbit.surface_intersect(
            tm,
            self.camera.sc_look_vector(fc, 0),
            self.corr_type,
            self.surface_intersect_method,
        )
        return gc

    def image_coordinate(self, gc: GroundCoordinate) -> ImageCoordinate:
        """Note I'm not sure how well this will work in practice. The
        ground_coordinate should always work, the inverse will *probably*
        work. But if you are doing any real work you should create another
        ImageGroundConnection based on this one - it will likely be must
        faster also."""

        def min_sample(ln: float) -> float:
            def f(x: float) -> float:
                return distance(self.ground_coordinate(ImageCoordinate(ln, x)), gc)

            r = scipy.optimize.minimize_scalar(
                f, bounds=(0, self.camera.number_sample(0))
            )
            return r.x

        def f(ln: float) -> float:
            return distance(
                self.ground_coordinate(ImageCoordinate(ln, min_sample(ln))), gc
            )

        r = scipy.optimize.minimize_scalar(f, bounds=(0, self.time_table.max_line))
        if not r.success or r.fun > 10:
            # Same error the C++ ImageGroundConnectionFailed gives in python.
            # ImageGroundConnectionFailed itself isn't a python exception.
            raise RuntimeError("ImageGroundConnectionFailed")
        return ImageCoordinate(r.x, min_sample(r.x))
