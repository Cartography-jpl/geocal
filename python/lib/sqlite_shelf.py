# The Shelve package is a very useful way to easily implement persistence.
# But it has the disadvantage that depending on the system we are on different
# databases will be available (e.g., not every system has berkely db).
# Also the Shelve package is apparently deprecated in future version of python. 
# I spent some time googling, and several places recommended using sqlite3
# to implement this. In addition to being available (since we install this
# already as part of afids) it is also faster.
#
# This class gives a Shelve like interface to sqlite.

import UserDict
import pickle
import sqlite3

def to_db_type(value):
    """If value's type is supported natively in SQLite, return value.
    Otherwise, return a pickled representation. """
    if value is None or isinstance(value, (int, long, float,
                                           basestring)):
        return value
    else:
        return buffer(pickle.dumps(value))

def from_db_type(value):
    """ Converts a value from the database to a Python object. """
    if isinstance(value, buffer):
        return pickle.loads(value)
    else:
        return value

def read_shelve(f):
    '''This handles reading a value from a shelve file. The string f should
    be of the form file_name:key. We open the given file, and read the
    value for the given key.'''
    fname, key = f.split(':')
    t = SQLiteShelf(fname, "r")
    return t[key]

def write_shelve(f, val):
    '''This handles writing a value to a shelve file, possibly creating the
    file is it doesn't exist. The string f should be of the form
    file_name:key. We open/create the given file and write the value for
    the given key.'''
    fname, key = f.split(':')
    d = SQLiteShelf(fname)
    d[key] = val
    d.close()

class SQLiteShelf(UserDict.DictMixin):
    """Shelf implementation using an SQLite3 database. """
    def __init__(self, filename, mode = "r+"):
        '''Open an existing file, or create a new one if it doesn't exist.
        The mode can be "r+" for read/write or "r" for read only.'''
        self._database = sqlite3.connect(filename)
        self._database.execute("CREATE TABLE IF NOT EXISTS Shelf "
                               "(Key TEXT PRIMARY KEY NOT NULL, Value BLOB)")
        self._open = True
        self._read_only = (mode == "r")

    def __del__(self):
        self.close()

    def __getitem__(self, key):
        row = self._database.execute("SELECT Value FROM Shelf WHERE Key=?",
                                     [key]).fetchone()
        if row:
            return from_db_type(row[0])
        else:
            raise KeyError(key)

    def __setitem__(self, key, value):
        if(self._read_only):
            raise RuntimeError("Attempt to write to read only shelve.")
        self._database.execute("INSERT OR REPLACE INTO Shelf VALUES (?, ?)",
                               [key, to_db_type(value)])

    def __delitem__(self, key):
        if(self._read_only):
            raise RuntimeError("Attempt to delete from read only shelve.")
        self._database.execute("DELETE FROM Shelf WHERE Key=?", [key])

    def keys(self):
        """Return a list of keys in the shelf."""
        return [row[0] for row in
                self._database.execute("SELECT Key FROM Shelf")]

    def close(self):
        """Commit changes and close the file."""
        if self._database is not None:
            self._database.commit()
            self._database.close()
            self._database = None 

