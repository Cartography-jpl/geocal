#include "gdal_raster_image.h"
#include "memory_raster_image.h"

using namespace GeoCal;

/****************************************************************//**
  This is a simple class that has a number of bands all in memory. 
  When close() or the destructor is called, this is then written
  out to an Erdas file. The includes calculating statistics and image
  pyramid. 
*******************************************************************/

class ErdasBands {
public:
//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  ErdasBands(const std::string& Fname, int Number_line, int Number_sample,
	     int Number_band, int Dt, int Fill_value = 0)
  : is_closed(false), fname(Fname)
  {
    range_min_check(Number_band, 1);
    image_.push_back(boost::shared_ptr<GdalRasterImage>(new
      GdalRasterImage("", "MEM", Number_line, Number_sample, Number_band, Dt)));
    for(int i = 1; i < Number_band; ++i)
      image_.push_back(boost::shared_ptr<GdalRasterImage>(new
     GdalRasterImage(image(0).data_set(), i + 1)));
    for(int i = 0; i < Number_band; ++i)
      image_[i]->raster_band().SetNoDataValue(Fill_value);
  }
  virtual ~ErdasBands() {close();}

//-----------------------------------------------------------------------
/// Close the file. This copies the data from memory and writes it out
/// to the Erdas file. At the same time, we generate statistics and 
/// the image pyramids.
//-----------------------------------------------------------------------

  void close()
  {
    if(!is_closed)
      gdal_create_erdas(fname, *image(0).data_set());
    is_closed = true;
  }

//-----------------------------------------------------------------------
/// Set the mapinfo
//-----------------------------------------------------------------------
  void set_map_info(const MapInfo& Mi)
  {
    BOOST_FOREACH(boost::shared_ptr<GdalRasterImage>& i, image_)
      i->set_map_info(Mi);
  }

//-----------------------------------------------------------------------
/// Return the GdalRasterImage for the given band.
//-----------------------------------------------------------------------

  const GdalRasterImage& image(int band) const 
  { range_check(band, 0, number_band()); return *image_[band];}

//-----------------------------------------------------------------------
/// Return the GdalRasterImage for the given band.
//-----------------------------------------------------------------------

  GdalRasterImage& image(int band)
  { range_check(band, 0, number_band()); return *image_[band];}

//-----------------------------------------------------------------------
/// Number of bands of data.
//-----------------------------------------------------------------------

  int number_band() const {return (int) image_.size(); }
private:
  std::vector<boost::shared_ptr<GdalRasterImage> > image_;
  bool is_closed;
  std::string fname;
};

//-----------------------------------------------------------------------
/// Constructor to create a new file. This is a thin wrapper around
/// the GDAL library that gets the requested driver and creates the
/// file with the given options. 
///
/// We create a file with the given number of bands, but this always
/// opens band 1. However you can use the GDALDataset created in other
/// GdalRasterImage to write the other bands.
///
/// Because the option interface of GDAL is a bit awkward to use, we
/// supply an interface that takes a single string, with the options
/// separated by " ", so for example for a TIFF file we might have
/// "TILED=YES BLOCKXSIZE=1024 BLOCKYSIZE=1024"
//-----------------------------------------------------------------------

