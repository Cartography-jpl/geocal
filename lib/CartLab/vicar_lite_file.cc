#include "geocal_internal_config.h"
#include "vicar_lite_file.h"
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include "geocal_serialize_support.h"
#define BOOST_LEXICAL_CAST_ASSUME_C_LOCALE
#include <boost/lexical_cast.hpp>
#ifdef HAVE_VICAR_RTL
#ifdef HAVE_GDAL
#define USE_VICAR_OGR /**/
#endif
#include "vicar_ogr.h"
#endif
using namespace GeoCal;

#ifdef USE_VICAR_OGR
static VicarOgr vlogr;
#endif

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void VicarLiteFile::save(Archive & ar, const unsigned int version) const
{
  GEOCAL_GENERIC_BASE(VicarLiteFile);
  ar & GEOCAL_NVP_(fname) & GEOCAL_NVP_(access)
    & GEOCAL_NVP_(force_area_pixel);
}

template<class Archive>
void VicarLiteFile::load(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(VicarLiteFile);
  ar & GEOCAL_NVP_(fname) & GEOCAL_NVP_(access)
    & GEOCAL_NVP_(force_area_pixel);
  initialize(fname_, access_, force_area_pixel_);
}

template<class Archive>
void VicarLiteRasterImage::save(Archive & ar, const unsigned int version) const
{
  GEOCAL_BASE(VicarLiteRasterImage, RasterImage);
  GEOCAL_GENERIC_BASE(RasterImage);
  ar & GEOCAL_NVP_(band) & GEOCAL_NVP_(f)
    & GEOCAL_NVP_(force_map_info)
    & GEOCAL_NVP_(number_tile_line)
    & GEOCAL_NVP_(number_tile_sample);
}

template<class Archive>
void VicarLiteRasterImage::load(Archive & ar, const unsigned int version)
{
  GEOCAL_BASE(VicarLiteRasterImage, RasterImage);
  GEOCAL_GENERIC_BASE(RasterImage);
  ar & GEOCAL_NVP_(band) & GEOCAL_NVP_(f)
    & GEOCAL_NVP_(force_map_info)
    & GEOCAL_NVP_(number_tile_line)
    & GEOCAL_NVP_(number_tile_sample);
  initialize();
}

template<class Archive>
void VicarLiteDem::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(DemMapInfo)
    & GEOCAL_NVP_(band)
    & GEOCAL_NVP_(f);
}

GEOCAL_SPLIT_IMPLEMENT(VicarLiteFile);
GEOCAL_SPLIT_IMPLEMENT(VicarLiteRasterImage);
GEOCAL_IMPLEMENT(VicarLiteDem);
#endif

//-----------------------------------------------------------------------
/// Determine if a given file is a VICAR file or not.
///
/// This looks for the first few characters of the file being "LBLSIZE="
//-----------------------------------------------------------------------

bool VicarLiteFile::is_vicar_file(const std::string& Fname)
{
  std::ifstream in(Fname.c_str());
  char buf[8  + 1];		// 8 is length of "LBLSIZE="
  buf[8] = '\0';
  in.read(buf, 8);
  return (std::string(buf) == "LBLSIZE=");
}

//-----------------------------------------------------------------------
/// Open an existing VICAR file for reading or update.
///
/// The Force_area_pixel forces the file to be treated as
/// "pixel as area" rather than "pixel as point". This is really just
/// meant as a work around for the SRTM data, which incorrectly labels
/// the data as "point" rather than "area". Since this is a 15 meter
/// difference, it matters for many applications. Most users should
/// just ignore this value.
//-----------------------------------------------------------------------

