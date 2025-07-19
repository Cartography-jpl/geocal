import geocal_swig
import subprocess
import os
import tempfile

# Useful extensions to CartLabMultifile
def _create_image_chip(self, out_fname, gdal_img_to_match, out_format="VICAR", border=30,
                       resampling_method="bilinear", error_threshold=0.125):
    '''CartLabMultifile already has a create_subset_file which is good for
    creating a file we can match against. However this assumes that we are
    generating a image in the same projection (lat/lon). We have case such
    as matching against the GRI image chips where we also want to be able
    to change the map projection (e.g. UTM). This procedure handles both
    of these steps - subsetting the data and changing projection. This will
    generate an image with the same projection as the gdal_img, along with
    the same resolution.

    The error_threshold is in pixels, and is the amount of error that can
    be introduced in the reprojection. This can be set to 0 to not do any
    approximation (see gdalwarp documentation for a description of this).

    Note that the image will generally not be the same size - we add the given
    border. But the center of the images should match up.

    The resampling method is anything gdalwarp uses. If the gdal_img is at a
    higher resolution we usually use "bilinear". For a lower resolution, we can
    use "average".

    We use a temporary file in the directory that the output goes.
    '''
    img_to_match_fname = gdal_img_to_match.file_names[0]
    workdir = os.path.dirname(os.path.abspath(out_fname))
    with tempfile.TemporaryDirectory(dir=workdir) as tdir:
        gc1 = gdal_img_to_match.ground_coordinate(
            geocal_swig.ImageCoordinate(0,0))
        gc2 = gdal_img_to_match.ground_coordinate(
            geocal_swig.ImageCoordinate(0,gdal_img_to_match.number_sample-1))
        gc3 = gdal_img_to_match.ground_coordinate(
            geocal_swig.ImageCoordinate(gdal_img_to_match.number_line-1,0))
        gc4 = gdal_img_to_match.ground_coordinate(
            geocal_swig.ImageCoordinate(gdal_img_to_match.number_line-1,
                                        gdal_img_to_match.number_sample-1))
        self.create_subset_file(f"{tdir}/ichip_landsat.tif", "gtiff", [gc1,gc2,gc3,gc4], None,
                                "", "", border, False)
        gc1=gdal_img_to_match.ground_coordinate(
            geocal_swig.ImageCoordinate(gdal_img_to_match.number_line//2,
                                        gdal_img_to_match.number_sample//2))
        gc2=gdal_img_to_match.ground_coordinate(
            geocal_swig.ImageCoordinate(gdal_img_to_match.number_line//2+1,
                                        gdal_img_to_match.number_sample//2+1))
        res_x = abs(gc1.x - gc2.x)
        res_y = abs(gc1.y - gc2.y)
        subprocess.run(f"gdalsrsinfo -o wkt {img_to_match_fname} > {tdir}/ichip.wkt",
                       shell=True, check=True)
        subprocess.run(f"gdalwarp -q -t_srs {tdir}/ichip.wkt -tr {res_x} {res_y} {tdir}/ichip_landsat.tif -overwrite -of {out_format} -r {resampling_method} {out_fname}", shell=True, check=True)

geocal_swig.CartLabMultifile.create_image_chip = _create_image_chip


        
        
