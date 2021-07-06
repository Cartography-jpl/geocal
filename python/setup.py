import os
from setuptools import setup, find_packages

# Not fully integrated in, we'll get the build to use pip instead of
# explicit files at some point. For now, manually run this with
# something like:
#
# pip install --prefix=/home/smyth/Local/geocal-build/install .
#
# or
#
# pip install --prefix=/home/smyth/Local/geocal-build/install -e .
#

setup(
    name='geocal',
    version='1.26.1',
    description='GeoCal',
    author='Mike Smyth',
    author_email='Mike.M.Smyth@jpl.nasa.gov',
    packages=find_packages(include=["geocal"],
                           exclude=["pynitf", "test_support", "lib", "init"]),
    install_requires=[
        'numpy', 'scipy', 'matplotlib', 'h5py', 'scikit-umfpack', 'pandas'
    ],
    # May want a more automatic way to handle this, but for
    # now we just list all our scripts. Note this doesn't include the
    # AFIDS TAE scripts that wrap these, I think that is how we want this
    # setup.
    scripts = ["bin/abachd",
               "bin/airmspi_dem",
               "bin/boost_nitf_glas_gfm",
               "bin/boost_nitf_rsm",
               "bin/convert_reflectance",
               "bin/create_kernel_json",
               "bin/dem_generate",
               "bin/doughnut_diff",
               "bin/gdal_to_erdas",
               "bin/gdal_to_nitf",
               "bin/gen_geneplot_cmd.py",
               "bin/height_to_color",
               "bin/igc_project",
               "bin/igc_raycast_project",
               "bin/igc_to_rpc",
               "bin/job_database_check_and_run",
               "bin/mars_generate_rsm",
               "bin/mars_import_vicar",
               "bin/mars_nest",
               "bin/nitfinfo",
               "bin/nitf_to_vicar",
               "bin/pan_to_ms_igc",
               "bin/rsm_improve",
               "bin/rsm_project",
               "bin/rsm_tiepoint",
               "bin/sba",
               "bin/shelve_airmspi_igc",
               "bin/shelve_dem",
               "bin/shelve_dir",
               "bin/shelve_igccol",
               "bin/shelve_image",
               "bin/shelve_job_run",
               "bin/shelve_show",
               "bin/srtm_90m_create_ocean_tile",
               "bin/surface_image_project",
               "bin/tiepoint_to_error",
               "bin/tp_collect",
               "bin/tpcol_to_table",
               "bin/tp_show",
               "bin/usgs_create_ocean_tile",
               "bin/write_image"]
)


