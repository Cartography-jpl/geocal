#! /usr/bin/env python
#
# This is a utility script that takes a nitro source code file found in
# the c/nitf/shared directory and attempts to rewrite the information into
# our python format. This is really a one time thing to do the population
# of these TRE python code initially, but we'll go ahead and keep this script
# around in case it is needed in the future.

from geocal import *
import re
version="%prog April 4, 2017"
usage="""Usage:
  nitro_to_python.py [options] <nitro_source_file>
  nitro_to_python.py -h | --help
  nitro_to_python.py -v | --version

This is a utility script that takes a nitro source code file found in
the c/nitf/shared directory and attempts to rewrite the information into
our python format. This is really a one time thing to do the population
of these TRE python code initially, but we'll go ahead and keep this script
around in case it is needed in the future.

Note that you need to edit the resulting python. We have a few "blahs" in
the help message where you need to specify thing (e.g. the reference for the 
TRE). Also, nitro doesn't distinguish between str and float types, you will
often need to manually change str to float and add a frmt string. Also, nitro
doesn't have defaults.

Options:
  -h --help         
       Print this message

  -v --version      
       Print program version

"""
args = docopt_simple(usage, version=version)

txt = open(args.nitro_source_file).read()
m = re.search('NITF_DECLARE_SINGLE_PLUGIN\((\w+), description\)', txt)
if(not m):
    raise RuntimeError("We only work with NITF_DECLARE_SINGLE_PLUGIN, and this wasn't found in file %s" % args.nitro_source_file)
tre_name = m.group(1)
m = re.search('description\[\]\s*=\s*\{(.*?)\};', txt, re.DOTALL)
if(not m):
    raise RuntimeError("Couldn't find description in file %s" % args.nitro_source_file)
desc = m.group(1)
out = '''hlp = \'\'\'This is the %s TRE, blah. 

The field names can be pretty cryptic, but are documented in detail in 
the NITF TRE documentation (STDI-0002 V4.0, available at 
http://www.gwg.nga.mil/ntb/baseline/docs/stdi0002).

There is a table in the main body on page vii that gives the a pointer for 
where in the document a particular TRE is defined.

%s is documented at blah.
\'\'\'
desc = ["%s",
''' % (tre_name, tre_name, tre_name)

for ln in re.split('\},?', desc):
    ln = re.sub('\s*{','',ln)
    if(ln[0:4]=="NITF"):
        typ, sz, d, v = re.split('\s*"?,\s*"?', ln)
        v = re.sub('"','', v).lower().strip()
        if(typ == "NITF_BCS_N"):
            out += '        ["%s", "%s", %s, int],\n' % (v, d, sz)
        elif(typ == "NITF_BCS_A"):
            if(re.match('rsrvd\d*', v)):
                out += '        [None, None, %s, str],\n' % (sz)
            else:
                out += '        ["%s", "%s", %s, str],\n' % (v, d, sz)
        elif(typ == "NITF_END"):
            pass
        else:
            print("Skipping for now %s" % typ)
    
out+= ''']

Tre%s = create_nitf_tre_structure("Tre%s",desc,hlp=hlp)
''' % (tre_name, tre_name)
print(out)