void VicarLiteFile::initialize(const std::string& Fname, access_type Access,
			       bool Force_area_pixel)
{
  access_ = Access;
  fname_ = Fname;
  force_area_pixel_ = Force_area_pixel;
  f_.reset(new std::fstream(fname_.c_str(), (Access ==READ ? std::ios_base::in :
           std::ios_base::in | std::ios_base::out)));

  f_->exceptions(std::ios_base::badbit|std::ios_base::failbit);

//-----------------------------------------------------------------------
// Read in labels and process them.
//-----------------------------------------------------------------------

  std::string label_data = read_label(data_offset_);
  process_label(label_data);
  int byte_per_pixel;
  if(label<std::string>("FORMAT") =="BYTE") {
    byte_per_pixel = 1;
    type_ = VICAR_BYTE;
  } else if(label<std::string>("FORMAT") == "HALF") {
    byte_per_pixel = 2;
    type_ = VICAR_HALF;
  } else if(label<std::string>("FORMAT") == "FULL") {
    byte_per_pixel = 4;
    type_ = VICAR_FULL;
  } else if(label<std::string>("FORMAT") == "REAL") {
    byte_per_pixel = 4;
    type_ = VICAR_FLOAT;
  } else if(label<std::string>("FORMAT") == "DOUB") {
    byte_per_pixel = 8;
    type_ = VICAR_DOUBLE;
  } else {
    throw Exception("Unrecognized type");
  }
  number_line_ = label<int>("NL");
  number_sample_ = label<int>("NS");
  number_band_ = label<int>("NB");
  number_line_binary_ = label<int>("NLB");
  data_offset_ += number_line_binary_ * label<int>("RECSIZE");
  number_byte_binary_ = label<int>("NBB");

//-----------------------------------------------------------------------
/// Don't currently support binary labels that aren't even multiple of
/// pixel size,
//-----------------------------------------------------------------------

  if(number_byte_binary_ % byte_per_pixel != 0) {
    Exception e;
    e << "VicarLiteFile doesn't support binary labels with the number of "
      << "bytes not an even multiple of the byte_per_pixel."
      << "The file " << file_name() << " has the label NBB="
      << number_byte_binary_ << " and bytes per pixel="
      << byte_per_pixel << ". Note the full VicarFile (which "
      << "depends on the VICAR RTL) does support this.";
    throw e;
  }
  number_sample_binary_ = number_byte_binary_ / byte_per_pixel;
  
//-----------------------------------------------------------------------
// Check if there are any more labels at the end of the file. If there
// are, add this to label_data and reprocess the full list.
//-----------------------------------------------------------------------

  if(label_["EOL"] == "1") {
    int data_size = number_line_ *
      (number_sample_ * byte_per_pixel + number_byte_binary_) * number_band_ ;
    f_->seekg(data_offset_ + data_size, std::ios_base::beg);
    int trash;
    label_data += read_label(trash);
    label_.clear();
    prop_set_.clear();
    process_label(label_data);
  }

//-----------------------------------------------------------------------
// Close file so we don't need to worry about interference with the
// memory map
//-----------------------------------------------------------------------

  f_.reset();

//-----------------------------------------------------------------------
// Determine endianess of data, and if it doesn't match that of this
// system then set swap_needed_ to true.
//-----------------------------------------------------------------------

  std::string intfmt = "LOW";
  if(has_label("INTFMT"))
    intfmt = (*label_.find("INTFMT")).second;
  if(intfmt == "LOW") {
#ifdef WORDS_BIGENDIAN
    swap_needed_ = true;
#else
    swap_needed_ = false;
#endif
  } else if(intfmt =="HIGH") {
#ifdef WORDS_BIGENDIAN
    swap_needed_ = false;
#else
    swap_needed_ = true;
#endif
  } else {
    throw Exception("Unrecognized intfmt " + intfmt + " in file " + 
		    file_name());
  }

//-----------------------------------------------------------------------
// Memory map data.
//-----------------------------------------------------------------------

  boost::multi_array<char, 4>::size_type ordering[4];
  bool ascending[] = {true, true, true, true};
  std::string ord = "BSQ";
  if(has_label("ORG"))
    ord = (*label_.find("ORG")).second;
  if(ord == "BSQ") {
    ordering[0] = 3; ordering[1] = 2; ordering[2] = 1; ordering[3] = 0;
  } else if(ord =="BIL") {
    ordering[0] = 3; ordering[1] = 2; ordering[2] = 0; ordering[3] = 1;
  } else if(ord =="BIP") {
    ordering[0] = 3; ordering[1] = 0; ordering[2] = 2; ordering[3] = 1;
  } else {
    throw Exception("Unrecognized order " + ord + " in file " + file_name());
  }
  boost::general_storage_order<4> storage(ordering, ascending);
  // We can't actually read compressed data, although it can be useful
  // to look at the labels. If the data is compressed, punt for now.
  // We might do something more sophisticated in the future.
  if(!has_label("COMPRESS") ||
     label_["COMPRESS"] == "NONE") {
    is_compressed_ = false;
    data_raw.reset(new MemoryMapArray<char, 4>
		   (file_name(), 
		    boost::extents[number_band()][number_line()]
		    [number_sample() + number_sample_binary_][byte_per_pixel],
		    (Access == READ ? MemoryMapArray<char, 4>::READ :
		     MemoryMapArray<char, 4>::UPDATE), data_offset(),
		    storage));
  } else {
    is_compressed_ = true;
  }
}

