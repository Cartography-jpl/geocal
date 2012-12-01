#include "geocal_gdal.h"
#include "ogr_coordinate.h"
#include "simple_dem.h"
#include <gdal_priv.h>
using namespace GeoCal;

//-----------------------------------------------------------------------
/// File names. This can contain zero or more files (zero for instance
/// for a virtual dataset).
//-----------------------------------------------------------------------

std::vector<std::string> GdalBase::file_names() const
{
// This uses newer features of GDAL than we want to depend on.
//   GDALDataset& ds = const_cast<GDALDataset&>(*data_set());
//   char **flist =  ds.GetFileList();
//   std::vector<std::string> res;
//   for(char **f = flist; f != 0; ++f)
//     res.push_back(std::string(*f));
//   CSLDestroy(flist);
//   return res;
  return fnames_;
}

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

void GdalBase::print(std::ostream& Os) const 
{
  Os << "Gdal file, files (";
  std::vector<std::string> fn = file_names();
  for(std::vector<std::string>::size_type i = 0; i < fn.size(); ++i) {
    Os << fn[i];
    if(i < fn.size() - 1)
      Os << ", ";
  }
  Os << ")\n";
}

//-----------------------------------------------------------------------
/// Read the MapInfo for the file. An exception is thrown if there
/// isn't map projection information in the file or if it is an
/// unsupported map projection. 
//-----------------------------------------------------------------------

MapInfo GdalBase::map_info() const
{
  if(!has_map_info())
    throw Exception("Attempt to read map_info() data from a GDAL file without map_info()");
  // Read in the coordinate system. Because it is so common, we handle
  // a Geographic coordinate system based on WGS84 as special and use
  // the GeodeticConverter. Otherwise, we use a more generic 
  // OgrCoordinateConverter.
  boost::shared_ptr<CoordinateConverter> coor_conv;
  boost::shared_ptr<OGRSpatialReference> 
    ogr(new OGRSpatialReference(data_set_->GetProjectionRef()));
  OGRSpatialReference sr_wgs84;
  int status = sr_wgs84.SetWellKnownGeogCS("WGS84");
  if(status != OGRERR_NONE)
    throw Exception("Call to SetWellKnownGeogCS failed");
  if(sr_wgs84.IsSame(ogr.get()))
    coor_conv.reset(new GeodeticConverter());
  else 
    coor_conv.reset(new OgrCoordinateConverter(boost::shared_ptr<OgrWrapper>
					       (new OgrWrapper(ogr))));
  blitz::Array<double, 1> param(6);
  status = data_set_->GetGeoTransform(param.data());
  if(status ==CE_Failure)
    throw Exception("Call to GetGeoTransform failed");
  boost::array<index, 2> size = 
    {{const_cast<GDALRasterBand&>(raster_band()).GetYSize(),
      const_cast<GDALRasterBand&>(raster_band()).GetXSize()}};

// For Geotiff, there are two ways the parameters are specified. For
// area based pixels, the ulc is the ulc of the pixel, which is what
// MapInfo expects. For point, the ulc given is for the center of the
// pixel. That means we need to offset this by 1/2 pixel to match what
// we expect in MapInfo. We check and see if the file has metadata
// giving this information, and if so if we need to offset the pixel.

  if(data_set_->GetMetadataItem(GDALMD_AREA_OR_POINT) &&
     data_set_->GetMetadataItem(GDALMD_AREA_OR_POINT) == 
     std::string(GDALMD_AOP_POINT)) {
    param(0) -= 0.5 * param(1) + 0.5 * param(2);
    param(3) -= 0.5 * param(4) + 0.5 * param(5);
  }

  MapInfo res(coor_conv, param, size[1], size[0]);
  return res;
}

//-----------------------------------------------------------------------
/// Set MapInfo for the file.
//-----------------------------------------------------------------------

