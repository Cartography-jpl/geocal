# This contains support for doing parallel processing.

from __future__ import annotations

import logging
from collections.abc import Callable
from functools import partial
from multiprocessing import Pool
from typing import Any

import geocal_swig
import numpy as np

from .sqlite_shelf import write_shelve


def parallel_process_tile(
    img_in: Any, lstart: int, sstart: int, nline: int, nsamp: int, out: np.ndarray
) -> None:
    log = logging.getLogger("geocal-python.parallel_process")
    log.info(
        f"Processing ({lstart:d}, {sstart:d}) to ({lstart + nline:d}, {sstart + nsamp:d})"
    )
    # Initialize to 0 in case an error occurs. Better to have a blank
    # block in an image than a block with mangled data
    out[lstart : (lstart + nline), sstart : (sstart + nsamp)] = 0
    for h in logging.getLogger("geocal-python").handlers:
        h.flush()
    if out.dtype == np.float32 or out.dtype == np.float64:
        out[lstart : (lstart + nline), sstart : (sstart + nsamp)] = img_in.read_double(
            lstart, sstart, nline, nsamp
        )
    else:
        out[lstart : (lstart + nline), sstart : (sstart + nsamp)] = img_in.read(
            lstart, sstart, nline, nsamp
        )
    for h in logging.getLogger("geocal-python").handlers:
        h.flush()


# Variation where img_in in a RasterImageMultiBand.
def parallel_process_tile2(
    img_in: Any, lstart: int, sstart: int, nline: int, nsamp: int, out: np.ndarray
) -> None:
    log = logging.getLogger("geocal-python.parallel_process")
    log.info(
        f"Processing ({lstart:d}, {sstart:d}) to ({lstart + nline:d}, {sstart + nsamp:d})"
    )
    for h in logging.getLogger("geocal-python").handlers:
        h.flush()
    if out[0].dtype == np.float32 or out[0].dtype == np.float64:
        t = img_in.read_double(lstart, sstart, nline, nsamp)
    else:
        t = img_in.read(lstart, sstart, nline, nsamp)
    for i in range(img_in.number_band):
        out[i][lstart : (lstart + nline), sstart : (sstart + nsamp)] = t[i, :, :]
    for h in logging.getLogger("geocal-python").handlers:
        h.flush()


def do_parallel_process(p: Callable[[], Any]) -> bool:
    p()
    return True


def parallel_process_image(
    img_in: geocal_swig.RasterImage | geocal_swig.RasterImageMultiBand,
    out: np.ndarray,
    process_nline: int,
    process_nsamp: int,
    number_process: int,
    shelve_name: str | None = None,
) -> int | None:
    """This processes an input image, writing to a mmap_file output file. We
    process the data by tiles of the given number of lines and samples, using
    the given number of processes to run.

    Instead of actually processing, you can supply a database name. We then
    store data suitable for use by shelve_job_run. In this case, we return
    the number of jobs stored, so you can use this to then actually process
    the jobs."""
    process_list = []
    job_index = 0
    if isinstance(img_in, geocal_swig.RasterImageMultiBand):
        nline = img_in.raster_image(0).number_line
        nsamp = img_in.raster_image(0).number_sample
        func = parallel_process_tile2
    else:
        nline = img_in.number_line
        nsamp = img_in.number_sample
        func = parallel_process_tile
    for lstart in range(0, nline, process_nline):
        tile_nline = process_nline
        if lstart + tile_nline > nline:
            tile_nline = nline - lstart
        for sstart in range(0, nsamp, process_nsamp):
            tile_nsamp = process_nsamp
            if sstart + tile_nsamp > nsamp:
                tile_nsamp = nsamp - sstart
            job = partial(func, img_in, lstart, sstart, tile_nline, tile_nsamp, out)
            if shelve_name is not None:
                write_shelve(shelve_name + f":job_{job_index:d}", job)
            job_index += 1
            process_list.append(job)
    if shelve_name is None:
        if number_process == 1:
            _ = list(map(do_parallel_process, process_list))
        else:
            pool = Pool(number_process)
            _ = pool.map(do_parallel_process, process_list)
    return job_index


__all__ = ["parallel_process_image"]