//-----------------------------------------------------------------------
/// Create a new VICAR file with the given size.
//-----------------------------------------------------------------------

VicarLiteFile::VicarLiteFile(const std::string& Fname, int Number_line, 
		     int Number_sample, const std::string& Type)
: access_(VicarLiteFile::WRITE),
  fname_(Fname), force_area_pixel_(false),
  number_line_(Number_line), 
  number_sample_(Number_sample)
{
  throw Exception("Not implemented yet");
}

//-----------------------------------------------------------------------
/// Read in the label from the current f_ position. Optionally set 
//-----------------------------------------------------------------------

std::string VicarLiteFile::read_label(int& lblsize) 
{
//-----------------------------------------------------------------------
// Read in label size.
//-----------------------------------------------------------------------

  std::string t;
  (*f_) >> t;
  lblsize = atoi(t.c_str() + 8);

//-----------------------------------------------------------------------
// Read in labels.
//-----------------------------------------------------------------------

  std::string label_data(lblsize - t.size(), '\0');
  f_->read(&label_data[0], lblsize - t.size());
  return label_data;
}

//-----------------------------------------------------------------------
/// Function to doing tokens.
//-----------------------------------------------------------------------

class TokenizerFunction {
public:
  void reset() {}
  template <typename InputIterator, typename Token>
  bool operator()(InputIterator& next,InputIterator end,Token& tok) {
    bool inquote = false;
    tok = Token();
    if(next == end)
      return false;
    while((*next == ' ' || *next == '=' || *next == '\0') && next != end)
      ++next;
    for(;next != end; ++next) {
      if(*next == '\'')
	inquote = (inquote ? false : true);
      else if(!inquote && (*next == ' ' || *next == '=' || *next == '\0'))
	return true;
      else
	tok += *next;
    }
    return (tok.size() != 0);
  }
};

//-----------------------------------------------------------------------
/// Process labels. This is just a string of keyword=value, where
/// value might have "'" around it. We store the results in label_.
/// We also handle properties, this are stored in label_ as the string
/// "<property name> <keyword>", but are otherwise handled as labels.
/// We fill in the set prop_set_ also.
//-----------------------------------------------------------------------

void VicarLiteFile::process_label(const std::string& label) 
{
//-----------------------------------------------------------------------
// Break into tokens. When this is done, we'll read things as <key>,
// <value>, and then repeat. Spaces separate key/value pairs, and "="
// separates key and value. We need to be careful about "'", anything
// is allowed between these two values.
//-----------------------------------------------------------------------

  std::string prop = "";
  typedef boost::tokenizer<TokenizerFunction> tok_type;
  TokenizerFunction tf;
  tok_type tok(label, tf);
  for(tok_type::iterator i = tok.begin(); i != tok.end();) {
    std::string k = *i++;
    if(i != tok.end()) {
      std::string v = *i++;
      if(k == "PROPERTY") {	// Starting a new property
	prop_set_.insert(v);
	prop = v + " ";
      } else if(k =="TASK") { // End of  property section
	prop = "";
      } else {
	label_[prop + k] = v;
      }
    }
  }
}

//-----------------------------------------------------------------------
/// Read metadata for MapInfo.
//-----------------------------------------------------------------------

