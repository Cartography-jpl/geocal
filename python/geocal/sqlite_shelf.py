# The Shelve package is a very useful way to easily implement persistence.
# But it has the disadvantage that depending on the system we are on different
# databases will be available (e.g., not every system has berkely db).
# Also the Shelve package is apparently deprecated in future version of python.
# I spent some time googling, and several places recommended using sqlite3
# to implement this. In addition to being available (we check this at as
# part of the configure script) it is also faster.
#
# This class gives a Shelve like interface to sqlite.

from __future__ import annotations
from collections.abc import MutableMapping as DictMixin

import pickle
import sqlite3
import os.path
import geocal_swig  # type: ignore
import os
from pathlib import Path
from typing import Any
from collections.abc import Iterator

# Allow jsonpickle to be used, if available.
try:
    import jsonpickle  # type: ignore

    have_jsonpickle = True
    jsonpickle.set_encoder_options(
        "json", sort_keys=True, indent=4, separators=(",", ": ")
    )
except ImportError:
    have_jsonpickle = False


def to_db_type(value: Any) -> Any:
    """If value's type is supported natively in SQLite, return value.
    Otherwise, return a pickled representation."""
    if value is None or isinstance(value, (int, int, float, bytes, str)):
        return value
    else:
        return bytes(pickle.dumps(value))


def from_db_type(value: Any) -> Any:
    """Converts a value from the database to a Python object."""
    if isinstance(value, bytes):
        return pickle.loads(value)
    return value


def read_shelve(fn: str | os.PathLike[str]) -> Any:
    """This handles reading a value from a shelve file. The string f should
    be of the form file_name:key, file_name.xml, file_name.bin,
    or file_name.json. We
    open the given file, and read the value for the given key.

    A problem with the python shelve/pickle files is that it can't
    communicate directly with a C++ program, and also the files aren't
    human readable or portable.  So we also support xml files, we key
    off of the file name and if it is something like "foo.xml" we read
    that rather than a shelve file. "foo.bin" is the binary version of
    of the serialized data.

    We also support files with the extension "foo.json" as json pickle
    files. This doesn't have any additional functionality over the
    sqlite python, but it does make for more human readable files that
    are sometimes preferable. We use jsonpickle for reading these
    files.

    Note that it can be useful to execute python code before using
    a shelve file, e.g., we are using python modules not already
    included in AFIDS. If the special key "_extra_python_init" is
    found, we execute the code found there. This can do things like
    import modules. For xml, bin and json files, we look for the file
    "extra_python_init.py" found in the same directory.

    Because we often use relative names for files, we first chdir to
    the same directory as the database file (if different than the current
    one). We change back to the current directory when done.

    """
    f = str(Path(fn))
    fname = f.split(":")[0]
    dirn, fb = os.path.split(fname)
    curdir = os.getcwd()
    try:
        if dirn:
            os.chdir(dirn)
        if os.path.splitext(f)[1] == ".xml":
            if os.path.exists("extra_python_init.py"):
                exec(open("extra_python_init.py").read())
            return geocal_swig.serialize_read_generic(fb)
        if os.path.splitext(f)[1] == ".bin":
            if os.path.exists("extra_python_init.py"):
                exec(open("extra_python_init.py").read())
            return geocal_swig.serialize_read_binary_generic(fb)
        if os.path.splitext(f)[1] == ".json":
            if os.path.exists("extra_python_init.py"):
                exec(open("extra_python_init.py").read())
            if have_jsonpickle:
                return jsonpickle.decode(open(fb).read())
            else:
                raise RuntimeError(
                    "Use of .json file requires jsonpickle package to be installed"
                )
        t = SQLiteShelf(fb, "r")
        if "_extra_python_init" in list(t.keys()):
            exec(t["_extra_python_init"])
        key = f.split(":")[1]
        return t[key]
    finally:
        os.chdir(curdir)


def shelve_time_after(f1n: str | os.PathLike[str], f2n: str | os.PathLike[str]) -> bool:
    """Compare the update time on 2 shelve objects, return if f1 update time
    >= f2 update time. Note that either f1 or f2 can be files, in which case
    we use the file modify time instead.
    It is ok if f1 doesn't exist, in that case always return False."""
    f1 = str(Path(f1n))
    f2 = str(Path(f2n))
    if ":" in f1:
        fname, key = f1.split(":")
        t = SQLiteShelf(fname, "r")
        if key in t:
            f1time = t.update_time_unix(key)
        else:
            return False
    else:
        if os.path.exists(f1):
            f1time = os.path.getmtime(f1)
        else:
            return False
    if ":" in f2:
        fname, key = f2.split(":")
        t = SQLiteShelf(fname, "r")
        f2time = t.update_time_unix(key)
    else:
        f2time = os.path.getmtime(f2)
    return f1time >= f2time


