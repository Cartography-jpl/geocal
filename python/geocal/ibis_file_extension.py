# This contains extensions to give a more python like interface to
# IbisFile.
from __future__ import annotations

from types import TracebackType
from typing import Any

import numpy as np
from geocal_swig import IbisFile, VicarFile

# We might not have IbisFile defined
have_ibis_file = VicarFile.vicar_available()


def _column(self: IbisFile, cindex: int) -> Any:
    """Return the proper IbisColumn<T> type for the given column index."""
    t = self.column_data_type(cindex)
    if t == IbisFile.VICAR_BYTE:
        return self.column_byte(cindex)
    elif t == IbisFile.VICAR_HALF:
        return self.column_half(cindex)
    elif t == IbisFile.VICAR_FULL:
        return self.column_full(cindex)
    elif t == IbisFile.VICAR_FLOAT:
        return self.column_float(cindex)
    elif t == IbisFile.VICAR_DOUBLE:
        return self.column_double(cindex)
    elif t == IbisFile.VICAR_ASCII:
        return self.column_string(cindex)
    else:
        raise RuntimeError(f"Unrecognized type {t:d}")


if have_ibis_file:
    # Python functions are added to SWIG classes below. mypy doesn't know
    # about attributes added after a class is defined, so those lines need
    # a type: ignore.
    IbisFile.column = _column  # type: ignore[attr-defined]


def _getitem(self: IbisFile, index: tuple[int | slice, int | slice]) -> Any:
    if isinstance(index[1], slice):
        r = range(*index[1].indices(self.number_col))
        if isinstance(index[0], slice):
            r2 = range(*index[0].indices(self.number_row))
            return [[self.column(i)[j] for i in r] for j in r2]  # type: ignore[attr-defined]
        return [self.column(i)[index[0]] for i in r]  # type: ignore[attr-defined]
    return self.column(index[1])[index[0]]  # type: ignore[attr-defined]


if have_ibis_file:
    IbisFile.__getitem__ = _getitem  # type: ignore[misc]


def _setitem(
    self: IbisFile, index: tuple[int | slice, int | slice], value: Any
) -> None:
    if isinstance(index[1], slice):
        r = range(*index[1].indices(self.number_col))
        if isinstance(index[0], slice):
            r2 = range(*index[0].indices(self.number_row))
            for i2, j2 in enumerate(r2):
                for i1, j1 in enumerate(r):
                    self.column(j1)[j2] = value[i2][i1]  # type: ignore[attr-defined]
            return
        for i1, j1 in enumerate(r):
            self.column(j1)[index[0]] = value[i1]  # type: ignore[attr-defined]
        return
    self.column(index[1])[index[0]] = value  # type: ignore[attr-defined]


if have_ibis_file:
    IbisFile.__setitem__ = _setitem  # type: ignore[index]


def _shape(self: IbisFile) -> tuple[int, int]:
    return (self.number_row, self.number_col)


if have_ibis_file:
    IbisFile.shape = property(_shape)  # type: ignore[attr-defined]


def _enter(self: IbisFile) -> IbisFile:
    return self


if have_ibis_file:
    IbisFile.__enter__ = _enter  # type: ignore[attr-defined]


def _exit(
    self: IbisFile,
    type: type[BaseException] | None,
    value: BaseException | None,
    traceback: TracebackType | None,
) -> None:
    self.close()


if have_ibis_file:
    IbisFile.__exit__ = _exit  # type: ignore[attr-defined]


def _create(fname: str, data: np.ndarray, type: str = "DOUB") -> None:
    """Create a ibis file that exactly contains the given array, all of
    the given type."""
    # __enter__, __exit__ and __setitem__ are added to IbisFile above
    with IbisFile(fname, data.shape[0], [type] * data.shape[1]) as f:  # type: ignore[attr-defined]
        f[:, :] = data


if have_ibis_file:
    IbisFile.create = staticmethod(_create)  # type: ignore[attr-defined]

__all__ = []
