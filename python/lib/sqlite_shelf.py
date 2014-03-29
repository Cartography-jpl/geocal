# The Shelve package is a very useful way to easily implement persistence.
# But it has the disadvantage that depending on the system we are on different
# databases will be available (e.g., not every system has berkely db).
# Also the Shelve package is apparently deprecated in future version of python. 
# I spent some time googling, and several places recommended using sqlite3
# to implement this. In addition to being available (we check this at as 
# part of the configure script) it is also faster.
#
# This class gives a Shelve like interface to sqlite.

import UserDict
import pickle
import sqlite3
import os.path

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
    value for the given key.

    Note that it can be useful to execute python code before using
    a shelve file, e.g., we are using python modules not already 
    included in AFIDS. If the special key "_extra_python_init" is 
    found, we execute the code found there. This can do things like 
    import modules.

    Because we often use relative names for files, we first chdir to 
    the same directory as the database file (if different than the current
    one). We change back to the current directory when done.
    '''
    fname, key = f.split(':')
    dirn, f = os.path.split(fname)
    curdir = os.getcwd()
    try:
        if(dirn):
            os.chdir(dirn)
        t = SQLiteShelf(f, "r")
        if("_extra_python_init" in t.keys()):
            exec(t["_extra_python_init"])
        return t[key]
    finally:
        os.chdir(curdir)

def shelve_time_after(f1, f2):
    '''Compare the update time on 2 shelve objects, return if f1 update time
    >= f2 update time. Note that either f1 or f2 can be files, in which case
    we use the file modify time instead.
    It is ok if f1 doesn't exist, in that case always return False. '''
    if(':' in f1):
        fname, key = f1.split(':')
        t = SQLiteShelf(fname, "r")
        if key in t:
            f1time = t.update_time_unix(key)
        else:
            return False
    else:
        if(os.path.exists(f1)):
            f1time = os.path.getmtime(f1)
        else:
            return False
    if(':' in f2):
        fname, key = f2.split(':')
        t = SQLiteShelf(fname, "r")
        f2time = t.update_time_unix(key)
    else:
        f2time = os.path.getmtime(f2)
    return f1time >= f2time
        

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
        The mode can be "r+" for read/write or "r" for read only.
        '''
        self._database = sqlite3.connect(filename)
        self._database.execute("CREATE TABLE IF NOT EXISTS Shelf "
                               "(Key TEXT PRIMARY KEY NOT NULL, Value BLOB, Updated datetime)")
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

    def update_time(self, key):
        '''Return updated time as a string.'''
        row = self._database.execute("SELECT Updated FROM Shelf WHERE Key=?",
                                     [key]).fetchone()
        if row:
            return from_db_type(row[0])
        else:
            raise KeyError(key)

    def update_time_julian(self, key):
        '''Return updated time as Julian day, including fraction'''
        row = self._database.execute("SELECT julianday(Updated) FROM Shelf WHERE Key=?",
                                     [key]).fetchone()
        if row:
            return from_db_type(row[0])
        else:
            raise KeyError(key)

    def update_time_unix(self, key):
        '''Return updated time as unix time, including fraction'''
        # Unix epoch in Julian days is 2440587.5
        return (self.update_time_julian(key) - 2440587.5) * 86400.0

    def touch(self, key):
        '''Change the updated time to now (like the unix command "touch")'''
        if(self._read_only):
            raise RuntimeError("Attempt to write to read only shelve.")
        self._database.execute("UPDATE Shelf SET Updated=strftime('%Y-%m-%d %H:%M:%f', 'now') WHERE Key=?", [key])

    def __setitem__(self, key, value):
        if(self._read_only):
            raise RuntimeError("Attempt to write to read only shelve.")
        self._database.execute("INSERT OR REPLACE INTO Shelf VALUES (?, ?, strftime('%Y-%m-%d %H:%M:%f', 'now'))",
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