MapInfo VicarLiteFile::map_info() const
{
  using std::string;
  if(property().count("GEOTIFF") != 1)
    throw Exception("File " + fname_ + 
       " doesn't contain the GEOTIFF property needed for MapInfo");
  std::istringstream isp(label<string>("GTMODELTYPEGEOKEY", "GEOTIFF"));
  int projection_id;
  isp >> projection_id;
  if(projection_id != 2)
#ifdef USE_VICAR_OGR
    return vlogr.from_vicar(*this);
#else
    throw Exception("Right now, only Geographic projections are supported. File "
		    + fname_ + " had projection " + 
		    label<string>("GTMODELTYPEGEOKEY", "GEOTIFF"));
#endif
  if(has_label("GEOTIFF GEOGELLIPSOIDGEOKEY")) {
    std::istringstream ise(label<string>("GEOGELLIPSOIDGEOKEY", "GEOTIFF"));
    int ellipsoid_id;
    ise >> ellipsoid_id;
    if(ellipsoid_id != 7030)
#ifdef USE_VICAR_OGR
      return vlogr.from_vicar(*this);
#else
      throw Exception("Right now, only WGS-84 reference ellipsoid is supported. File "
		      + fname_ + " had reference ellipsoid " + 
		      label<string>("GTMODELTYPEGEOKEY", "GEOTIFF"));
#endif
  } else if(has_label("GEOTIFF GEOGRAPHICTYPEGEOKEY")) {
    std::istringstream ise(label<string>("GEOGRAPHICTYPEGEOKEY", "GEOTIFF"));
    int geo_id;
    ise >> geo_id;
    if(geo_id != 4326)
#ifdef USE_VICAR_OGR
      return vlogr.from_vicar(*this);
#else
      throw Exception("Right now, only WGS-84 reference ellipsoid is supported. File "
		      + fname_ + " had reference ellipsoid " + 
		      label<string>("GEOGRAPHICTYPEGEOKEY", "GEOTIFF"));
#endif
  } else {
    throw Exception("Must have GEOGELLIPSOIDGEOKEY or GEOGRAPHICTYPEGEOKEY. File" + fname_);
  }
  if(has_label("GEOTIFF MODELTRANSFORMATIONTAG")) {
    string t1 = label<string>("MODELTRANSFORMATIONTAG", "GEOTIFF");
    BOOST_FOREACH(char& c, t1)
      if(c == '(' || c == ')' || c == ',')
	c = ' ';
    std::istringstream is1(t1);
    double a[4][4];
    for(int i = 0; i < 4; ++i)
      for(int j = 0; j < 4; ++j)
	is1 >> a[i][j];
    if(fabs(a[0][2]) > 1e-8 || fabs(a[1][2]) > 1e-8 || fabs(a[2][0]) > 1e-8 || 
       fabs(a[2][1]) > 1e-8 || fabs(a[2][2]) > 1e-8 || fabs(a[2][3]) > 1e-8 ||
       fabs(a[3][0]) > 1e-8 || fabs(a[3][1]) > 1e-8 || fabs(a[3][2]) > 1e-8)
      throw Exception("We don't work with 3-D data for file " + fname_);
    blitz::Array<double, 1> param(6);
    param = a[0][3], a[0][0], a[0][1],
      a[1][3], a[1][0], a[1][1];
    MapInfo res(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter),
		param, number_sample(), number_line());
    return res;
  }
  string t1 = label<string>("MODELTIEPOINTTAG", "GEOTIFF");
  BOOST_FOREACH(char& c, t1)
    if(c == '(' || c == ')' || c == ',')
      c = ' ';
  std::istringstream is1(t1);
  double samp, line, h1, lon, lat, h2;
  is1 >> samp >> line >> h1 >> lon >> lat >> h2;
  if(fabs(h1) > 1e-8 || fabs(h2) > 1e-8)
    throw Exception("We don't work with 3-D data for file " + fname_);
  string t2 = label<string>("MODELPIXELSCALETAG", "GEOTIFF");
  BOOST_FOREACH(char& c, t2)
    if(c == '(' || c == ')' || c == ',')
      c = ' ';
  std::istringstream is2(t2);
  double lon_scale, lat_scale;
  is2 >> lon_scale >> lat_scale;
  lat_scale *= -1;		// Convention of lat_scale in GeoTiff
				// is opposite of ours.
  lon -= lon_scale * samp;
  lat -= lat_scale * line;

