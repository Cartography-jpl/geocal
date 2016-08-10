from .target import Target
from .misc import makedirs_p
import os
import subprocess
import shutil
import re
import xml.etree.ElementTree as ET

class InLocalTarget(Target):
    '''Implementation of Target where we use a local directory to store
    the file. The file is copied from its location to the given
    directory (if it isn't already there). We do *not* remove the
    local directory when running finish, a separate cleanup of the
    directory should be done when all Tasks have finished. This is
    because the local directory is usually used by multiple targets,
    and the same target might be used more than once (e.g., navigation
    file is used as input to multiple processes in a pipeline).
    '''
    def __init__(self, fname, local_directory):
        '''Give a file, and local directory to copy and read the file from.
        Note that it is ok for local_directory to be None, in that case this
        acts the same as InFileTarget'''
        self.fname = os.path.abspath(fname)
        self.local_directory = local_directory
        if(self.local_directory is not None):
            self.lfname = os.path.abspath(self.local_directory + self.fname)
        else:
            self.lfname = self.fname

    def prepare(self):
        '''Do any work with the target needed to get the local_filename
        file ready.'''
        if(self.local_directory is None):
            return
        makedirs_p(os.path.dirname(self.lfname))
        if(not os.path.exists(self.lfname)):
            subprocess.run(["rsync", "--copy-links", self.fname, self.lfname],
                           check=True)

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
        '''The working, local name of the file.'''
        return self.lfname

    def remove(self):
        if(self.filename() != self.local_filename()):
            self.silent_remove(self.local_filename())
    

class OutLocalTarget(Target):
    '''Implementation of Target where we use a local directory to store the
    file. The file is generated in given directory.

    We do *not* remove the local directory on running finish, a
    separate cleanup of the directory should be done when all Tasks
    have finished. This is because the local directory is usually used
    by multiple targets, and the same target might be used more than
    once (e.g., navigation file is used as input to multiple processes
    in a pipeline).

    On finish, we copy the file from it local copy to it final copy.
    '''
    def __init__(self, fname, local_directory,
                 generating_extension=".generating"):
        '''Give a file, and local directory to write the file to and copy to
        the final location. Note that it is ok for local_directory to be 
        None, in that case this acts the same as OutFileTarget'''
        self.fname = os.path.abspath(fname)
        self.generating_extension=generating_extension
        self.local_directory = local_directory
        if(self.local_directory is not None):
            self.lfname = os.path.abspath(self.local_directory + self.fname)
        else:
            self.lfname = self.fname
        makedirs_p(os.path.dirname(self.lfname))

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
        '''The working, local name of the file.'''
        return self.lfname + self.generating_extension

    def remove(self):
        self.silent_remove(self.fname)
        self.silent_remove(self.fname + ".error")
        self.silent_remove(self.local_filename())
        self.silent_remove(self.lfname)

    def finish(self):
        '''Do any work with the Target needed when we are finished generating
        it.'''
        if(self.local_filename() != self.lfname):
            os.rename(self.local_filename(), self.lfname)
        if(self.local_directory is None):
            return
        makedirs_p(os.path.dirname(self.fname))
        subprocess.run(["rsync", "--links", self.lfname, self.fname],
                       check=True)

    def finish_error(self):
        '''Do any work needed with Target when an error occurred in the 
        generation of the file.'''
        makedirs_p(os.path.dirname(self.fname))
        try:
            os.rename(self.local_filename(), self.lfname + ".error")
            # Don't bother checking run, we just give up if rsync fails
            if(self.local_directory is not None):
                subprocess.run(["rsync", "--links", self.lfname + ".error",
                                self.fname + ".error"])
        except OSError:
            # File might not have been generated. If it is missing, just
            # silently ignore this problem
            pass