void GeoCal::gdal_map_info(GDALDataset& D, const MapInfo& M)
{
  if(M.number_x_pixel() != D.GetRasterXSize())
    throw Exception("Map info number_x_pixel doesn't match the number of samples");
  if(M.number_y_pixel() != D.GetRasterYSize())
    throw Exception("Map info number_y_pixel doesn't match the number of lines");
  OGRSpatialReference ogr;
  if(dynamic_cast<const GeodeticConverter*>(&M.coordinate_converter())) {
    int status = ogr.SetWellKnownGeogCS("WGS84");
    if(status != OGRERR_NONE)
      throw Exception("Call to SetWellKnownGeogCS failed");
  }
  else if(const OgrCoordinateConverter* ogrconv =
      dynamic_cast<const OgrCoordinateConverter*>(&M.coordinate_converter()))
    ogr = ogrconv->ogr().ogr();
  else
    throw "Unrecognized coordinate converter in MapInfo";
  char* d;
  ogr.exportToWkt(&d);
  int status = D.SetProjection(d);
  CPLFree(d);
  if(status ==CE_Failure)
    throw Exception("SetProjection failed");
  status = D.SetGeoTransform(M.transform().data());
  if(status ==CE_Failure)
    throw Exception("SetGeoTransform failed");
}


//-----------------------------------------------------------------------
/// Register GDAL drivers once. Not normally directly called by the
/// user, this is an internal function.
//-----------------------------------------------------------------------

void GdalRegister::gdal_register()
{
  static bool registered = false;
  if(!registered) {
    GDALAllRegister();
    registered = true;
  }
}

/** \defgroup Gdal Gdal support routines **/
//-----------------------------------------------------------------------
/// \ingroup Gdal
/// Utility function to return the type of a raster band.
//-----------------------------------------------------------------------

GDALDataType GeoCal::gdal_band_type(const std::string& Fname, int Band_id)
{
  GdalRegister::gdal_register();
  boost::scoped_ptr<GDALDataset> data_set((GDALDataset *) GDALOpen(Fname.c_str(), GA_ReadOnly ));
  if(!data_set.get()) {
    Exception e;
    e << "Opening GDAL file " << Fname << " to get band type failed";
    throw e;
  }
  range_check(Band_id, 1, data_set->GetRasterCount() + 1);
  return data_set->GetRasterBand(Band_id)->GetRasterDataType();
}

//-----------------------------------------------------------------------
/// \ingroup Gdal
/// Utility function that returns true if the file contains projection
/// information, and false otherwise.
//-----------------------------------------------------------------------

bool GeoCal::gdal_has_map_info(const std::string& Fname)
{
  GdalRegister::gdal_register();
  boost::scoped_ptr<GDALDataset> data_set((GDALDataset *) GDALOpen(Fname.c_str(), GA_ReadOnly ));
  if(!data_set.get()) {
    Exception e;
    e << "Opening GDAL file " << Fname << " to get map info failed";
    throw e;
  }
  if(!data_set->GetProjectionRef() ||
     std::string(data_set->GetProjectionRef()) =="")
    return false;
  double param[6];
  if(data_set->GetGeoTransform(param) ==CE_Failure)
    return false;
  return true;
}

//-----------------------------------------------------------------------
/// Returns true if the file contains projection information, and
/// false otherwise.
//-----------------------------------------------------------------------

bool GdalBase::has_map_info() const
{
  if(!data_set_->GetProjectionRef() ||
     std::string(data_set_->GetProjectionRef()) =="")
    return false;
  double param[6];
  if(data_set_->GetGeoTransform(param) ==CE_Failure)
    return false;
  return true;
}

