#include "argus_orbit.h"
#include "gdal_raster_image.h"
#include "gdal_raster_image.h"
#include "vicar_raster_image.h"
#include "memory_raster_image.h"
#include "sub_raster_image.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

using namespace GeoCal;

/// Tolerance in seconds for two orbit data records to be in the same "row"
double ArgusOrbit::row_time_tolerance = 0.2;

struct null_deleter
{
  void operator()(void const *) const
  {
  }
};

//-----------------------------------------------------------------------
/// Add three color orthorectified image to an existing set of three
/// images. We currently don't try to do any feathering, instead we
/// simply add imagery if it is currently fill (value of 0), or crop
/// the data if it already has data.
//-----------------------------------------------------------------------

void ArgusOrbitData::add_ortho_to_image(
const boost::shared_ptr<RasterImage>& M1,
const boost::shared_ptr<RasterImage>& M2,
const boost::shared_ptr<RasterImage>& M3,
const Camera& Cam,
const Dem& D,
int Border) const
{
  // The OrbitMapProjected here has been replace with
  // IgcMapProject. We could change this function to use this, but we
  // don't have any test data for this and don't use this function
  // anyways. For now, just throw an exception if this somehow gets
  // called.
  throw Exception("Not implemented");
 
  std::vector<boost::shared_ptr<GroundCoordinate> > fp =
    footprint(Cam, D);
  SubRasterImage m1out(M1,fp, Border);
  SubRasterImage m2out(M2,fp, Border);
  SubRasterImage m3out(M3,fp, Border);
  std::vector<boost::shared_ptr<RasterImage> > img =
    GdalRasterImage::read_all(file_name());
  boost::shared_ptr<OrbitData> odp(const_cast<ArgusOrbitData*>(this),
				   null_deleter());
  boost::shared_ptr<Camera> camp(const_cast<Camera*>(&Cam),null_deleter());
  boost::shared_ptr<Dem> demp(const_cast<Dem*>(&D),null_deleter());
  // OrbitMapProjected m1(m1out.map_info(), odp, img[0], camp, demp);
  // OrbitMapProjected m2(m2out.map_info(), odp, img[1], camp, demp);
  // OrbitMapProjected m3(m3out.map_info(), odp, img[2], camp, demp);
  // copy_only_to_fill(m1, m1out);
  // copy_only_to_fill(m2, m2out);
  // copy_only_to_fill(m3, m3out);
}

//-----------------------------------------------------------------------
/// Save an three color orthorectified image to the given file. The
/// type should be "img", "tif", "tifjpeg" or "vicar" ("tif" uses LZW
/// lossless compression, "tifjpeg" uses jpeg lossy compression).
///
/// The MapInfo governs the map projection and pixel size used. We
/// however only write out the subset of MapInfo that is needed to
/// cover the footprint of this orthorectified image.
///
/// To speed up the processing, you can give a Grid_spacing > 1. We
/// calculate the position in the input image exactly at the
/// Grid_spacing locations, and do a bilinear interpolation in
/// between. If the Grid_spacing is 1, then no interpolation is done.
///
/// This is a somewhat specific function, but I had similar
/// functionality in Ruby code working with ARGUS data. Ruby is a nice
/// language, but just sucks at garbage collection. So we move this
/// functionality into this routine where we can control the lifetimes
/// of all of the data involved.
//-----------------------------------------------------------------------

void ArgusOrbitData::save_ortho(const MapInfo& Mi, const Camera& Cam, 
				const Dem& D, const std::string& Fname,
				const std::string& Type, int Border,
				int Grid_spacing) const
{
  // The OrbitMapProjected here has been replace with
  // IgcMapProject. We could change this function to use this, but we
  // don't have any test data for this and don't use this function
  // anyways. For now, just throw an exception if this somehow gets
  // called.
  throw Exception("Not implemented");

  MapInfo misub = Mi.cover(footprint(Cam, D), Border);
  std::vector<boost::shared_ptr<RasterImage> > img =
    GdalRasterImage::read_all(file_name());
  boost::shared_ptr<OrbitData> odp(const_cast<ArgusOrbitData*>(this),
				   null_deleter());
  boost::shared_ptr<Camera> camp(const_cast<Camera*>(&Cam),null_deleter());
  boost::shared_ptr<Dem> demp(const_cast<Dem*>(&D),null_deleter());
  // OrbitMapProjected m(misub, odp, img, camp, demp);
  // std::vector<boost::shared_ptr<RasterImage> > mres;
  // mres.push_back(boost::shared_ptr<RasterImage>(new 
  //        MemoryRasterImage(misub)));
  // mres.push_back(boost::shared_ptr<RasterImage>(new 
  //        MemoryRasterImage(misub)));
  // mres.push_back(boost::shared_ptr<RasterImage>(new 
  //        MemoryRasterImage(misub)));
  // if(Grid_spacing == 1)
  //   m.write_multiple(mres);
  // else
  //   m.write_multiple(mres, Grid_spacing);
  // save_to_file(Fname, *mres[0], *mres[1], *mres[2], Type);
}

