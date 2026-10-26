from __future__ import annotations

import numpy as np
from geocal_swig import VicarRasterImage

from .mmap_file import mmap_file
from .vicar_interface import VicarInterface


class GaussianStretch(VicarInterface):
    def __init__(self, data: np.ndarray) -> None:
        super().__init__()
        self.data = data
        self.timing = False
        self.cmd = "stretch in.img out.img 'gauss DNMIN=0 DNMAX=255"

    def pre_run(self) -> None:
        # Create the file, and close it again by dropping the reference
        t: VicarRasterImage | None = VicarRasterImage(
            "in.img", "HALF", self.data.shape[0], self.data.shape[1]
        )
        t = None  # noqa: F841
        d = mmap_file("in.img", mode="r+")
        d[:] = np.where(
            self.data > 0, self.data * 32768.0 / max(self.data.max(), 1e-8), 0
        ).astype(np.int16)
        self.run_out = b""

    def post_run(self) -> None:
        self.out = VicarRasterImage("out.img").read_all()  # type: ignore[attr-defined]


def gaussian_stretch(data: np.ndarray) -> np.ndarray:
    """This does histogram equalization on the given data, doing a Gaussian
    stretch.

    Currently this uses the vicar program 'stretch'. We could probably move
    this all into python, but there seems little point.

    We assume that all negative values are fill, and map them to 0. Data is
    returned as integer data."""
    # Special handling for all 0 data, stretch fails so just return
    # the original data
    if data.max() == 0:
        return data
    gs = GaussianStretch(data)
    gs.vicar_run()
    return gs.out


__all__ = ["gaussian_stretch"]
