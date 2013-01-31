Generate DEM
============

This contains directions for generating a DEM. 

We'll use a real example for the this document. The example used is a pair
of Worldview 2 images acquired over Port Gaston in Nevada. This was acquired
May 21, 2010.

Note that all of the DEM software is normal unix commands. You can call them 
directly from the unix shell, or wrap them in a shell script. If you wish to
use VICAR TAE, you can simply put a "ush" command before each of the unix 
commands.

There are several steps to creating the DEM:

#. Set up to use the latest version of AFIDS/Python
#. Set up an area to work
#. Locate the input data
#. Convert the NITF to VICAR format
#. Move your data from the local disk to the raids

Setting up your environment
----------------------------

Set up your environment for the latest version of Afids/Python. If you
are on pistol, this can be done by:: 

  source /opt/afids/setup_afids_env.sh

or :: 

  source /opt/afids/setup_afids_env.csh 

depending on if you are using bash or csh as your login shell.

Setting up an area to work
--------------------------

The DEM is very resource intensive. For performance reasons, it also makes
use of memory mapped files. For both these reasons, it is required that you
work with files on a local disk, rather than using NFS mounted drived 
(e.g., the raids). So the data should be generated on a local disk, and then
moved to one of the raids after generating the DEM. 

There is no reason to move the original input files to the local disk, only
the intermediate files we'll be generating need to be on the local disk.

For pistol, the local disk is /data.  You can make an area to work in by
something like::

   mkdir -p /data/smyth/DemWork
   cd /data/smyth/DemWork

(replace the directory with your own name and scratch area).

Locate the input data
---------------------
There are four pieces of input data required:

1. A Worldview 1 or 2 panchromatic NITF file.
2. A second Worlview 1 or 2 panchromatic NITF file acquired of the same area in the same path (so something on the order of 10 seconds later).
3. A reference image.
4. An initial DEM.

The reference image is used to correct errors in the original
Worldview RPCs.  This can be from a number of sources, such as the
Digital Orthophoto Quadrangle (DOQ, see `here
<http://egsc.usgs.gov/isb/pubs/factsheets/fs05701.html>`_) or the
National Map (see `here
<http://egsc.usgs.gov/isb/pubs/factsheets/fs10702.html>`_).

The reference image can be in VICAR format, but this isn't required. Any format
GDAL can read (e.g., geotiff) is fine also.

Likewise, the initial DEM is used both to correct errors in the original
Worldview RPCs and as a initial starting point when generating the DEM.  This
can come from any source (e.g., the National Map). If you don't have a better
source, you can use the SRTM database that is included in AFIDS.

For our example, the input data is:

1. /raid22/nevada/10MAY21184820-P1BS-052366903050_01_P001.NTF
2. /raid22/nevada/10MAY21184840-P1BS-052366905020_01_P001.NTF
3. /raid22/nevada/nevada_doq.img
4. /raid22/nevada/nevada_elv.hlf

Convert the NITF to VICAR format
--------------------------------
The NITF format is pretty slow, so we convert the Worldview 1 or 2 data to
vicar format. This uses the standard tool "`gdal_translate <http://www.gdal.org/gdal_translate.html>`_". This tool converts from any format GDAL can read to
any other format (including VICAR). The default output is Geotiff, so to get
VICAR format you need to specify "-of VICAR". The program matches the data
type of the input to the output by default, which works fine for the NITF data
(which is "HALF"). You can force a particular type by using the "-ot" option.

For our example, the conversion would be as follows::

  gdal_translate -of VICAR /raid22/nevada/10MAY21184820-P1BS-052366903050_01_P001.NTF 10MAY-1.img
  gdal_translate -of VICAR /raid22/nevada/10MAY21184840-P1BS-052366905020_01_P001.NTF 10MAY-2.img

If desired, you can also make a symbolic link (i.e., "ln -s") from the
original location of the input DEM and reference image. This is
entirely a matter of preference, I often do this just to keep straight where
all the input data is. For the purpose of this example, we'll skip this step.
If you do create symbolic links, you can just replace the later steps with the
full path with a local path.

Move your data from the local disk to the raids
-----------------------------------------------
The local disk on pistol is much faster, but it is also relatively small.
You shouldn't leave the data on it once you are done generating the DEM.
The various intermediate files generated during the DEM creation do not need
to be kept, unless you want them for a record. So you can do something like::

  mkdir -p /raid1/smyth/NevadaDem
  mv dem* /raid1/smyth/NevadaDem
  rm *

(replace with your own directories). You can leave off the "rm \*" and just
move everything if you want to keep the intermediate files.