//-----------------------------------------------------------------------
/// \ingroup Gdal
/// Utility to create a new file. This is a thin wrapper around
/// the GDAL library that gets the requested driver and creates the
/// file with the given options. 
///
/// Because the option interface of GDAL is a bit awkward to use, we
/// supply an interface that takes a single string, with the options
/// separated by " ", so for example for a TIFF file we might have
/// "TILED=YES BLOCKXSIZE=1024 BLOCKYSIZE=1024"
//-----------------------------------------------------------------------
boost::shared_ptr<GDALDataset> 
GeoCal::gdal_create_dataset(const std::string& Fname, 
			    const std::string& Driver_name, int Number_line, 
			    int Number_sample, int Number_band,
			    GDALDataType Dt, const std::string& Options)
{
  GdalRegister::gdal_register();
  GDALDriver* driver = (GDALDriver*) GDALGetDriverByName(Driver_name.c_str());
  if(!driver)
    throw Exception("Can't find GDAL driver " + Driver_name);
  std::vector<std::string> opt_str;
  std::vector<char*> opt;
  size_t start = 0;
  std::string options = Options;
  for(size_t i = 0; i < options.length(); ++i)
    if(options[i] == ' ') {
      opt_str.push_back(options.substr(start, i - start));
      opt.push_back(const_cast<char*>(opt_str.back().c_str()));
      while(options[i] == ' ' && i < options.length())
	++i;
      start = i;
    }
  if(start + 1 < options.length()) {
    opt_str.push_back(options.substr(start));
    opt.push_back(const_cast<char*>(opt_str.back().c_str()));
  }
  opt.push_back(0);
  boost::shared_ptr<GDALDataset> data_set(
		    driver->Create(Fname.c_str(), Number_sample,
		    Number_line, Number_band, Dt, &(*opt.begin())));
  if(!data_set.get()) {
    Exception e;
    e << "Creating GDAL file " << Fname << " failed";
    throw e;
  }
  return data_set;
}

//-----------------------------------------------------------------------
/// \ingroup Gdal
/// Utility to create a new file. This is a thin wrapper around
/// the GDAL library that gets the requested driver and creates the
/// file with the given options. 
///
/// This uses the "CreateCopy" command rather than "Create", and takes
/// an existing GDALDataset. There are some drivers that only have
/// certain features available when using "CreateCopy" vs. "Create",
/// e.g., the statistics calculation for a Erdas HFA driver. Other
/// write once drivers only implement a "CreateCopy" without
/// implementing "Create", e.g., the JPEG driver. If you don't already
/// have an existing GDAL file it is often useful to use the "MEM"
/// driver that creates a file in memory only, this can then be used
/// to do a CreateCopy.
///
/// Because the option interface of GDAL is a bit awkward to use, we
/// supply an interface that takes a single string, with the options
/// separated by " ", so for example for a TIFF file we might have
/// "TILED=YES BLOCKXSIZE=1024 BLOCKYSIZE=1024"
//-----------------------------------------------------------------------
boost::shared_ptr<GDALDataset> 
GeoCal::gdal_create_copy(const std::string& Fname, 
			 const std::string& Driver_name, 
			 const GDALDataset& Source_dataset,
			 const std::string& Options,
			 bool Log_progress)
{
  GdalRegister::gdal_register();
  GDALDriver* driver = (GDALDriver*) GDALGetDriverByName(Driver_name.c_str());
  if(!driver)
    throw Exception("Can't find GDAL driver " + Driver_name);
  std::vector<std::string> opt_str;
  std::vector<char*> opt;
  size_t start = 0;
  std::string options = Options;
  for(size_t i = 0; i < options.length(); ++i)
    if(options[i] == ' ') {
      opt_str.push_back(options.substr(start, i - start));
      opt.push_back(const_cast<char*>(opt_str.back().c_str()));
      while(options[i] == ' ' && i < options.length())
	++i;
      start = i;
    }
  if(start + 1 < options.length()) {
    opt_str.push_back(options.substr(start));
    opt.push_back(const_cast<char*>(opt_str.back().c_str()));
  }
  opt.push_back(0);
  GDALProgressFunc progress = GDALDummyProgress;
  if(Log_progress)
    progress = GDALTermProgress;
  boost::shared_ptr<GDALDataset> 
    data_set(driver->CreateCopy(Fname.c_str(), 
				const_cast<GDALDataset*>(&Source_dataset),
				0, &(*opt.begin()), progress, 0));
  if(!data_set.get()) {
    Exception e;
    e << "CreatCopy for GDAL file " << Fname << " failed";
    throw e;
  }
  return data_set;
}