GdalRasterImage::GdalRasterImage(const std::string& Fname, 
    const std::string& Driver_name, int Number_line, 
     int Number_sample, int Number_band,
    int Dt, const std::string& Options, int Number_tile, 
    int Tile_number_line, int Tile_number_sample)
{
  boost::shared_ptr<GDALDataset> data_set = 
    gdal_create_dataset(Fname, Driver_name, Number_line, 
			Number_sample, Number_band, 
			(GDALDataType) Dt, Options);
  switch(Dt) {
  case GDT_Byte:
    initialize<GByte>(data_set, 1, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_UInt16:
    initialize<GUInt16>(data_set, 1, Number_tile,
			Tile_number_line, Tile_number_sample);
    break;
  case GDT_Int16:
    initialize<GInt16>(data_set, 1, Number_tile,
		       Tile_number_line, Tile_number_sample);
    break;
  case GDT_UInt32:
    initialize<GUInt32>(data_set, 1, Number_tile,
			Tile_number_line, Tile_number_sample);
    break;
  case GDT_Int32:
    initialize<GInt32>(data_set, 1, Number_tile,
		       Tile_number_line, Tile_number_sample);
    break;
  case GDT_Float32:
    initialize<float>(data_set, 1, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Float64:
    initialize<double>(data_set, 1, Number_tile,
		       Tile_number_line, Tile_number_sample);
    break;
  default:
    throw Exception("Unrecognized data type");
  }
}

//-----------------------------------------------------------------------
/// Constructor to create a new file. This is a thin wrapper around
/// the GDAL library that gets the requested driver and creates the
/// file with the given options. 
///
/// We create a file with the given number of bands, but this always
/// opens band 1. However you can use the GDALDataset created in other
/// GdalRasterImage to write the other bands.
///
/// Because the option interface of GDAL is a bit awkward to use, we
/// supply an interface that takes a single string, with the options
/// separated by " ", so for example for a TIFF file we might have
/// "TILED=YES BLOCKXSIZE=1024 BLOCKYSIZE=1024"
//-----------------------------------------------------------------------

GdalRasterImage::GdalRasterImage(const std::string& Fname, 
    const std::string& Driver_name, const MapInfo& Mi, int Number_band,
    int Dt, const std::string& Options, int Number_tile, 
    int Tile_number_line, int Tile_number_sample)
{
  boost::shared_ptr<GDALDataset> data_set = 
    gdal_create_dataset(Fname, Driver_name, Mi.number_y_pixel(),
			Mi.number_x_pixel(), Number_band, 
			(GDALDataType) Dt, Options);
  switch(Dt) {
  case GDT_Byte:
    initialize<GByte>(data_set, 1, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_UInt16:
    initialize<GUInt16>(data_set, 1, Number_tile,
			Tile_number_line, Tile_number_sample);
    break;
  case GDT_Int16:
    initialize<GInt16>(data_set, 1, Number_tile,
		       Tile_number_line, Tile_number_sample);
    break;
  case GDT_UInt32:
    initialize<GUInt32>(data_set, 1, Number_tile,
			Tile_number_line, Tile_number_sample);
    break;
  case GDT_Int32:
    initialize<GInt32>(data_set, 1, Number_tile,
		       Tile_number_line, Tile_number_sample);
    break;
  case GDT_Float32:
    initialize<float>(data_set, 1, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Float64:
    initialize<double>(data_set, 1, Number_tile,
		       Tile_number_line, Tile_number_sample);
    break;
  default:
    throw Exception("Unrecognized data type");
  }
  gdal_data_base_->map_info(Mi);
  map_info_.reset(new MapInfo(Mi));
}

//-----------------------------------------------------------------------
/// Constructor to read and/or write a new file (or an existing file
/// where you want to share the GDALDataset with some other object).
/// This constructor doesn't try to actually create the file, that is
/// done outside of this class. Normally, you just call
/// GDALDriver::Create for whatever driver you are using (see the GDAL
/// documentation at the web site listed above for details).
///
/// You can optionally pass the desired tile size. If you don't pass
/// this in, we use the blocking size of the underlying dataset.
//-----------------------------------------------------------------------

GdalRasterImage::GdalRasterImage(const boost::shared_ptr<GDALDataset>&
				 Data_set, int Band_id, 
				 int Number_tile, int Tile_number_line,
				 int Tile_number_sample)
{
  switch(Data_set->GetRasterBand(Band_id)->GetRasterDataType()) {
  case GDT_Byte:
    initialize<GByte>(Data_set, Band_id, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_UInt16:
    initialize<GUInt16>(Data_set, Band_id, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Int16:
    initialize<GInt16>(Data_set, Band_id, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_UInt32:
    initialize<GUInt32>(Data_set, Band_id, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Int32:
    initialize<GInt32>(Data_set, Band_id, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Float32:
    initialize<float>(Data_set, Band_id, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Float64:
    initialize<double>(Data_set, Band_id, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  default:
    throw Exception("Unrecognized data type");
  }
}

//-----------------------------------------------------------------------
/// Copy an existing GdalRasterImage using CreateCopy.
//-----------------------------------------------------------------------

GdalRasterImage::GdalRasterImage(const std::string& Fname,
		  const std::string& Driver_name,
		  const GdalRasterImage& Source,
		  const std::string& Options, 
 		  bool Log_progress,
		  int  Band_id,
		  int Number_tile,
		  int Tile_number_line, int Tile_number_sample)
{
  boost::shared_ptr<GDALDataset> ds =
    gdal_create_copy(Fname, Driver_name, *Source.data_set(),
		     Options, Log_progress);
  switch(ds->GetRasterBand(Band_id)->GetRasterDataType()) {
  case GDT_Byte:
    initialize<GByte>(ds, Band_id, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_UInt16:
    initialize<GUInt16>(ds, Band_id, Number_tile,
			Tile_number_line, Tile_number_sample);
    break;
  case GDT_Int16:
    initialize<GInt16>(ds, Band_id, Number_tile,
		       Tile_number_line, Tile_number_sample);
    break;
  case GDT_UInt32:
    initialize<GUInt32>(ds, Band_id, Number_tile,
			Tile_number_line, Tile_number_sample);
    break;
  case GDT_Int32:
    initialize<GInt32>(ds, Band_id, Number_tile,
		       Tile_number_line, Tile_number_sample);
    break;
  case GDT_Float32:
    initialize<float>(ds, Band_id, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Float64:
    initialize<double>(ds, Band_id, Number_tile,
		       Tile_number_line, Tile_number_sample);
    break;
  default:
    throw Exception("Unrecognized data type");
  }
  
}

GdalRasterImage::GdalRasterImage(const boost::shared_ptr<GDALDataset>&
				 Data_set, GDALRasterBand* Rb,
				 int Number_tile, int Tile_number_line,
				 int Tile_number_sample)
{
  switch(Rb->GetRasterDataType()) {
  case GDT_Byte:
    initialize<GByte>(Data_set, Rb, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_UInt16:
    initialize<GUInt16>(Data_set, Rb, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Int16:
    initialize<GInt16>(Data_set, Rb, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_UInt32:
    initialize<GUInt32>(Data_set, Rb, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Int32:
    initialize<GInt32>(Data_set, Rb, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Float32:
    initialize<float>(Data_set, Rb, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Float64:
    initialize<double>(Data_set, Rb, Number_tile,
		      Tile_number_line, Tile_number_sample);
    break;
  default:
    throw Exception("Unrecognized data type");
  }
}

//-----------------------------------------------------------------------
/// Set the map info. This overwrites whatever was previously set.
//-----------------------------------------------------------------------

void GdalRasterImage::set_map_info(const MapInfo& Mi)
{
  gdal_data_base_->map_info(Mi);
  map_info_.reset(new MapInfo(Mi));
}

//-----------------------------------------------------------------------
/// In AFIDS, we calculate what is called the "nitf corners". This
/// gives approximate corner coordinates for an image. Depending on
/// the application, it can be useful to treat this as an approximate
/// MapInfo.
///
/// This return a null pointer if we don't have the NITF corner metadata.
//-----------------------------------------------------------------------

boost::shared_ptr<MapInfo> GdalRasterImage::map_info_from_nitf_corner(bool Approx_ok) const
{
  if(!has_metadata("NITF_CORNERLAT1") ||
     !has_metadata("NITF_CORNERLAT2") ||
     !has_metadata("NITF_CORNERLAT3") ||
     !has_metadata("NITF_CORNERLAT4") ||
     !has_metadata("NITF_CORNERLON1") ||
     !has_metadata("NITF_CORNERLON2") ||
     !has_metadata("NITF_CORNERLON3") ||
     !has_metadata("NITF_CORNERLON4"))
    return boost::shared_ptr<MapInfo>();
  int ngcp = 4;
  GDAL_GCP* gcps = (GDAL_GCP*) CPLMalloc(sizeof(GDAL_GCP) * ngcp);
  GDALInitGCPs(ngcp, gcps);
  gcps[0].dfGCPPixel	= 0.5;
  gcps[0].dfGCPLine = 0.5;
  gcps[1].dfGCPPixel = number_sample()-0.5;
  gcps[1].dfGCPLine = 0.5;
  gcps[2].dfGCPPixel = number_sample()-0.5;
  gcps[2].dfGCPLine = number_line()-0.5;
  gcps[3].dfGCPPixel = 0.5;
  gcps[3].dfGCPLine = number_line()-0.5;
  gcps[0].dfGCPX = atof(metadata<std::string>("NITF_CORNERLON1").c_str());
  gcps[0].dfGCPY = atof(metadata<std::string>("NITF_CORNERLAT1").c_str());
  gcps[1].dfGCPX = atof(metadata<std::string>("NITF_CORNERLON2").c_str());
  gcps[1].dfGCPY = atof(metadata<std::string>("NITF_CORNERLAT2").c_str());
  gcps[2].dfGCPX = atof(metadata<std::string>("NITF_CORNERLON3").c_str());
  gcps[2].dfGCPY = atof(metadata<std::string>("NITF_CORNERLAT3").c_str());
  gcps[3].dfGCPX = atof(metadata<std::string>("NITF_CORNERLON4").c_str());
  gcps[3].dfGCPY = atof(metadata<std::string>("NITF_CORNERLAT4").c_str());
  blitz::Array<double, 1> parm(6);
  int status = GDALGCPsToGeoTransform(ngcp, gcps, parm.data(), 
				      (Approx_ok ? TRUE : FALSE));
  if(!status)
    throw Exception("Call to GDALGCPsToGeoTransform failed");
  CPLFree(gcps);
  boost::shared_ptr<CoordinateConverter> conv(new GeodeticConverter());
  return boost::shared_ptr<MapInfo>(new MapInfo(conv, parm, number_sample(),
						number_line()));
}

//-----------------------------------------------------------------------
/// Set the RPC info. This overwrites whatever was previously set.
//-----------------------------------------------------------------------

void GdalRasterImage::set_rpc(const Rpc& R)
{
  gdal_data_base_->rpc(R);
  rpc_.reset(new Rpc(R));
}


//-----------------------------------------------------------------------
/// Constructor to read and/or update an existing file.
//-----------------------------------------------------------------------

GdalRasterImage::GdalRasterImage(const std::string& Fname, int Band_id, int
				 Number_tile, bool Update, 
				 int Tile_number_line, int Tile_number_sample)
{
  switch(gdal_band_type(Fname, Band_id)) {
  case GDT_Byte:
    initialize<GByte>(Fname, Band_id, Number_tile, Update, 
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_UInt16:
    initialize<GUInt16>(Fname, Band_id, Number_tile, Update, 
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Int16:
    initialize<GInt16>(Fname, Band_id, Number_tile, Update, 
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_UInt32:
    initialize<GUInt32>(Fname, Band_id, Number_tile, Update, 
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Int32:
    initialize<GInt32>(Fname, Band_id, Number_tile, Update, 
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Float32:
    initialize<float>(Fname, Band_id, Number_tile, Update, 
		      Tile_number_line, Tile_number_sample);
    break;
  case GDT_Float64:
    initialize<double>(Fname, Band_id, Number_tile, Update, 
		      Tile_number_line, Tile_number_sample);
    break;
  default:
    throw Exception("Unrecognized data type");
  }
}


// Actual work of reading, parametrized by type.

template<class T> inline std::vector<boost::shared_ptr<RasterImage> > 
do_read_all(const GdalRasterImage& d)
{
  int n = d.gdal_data_base().data_set()->GetRasterCount();
  boost::shared_ptr<std::vector<T> > 
    buf(new std::vector<T>(d.number_line() * d.number_sample() * n));
  d.gdal_data_base().data_set()->RasterIO(GF_Read, 0,0,d.number_sample(), 
				  d.number_line(),
				  (void*) &(*buf->begin()), 
				  d.number_sample(), d.number_line(),
				  (GDALDataType) GdalType::gdal_data_type<T>(), 
				  n, 0, 0, 0, 0);
  std::vector<boost::shared_ptr<RasterImage> > res;
  for(int i = 0; i < n; ++i)
    res.push_back(boost::shared_ptr<RasterImage>(
      new MemoryRasterImageRef<T>(buf, i * d.number_line() * 
          d.number_sample(), d.number_line(), d.number_sample())));
  return res;
}

//-----------------------------------------------------------------------
/// For some formats, reading all of the data for all the bands at one
/// time is much quicker than reading each band separately (assuming
/// you actually want all the data). For example, for jpeg reading
/// three separate bands requires decompressing the data three times
/// while reading all at once only decompresses once. Since almost the
/// entire time reading jpeg is used in decompressing the data this
/// means it takes about three times as long to read all the bands as
/// to read them all at once.
///
/// This function reads all of the data into memory and returns
/// RasterImages that can be used to read the data.
//-----------------------------------------------------------------------

std::vector<boost::shared_ptr<RasterImage> > 
GdalRasterImage::read_all(const std::string& Fname)
{
  GdalRasterImage img(Fname);
  
  switch(img.gdal_data_base().raster_band().GetRasterDataType()) {
  case GDT_Byte:
    return do_read_all<GByte>(img);
    break;
  case GDT_UInt16:
    return do_read_all<GUInt16>(img);
    break;
  case GDT_Int16:
    return do_read_all<GInt16>(img);
    break;
  case GDT_UInt32:
    return do_read_all<GUInt32>(img);
    break;
  case GDT_Int32:
    return do_read_all<GInt32>(img);
    break;
  case GDT_Float32:
    return do_read_all<float>(img);
    break;
  case GDT_Float64:
    return do_read_all<double>(img);
    break;
  default:
    throw Exception("Unrecognized data type");
  }
  // We never actually get here, but we put this in so the compiler
  // doesn't issue a warning. The warning doesn't hurt anything, but
  // we want to remove all warnings that aren't "real".
  std::vector<boost::shared_ptr<RasterImage> > trash;
  return trash;
}

//-----------------------------------------------------------------------
/// Copy the given RasterImage into a new Erdas file. This
/// calculates the statistics and image pyramid needed by Erdas also.
/// The given Fill_value is excluded from the statistics calculation.
//-----------------------------------------------------------------------

void GdalRasterImage::save_to_erdas(const std::string& Oname, 
				    const RasterImage& Mi, 
				    int Dt,
				    int Fill_value)
{
  ErdasBands b(Oname, Mi.number_line(), Mi.number_sample(), 1, 
	       Dt, Fill_value);
  if(Mi.has_map_info())
    b.set_map_info(Mi.map_info());
  copy(Mi, b.image(0));
}
  
//-----------------------------------------------------------------------
/// Copy the given RasterImages into a new Erdas file. This
/// calculates the statistics and image pyramid needed by Erdas also.
/// The given Fill_value is excluded from the statistics calculation.
//-----------------------------------------------------------------------

void GdalRasterImage::save_to_erdas(const std::string& Oname, 
					  const RasterImage& Mi_r, 
					  const RasterImage& Mi_g, 
					  const RasterImage& Mi_b, 
					  int Dt,
					  int Fill_value)
{
  ErdasBands b(Oname, Mi_r.number_line(), Mi_r.number_sample(), 3, Dt, 
	       Fill_value);
  if(Mi_r.has_map_info())
    b.set_map_info(Mi_r.map_info());
  copy(Mi_r, b.image(0));
  copy(Mi_g, b.image(1));
  copy(Mi_b, b.image(2));
}

//-----------------------------------------------------------------------
/// Copy the given RasterImages into a new Erdas file. This
/// calculates the statistics and image pyramid needed by Erdas also.
/// The given Fill_value is excluded from the statistics calculation.
//-----------------------------------------------------------------------

void GdalRasterImage::save_to_erdas(const std::string& Oname, 
					  const RasterImage& Mi_r, 
					  const RasterImage& Mi_g, 
					  const RasterImage& Mi_b, 
					  const RasterImage& Mi_ir, 
					  int Dt,
					  int Fill_value)
{
  ErdasBands b(Oname, Mi_r.number_line(), Mi_r.number_sample(), 4, Dt, 
	       Fill_value);
  if(Mi_r.has_map_info())
    b.set_map_info(Mi_r.map_info());
  copy(Mi_r, b.image(0));
  copy(Mi_g, b.image(1));
  copy(Mi_b, b.image(2));
  copy(Mi_ir, b.image(3));
}

//-----------------------------------------------------------------------
/// Copy the given RasterImages into a new Erdas file. This
/// calculates the statistics and image pyramid needed by Erdas also.
/// The given Fill_value is excluded from the statistics calculation.
//-----------------------------------------------------------------------

void GdalRasterImage::save_to_erdas(const std::string& Oname, 
	    const std::vector<boost::shared_ptr<RasterImage> >& d,
	    int Dt, int Fill_value)
{
  if(d.size() <= 0)
    return;
  ErdasBands b(Oname, d[0]->number_line(), d[0]->number_sample(), 
	       (int) d.size(), Dt, Fill_value);
  if(d[0]->has_map_info())
    b.set_map_info(d[0]->map_info());
  for(int i = 0; i < b.number_band(); ++i)
    copy(*d[i], b.image(i));
}

//-----------------------------------------------------------------------
/// Copy the given RasterImages into a new Erdas file. This
/// calculates the statistics and image pyramid needed by Erdas also.
/// The given Fill_value is excluded from the statistics calculation.
//-----------------------------------------------------------------------

void GdalRasterImage::save_to_erdas(const std::string& Oname, 
	    const RasterImageMultiBand& d,
	    int Dt, int Fill_value)
{
  if(d.number_band() <= 0)
    return;
  ErdasBands b(Oname, d.raster_image(0).number_line(), 
	       d.raster_image(0).number_sample(), 
	       d.number_band(), Dt, Fill_value);
  if(d.raster_image(0).has_map_info())
    b.set_map_info(d.raster_image(0).map_info());
  for(int i = 0; i < b.number_band(); ++i)
    copy(d.raster_image(i), b.image(i));
}

//-----------------------------------------------------------------------
/// Copy the given RasterImages into a new file, using the given
/// driver and options. Some drivers only allow "CreateCopy" rather
/// than "Create", requiring us to use an intermediate memory version
/// of Gdal. In that case, set "require_copy" to true.
//-----------------------------------------------------------------------

void GdalRasterImage::save(const std::string& Oname,
				 const std::string& Driver,
				 const std::vector<const RasterImage*>& d,
				 int Dt,
				 const std::string& Option,
				 bool Require_copy,
				 int Fill_value)
{
  if(d.size() < 1)
    return;
  if(Require_copy) {
    GdalRasterImage mb("", "MEM", d[0]->number_line(), 
		       d[0]->number_sample(), 
		       (int) d.size(), Dt);
    if(Fill_value != -1000)
      mb.raster_band().SetNoDataValue(Fill_value);
    for(int i = 0; i < (int) d.size(); ++i) {
      GdalRasterImage m(mb.data_set(), i + 1);
      if(Fill_value != -1000)
	m.raster_band().SetNoDataValue(Fill_value);
      if(d[0]->has_map_info())
	m.set_map_info(d[0]->map_info());
      if(d[0]->has_rpc())
	m.set_rpc(d[0]->rpc());
      copy(*d[i], m);
    }
    gdal_create_copy(Oname, Driver, *mb.data_set(),Option);
  } else {
    GdalRasterImage mb(Oname, Driver, d[0]->number_line(),
		       d[0]->number_sample(),
		       (int) d.size(), Dt, Option);
    for(int i = 0; i < (int) d.size(); ++i) {
      GdalRasterImage m(mb.data_set(), i + 1);
      if(Fill_value != -1000)
	m.raster_band().SetNoDataValue(Fill_value);
      if(d[0]->has_map_info())
	m.set_map_info(d[0]->map_info());
      if(d[0]->has_rpc())
	m.set_rpc(d[0]->rpc());
      copy(*d[i], m);
    }
  }
}

// See base class for description.
boost::shared_ptr<RasterImage>
GdalRasterImage::overview(int Min_number_sample) const
{
// This function doesn't actually work with GDAL 1.8.0, so instead
// just go through all the overviews and we'll select the best one.
//   GDALRasterBand* rb = 
//     gdal_data_base_->raster_band().GetRasterSampleOverview(Min_number_sample);

  int index = -1;
  int nsamp = -1;
  for(int i = 0; i < gdal_data_base_->raster_band().GetOverviewCount(); ++i)
    if(gdal_data_base_->raster_band().GetOverview(i)->GetXSize() 
       > Min_number_sample) {
      if(nsamp ==-1 || 
	 nsamp > gdal_data_base_->raster_band().GetOverview(i)->GetXSize()) {
	index = i;
	nsamp = gdal_data_base_->raster_band().GetOverview(i)->GetXSize();
      }
    }

// Don't bother returning a copy to this band, we just return a null
// and let the calling routine handle this.

  if(index ==-1)
    return boost::shared_ptr<RasterImage>();
  return boost::shared_ptr<RasterImage>
    (new GdalRasterImage(gdal_data_base_->data_set(), 
			 gdal_data_base_->raster_band().GetOverview(index)));
}
