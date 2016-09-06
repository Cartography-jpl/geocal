from .misc import makedirs_p
import os
import hashlib
import shutil
import subprocess
import glob
# This is python 3 only
try:
    from contextlib import suppress
except ImportError:
    # Don't fail, we just can't use target with python 2
    pass

class OutputCache(object):
    '''This class is used to manage a 'output cache'. This is used to support
    doing unit and system testing. It maintains a copy of the output from
    a long running task, e.g., running one of the MSPI executables. We store
    a copy of the output files and/or STDOUT. The output can consist of 0 or
    more files, and STDOUT can be present or None if not used.

    The date takes a array of strings as a key, we use this to form a hash
    code (based on SHA1) that is used to index into the data. We store the
    data in the directory supplied to this class.

    The array of strings can be whatever is useful, e.g., the set of run 
    arguments used to run the MSPI executable (stripped of paths that may
    vary from one run to the next).
    '''
    def __init__(self, output_cache_dir):
        self.output_cache_dir = os.path.abspath(output_cache_dir)

    def _cache_dir(self, key_arr):
        '''Create directory we will use to store output cache data.'''
        m = hashlib.sha1()
        m.update(" ".join(key_arr).encode('utf-8'))
        return self.output_cache_dir + "/output_cache_" + m.hexdigest()

    def contains(self, key_arr):
        '''Returns True if we have a cache entry for the given key. 
        Note key_arr should be an array of strings.'''
        return os.path.exists(self._cache_dir(key_arr))

    def load(self, key_arr, targets, overwrite=False):
        '''This copies the cached output to the given list of targets. Note
        that there is no requirement that targets use the same name as what
        was originally saved, we just match up the first target to the first
        cached file, the second the second cached, etc.

        Note that we create a symbolic link to the cached data, rather than
        copying this. I think this is what we want, it speeds tests up by
        not doing potentially large file copies. We can revisit this if we
        actually want to do something else.

        Normally it is an error if the output target already exists. However
        you can pass in overwrite=True to allow overwriting

        We return stdout if it was stored, or none if it wasn't.
        '''
        if(not self.contains(key_arr)):
            raise RuntimeError("The requested key isn't found in the OutputCache")
        d = self._cache_dir(key_arr)
        for i, t in enumerate(targets):
            g = glob.glob("%s/%d_*" % (d, i))
            if(len(g) == 0):
                raise RuntimeError("Output %d not found in the OutputCache %s" % (i, d))
            if(len(g) > 1):
                raise RuntimeError("This shouldn't be able to happen")
            t.local_mkdir()
            if(overwrite):
                with suppress(FileNotFoundError):
                    os.remove(t.local_filename())
            os.symlink(g[0], t.local_filename())
        f = d +"/STDOUT"
        stdout = None
        if(os.path.exists(f)):
            with open(f, "rb") as fh:
                stdout = fh.read()
        return stdout

    def store(self, key_arr, targets, stdout = None):
        '''Store the given list of Targets, and if supplied stdout.'''
        d = self._cache_dir(key_arr)
        shutil.rmtree(d, True)
        makedirs_p(d)
        for i, t in enumerate(targets):
            if not os.path.exists(t.local_filename()):
                raise RuntimeError("Output target %s doesn't eixst" %
                                   t.local_filename())
            cfile = "%s/%d_%s" % (d, i, os.path.basename(t.filename()))
            if(os.path.isdir(t.local_filename())):
                subprocess.run(["rsync", "--copy-links", "-r",
                                t.local_filename() + "/",
                                cfile + "/"], check=True)
                
            else:
                subprocess.run(["rsync", "--copy-links", t.local_filename(),
                                cfile], check=True)
        if(stdout is not None):
            with open(d + "/STDOUT", "wb") as fh:
                fh.write(stdout)
        