//-----------------------------------------------------------------------
/// \ingroup Gdal
/// Utility to create a new Erdas file. 
///
/// This is similar to gdal_create_copy, except we also create the
/// statistics and image pyramid used by Erdas to view a file. Note
/// that often there is a "fill value" such as 0 which should be
/// excluded from the statistics calculation. If that is the case,
/// then the input Source_dataset should have had SetNoDataValue
/// called on each of the bands to set this value.
///
/// This function is more expensive to call than just to do a
/// gdal_create_copy to an erdas file. We 1) calculate statistics, 2)
/// calculate an image pyramid, 3) store the data compressed.  If you
/// don't want these additional features, you shouldn't call this
/// function - instead just call gdal_create_dataset or
/// gdal_create_copy. 
//-----------------------------------------------------------------------
boost::shared_ptr<GDALDataset> 
GeoCal::gdal_create_erdas(const std::string& Fname, 
			  const GDALDataset& Source_dataset,
			  bool Log_progress)
{
  boost::shared_ptr<GDALDataset> d = 
    gdal_create_copy(Fname.c_str(), "hfa", Source_dataset,
		     "STATISTICS=YES COMPRESSED=YES", Log_progress);
  int xblocksize, yblocksize;
  d->GetRasterBand(1)->GetBlockSize(&xblocksize, &yblocksize);
  // Erdas wants image pyramids down to a single block
  std::vector<int> olist;
  for(int i = 2; 
      d->GetRasterXSize() / i > xblocksize && 
	d->GetRasterYSize() / i > yblocksize; i *= 2)
    olist.push_back(i);
  GDALProgressFunc progress = GDALDummyProgress;
  if(Log_progress)
    progress = GDALTermProgress;
  d->BuildOverviews("NEAREST", olist.size(), &(*olist.begin()), 0, 0, 
		    progress, 0);
  return d;
}

//-----------------------------------------------------------------------
/// \ingroup Gdal
/// This reads in the Rpc for the given file.
//-----------------------------------------------------------------------

Rpc GeoCal::gdal_rpc(const std::string& Fname)
{
  GdalRegister::gdal_register();
  boost::scoped_ptr<GDALDataset> data_set((GDALDataset *) GDALOpen(Fname.c_str(), GA_ReadOnly ));
  if(!data_set.get()) {
    Exception e;
    e << "Opening GDAL file " << Fname << " to get RPC failed";
    throw e;
  }
  return gdal_rpc(*data_set);
}

//-----------------------------------------------------------------------
/// \ingroup Gdal
/// Indicates if we have RPC metadata.
//-----------------------------------------------------------------------

bool GeoCal::gdal_has_rpc(const GDALDataset& D)
{
  if(const_cast<GDALDataset&>(D).GetMetadataItem("RPC_HEIGHT_OFF", ""))
    return true;
  if(const_cast<GDALDataset&>(D).GetMetadataItem("HEIGHT_OFF", "RPC"))
    return true;
  return false;
}

//-----------------------------------------------------------------------
/// \ingroup Gdal
/// This reads in the Rpc for the given data.
//-----------------------------------------------------------------------

