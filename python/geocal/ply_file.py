from __future__ import annotations
import numpy as np
import matplotlib.cm as cm
import matplotlib.colors as mcolors
import struct
from typing import Self
import os
from types import TracebackType


class PlyFile(object):
    """This is a simple class for writing basic Stanford PLY files. Note
    that there are more complete libraries such as RPly
    (http://w3.impa.br/~diego/software/rply/), but our currently needs are
    simple enough that it is easier to write our own code.

    Like a normal file handle, this class can be used in a "with" statement
    if desired, with the file being closed when you exit the context block.

    You can optionally supply a value, a range, and a color map. The color
    map should be one from matplot lib. The value should be supplied as the
    last column of vertex.
    """

    def __init__(
        self,
        filename: str | os.PathLike[str],
        vertex: np.ndarray | None = None,
        cmap: mcolors.Colormap = cm.hot,  # type: ignore[attr-defined]
        vmin: float = 0.0,
        vmax: float = 1.0,
        binary_format: bool = True,
    ) -> None:
        self.filename = filename
        self.vertex = vertex if vertex is not None else np.array([])
        self.is_closed = False
        self.binary_format = binary_format
        self.color_map = cmap
        self.vmin = vmin
        self.vmax = vmax

    def close(self) -> None:
        """Write out the file."""
        if self.is_closed:
            return
        v = np.array(self.vertex)
        if len(v.shape) != 2 or (v.shape[1] != 3 and v.shape[1] != 4):
            raise RuntimeError("Vertex needs to be n x 3 array or n x 4 array")

        if v.shape[1] == 4:
            # Need to apply color map
            have_color = True
            cdata = self.color_map((v[:, 3] - self.vmin) / (self.vmax - self.vmin))
            cdata = (np.round(cdata * 255)).astype(np.uint8)
        else:
            have_color = False
        if self.binary_format:
            filemode = "wb"
        else:
            filemode = "w"
        with open(self.filename, filemode) as fh:
            h = "ply\n"
            if self.binary_format:
                h += "format binary_little_endian 1.0\n"
            else:
                h += "format ascii 1.0\n"
            h += """element vertex %d
property float x
property float y
property float z\n""" % v.shape[0]
            if have_color:
                h += """property uchar red
property uchar green
property uchar blue
property uchar alpha\n"""
            h += "end_header"
            if self.binary_format:
                h += "\n"
                fh.write(h.encode("utf-8"))
            else:
                print(h, file=fh)
            if self.binary_format:
                for i in range(v.shape[0]):
                    fh.write(struct.pack("<3f", *v[i, 0:3]))
                    if have_color:
                        fh.write(struct.pack("<4B", *cdata[i, :]))
            else:
                for i in range(v.shape[0]):
                    print(
                        v[i, 0],
                        v[i, 1],
                        v[i, 2],
                        end=" ",
                        file=fh,
                    )
                    if have_color:
                        print(
                            cdata[i, 0],
                            cdata[i, 1],
                            cdata[i, 2],
                            cdata[i, 3],
                            end=" ",
                            file=fh,
                        )
                    print("", file=fh)
        self.is_closed = True

    def __enter__(self) -> Self:
        return self

    def __exit__(
        self,
        typ: type[BaseException] | None,
        value: BaseException | None,
        tb: TracebackType | None,
    ) -> None:
        self.close()

    def __del__(self) -> None:
        self.close()


__all__ = ["PlyFile"]