class OutTempLocalTarget(Target):
    '''Implementation of Target where we use a local directory to store the
    file. The file is generated in given directory.

    We do *not* remove the local directory on running finish, a
    separate cleanup of the directory should be done when all Tasks
    have finished. This is because the local directory is usually used
    by multiple targets, and the same target might be used more than
    once (e.g., navigation file is used as input to multiple processes
    in a pipeline).

    This particular version is similar to OutLocalTarget, but we do
    *not* copy the file out once we are done. Instead, we allow the file
    to be deleted.
    '''
    def __init__(self, fname, local_directory):
        self.fname = os.path.abspath(fname)
        self.lfname = os.path.abspath(local_directory + self.fname)
        makedirs_p(os.path.dirname(self.lfname))

    def exists(self):
        '''Return true if the target exists already, false otherwise. This 
        checks the local version of the filename, not the file final version.
        I think that is what we want, but we can rethink the logic of this
        if needed.'''
        return os.path.exists(self.lfname)

    def error_exists(self):
        '''Return true if an error version of the output exists.'''
        return os.path.exists(self.lfname() + ".error")

    def filename(self):
        '''The real, permanent name of the file. We never actually create
        this file.'''
        return self.fname

    def local_filename(self):
        '''The working, local name of the file.'''
        return self.lfname  + ".generating"

    def remove(self):
        self.silent_remove(self.fname)
        self.silent_remove(self.lfname + ".error")
        self.silent_remove(self.local_filename())
        self.silent_remove(self.lfname)
        
    def remove_temporary(self):
        '''This immediately removes the temporary file. You don't need to
        call this usually, but it can be useful if you know you are done
        with the file and want to clean up the disk space. Otherwise you can
        just wait until the local directory is removed.'''
        for f in (self.lfname, self.lfname + ".generating",
                  self.lfname + ".error"):
            try:
                os.unlink(f)
            except OSError as exc:
                pass  # Ok if this cleanup fails

    def finish(self):
        '''Do any work with the Target needed when we are finished generating
        it.'''
        os.rename(self.lfname + ".generating", self.lfname)

    def finish_error(self):
        '''Do any work needed with Target when an error occurred in the 
        generation of the file.'''
        makedirs_p(os.path.dirname(self.fname))
        try:
            os.rename(self.lfname + ".generating", self.lfname + ".error")
        except OSError:
            # File might not have been generated. If it is missing, just
            # silently ignore this problem
            pass

class OutTempDirLocalTarget(Target):
    '''This is like OutTempLocalTarget, but instead of a file this points to 
    a directory. This is how the ray caster code works
    '''
    def __init__(self, dname, local_directory):
        self.dname = os.path.abspath(dname)
        self.lfname = os.path.abspath(local_directory + self.dname)
        self.gfname = os.path.dirname(self.lfname) + "_generating/" + \
                      os.path.basename(self.lfname)
        self.efname = os.path.dirname(self.lfname) + "_error/" + \
                      os.path.basename(self.lfname)
        makedirs_p(os.path.dirname(self.gfname))
        # Come up with a unique "filename". This doesn't actually exist,
        # but because all the directory names from ray caster are the
        # same for all view, we combine the 2 to come up with a "name".
        # This is really just a key used to uniquely identify this file
        self.fname = os.path.dirname(self.dname) + "_" + os.path.basename(self.dname)

    def exists(self):
        '''Return true if the target exists already, false otherwise. This 
        checks the local version of the filename, not the file final version.
        I think that is what we want, but we can rethink the logic of this
        if needed.'''
        return os.path.exists(self.lfname)


    def error_exists(self):
        '''Return true if an error version of the output exists.'''
        return os.path.exists(self.lfname + ".error")

    def filename(self):
        '''The real, permanent name of the file. We never actually create
        this file.'''
        return self.fname

    def local_filename(self):
        '''The working, local name of the file.'''
        return self.gfname

    def remove(self):
        for f in (self.lfname, self.gfname, self.efname):
            try:
                shutil.rmtree(f, True)
            except OSError as exc:
                pass  # Ok if this cleanup fails
        
    def remove_temporary(self):
        '''This immediately removes the temporary file. You don't need to
        call this usually, but it can be useful if you know you are done
        with the file and want to clean up the disk space. Otherwise you can
        just wait until the local directory is removed.'''
        for f in (self.lfname, self.gfname, self.efname):
            try:
                shutil.rmtree(f, True)
            except OSError as exc:
                pass  # Ok if this cleanup fails

    def finish(self):
        '''Do any work with the Target needed when we are finished generating
        it.'''
        makedirs_p(os.path.dirname(self.lfname))
        os.rename(self.gfname, self.lfname)

    def finish_error(self):
        '''Do any work needed with Target when an error occurred in the 
        generation of the file.'''
        makedirs_p(os.path.dirname(self.efname))
        try:
            os.rename(self.gfname, self.efname)
        except OSError:
            # File might not have been generated. If it is missing, just
            # silently ignore this problem
            pass
        