Rpc GeoCal::gdal_rpc(const GDALDataset& D)
{
    Rpc res;
    if(const_cast<GDALDataset&>(D).GetMetadataItem("NITF_CETAG")) {
      if(gdal_metadata<std::string>(D, "NITF_CETAG") =="RPC00A")
	res.rpc_type = Rpc::RPC_A;
      else if(gdal_metadata<std::string>(D, "NITF_CETAG") =="RPC00B")
	res.rpc_type = Rpc::RPC_B;
      else
	throw Exception("Unrecognized NITF_CETAG value of '" + 
			gdal_metadata<std::string>(D, "NITF_CETAG") + "'");
    } else
      res.rpc_type = Rpc::RPC_B;

    // There are a few ways that the RPC might be encoded in the File.
    // Check first to see which way works with this data.
    if(const_cast<GDALDataset&>(D).GetMetadataItem("RPC_HEIGHT_OFF", "")) {
      if(const_cast<GDALDataset&>(D).GetMetadataItem("RPC_ERR_BIAS", ""))
	res.error_bias = gdal_metadata<double>(D, "RPC_ERR_BIAS");
      else
	res.error_bias = 0.0;
      if(const_cast<GDALDataset&>(D).GetMetadataItem("RPC_ERR_BAND", ""))
	res.error_random = gdal_metadata<double>(D, "RPC_ERR_RAND");
      else
	res.error_random = 0.0;
      res.height_offset = gdal_metadata<double>(D, "RPC_HEIGHT_OFF");
      res.height_scale = gdal_metadata<double>(D, "RPC_HEIGHT_SCALE");
      res.latitude_offset = gdal_metadata<double>(D, "RPC_LAT_OFF");
      res.latitude_scale = gdal_metadata<double>(D, "RPC_LAT_SCALE");
      res.longitude_offset = gdal_metadata<double>(D, "RPC_LONG_OFF");
      res.longitude_scale = gdal_metadata<double>(D, "RPC_LONG_SCALE");
      res.line_offset = gdal_metadata<double>(D, "RPC_LINE_OFF");
      res.line_scale = gdal_metadata<double>(D, "RPC_LINE_SCALE");
      res.sample_offset = gdal_metadata<double>(D, "RPC_SAMP_OFF");
      res.sample_scale = gdal_metadata<double>(D, "RPC_SAMP_SCALE");
      res.line_numerator = 
	gdal_metadata<boost::array<double, 20> >(D, "RPC_LINE_NUM_COEFF");
      res.line_denominator = 
	gdal_metadata<boost::array<double, 20> >(D, "RPC_LINE_DEN_COEFF");
      res.sample_numerator = 
	gdal_metadata<boost::array<double, 20> >(D, "RPC_SAMP_NUM_COEFF");
      res.sample_denominator = 
	gdal_metadata<boost::array<double, 20> >(D, "RPC_SAMP_DEN_COEFF");
    } else if(const_cast<GDALDataset&>(D).GetMetadataItem("HEIGHT_OFF", 
							  "RPC")) {
      if(const_cast<GDALDataset&>(D).GetMetadataItem("ERR_BIAS", "RPC"))
	res.error_bias = gdal_metadata<double>(D, "ERR_BIAS", "RPC");
      else
	res.error_bias = 0.0;
      if(const_cast<GDALDataset&>(D).GetMetadataItem("ERR_BAND", "RPC"))
	res.error_random = gdal_metadata<double>(D, "ERR_BIAS", "RPC");
      else
	res.error_random = 0.0;
      res.height_offset = gdal_metadata<double>(D, "HEIGHT_OFF", "RPC");
      res.height_scale = gdal_metadata<double>(D, "HEIGHT_SCALE", "RPC");
      res.latitude_offset = gdal_metadata<double>(D, "LAT_OFF", "RPC");
      res.latitude_scale = gdal_metadata<double>(D, "LAT_SCALE", "RPC");
      res.longitude_offset = gdal_metadata<double>(D, "LONG_OFF", "RPC");
      res.longitude_scale = gdal_metadata<double>(D, "LONG_SCALE", "RPC");
      res.line_offset = gdal_metadata<double>(D, "LINE_OFF", "RPC");
      res.line_scale = gdal_metadata<double>(D, "LINE_SCALE", "RPC");
      res.sample_offset = gdal_metadata<double>(D, "SAMP_OFF", "RPC");
      res.sample_scale = gdal_metadata<double>(D, "SAMP_SCALE", "RPC");
      res.line_numerator = 
	gdal_metadata<boost::array<double, 20> >(D, "LINE_NUM_COEFF", "RPC");
      res.line_denominator = 
	gdal_metadata<boost::array<double, 20> >(D, "LINE_DEN_COEFF", "RPC");
      res.sample_numerator = 
	gdal_metadata<boost::array<double, 20> >(D, "SAMP_NUM_COEFF", "RPC");
      res.sample_denominator = 
	gdal_metadata<boost::array<double, 20> >(D, "SAMP_DEN_COEFF", "RPC");
    } else {
      throw MetadataMissing("Does not appear to be any RPC metadata in file");
    }
    return res;
}

// Couple of helper routines for formating rpc labels

inline std::string to_s1(double x)
{
  const int bufsize = 1000;
  char buf[bufsize];
  snprintf(buf, bufsize, "%.4f", x);
  return std::string(buf);
}

inline std::string to_s2(double x)
{
  const int bufsize = 1000;
  char buf[bufsize];
  snprintf(buf, bufsize, "%.16e", x);
  return std::string(buf);
}

//-----------------------------------------------------------------------
/// \ingroup Gdal
/// This writes the Rpc for the given data.
//-----------------------------------------------------------------------

