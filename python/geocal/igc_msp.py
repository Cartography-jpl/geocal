from __future__ import annotations
from geocal_swig import (  # type: ignore
    CartesianFixed,
    CartesianFixedLookVector,
    Ecr,
    ImageGroundConnection,
    SimpleDem,
    distance,
    ImageCoordinate,
    Geodetic,
    GroundCoordinate,
    Dem,
    Time
)
import os
import typing
from typing import Any
import numpy as np

have_msp = False
try:
    from msp_swig import Msp  # type: ignore

    have_msp = True
except ImportError:
    pass

if typing.TYPE_CHECKING:
    import pynitf  # type: ignore


class classproperty:
    def __init__(self, func: Any) -> None:
        self.fget = func

    def __get__(self, instance: Any, owner: Any) -> Any:
        return self.fget(owner)


class IgcMsp(ImageGroundConnection):
    def __init__(
        self,
        fname: str | os.PathLike[str],
        dem: Dem | None = None,
        image_index: int = 0,
        plugin_name: str | None = None,
        model_name: str | None = None,
    ) -> None:
        super().__init__(SimpleDem() if dem is None else dem, None, None, "")
        self._msp = Msp(
            str(fname),
            image_index,
            plugin_name if plugin_name is not None else "",
            model_name if model_name is not None else "",
        )

    def desc(self) -> str:
        return "IgcMsp"

    def _v_number_line(self) -> int:
        return int(self._msp.image_size()[0])

    def _v_number_sample(self) -> int:
        return int(self._msp.image_size()[1])

    def sensor_velocity(self, ic: ImageCoordinate) -> np.ndarray:
        """Return sensor velocity. I think this is m/s, but I'm not sure"""
        return self._msp.sensor_velocity(ic.line, ic.sample)

    # blitz::Array<double, 2> covariance() const;
    # blitz::Array<double, 2> joint_covariance(const IgcMsp& igc2) const;
    # std::string generate_rsm_tre(const std::string& Report = "",
    # const std::string& Rsm_config = "") const;
    # void ground_coordinate_with_cov(const ImageCoordinate& Ic,
    # const blitz::Array<double, 2>& Ic_cov,
    # double H,
    # double H_var,
    # boost::shared_ptr<GroundCoordinate>& Gp,
    # blitz::Array<double, 2>& Gp_cov) const;
    # void ce90_le90(const ImageCoordinate& Ic, double H,
    # double& Ce90, double& Le90) const;
    # virtual bool has_time() const { return true; }
    # virtual Time pixel_time(const ImageCoordinate& Ic) const;

    def _v_has_time(self) -> bool:
        return True

    def pixel_time(self, ic: ImageCoordinate) -> Time:
        return Time.parse_time(self._msp.pixel_time_base(ic.line, ic.sample)) + self._msp.pixel_time_offset(ic.line, ic.sample)
    
    def ground_coordinate(self, ic: ImageCoordinate, dem=None):
        return self.ground_coordinate_dem(ic, self.dem)

    def ground_coordinate_dem(self, ic: ImageCoordinate, dem: Dem) -> GroundCoordinate:
        if not isinstance(dem, SimpleDem):
            raise RuntimeError(
                "Right now, IgcMsp only works with constant DEMs for SimpleDem"
            )
        return self.ground_coordinate_approx_height(ic, dem.h)

    def ground_coordinate_approx_height(
        self, ic: ImageCoordinate, h: float
    ) -> GroundCoordinate:
        # Note, MSP really is just Ecr rather than a more general CartesianFixed.
        # Although our general ImageGroundConnection supports other planets,
        # the MSP library only supports Earth.
        return Ecr(*self._msp.ground_coordinate_approx_height(ic.line, ic.sample, h))

    def image_coordinate(self, gc: GroundCoordinate) -> ImageCoordinate:
        # Note, MSP really is just Ecr rather than a more general CartesianFixed.
        # Although our general ImageGroundConnection supports other planets,
        # the MSP library only supports Earth.
        gc_ecr = Ecr(gc)
        return ImageCoordinate(*self._msp.image_coordinate([*gc_ecr.position]))

    def cf_look_vector(
        self, ic: ImageCoordinate
    ) -> tuple[CartesianFixedLookVector, CartesianFixed]:
        res = self._msp.cf_look_vector(ic.line, ic.sample)
        return CartesianFixedLookVector(*res[1, :]), Ecr(*res[0, :])

    @classmethod
    def print_plugin_list(cls) -> None:
        """Print the plugin list, a basic diagnostic we use to make sure things
        are working."""
        Msp.msp_print_plugin_list()

    @classproperty
    def plugin_list(cls) -> list[str]:
        """Return the plugin list, as a list of str"""
        return Msp.msp_plugin_list()

    @classproperty
    def model_dict(cls) -> dict[str, list[str]]:
        """Return dict going from a plugin name to the models that plugin supports."""
        res = {}
        for p in cls.plugin_list:
            res[p] = Msp.msp_model_list(p)
        return res

    @property
    def file_name(self) -> str:
        return self._msp.file_name()

    @property
    def image_index(self) -> int:
        return self._msp.image_index()

    @property
    def family(self) -> str:
        return self._msp.family()

    @property
    def version(self) -> str:
        return self._msp.version()

    @property
    def model_name(self) -> str:
        return self._msp.model_name()

    @property
    def pedigree(self) -> str:
        return self._msp.pedigree()

    @property
    def image_identifer(self) -> str:
        return self._msp.image_identifer()

    @property
    def sensor_identifer(self) -> str:
        return self._msp.sensor_identifer()

    @property
    def platform_identifer(self) -> str:
        return self._msp.platform_identifer()

    @property
    def collection_identifer(self) -> str:
        return self._msp.collection_identifer()

    @property
    def trajectory_identifer(self) -> str:
        return self._msp.trajectory_identifer()

    @property
    def sensor_type(self) -> str:
        return self._msp.sensor_type()

    @property
    def sensor_mode(self) -> str:
        return self._msp.sensor_mode()

    @property
    def reference_date_time(self) -> str:
        return self._msp.reference_date_time()

    @classmethod
    def check_plugin(
        cls,
        f: pynitf.NitfFile,
        iid1: str,
        plugin: str,
        check_corner: bool = True,
        corner_tol: float = 1.0,
        dist_tol: float = -1,
    ) -> bool:
        """This does a basic check of using the MSP library to read the
        data for a file. This is meant for diagnostic purposes, there are
        all kinds of reasons why a file we generate doesn't work with MSP.
        We can optionally check the corners stored in CSCRNA TRE.

        f should be a pynitf.NitfFile.

        We simple print out information about using this plugin, and then return
        a generic True or False to indicate if the plugin worked. We may want
        to change this over time, but right now this is the interface that makes
        the most sense.

        If the corner_tol is > 0, then if we check the corners we compare against
        that tolerance. Similarly if distance_tol > 0, we check that tolerance.
        If either is <=0, we skip that check."""
        desc = "file '%s' image id '%s' plugin '%s'" % (f.file_name, iid1, plugin)
        try:
            iseg = f.iseg_by_iid1(iid1)
            if len(iseg) == 0:
                print("Image id not found in " + desc)
                return False
            if len(iseg) > 1:
                print("More than one image id found in " + desc)
                return False
            iseg = iseg[0]
            segindx = [t for t, isg in enumerate(f.image_segment) if isg is iseg][0]
            try:
                igc = cls(f.file_name, None, segindx, plugin, plugin)
            except RuntimeError:
                print("MSP failed to read " + desc)
                return False
            if check_corner:
                t = iseg.find_tre("CSCRNA")
                if len(t) == 0:
                    print("Could not find CSCRNA " + desc)
                    return False
                if len(t) > 1:
                    print("Found more than one CSCRNA " + desc)
                    return False
                t = t[0]
                gpt1 = Geodetic(t.ulcnr_lat, t.ulcnr_long, t.ulcnr_ht)
                gpt2 = Geodetic(t.urcnr_lat, t.urcnr_long, t.urcnr_ht)
                gpt3 = Geodetic(t.lrcnr_lat, t.lrcnr_long, t.lrcnr_ht)
                gpt4 = Geodetic(t.llcnr_lat, t.llcnr_long, t.llcnr_ht)
                ic1 = igc.image_coordinate(gpt1)
                ic2 = igc.image_coordinate(gpt2)
                ic3 = igc.image_coordinate(gpt3)
                ic4 = igc.image_coordinate(gpt4)
                nline = iseg.subheader.nrows
                nsamp = iseg.subheader.ncols
                print("ic1: ", ic1, " (Expected %d, %d)" % (0, 0))
                print("ic2: ", ic2, " (Expected %d, %d)" % (0, nsamp))
                print("ic3: ", ic3, " (Expected %d, %d)" % (nline, nsamp))
                print("ic4: ", ic4, " (Expected %d, %d)" % (nline, 0))
                gpt1_calc = igc.ground_coordinate_approx_height(
                    ImageCoordinate(0, 0), gpt1.height_reference_surface
                )
                gpt2_calc = igc.ground_coordinate_approx_height(
                    ImageCoordinate(0, nsamp), gpt2.height_reference_surface
                )
                gpt3_calc = igc.ground_coordinate_approx_height(
                    ImageCoordinate(nline, nsamp), gpt3.height_reference_surface
                )
                gpt4_calc = igc.ground_coordinate_approx_height(
                    ImageCoordinate(nline, 0), gpt4.height_reference_surface
                )
                print("dist1: ", distance(gpt1_calc, gpt1), "m")
                print("dist2: ", distance(gpt2_calc, gpt2), "m")
                print("dist3: ", distance(gpt3_calc, gpt3), "m")
                print("dist4: ", distance(gpt4_calc, gpt4), "m")
                print("Image %d x %d" % (nline, nsamp))
                if corner_tol > 0:
                    if (
                        abs(ic1.line - 0) > corner_tol
                        or abs(ic1.sample - 0) > corner_tol
                        or abs(ic2.line - 0) > corner_tol
                        or abs(ic2.sample - nsamp) > corner_tol
                        or abs(ic3.line - nline) > corner_tol
                        or abs(ic3.sample - nsamp) > corner_tol
                        or abs(ic4.line - nline) > corner_tol
                        or abs(ic4.sample - 0) > corner_tol
                    ):
                        print("Corner tolerance not met " + desc)
                        return False
                    else:
                        print("Corner tolerance met " + desc)
                if dist_tol > 0:
                    if (
                        distance(gpt1_calc, gpt1) > dist_tol
                        or distance(gpt2_calc, gpt2) > dist_tol
                        or distance(gpt3_calc, gpt3) > dist_tol
                        or distance(gpt4_calc, gpt4) > dist_tol
                    ):
                        print("Distance tolerance not met " + desc)
                        return False
                    else:
                        print("Distance tolerance met " + desc)
            print("MSP succeeded to read " + desc)
            return True
        except RuntimeError as e:
            print("General failure for " + desc)
            print(e)
            return False


__all__ = ["IgcMsp", "have_msp"]
