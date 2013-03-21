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
#. Create shelve objects for the initial DEM and reference image
#. Convert the NITF to VICAR format
#. Create the initial image ground connection collection
#. Correct the RPCs for the input images
#. Generate approximate orthorectified images
#. Set up for doing DEM generation
#. Actually run the DEM generation
#. Move your data from the local disk to the raids

There is a Makefile at /data/smyth/DemSample/Makefile that you can use
to run all these commands.

Shelve objects
--------------

We have a number of complicated objects used in the generation of DEM. Each
object may be made up of several pieces. For example, a DEM might contain
a height image file, along with a second datum file (e.g., National Map DEMs
that tend to be against NAVD 88). The code will create these objects once,
and then allow them to be used in multiple programs.

As a convenience, we store the objects as “pickled” python
objects. This is a built in method in python of creating persistent
objects. To allow multiple objects to be stored, we store these
pickled objects in a sqlite database, which we then can access in
other pieces of software by giving it a database file along with an
object name. The data is stored as a key/value pair, where the key is
an arbitrary name we choose to assign to it.

This is somewhat analogous to the IBIS files used in AFIDS which can contain 
various parameters (e.g., a RPC that Al's code has fitted for), 
although the shelve objects are much more general.

We refer to objects in the various programs by a "file\:key" notation.
For example, if we store a reference image in the file "nevada.db" as
"ref_image", we would refer to this as "nevada.db:ref_image"

There are a few useful general commands that work with shelve objects.
To find out what keys are in a file, you can use "shelve_dir"::

  shelve_dir nevada.db

To find out more details about a particular object, you can use the command
shelve_show::

  shelve_show nevada.db:ref_image

which returns::

  VicarLiteRasterImage:
    File:          nevada_doq.img
    Band:          0
    Number line:   33338
    Number sample: 33338
    Map Info:      
      Coordinate: Geodetic Coordinate Converter
      ULC:       (-116.3, 36.95)
      LRC:       (-116, 36.65)
      Number:    (33338, 33338)
    RPC:           None

Setting up your environment
----------------------------

Set up your environment for the latest version of Afids/Python. If you
are on pistol, this can be done by:: 

  export GDAL_DRIVER_PATH=/opt/afids_support/lib/gdalplugins
  export PATH=/opt/afids_support/bin:${PATH}
  export LD_LIBRARY_PATH=/opt/afids_support/lib64:/opt/afids_support/lib:${LD_LIBRARY_PATH}
  source /opt/afids/setup_afids_env.sh

or :: 

  setenv GDAL_DRIVER_PATH /opt/afids_support/lib/gdalplugins
  setenv PATH /opt/afids_support/bin:${PATH}
  setenv LD_LIBRARY_PATH /opt/afids_support/lib64:/opt/afids_support/lib:${LD_LIBRARY_PATH}
  source /opt/afids/setup_afids_env.csh 

depending on if you are using bash or csh as your login shell.

Note that the first part there sets up of afids support stuff like GDAL. This
can be placed in your .bashrc or .cshrc.


Setting up an area to work
--------------------------

The DEM is very resource intensive. For performance reasons, it also makes
use of memory mapped files. For both these reasons, it is required that you
work with files on a local disk, rather than using NFS mounted drives
(e.g., the raids). So the data should be generated on a local disk, and then
moved to one of the raids after generating the DEM. 

There is no reason to move the original input files to the local disk, only
the intermediate files we'll be generating need to be on the local disk,
along with the reference image and DEM.a

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

Create shelve objects for the initial DEM and reference image
-------------------------------------------------------------

To start, we need to create shelve objects for our reference image and
initial DEM. This will then allow these to be used in future programs.

We use the programs "shelve_dem" and "shelve_image" to set up these 
objects::

  cp /raid22/nevada/nevada_doq.img .
  cp /raid22/nevada/nevada_elv.hlf .
  shelve_image nevada_doq.img nevada.db:ref_image
  shelve_dem  nevada_elv.hlf nevada.db:dem_initial

Note that shelve_dem can also be directed to use the SRTM Level 2 data we
have in AFIDS by passing "--srtm" option, and it can also be passed a datum
file if the elevation is relative to mean sea level rather than a reference
ellipsoid.

In general, you can pass the option "-h" or "--help" to a program to get
a list of all the options and arguments.

Once we have created the shelve database, we can find out what the contents
are::

  shelve_dir nevada.db

This prints out::

  dem_initial
  ref_image

Looking at the DEM we created::

   shelve_show nevada.db:dem_initial

which returns::

  Vicar Lite Dem:
    File: nevada_elv.hlf
    Band: 0
    Map info:
      Coordinate: Geodetic Coordinate Converter
      ULC:       (-117.001, 38.0007)
      LRC:       (-114.999, 35.9992)
      Number:    (21617, 21616)
    Datum:
      Simple Datum, undulation 0m 
    Outside Dem is error: 1

And the reference image::

  shelve_show nevada.db:ref_image

which returns::

  VicarLiteRasterImage:
    File:          nevada_doq.img
    Band:          0
    Number line:   33338
    Number sample: 33338
    Map Info:      
      Coordinate: Geodetic Coordinate Converter
      ULC:       (-116.3, 36.95)
      LRC:       (-116, 36.65)
      Number:    (33338, 33338)
    RPC:           None


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


Create the initial image ground connection collection
-----------------------------------------------------

One of the central things used by the software is called a “Image
Ground Connection” or “IGC”. This is a generalization of the
traditional orbit/camera model used in photogrammetry to an object
that can support frame cameras, push broom cameras, and systems using
RPCs. We have an image, and a generic invertible function that maps
that image to and from the ground. For WV-2 we use an image plus an
RPC.  A set of 1 or more IGCs forms a “IGC Collection”. This is the
central object used by the simultaneous bundle adjustment and DSM
generation software.

We need to create the initial image ground collection using the program
"shelve_igccol". This takes a "file\:key" pair for the output, the set
of RPC parameters to fit for, the DEM to use, and each of the VICAR images.
We include a description of the images, which can be used in various plots.

For World view 1 or 2, we can correct just the first RPC parameters, this
corresponds to doing an overall block adjustment. 

**Note** see the optional subset command in the next section before running
this.

The command for the full DEM would be::

  shelve_igccol --rpc-line-fit=0 --rpc-sample-fit=0 \
       nevada.db:igc_initial nevada.db:dem_initial \
       10MAY-1.img "Image 1" 10MAY-2.img "Image 2"

If we then look at what was created::
 
  shelve_show nevada.db:igc_initial

we get::

  IgcArray
    Number of images: 2
    Images:
       Image 1
       Image 2
    Parameters:
       Igc 0 - RPC Line Numerator Parameter 0: 0.003955
       Igc 0 - RPC Sample Numerator Parameter 0: 0.000752
       Igc 1 - RPC Line Numerator Parameter 0: 0.003429
       Igc 1 - RPC Sample Numerator Parameter 0: 0.000342

*Optional* Create subsetted IGC collection
------------------------------------------

For normal runs, you want to generate the DEM that fully covers the input
images. However, it can be useful to work with a subset of the data. This
is particularly useful the first time you run through this example, since
everything will run much faster.

The trick to doing this is to

#. Create a subsetted version of the input data
#. Use the subsetted data in the IGC creation

You can create a subsetted version of the input data by creating a shelve
object to subset the data. Note that this doesn't actually generate a new
VICAR file, all it does is set things up so this looks like a subset in
future programs. This is done by using "shelve_image" like we have before,
but now specifying a "--subset". To get a 5000x5000 subset a little ways into
the image, you can run::

  shelve_image --subset 1000 1000 5000 5000 10MAY-1.img \
      nevada.db:img1_sub
  shelve_image --subset 1000 1000 5000 5000 10MAY-2.img \
      nevada.db:img2_sub

(Note here that this is start_line, start_sample, number_line, number_sample.
Also this is 0 based, rather than the 1 based used in VICAR).

Now if we look at what was created::

  shelve_show nevada.db:img1_sub

Returns::

  SubRasterImage from (1000, 1000) to (5999, 5999)
    Underlying RasterImage: 
      VicarLiteRasterImage:
        File:          10MAY-1.img
        Band:          0
        Number line:   25356
        Number sample: 35180
        Map Info:      None
        RPC:           
          RPC:
            <Blah Blah Blah...>

The shelve_igcol command is very similar, but we use the option 
"--from-shelve" to get the images from our shelve objects rather than from
existing files::

  shelve_igccol --rpc-line-fit=0 --rpc-sample-fit=0 \
    --from-shelve \
    nevada.db:igc_initial nevada.db:dem_initial \
   nevada.db:img1_sub "Image 1 Sub" nevada.db:img2_sub "Image 2 Sub"

After creating this subsetted IGC collection, the rest of the directions
are the same.

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

  tp_collect --number-process=24 nevada.db:igc_initial nevada.db:tpcol

The GCPs is generated in a similar way, although we need to supply a
new arguments. The default is to overwrite the output. Since we
want to *add* GCPs rather than overwrite the existing tie points, we need the
option "--add-tp". 

So we have::

  tp_collect --gcp --add-tp --number-process=24 \
  nevada.db:igc_initial nevada.db:ref_image nevada.db:tpcol

Looking at what we have generated::

  shelve_show nevada.db:tpcol

gives::

  TiePointCollection
    Total number tp: 13855
    Number GCPs:     7082

You can now run the simultaneous bundle adjustment to adjust the RPCs of the
initial IGC collection. This also adjusts the ground location of the 
tiepoints, so we generate 2 outputs. The command::

   sba nevada.db:igc_initial nevada.db:tpcol nevada.db:igc_sba \
       nevada.db:tpcol_sba

Note that you can pass the accuracy of the underlying DEM and GCP by
the arguments "--dem-sigma" and "--gcp-sigma". The defaults for both of
these are 10 meters. 

For DOQ data, the horizontal accuracy is required to
have 90% match the U.S. National Map Accuracy Standards for Horizontal 
Accuracy (see for `here <http://www.tceq.texas.gov/gis/natmap.html>`_) 
at 1:12,000 
scale for 3.75 minute quarter quadrangles, and 1:24,000 scale for 7.5 
minute quadrangles. This corresponds to 10.2 and 12.2 meters respectively,
so the default of 10 meters is reasonable good guess. You can modify this
for different data sources, giving more or less weight to the GCPs.

You should pay some attention to the final :math:`\chi^2`. This indicates
how well we fitted the given tiepoint, subject to the constraints of the
initial DEM. This should be a number on the order or 1 or so. 2 or 3
should also be fine, but much larger than that indicates we are not fitting
the data well. In that case, you'll want to investigate further why we
are fitting so poorly (this is outside the scope of this document).

Generate approximate orthorectified images
------------------------------------------

The next step is to produce orthorectified images using the improved RPCs.
These are approximate because we are using the initial DEM rather than
the final one. But this data is sufficient for us to do the stereo retrieval
on.

The program for this is "igc_project". The resolution of the output defaults
to 1.0. meter, which is a bit coarser than the WV-2. So we change this to 
0.5 meter. Another parameter is the "grid-spacing". By default the RPC is
calculated for every point. This is a bit of an overkill, particularly if
the initial DEM is at a coarser resolution. If we set the grid-spacing to 10,
we calculate the RPC for every 10th point and then do a linear interpolation
in between. This is almost as accurate as calculating every point, but is
much faster.

Even with this, it can take a couple of minutes per image to do the 
orthorectification. So we can use a number of processors to do this in 
parallel.

We can do this calculation by::

  igc_project --grid-spacing=10 --resolution=0.5 --number-process=24 \
      nevada.db:igc_sba 0 10MAY-1_proj.img
  igc_project --grid-spacing=10 --resolution=0.5 --number-process=24 \
      nevada.db:igc_sba 1 10MAY-2_proj.img

(The "0" and "1" here is the 0 based image number)

We then shelve these objects to use in the next step::

  shelve_image 10MAY-1_proj.img nevada.db:surface_1
  shelve_image 10MAY-2_proj.img nevada.db:surface_2

Set up for doing DEM generation
-------------------------------

The DEM generation takes a long time. We make use of the system "torque".
The home page is `here <http://www.adaptivecomputing.com/products/open-source/torque>`_, and a short introduction is `here <http://www.rcc.uh.edu/hpc-docs/49-using-torque-to-submit-and-monitor-jobs.html>`_.

We start with a program that creates a empty DEM file, along with a script
that can be used to submit all the torque jobs to fill in the DEM. We
need to supply the IGC Collection, the surface images, the 0-based indexes
to use the stereo retrieval, and the base name to use for the output::

  setup_dem_job nevada.db:igc_sba nevada.db:surface_1 nevada.db:surface_2 \
     0 1 nevada_generated_dem

Actually run the DEM generation
-------------------------------  

Once the jobs are set up, you can run them by executing the script
"nevada_generated_dem.job_script". You need to supply a queue to run this
on. Right now, we only have one queue "long" on pistol, so you submit this
by::

  ./nevada_generated_dem.job_script -q long

This will submitted a number of jobs in the queue, which you can monitor
using "qstat" (note various useful options like "qstat -q" to show a
queue-centric view, and "qstat -t" which shows all the jobs in a single
job array).

Note that you do *not* need to finish on DEM job before submitting another.
You can also *wait* to submit the jobs, say right before leaving for the day.
A useful scenario would be to run a number of DEM jobs through the 
"setup_dem_job" step, and then submitting all of the to the queue at the 
end of the day.

Final orthorectified images
-------------------------------  

An optional final step is to generate orthorectified images. This isn't
necessary for generating the DEM, but is often a desired output.

It can also be useful to generate these images with exactly the same map 
projection, so they can be overlaided on top of each other (e.g., open
in xvd). You can do this by::

  shelve_dem --outside-dem-not-error \
     nevada_generated_dem_filled.img nevada.db:dem_generated_filled
  igc_project --number-process=24 --resolution=0.5 \
     --dem=nevada.db:dem_generated_filled \
     nevada.db:igc_sba 0 10MAY-1_proj_final.img
  shelve_image 10MAY-1_proj_final.img nevada.db:final_proj_1
  igc_project --number-process=24 \
    --map-info=nevada.db:final_proj_1 \
     --dem=nevada.db:dem_generated_filled \
     nevada.db:igc_sba 1 10MAY-2_proj_final.img

Note that the grid spacing defaults to 1 here. Since we have a high resolution
DEM, we need to run the RPC on every point. The second image is fit to
the same map info as the first, rather than calculating it own value.


Move your data from the local disk to the raids
-----------------------------------------------

The local disk on pistol is much faster, but it is also relatively small.
You shouldn't leave the data on it once you are done generating the DEM.
The various intermediate files generated during the DEM creation do not need
to be kept, unless you want them for a record. So you can do something like::

  mkdir -p /raid1/smyth/NevadaDem
  mv nevada_generated_dem.img /raid1/smyth/NevadaDem
  rm *

(replace with your own directories). You can leave off the "rm \*" and just
move everything if you want to keep the intermediate files.