def write_shelve(fn: str | os.PathLike[str], val: Any) -> None:
    """This handles writing a value to a shelve file, possibly creating the
    file is it doesn't exist. The string f should be of the form
    file_name:key. We open/create the given file and write the value for
    the given key.

    A problem with the shelve files is that it can't communicate directly
    with a C++ program, and also the files aren't human readable or portable.
    So we also support xml and bin files, we key off of the file name and if it
    is something like "foo.xml" we write that rather than a shelve file.
    """
    f = str(Path(fn))
    if os.path.splitext(f)[1] == ".xml":
        geocal_swig.serialize_write(f, val)
        return
    if os.path.splitext(f)[1] == ".bin":
        geocal_swig.serialize_write_binary(f, val)
        return
    if os.path.splitext(f)[1] == ".json":
        if have_jsonpickle:
            with open(f, "w") as fh:
                fh.write(jsonpickle.encode(val))
            return
        else:
            raise RuntimeError(
                "Use of .json file requires jsonpickle package to be installed"
            )
    fname, key = f.split(":")
    d = SQLiteShelf(fname)
    d[key] = val
    d.close()


class SQLiteShelf(DictMixin):
    """Shelf implementation using an SQLite3 database."""

    def __init__(self, filename: str | os.PathLike[str], mode: str = "r+") -> None:
        """Open an existing file, or create a new one if it doesn't exist.
        The mode can be "r+" for read/write or "r" for read only.
        """
        self._database: None | sqlite3.Connection = sqlite3.connect(filename)
        self._database.execute(
            "CREATE TABLE IF NOT EXISTS Shelf "
            "(Key TEXT PRIMARY KEY NOT NULL, Value BLOB, Updated datetime)"
        )
        self._open = True
        self._read_only = mode == "r"

    def __del__(self) -> None:
        self.close()

    def __getitem__(self, key: str) -> Any:
        if self._database is None:
            raise RuntimeError("Attempt to use database after it has been closed()")
        row = self._database.execute(
            "SELECT Value FROM Shelf WHERE Key=?", [key]
        ).fetchone()
        if row:
            return from_db_type(row[0])
        else:
            raise KeyError(key)

    def update_time(self, key: str) -> str:
        """Return updated time as a string."""
        if self._database is None:
            raise RuntimeError("Attempt to use database after it has been closed()")
        row = self._database.execute(
            "SELECT Updated FROM Shelf WHERE Key=?", [key]
        ).fetchone()
        if row:
            return from_db_type(row[0])
        else:
            raise KeyError(key)

    def update_time_julian(self, key: str) -> float:
        """Return updated time as Julian day, including fraction"""
        if self._database is None:
            raise RuntimeError("Attempt to use database after it has been closed()")
        row = self._database.execute(
            "SELECT julianday(Updated) FROM Shelf WHERE Key=?", [key]
        ).fetchone()
        if row:
            return from_db_type(row[0])
        else:
            raise KeyError(key)

    def update_time_unix(self, key: str) -> float:
        """Return updated time as unix time, including fraction"""
        # Unix epoch in Julian days is 2440587.5
        return (self.update_time_julian(key) - 2440587.5) * 86400.0

    def touch(self, key: str) -> None:
        """Change the updated time to now (like the unix command "touch")"""
        if self._database is None:
            raise RuntimeError("Attempt to use database after it has been closed()")
        if self._read_only:
            raise RuntimeError("Attempt to write to read only shelve.")
        self._database.execute(
            "UPDATE Shelf SET Updated=strftime('%Y-%m-%d %H:%M:%f', 'now') WHERE Key=?",
            [key],
        )

    def __setitem__(self, key: str, value: Any) -> None:
        if self._database is None:
            raise RuntimeError("Attempt to use database after it has been closed()")
        if self._read_only:
            raise RuntimeError("Attempt to write to read only shelve.")
        self._database.execute(
            "INSERT OR REPLACE INTO Shelf VALUES (?, ?, strftime('%Y-%m-%d %H:%M:%f', 'now'))",
            [key, to_db_type(value)],
        )

    def __delitem__(self, key: str) -> None:
        if self._database is None:
            raise RuntimeError("Attempt to use database after it has been closed()")
        if self._read_only:
            raise RuntimeError("Attempt to delete from read only shelve.")
        self._database.execute("DELETE FROM Shelf WHERE Key=?", [key])

    def keys(self) -> list[str]:  # type: ignore[override]
        """Return a list of keys in the shelf."""
        if self._database is None:
            raise RuntimeError("Attempt to use database after it has been closed()")
        return [row[0] for row in self._database.execute("SELECT Key FROM Shelf")]

    def close(self) -> None:
        """Commit changes and close the file."""
        if self._database is not None:
            self._database.commit()
            self._database.close()
            self._database = None

    # These are needed by python 3, but not python 2
    def __len__(self) -> int:
        raise RuntimeError("Not implemented yet")

    def __iter__(self) -> Iterator[Any]:
        raise RuntimeError("Not implemented yet")


__all__ = ["read_shelve", "shelve_time_after", "write_shelve", "SQLiteShelf"]