//-----------------------------------------------------------------------
/// Save file to given type.
//-----------------------------------------------------------------------

void ArgusOrbitData::save_to_file(const std::string& Fname, 
			      const RasterImage& m1,
			      const RasterImage& m2, 
			      const RasterImage& m3, 
			      const std::string& Type)
{
  if(Type == "img") {
    GdalRasterImage::save_to_erdas(Fname, m1, m2, m3);
  } else if(Type == "tif") {
    // Compressed output seems to require that we generate a in memory
    // copy first. This lets GDAL write this out in the order it wants.
    GdalRasterImage::save(Fname, "GTiff", m1, m2, m3, 
				GdalRasterImage::Byte, 
				"PHOTOMETRIC=RGB COMPRESS=LZW", true);
  } else if(Type == "tifjpeg") {
    // Compressed output seems to require that we generate a in memory
    // copy first. This lets GDAL write this out in the order it wants.
    GdalRasterImage::save(Fname, "GTiff", m1, m2, m3, 
				GdalRasterImage::Byte, 
				"PHOTOMETRIC=RGB TILED=YES COMPRESS=JPEG", true);
  } else if(Type == "vicar") {
    VicarRasterImage v1(Fname + "_r.img", m1.map_info());
    copy(m1, v1);
    VicarRasterImage v2(Fname + "_g.img", m1.map_info());
    copy(m2, v2);
    VicarRasterImage v3(Fname + "_b.img", m1.map_info());
    copy(m3, v3);
  } else {
    throw Exception("Unrecognized type. Should be img, tif or vicar");
  }
}

//-----------------------------------------------------------------------
/// Access image for the given band. The band is 1 based, because that
/// is what Gdal uses.
//-----------------------------------------------------------------------

boost::shared_ptr<GdalRasterImage> ArgusOrbitData::image(int band) const
{
  if(gdal_data_.get())
    return boost::shared_ptr<GdalRasterImage>(new GdalRasterImage(gdal_data_, band));
  boost::shared_ptr<GdalRasterImage> g(new GdalRasterImage(file_name(), band));
  gdal_data_ = g->gdal_data_base().data_set();
  return g;
}

//-----------------------------------------------------------------------
/// Print to stream. 
//-----------------------------------------------------------------------

void ArgusOrbitData::print(std::ostream& Os) const
{
  Os << "ArgusOrbitData:\n"
     << "  Time:     " << time() << "\n"
     << "  File:     " << file_name() << "\n"
     << "  Camera:   " << camera_number() << "\n"
     << "  Position: " << position_geodetic() << "\n"
     << "  Roll:     " << roll() << " deg\n"
     << "  Pitch:    " << pitch() << " deg\n"
     << "  Heading:  " << heading() << " deg\n";
}


//-----------------------------------------------------------------------
/// Open the given file and use it to provide Orbit information. This
/// file should be a CSV file containing one record per line, with the
/// fields "File,Camera,Time,Lat,Lon,Alt,Roll,Pitch,Heading". The
/// first line is assumed to be a header, and is discarded. The second
/// line is the epoch that the times are measured relative to.
//-----------------------------------------------------------------------