class GeneratedOutLocalTarget(Target):
    '''This is like OutLocalTarget, however because of the way the
    underlying some airmspi executables works we can't use the
    'generating' trick in the filename like we do with
    OutLocalTarget. This is because we just pass a stem to the program
    and it generates all the output names. So this is a specialized
    Target to handle this.
    '''
    def __init__(self, fname, local_directory, generating_name = None):
        '''This can be fed the explicit name to use for the generating file.
        If not supplied, we replace whatever comes before the extension
        with "_generating".'''
        self.fname = os.path.abspath(fname)
        self.local_directory = local_directory
        if(self.local_directory is not None):
            self.lfname = os.path.abspath(local_directory + self.fname)
        else:
            self.lfname = self.fname
        makedirs_p(os.path.dirname(self.lfname))
        self.generating_name = generating_name
        if(self.generating_name is None):
            stem, ext = os.path.splitext(self.fname)
            self.generating_name = stem + "_generating" + ext
        if(self.local_directory is not None):
            self.generating_name = os.path.abspath(local_directory +
                                                   self.generating_name)

    def exists(self):
        return os.path.exists(self.filename())

    def error_exists(self):
        '''Return true if an error version of the output exists.'''
        return os.path.exists(self.filename() + ".error")

    def filename(self):
        return self.fname
        
    def local_filename(self):
        '''The working, local name of the file.'''
        return self.generating_name
    
    def remove(self):
        self.silent_remove(self.fname)
        self.silent_remove(self.fname + ".error")
        self.silent_remove(self.local_filename())
        self.silent_remove(self.lfname)
        self.silent_remove(self.lfname + ".error")

    def finish(self):
        os.rename(self.local_filename(), self.lfname)
        if(self.local_directory):
            makedirs_p(os.path.dirname(self.fname))
            subprocess.run(["rsync", "--links", self.lfname, self.fname],
                           check=True)
    def finish_error(self):
        makedirs_p(os.path.dirname(self.fname))
        try:
            os.rename(self.local_filename(), self.lfname + ".error")
            if(self.local_directory):
                # Don't bother checking run, we just give up if rsync fails
                subprocess.run(["rsync", "--links", self.lfname + ".error",
                                self.fname + ".error"])
        except OSError:
            # File might not have been generated. If it is missing, just
            # silently ignore this problem
            pass
        
class XmlLocalTarget(Target):
    '''The boost serialization xml files are a little complicated, because
    they point to multiple other files (e.g., the Igc points to the nav
    file). This means if we move the xml file to the local directory, we
    need to *also* move all the supporting files.

    This target handles that, moving both the xml file and all dependencies
    local.
    '''
    def __init__(self, fname, local_directory):
        '''Give a file, and local directory to copy and read the file from.
        Note that it is ok for local_directory to be None, in that case this
        acts the same as InFileTarget'''
        self.fname = os.path.abspath(fname)
        self.local_directory = local_directory
        if(self.local_directory is not None):
            self.lfname = os.path.abspath(self.local_directory + self.fname)
        else:
            self.lfname = self.fname

    def prepare(self):
        '''Do any work with the target needed to get the local_filename
        file ready.'''
        if(self.local_directory is None):
            return
        makedirs_p(os.path.dirname(self.lfname))
        if(not os.path.exists(self.lfname)):
            subprocess.run(["rsync", "--copy-links", self.fname, self.lfname],
                           check=True)

        doc = ET.parse(self.fname)
        curdir = os.getcwd()
        dirn, fb = os.path.split(self.fname)
        try:
            if(dirn):
                os.chdir(dirn)
                # Look for things that looks like they might be a relative
                # path in the XML file. This looks for strings that start
                # with a "./" or "../"
            possible_fname = [t.text for t in doc.getroot().iter()
                  if t.text is not None and re.match('\.?\./',t.text)]
            # Check that file exists (e.g., not just a string that looks
            # like a file name), and that it is a file 
            fname = [os.path.abspath(f) for f in possible_fname
                     if os.path.exists(f) and
                     os.path.isfile(os.path.realpath(f))]
        finally:
            os.chdir(curdir)
        # Now prepare each of these files
        for f in fname:
            InLocalTarget(f, self.local_directory).prepare()

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
        '''The working, local name of the file.'''
        return self.lfname
            
    def remove(self):
        self.silent_remove(self.fname)
        # Note we don't remove support files. I'm not 100% sure this
        # is what we want, but I think we only want to remove the xml file.
        # Not likely to ever be an issue, since this is all in the local
        # directory anyways.
        
    