void GeoCal::gdal_rpc(GDALDataset& D, const Rpc& R)
{
  if(R.rpc_type == Rpc::RPC_A)
    D.SetMetadataItem("NITF_CETAG", "RPC00A");
  else
    D.SetMetadataItem("NITF_CETAG", "RPC00B");

  D.SetMetadataItem("ERR_BIAS", to_s1(R.error_bias).c_str(), "RPC");
  D.SetMetadataItem("ERR_RAND", to_s1(R.error_random).c_str(), "RPC");
  D.SetMetadataItem("HEIGHT_OFF", to_s1(R.height_offset).c_str(), "RPC");
  D.SetMetadataItem("HEIGHT_SCALE", to_s1(R.height_scale).c_str(), "RPC");
  D.SetMetadataItem("LAT_OFF", to_s1(R.latitude_offset).c_str(), "RPC");
  D.SetMetadataItem("LAT_SCALE", to_s1(R.latitude_scale).c_str(), "RPC");
  D.SetMetadataItem("LONG_OFF", to_s1(R.longitude_offset).c_str(), "RPC");
  D.SetMetadataItem("LONG_SCALE", to_s1(R.longitude_scale).c_str(), "RPC");
  D.SetMetadataItem("LINE_OFF", to_s1(R.line_offset).c_str(), "RPC");
  D.SetMetadataItem("LINE_SCALE", to_s1(R.line_scale).c_str(), "RPC");
  D.SetMetadataItem("SAMP_OFF", to_s1(R.sample_offset).c_str(), "RPC");
  D.SetMetadataItem("SAMP_SCALE", to_s1(R.sample_scale).c_str(), "RPC");
  std::string line_num = "";
  std::string line_den = "";
  std::string sample_num = "";
  std::string sample_den = "";
  for(int i = 0; i < 20; ++i) {
    line_num += to_s2(R.line_numerator[i]);
    line_den += to_s2(R.line_denominator[i]);
    sample_num += to_s2(R.sample_numerator[i]);
    sample_den += to_s2(R.sample_denominator[i]);
    if(i != 19) {
      line_num += " ";
      line_den += " ";
      sample_num += " ";
      sample_den += " ";
    }
  }
  D.SetMetadataItem("LINE_NUM_COEFF", line_num.c_str(), "RPC");
  D.SetMetadataItem("LINE_DEN_COEFF", line_den.c_str(), "RPC");
  D.SetMetadataItem("SAMP_NUM_COEFF", sample_num.c_str(), "RPC");
  D.SetMetadataItem("SAMP_DEN_COEFF", sample_den.c_str(), "RPC");

  // VICAR expects NITF corners when it finds an RPC. We estimiate this
  // by finding the corners at the height offset of the RPC.
  SimpleDem d(R.height_offset);
  Geodetic g1 = R.ground_coordinate(ImageCoordinate(-0.5, -0.5), d);
  Geodetic g2 = R.ground_coordinate(ImageCoordinate(-0.5, 
					    D.GetRasterXSize() - 0.5), d);
  Geodetic g3 = R.ground_coordinate(ImageCoordinate(D.GetRasterYSize() - 0.5, 
					    D.GetRasterXSize() - 0.5), d);
  Geodetic g4 = R.ground_coordinate(ImageCoordinate(D.GetRasterYSize() - 0.5, 
					    -0.5), d);
  D.SetMetadataItem("NITF_CORNERLAT1", to_s2(g1.latitude()).c_str());
  D.SetMetadataItem("NITF_CORNERLON1", to_s2(g1.longitude()).c_str());
  D.SetMetadataItem("NITF_CORNERLAT2", to_s2(g2.latitude()).c_str());
  D.SetMetadataItem("NITF_CORNERLON2", to_s2(g2.longitude()).c_str());
  D.SetMetadataItem("NITF_CORNERLAT3", to_s2(g3.latitude()).c_str());
  D.SetMetadataItem("NITF_CORNERLON3", to_s2(g3.longitude()).c_str());
  D.SetMetadataItem("NITF_CORNERLAT4", to_s2(g4.latitude()).c_str());
  D.SetMetadataItem("NITF_CORNERLON4", to_s2(g4.longitude()).c_str());
}
