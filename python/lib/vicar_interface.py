from __future__ import print_function
from __future__ import absolute_import
from builtins import str
from builtins import object
import os
import shutil
import tempfile
import subprocess
import time
import geocal_swig
from .misc import makedirs_p, run_tee
import six

class VicarInterface(object):
    '''This provides a basic interface for calling a vicar routine 
    from Python. This include several helper routines for building up
    and running a command.'''
    def __init__(self):
        self.print_output = False
        self.log_file = False
        self.debug = False
        self.keep_run_dir = False
        self.input = []
        self.output = []
        self.cmd = ""
        self.before_body = ""
        self.title = "vicar command"
        self.timing = True
        self.force_cleanup = False
        self.run_dir_name = None
        
    def pre_run(self):
        '''This gets called after the temporary directory has been created
        and everything set up, but before we actually run. This can be 
        overridden by derived classes to put any special set up in place
        (e.g., copy other files)'''
        pass

    def post_run(self):
        '''This after the run, but before we have destroyed the
        temporary directory. This can be overridden by derived classes
        to put any special set up in place (e.g., copy other files)'''
        pass

    def build_command(self, arg_list, extra_text = []):
        '''This builds up a VICAR command by adding the given argument list
        to what is already in self.cmd. The arg_list is looked up in the
        attributes of this class. If a particular attribute is not found,
        then this is just left off the command. You can add extra text
        to be put at the end for things that don't fit the arg_list format
        (for example, passing in local variables - see vicar_wrap.scinterp.
        for an example).'''
        res = []
        for a in arg_list:
            if a in self.__dict__:
                res.append(self.argstring(a))
        res.extend(extra_text)
        if(len(res) > 0):
            self.cmd += " " + " +\n".join(res) + "\n"

    def argstring(self, arg):
        '''This build up a single argument, handling quotes and processing
        arrays'''
        if(getattr(self.__dict__[arg], '__iter__', False)):
            if(isinstance(self.__dict__[arg], six.string_types)):
                arg2 = [self.__dict__[arg]]
            else:
                arg2 = [str(x) for x in self.__dict__[arg]]
            if(len(arg2) ==0): return ""
            if(isinstance(self.__dict__[arg][0], int) or
               isinstance(self.__dict__[arg][0], float)):
                res = arg + "="
                is_string = False
            else: 
                res = arg + "=\""
                is_string = True
            if(len(arg2) ==1):
                res += arg2[0]
            else:
                res += "(" + ",".join(arg2) + ")"
        else:
            if(isinstance(self.__dict__[arg], int) or
               isinstance(self.__dict__[arg], float)):
                res = arg + "="
                is_string = False
            else: 
                res = arg + "=\""
                is_string = True
            res += str(self.__dict__[arg])
        if(is_string):
            res += "\""
        return res

    def vicar_run(self):
        '''This runs a set of vicar commands in a vicarb. To handle the files 
        that VICAR tends to vomit, and to allow parrallel execution with other
        vicar commands, we run in a temporary directory that is removed when
        done. You can list any input and output files that should be linked in
        or moved out. The input files will be available in the top level of the
        current directory, where the given cmd will run. The following 
        attributues 
        are used:

        print_output 
            If true, print output of VICAR function to stdout
        debug
            If true, print out command before running it. This also implies
            print_output and keep_run_dir.
        log_file
            If true, copy the output of the VICAR function to the given
            file name.
        keep_run_dir
            If true, keep run directory after completing function. This
            is primarily useful for debugging problems with the actual VICAR
            comand.
        run_dir_name
            If present, can specify the name of the run directory. Useful with
            keep_run_dir if we have a unique, meaningful name. Without this
            we use mkdtemp to create a unique directory name.
        input
            List of files the should be linked in to the temporary directory 
            to be available for the command.
        output
            List of files that will be copied out. These files should all
            be generated in the top level by VICAR (i.e., the VICAR command
            should not have a path), but the actual output file listed here
            can include path information to specify where the file should go.
        cmd 
            Vicar command to run.
        title
            Title used in timing message.
        timing
            If true, print out timing information as we run.
        force_cleanup
            Normally we leave the run directory in place if an error occurs.
            If this is true, then we always clean up the directory.
        before_body
            Any text before the body of the cmd (e.g., declaration of local
            variables)
            '''
    
        if(self.timing):
            time_start = time.time()
            print("Starting " + self.title)
        if(self.debug):
            print("Vicar command:")
            print(self.cmd)
        d = None
        curdir = None
        successfully_done = False
        try:
            if(self.run_dir_name is not None):
                d = self.run_dir_name
                makedirs_p(d)
            else:
                d = tempfile.mkdtemp(dir='./')
            for i in self.input:
                os.symlink(os.path.abspath(i), d + "/" + os.path.basename(i))
            outabs = [os.path.abspath(x) for x in self.output]
            if(self.log_file): 
                self.log_file = os.path.abspath(self.log_file)
            curdir = os.getcwd()
            os.chdir(d)
            # Don't think I need these anymore, so comment these out.
            # We could do this in pre_run if needed by a particular
            # vicar command.
            # These were used by the Shiva stuff. Not sure if we still
            # need these, but they are cheap to create so leave in
            # place for now
            #for f in ["basemos", "baseraw", "dtedmos", "dtedraw", 
            #          "finaltst", "rawtst", "scratch"]: 
            #    os.mkdir(f)
            with open("tmppdf.pdf", "w") as f:
                f.write(
"""procedure
%s
body
%s

end-proc
""" % (self.before_body, self.cmd))

            self.pre_run()
            try:
                fh = None
                if(self.log_file):
                    fh = open(self.log_file, "w")
                self.run_out = run_tee(["vicarb", "tmppdf"], out_fh = fh,
                         quiet = (not self.debug and not self.print_output))
            except subprocess.CalledProcessError as ex:
                print("Vicar call failed. Log of VICAR:")
                print(self.run_out)
                raise
            for f in outabs:
                shutil.move(os.path.basename(f), f)
            self.post_run()
            successfully_done = True
        finally:
            if(curdir):
                os.chdir(curdir)
            if(self.force_cleanup or 
               (successfully_done and d and not self.keep_run_dir and 
                not self.debug)):
                shutil.rmtree(d)
        if(self.timing):
            print("Done with " + self.title +". Time: ", \
                time.time() - time_start)


class __VicarToNarray(VicarInterface):
    def __init__(self, cmd, output_name):
        VicarInterface.__init__(self)
        self.cmd = cmd
        self.timing = False
        self.output_name = output_name

    def post_run(self):
        t = geocal_swig.VicarRasterImage(self.output_name)
        self.res = t.read(0, 0, t.number_line, t.number_sample)

def vicar_to_numpy(cmd, output_name = "out"):
    '''A common thing to do (particularly when testing), is to run
    a set of VICAR commands that result in an output file, and then
    reading the output into a numpy array. This helper function wraps this 
    into one easy call.'''
    vi = __VicarToNarray(cmd, output_name)
    vi.vicar_run()
    return vi.res

__all__ = ["VicarInterface", "vicar_to_numpy"]
