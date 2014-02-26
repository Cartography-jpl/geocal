#include "gdal_multi_band.h"
#include "gdal_raster_image.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

GdalMultiBand::GdalMultiBand(const std::string& Fname, int Number_tile,
		int Tile_number_line, int Tile_number_sample)
{
  boost::shared_ptr<GdalRasterImage> g(new GdalRasterImage(Fname));
  raster_image_.push_back(g);
  gd = g->data_set();
  // b is 1 based.
  for(int b = 2; b <= gd->GetRasterCount(); ++b)
    raster_image_.push_back(boost::shared_ptr<GdalRasterImage>
			    (new GdalRasterImage(gd, b, Number_tile, Tile_number_line,
						 Tile_number_sample)));
}

//-----------------------------------------------------------------------
/// Create a new file. All the arguments here are the same as for
/// GdalRasterImage. 
//-----------------------------------------------------------------------

GdalMultiBand::GdalMultiBand(const std::string& Fname, const std::string& 
			     Driver_name, int Number_line, int Number_sample,
			     int Number_band,
			     int Dt, const std::string& Options, 
			     int Number_tile,
			     int Tile_number_line, 
			     int Tile_number_sample)
{
  boost::shared_ptr<GdalRasterImage> g
    (new GdalRasterImage(Fname, Driver_name, Number_line, Number_sample, 
			 Number_band, Dt, Options, Number_tile, 
			 Tile_number_line, Tile_number_sample)); 
  raster_image_.push_back(g);
  gd = g->data_set();
  // b is 1 based.
  for(int b = 2; b <= gd->GetRasterCount(); ++b)
    raster_image_.push_back(boost::shared_ptr<GdalRasterImage>
			    (new GdalRasterImage(gd, b)));
}

//-----------------------------------------------------------------------
/// Create a new GdalMultiBand from and an existing GDAL dataset using
/// the CreateCopy command.
//-----------------------------------------------------------------------

GdalMultiBand::GdalMultiBand(const std::string& Fname,
			     const std::string& Driver_name,
			     const GdalRasterImage& Source,
			     const std::string& Options, 
			     bool Log_progress,
			     int Number_tile,
			     int Tile_number_line, int Tile_number_sample)
{
  boost::shared_ptr<GdalRasterImage> g
    (new GdalRasterImage(Fname, Driver_name, Source, Options, Log_progress,
			 1, Number_tile, 
			 Tile_number_line, Tile_number_sample)); 
  raster_image_.push_back(g);
  gd = g->data_set();
  // b is 1 based.
  for(int b = 2; b <= gd->GetRasterCount(); ++b)
    raster_image_.push_back(boost::shared_ptr<GdalRasterImage>
			    (new GdalRasterImage(gd, b,
		 Number_tile, Tile_number_line, Tile_number_sample)));
}

//-----------------------------------------------------------------------
/// Create a new GdalMultiBand from and an existing GDAL dataset using
/// the CreateCopy command.
//-----------------------------------------------------------------------


GdalMultiBand::GdalMultiBand(const std::string& Fname,
			     const std::string& Driver_name,
			     const GdalMultiBand& Source,
			     const std::string& Options, 
			     bool Log_progress,
			     int Number_tile,
			     int Tile_number_line, 
			     int Tile_number_sample)
{
  gd = gdal_create_copy(Fname, Driver_name, *Source.data_set(),
			Options, Log_progress);
  // b is 1 based.
  for(int b = 1; b <= gd->GetRasterCount(); ++b)
    raster_image_.push_back(boost::shared_ptr<GdalRasterImage>
			    (new GdalRasterImage(gd, b, 
		 Number_tile, Tile_number_line, Tile_number_sample)));
}

//-----------------------------------------------------------------------
/// Copy the RasterImageMultiBand into a new file. We copy map and RPC
/// information (if found) from the first band in RasterImageMultiBand.
//-----------------------------------------------------------------------

void GdalMultiBand::save(const std::string& Oname, 
			 const std::string& Driver,
			 const RasterImageMultiBand& d,
			 bool Log_progress,
			 int Dt,
			 const std::string& Options)
{
  GdalMultiBand m(Oname, Driver, d.raster_image(0).number_line(),
		  d.raster_image(0).number_sample(),
		  d.number_band(), Dt, Options);
  if(d.raster_image(0).has_map_info())
    m.set_map_info(d.raster_image(0).map_info());
  if(d.raster_image(0).has_rpc())
    m.set_rpc(d.raster_image(0).rpc());
  copy(d, m, Log_progress);
}

//-----------------------------------------------------------------------
/// Flush data to disk for all bands.
//-----------------------------------------------------------------------

void GdalMultiBand::flush() const
{
  BOOST_FOREACH(const boost::shared_ptr<RasterImage>& braw, 
		raster_image_) {
    boost::shared_ptr<GdalRasterImage> b = 
      boost::dynamic_pointer_cast<GdalRasterImage>(braw);
    if(b)
      b->flush();
  }
}

//-----------------------------------------------------------------------
/// Flush data to disk for all bands and close file. You don't
/// normally need to call this directly, since the destructor of the
/// files automatically does this. But this can be useful in some cases.
//-----------------------------------------------------------------------

void GdalMultiBand::close() const
{
  BOOST_FOREACH(const boost::shared_ptr<RasterImage>& braw, 
		raster_image_) {
    boost::shared_ptr<GdalRasterImage> b = 
      boost::dynamic_pointer_cast<GdalRasterImage>(braw);
    if(b)
      b->close();
  }
}

void GdalMultiBand::print(std::ostream& Os) const 
{ 
  Os << "GdalMultiBand\n"
     << "  Filename: " << gdal_raster_image(0)->file_names()[0] << "\n";
}