ArgusOrbit::ArgusOrbit(const std::string& Fname)
  : fname(Fname)
{
  std::ifstream is(Fname.c_str());
  is.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  std::string ln;
  std::string ln2;
  is >> ln;			// Skip first line, which is a header
  int ln_n = 1;
  is >> ln >> ln2 >> std::ws;	// Epoch
  Time epoch = Time::parse_time(ln + " " + ln2);
  ln_n += 1;
  boost::array<double, 3> vel = {{0.0, 0.0, 0.0}};
  typedef std::multimap<Time, boost::shared_ptr<ArgusOrbitData> > 
  time_data_map;

  /// Orbit data, indexed by the Time. This is a multimap because it
  /// is entirely possible to have multiple entries in the navigation
  /// file with the same time, e.g., two cameras collect data at the
  /// same time.
  time_data_map time_data;

  while(!is.eof()) {
    is >> ln >> std::ws;
    ln_n += 1;
    std::vector<std::string> ln_s;
    boost::algorithm::split(ln_s, ln, boost::algorithm::is_any_of(","));
    if(ln_s.size() != 9) {
      Exception e;
      e << "Trouble reading the ArgusOrbit file " << Fname 
	<< " at line number " << ln_n;
      throw e;
    }	
    std::string fname = ln_s[0];
    // cam_index is a name line "CAM01"
    boost::smatch m;
    if(!boost::regex_match(ln_s[1], m, boost::regex("\\D*(\\d+).*"))) {
      Exception e;
      e << "Trouble reading the ArgusOrbit file " << Fname 
	<< " at line number " << ln_n << ", trouble parsing camera name";
      throw e;
    }
    int cam_index = boost::lexical_cast<int>(m[1]);
    double toffset = boost::lexical_cast<double>(ln_s[2]);
    Geodetic pos(boost::lexical_cast<double>(ln_s[3]),
	       boost::lexical_cast<double>(ln_s[4]),
	       boost::lexical_cast<double>(ln_s[5]));
    double roll = boost::lexical_cast<double>(ln_s[6]);
    double pitch = boost::lexical_cast<double>(ln_s[7]);
    double heading = boost::lexical_cast<double>(ln_s[8]);
    boost::shared_ptr<ArgusOrbitData> 
      od(new ArgusOrbitData(epoch + toffset, fname, cam_index, pos, vel,
			    roll, pitch, heading));
    time_data.insert(std::make_pair(od->time(), od));
    orbit_data_map[od->time()] = od;
  }

  // Times that occur with row_time_tolerance seconds of each other
  // are considered to be in the same "row".
  int rown = -1;
  bool first = true;
  Time t;
  BOOST_FOREACH(const time_data_map::value_type& vt, time_data) {
    if(first || (vt.first - t > row_time_tolerance)) {
      ++rown;
      t = vt.first;
      first = false;
    }
    // This expression is a bit obscure looking. What this does is
    // insert the ArgusOrbitData at the entry rown, camera number. We
    // check to see if there was already an entry for this row and
    // camera, if so we print a warning message.
    if(!row_data.insert(std::make_pair(std::make_pair(rown, 
       vt.second->camera_number()), vt.second)).second) {
      std::cerr <<"Warning: While reading the ArgusOrbit file " << Fname 
		<< "\n "
		<< "We found two records for camera number " 
		<< vt.second->camera_number() 
		<< " that are near the time offset " 
		<< vt.second->time() - epoch << "\n"
		<< "Occurred at the image file " << vt.second->file_name()
		<< ". Using the second file, and throwing away the first\n";
    }
  }
  number_row_ = rown + 1;
  initialize();
}

//-----------------------------------------------------------------------
/// Print to stream. 
//-----------------------------------------------------------------------

void ArgusOrbit::print(std::ostream& Os) const
{
  Os << "ArgusOrbit:\n"
     << "  File:       " << fname << "\n"
     << "  Number row: " << number_row() << "\n";
}

//-----------------------------------------------------------------------
/// This finds the first row with orbit and image data for the given
/// camera. We then read the metadata from the image do determine the
/// reported focal length. If we don't have any orbit or image data
/// for the camera, we return -1.
//-----------------------------------------------------------------------

double ArgusOrbit::focal_length(int camera_num) const
{
  for(int i = 0; i < number_row(); ++i) 
    if(nav(i, camera_num).get() &&
       nav(i, camera_num)->file_name() != "")
      return nav(i, camera_num)->image()->metadata<double>("EXIF_FocalLength");
  return -1;
}

//-----------------------------------------------------------------------
/// Return the ArgusOrbitData for the given row and camera, or if we
/// don't have any data there return a null pointer.
//-----------------------------------------------------------------------

boost::shared_ptr<ArgusOrbitData> 
ArgusOrbit::nav(int row, int camera_num) const
{
  row_map::const_iterator i = row_data.find(std::make_pair(row, camera_num));
  if(i == row_data.end())
    return boost::shared_ptr<ArgusOrbitData>();
  return i->second;
}

//-----------------------------------------------------------------------
/// Generate a mosaic that fits the given MapInfo, sampling each of
/// the orbit data.
/// We save this to the given file, with the given Type.
//-----------------------------------------------------------------------

void ArgusOrbitData::mosaic(const 
			    std::vector<boost::shared_ptr<ArgusOrbitData> > &Od,
			    const std::vector<boost::shared_ptr<Camera> >& Cam,
			    const Dem& D,
			    const MapInfo& Mi, const std::string& Fname,
			    const std::string& Type,
			    int Border)
{
  boost::shared_ptr<MemoryRasterImage> m1(new MemoryRasterImage(Mi));
  boost::shared_ptr<MemoryRasterImage> m2(new MemoryRasterImage(Mi));
  boost::shared_ptr<MemoryRasterImage> m3(new MemoryRasterImage(Mi));
  std::fill(m1->data().data(), m1->data().data() + m1->data().num_elements(), 
	    0);
  std::fill(m2->data().data(), m2->data().data() + m2->data().num_elements(), 
	    0);
  std::fill(m3->data().data(), m3->data().data() + m3->data().num_elements(), 
	    0);
  BOOST_FOREACH(const boost::shared_ptr<ArgusOrbitData>& odv, Od) {
    std::cerr << "  Processing " << odv->file_name() << "\n";
    try {
      odv->add_ortho_to_image(m1, m2, m3, *Cam[odv->camera_number() - 1],
			      D, Border);
    } catch(const NoCoverage& E) {
      std::cerr << "  Skipping file, it has no coverage with MapInfo\n";
    }
  }
  save_to_file(Fname, *m1, *m2, *m3, Type);
}