// For Geotiff, there are two ways the parameters are specified. For
// area based pixels, the ulc is the ulc of the pixel, which is what
// MapInfo expects. For point, the ulc given is for the center of the
// pixel. That means we need to offset this by 1/2 pixel to match what
// we expect in MapInfo. We check and see if the file has metadata
// giving this information, and if so if we need to offset the pixel.

  if(has_label("GEOTIFF GTRASTERTYPEGEOKEY")) {
    std::istringstream ise(label<string>("GTRASTERTYPEGEOKEY", "GEOTIFF"));
    int rt_id;
    ise >> rt_id;
    if(rt_id ==2 && !force_area_pixel_) {		// Point type
      lon -= 0.5 * lon_scale;
      lat -= 0.5 * lat_scale;
    }
  }
  MapInfo res(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter),
	      lon, lat, lon + number_sample() * lon_scale,
	      lat + number_line() * lat_scale, number_sample(),
	      number_line());
  return res;
}

//-----------------------------------------------------------------------
/// Set metadata for MapInfo.
//-----------------------------------------------------------------------

void VicarLiteFile::map_info(const MapInfo& M)
{
  throw Exception("Not implemented yet");
}

//-----------------------------------------------------------------------
/// Return true if the file has a GEOTIFF label in it, indicating it
/// has map information.
//-----------------------------------------------------------------------

bool VicarLiteFile::has_map_info() const
{
  if(!(has_label("GEOTIFF MODELTRANSFORMATIONTAG") ||
       has_label("GEOTIFF MODELTIEPOINTTAG")))
     return false;
  // We *probably* have a map info, but VICAR can write out some
  // seriously mangled metadata. Try to actually read the map_info,
  // and if this fails we'll just say we don't have mapinfo
  try {
    map_info();
    return true;
  } catch(...) {
  }
  return false;
}

//-----------------------------------------------------------------------
/// Return true if the file has a NITF_CETAG label in it, indicating it
/// has RPC information.
//-----------------------------------------------------------------------

bool VicarLiteFile::has_rpc() const
{
  return (has_label("GEOTIFF NITF_CETAG"));
}

//-----------------------------------------------------------------------
/// Read metadata for Rpc.
//-----------------------------------------------------------------------

Rpc VicarLiteFile::rpc() const
{
  using std::string;
  string g = "GEOTIFF";
  Rpc res;
  std::string cetag = label<string>("NITF_CETAG", g);
  if(cetag == "RPC00B")
    res.rpc_type = Rpc::RPC_B;
  else if(cetag =="RPC00A")
    res.rpc_type = Rpc::RPC_A;
  else
    throw MetadataMissing("Don't recognize value of NITF_CETAG");
  if(has_label(g + " NAIF_CODE")) {
    int naif_code = label<int>("NAIF_CODE", g);
    std::cerr << "Faking NAIF CODE " << naif_code << "\n";
  }
  res.line_offset = atof(label<string>("RPC_FIELD4",  g).c_str());
  res.sample_offset = atof(label<string>("RPC_FIELD5",  g).c_str());
  res.latitude_offset = atof(label<string>("RPC_FIELD6",  g).c_str());
  res.longitude_offset = atof(label<string>("RPC_FIELD7",  g).c_str());
  res.height_offset = atof(label<string>("RPC_FIELD8",  g).c_str());
  res.line_scale = atof(label<string>("RPC_FIELD9",  g).c_str());
  res.sample_scale = atof(label<string>("RPC_FIELD10",  g).c_str());
  res.latitude_scale = atof(label<string>("RPC_FIELD11",  g).c_str());
  res.longitude_scale = atof(label<string>("RPC_FIELD12",  g).c_str());
  res.height_scale = atof(label<string>("RPC_FIELD13",  g).c_str());
  res.error_bias = 0.0;
  res.error_random = 0.0;
  for(int i = 1; i <= 20; ++i) {
    std::string is = boost::lexical_cast<std::string>(i);
    res.line_numerator[i - 1] = 
      atof(label<string>("RPC_FIELD14" + is, g).c_str());
    res.line_denominator[i - 1] = 
      atof(label<string>("RPC_FIELD15" + is, g).c_str());
    res.sample_numerator[i - 1] = 
      atof(label<string>("RPC_FIELD16" + is, g).c_str());
    res.sample_denominator[i - 1] = 
      atof(label<string>("RPC_FIELD17" + is, g).c_str());
  }
  return res;
}
