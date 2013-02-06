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
#. Create the initial image ground connection collection
#. Correct the RPCs for the input images
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
work with files on a local disk, rather than using NFS mounted drives
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
2. A second Worldview 1 or 2 panchromatic NITF file acquired of the same area in the same path (so something on the order of 10 seconds later).
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

It is not necessary that the reference image and DEM be in at the
same resolution or map projection as the resulting DEM - e.g., you can use
3 meter imagery in UTM and 10 meter DEM to generate a 1 meter DEM in 
geographic projection.

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

Create the initial image ground connection collection
-----------------------------------------------------
One of the central things used in DEM generation is an
"image ground connection collection". 
This is used internally by the various programs, and as a user you can
ignore most of the details about this object. But it get created by supplying
a reference DEM, and set of images (in our example, 2 for the stereo pair),
and a list of RPC parameters to adjust.

We keep track of this information a "sqlite shelve object". This is a file
that contains various names values.  The file typically has a ".db" extension
and can contain any number of key/value pairs. Various programs access
these values by being passed a "file\:key" pair, e.g., "my_data.db:tp" would
refer to the key/value pair "tp" found in the file "my_data.db". This is
somewhat analogous to the IBIS files used in AFIDS which can contain 
various parameters, although the "sqlite shelve objects" are much more 
general.

We need to create the initial image ground collection using the program
"create_gdal_igc". This takes a "file\:key" pair for the output, the set
of RPC parameters to fit for, the DEM to use, and each of the VICAR images.
We include a description of the images, which can be used in various plots.

For World view 1 or 2, we can correct just the first RPC parameters, this
corresponds to doing an overall block adjustment. So the command would be::

  create_gdal_igc --rpc-line-fit=0 --rpc-sample-fit=0 \
       nevada.db:igc_original /raid22/nevada/nevada_elv.hlf \
       10MAY-1.img "Image 1" 10MAY-2.img "Image 2"

If you don't have another DEM source to use, you can use the option
"--srtm" rather than supplying the DEM file.

This creates the file "nevada.db". You can mostly ignore exactly what
is in this file, but it can be useful to find a list of all the key/values
in it (e.g., "did I create the tiepoints yet?").  You can use the command
"shelve_dir" to find out what is in a ".db" file::
  
   shelve_dir nevada.db

This returns::
 
    igc_original


Correct the RPCs for the input images
-------------------------------------
While the RPC for WV 1 or 2 data is usually close to correct, it isn't 
sufficiently accurate for generating a DEM. We need to correct this. This
is done by running a "Simultaneous Bundle Adjustment". To run this, we 
need to do the following steps:

#. Collect tiepoints between the images. This is used to register one image
   to the other.
#. Collect ground control points between the images and a reference image.
   The is used to register the images to ground truth.
#. Run a simultaneous bundle adjustment to adjust the RPC parameters to best
   fit the tiepoints and ground control points.

Both the tiepoints and the ground control points are collected using the
tp_collect program, all that differs is the arguments passed to it.

The tp_collect program takes a "file\:key" pair to use for the input, and
a second to use as the output. Note that the file can either be the same
file or it can be different. For our example, we'll keep everything in one
file.

The tp_collect can take a bit of time to run, in our example about 2 minutes.
To speed this up, you can direct the software to run in parallel. For pistol,
we can use all 24 processors to generate this in about 12 seconds. This is 
a convenience, not a requirement.

You can generate a tie point collection by::

  tp_collect --number-process=24 nevada.db:igc_original nevada.db:tpcol

The GCPs is generated in a similar way, although we need to supply a few
other arguments. 

First, the default is to overwrite the output. Since we
want to *add* GCPs rather than overwrite the existing tie points, we need the
option "--add-tp". 

We then need to supply both the reference image and DEM.
Note that we already gave the DEM when we created the image ground
collection, but in general there can be a *different* DEM used for the
reference image vs. the image ground collection. In our example, they happen
to be the same.

Finally, it isn't necessary to have as many GCPs as we do tiepoints. The
tiepoints are generated by default as a 100 x 100 grid, i.e. 10,000 points.
This is overkill for the GCPs, we can generate just a 10x10 grid. 

Putting this all together, we have::

  tp_collect --gcp --add-tp --number-process=24 --number-x=10 --number-y=10 \
  nevada.db:igc_original /raid22/nevada/nevada_doq.img \
  /raid22/nevada/nevada_elv.hlf nevada.db:tpcol


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

