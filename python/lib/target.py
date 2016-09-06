from .misc import makedirs_p
import os
# This is python 3 only
try:
    from contextlib import suppress
except ImportError:
    # Don't fail, we just can't use target with python 2
    pass

class Target(object):
    '''A Target is a generalization of a file, it has a permanent
    "filename" and a "local_filename".  Depending on the type of
    target, these file names may or may not be the same. A Target has
    a "prepare" function that is called before it is used, this might
    for example copy an input file from its permanent "filename"
    location to a local disk using the "local_filename".
    
    Once a Target has been generated, a "finish" routine is
    called. This might copy a file from a local directory to it final
    permanent "filename" location.  If an error occurs when the Target
    was being generated, "finish_error" is called instead.

    Note that the term "Target" has nothing to do with a AirMSPI
    target, this is just an unfortunate similarity in the name. The
    term is inspired by the Luigi package (see
    https://luigi.readthedocs.io/en/stable/index.html). We don't
    actually use this package, but we use the same name.

    '''
    def exists(self):
        '''Return true if the target exists already, false otherwise'''
        return False

    def error_exists(self):
        '''Return true if an error version of the output exists.'''
        return False

    def remove(self):
        '''Delete all the files associated with the target. Normally an input
        target will only delete possibly cached versions of these (leaving the
        original file in place), but the output targets will delete all the
        files.'''
        pass

    def filename(self):
        '''The real, permanent name of the file.'''
        raise NotImplementedError()

    def local_filename(self):
        '''The working, local name of the file. Might be a copy on a local disk
        of the final file.'''
        return self.filename()

    def local_mkdir(self):
        '''Make the directory local_filename is in.'''
        makedirs_p(os.path.dirname(self.local_filename()))

    def prepare(self):
        '''Do any work with the target needed to get the local_filename
        file ready.'''
        pass
    
    def finish(self):
        '''Do any work with the Target needed when we are finished generating
        it.'''
        pass

    def finish_error(self):
        '''Do any work needed with Target when an error occurred in the 
        generation of the file.'''
        pass

    def silent_remove(self, fname):
        '''This removes a file fname, but doesn't complain if the file
        doesn't exist. All other errors do trigger an exception (e.g.,
        permission problem when removing a file).'''
        with suppress(FileNotFoundError):
            os.remove(fname)

class InFileTarget(Target):
    '''Implementation of Target which is just a file on the disk.'''
    def __init__(self, fname):
        self.fname = fname

    def exists(self):
        '''Return true if the target exists already, false otherwise'''
        return os.path.exists(self.filename())
    
    def error_exists(self):
        '''Return true if an error version of the output exists.'''
        return os.path.exists(self.filename() + ".error")

    def filename(self):
        '''The real, permanent name of the file.'''
        return self.fname

class LogFileTarget(Target):
    '''Implementation of Log file. We don't do anything with renaming these.'''
    def __init__(self, fname):
        self.fname = fname
        if(os.path.dirname(self.fname) != ""):
            makedirs_p(os.path.dirname(self.fname))

    def exists(self):
        '''Return true if the target exists already, false otherwise'''
        # Always consider this as existing. We don't want to force a rerun
        # just to get a log file.
        return True

    def remove(self):
        self.silent_remove(self.fname)
        
    def filename(self):
        '''The real, permanent name of the file.'''
        return self.fname
    
class OutFileTarget(Target):
    '''Implementation of Target which is a file on the disk. We initially
    give the file a ".generating" extension when creating, then move this
    to the final name when done. If an error occurs, we rename the file 
    ".error"'''
    def __init__(self, fname, generating_extension=".generating"):
        self.fname = fname
        self.generating_extension=generating_extension
        if(os.path.dirname(self.fname) != ""):
            makedirs_p(os.path.dirname(self.fname))
        
    def exists(self):
        '''Return true if the target exists already, false otherwise'''
        return os.path.exists(self.filename())

    def error_exists(self):
        '''Return true if an error version of the output exists.'''
        return os.path.exists(self.filename() + ".error")

    def filename(self):
        '''The real, permanent name of the file.'''
        return self.fname

    def local_filename(self):
        '''The working, name of file.'''
        return self.filename() + self.generating_extension

    def remove(self):
        self.silent_remove(self.fname)
        self.silent_remove(self.fname + ".error")
        self.silent_remove(self.local_filename())
        
    def finish(self):
        '''Do any work with the Target needed when we are finished generating
        it.'''
        if(self.local_filename() != self.fname):
            os.rename(self.local_filename(), self.fname)
    
    def finish_error(self):
        '''Do any work needed with Target when an error occurred in the 
        generation of the file.'''
        try:
            os.rename(self.local_filename(), self.fname + ".error")
        except OSError:
            # File might not have been generated. If it is missing, just
            # silently ignore this problem
            pass

        
        
    
