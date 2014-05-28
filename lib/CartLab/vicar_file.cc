#include "vicar_file.h"
#ifdef HAVE_GDAL
#include "vicar_ogr.h"
#endif
#include "geocal_exception.h"
#include "simple_dem.h"
#include <string>
#include <boost/foreach.hpp>
#define BOOST_LEXICAL_CAST_ASSUME_C_LOCALE
#include <boost/lexical_cast.hpp>
#undef EQUAL
#ifdef HAVE_VICAR_RTL
#include <zvproto.h>
#include <errdefs.h>
#include <defines.h>
#endif
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace GeoCal;

int VicarFile::instance = 1;

#ifdef HAVE_GDAL
static VicarOgr vogr;
#endif

//-----------------------------------------------------------------------
/// Return a VICAR unit number that is tied to the given file name.
//-----------------------------------------------------------------------

int VicarFile::file_name_to_unit(const std::string& Fname)
{
#ifdef HAVE_VICAR_RTL
  int res;
  int status = zvunit(&res, const_cast<char*>("NONEGEOCAL"), instance, 
		      const_cast<char*>("U_NAME"), Fname.c_str(), 
		      NULL);
  instance += 1;
  if(status != 1)
    throw VicarException(status, "Call to zvunit failed for file " + Fname);
  // Suppress copying of labels from primary input to the output file.
  zvselpi(0);
  return res;
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Determine if a given file is a VICAR file or not.
///
/// This looks for the first few characters of the file being "LBLSIZE="
//-----------------------------------------------------------------------

bool VicarFile::is_vicar_file(const std::string& Fname)
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
/// difference, it matters for may applications. Most users should
/// just ignore this value.
//-----------------------------------------------------------------------

VicarFile::VicarFile(const std::string& Fname, access_type Access, 
		     bool Force_area_pixel)
: fname_(Fname), force_area_pixel_(Force_area_pixel), 
  unit_(-1), access_(Access)
{
  unit_ = file_name_to_unit(Fname);
  open_unit();
}

//-----------------------------------------------------------------------
/// Create a new VICAR file with the given size.
//-----------------------------------------------------------------------

VicarFile::VicarFile(const std::string& Fname, int Number_line, 
		     int Number_sample, const std::string& Type,
		     compression Compress)
  : fname_(Fname), force_area_pixel_(false), 
    unit_(-1), number_line_(Number_line), 
    number_sample_(Number_sample), number_band_(1), access_(WRITE)
{
#ifdef HAVE_VICAR_RTL
  unit_ = file_name_to_unit(Fname);
  int status;
  switch(Compress) {
  case NONE:
    status = zvopen(unit(), "OP", "WRITE", "U_NL", Number_line, "U_NS",
		    Number_sample, "O_FORMAT", Type.c_str(), 
		    "U_FORMAT", Type.c_str(), NULL);
    break;
  case BASIC:
    status = zvopen(unit(), "OP", "WRITE", "U_NL", Number_line, "U_NS",
		    Number_sample, "O_FORMAT", Type.c_str(), 
		    "U_FORMAT", Type.c_str(), "COMPRESS", "BASIC", NULL);
    break;
  case BASIC2:
    status = zvopen(unit(), "OP", "WRITE", "U_NL", Number_line, "U_NS",
		    Number_sample, "O_FORMAT", Type.c_str(), 
		    "U_FORMAT", Type.c_str(), "COMPRESS", "BASIC2", NULL);
    break;
  default:
    throw Exception("Unrecognized compression type");
  }    
  if(status != 1)
    throw VicarException(status, "Call to zvopen failed for file " + fname_);
  set_type(Type);
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Create a new VICAR file with the given size.
//-----------------------------------------------------------------------

VicarFile::VicarFile(const std::string& Fname, int Number_line, 
		     int Number_sample, int Number_band,
		     const std::string& Type,
		     const std::string& Org,
		     compression Compress)
  : fname_(Fname), force_area_pixel_(false), 
    unit_(-1), number_line_(Number_line), 
    number_sample_(Number_sample), number_band_(Number_band), access_(WRITE)
{
#ifdef HAVE_VICAR_RTL
  if(Org == "BIP")
    throw Exception("Note that BIP doesn't seem to currently work. This hasn't been worth fixing, so we'll need to fix it only if you actually need BIP");
  unit_ = file_name_to_unit(Fname);
  int status;
  switch(Compress) {
  case NONE:
    status = zvopen(unit(), "OP", "WRITE", "U_NL", Number_line, "U_NS",
		    Number_sample, "U_NB", Number_band, "U_ORG", Org.c_str(),
		    "O_FORMAT", Type.c_str(), 
		    "U_FORMAT", Type.c_str(), NULL);
    break;
  case BASIC:
    status = zvopen(unit(), "OP", "WRITE", "U_NL", Number_line, "U_NS",
		    Number_sample, "U_NB", Number_band, "U_ORG", Org.c_str(),
		    "O_FORMAT", Type.c_str(), 
		    "U_FORMAT", Type.c_str(), "COMPRESS", "BASIC", NULL);
    break;
  case BASIC2:
    status = zvopen(unit(), "OP", "WRITE", "U_NL", Number_line, "U_NS",
		    Number_sample, "U_NB", Number_band, "U_ORG", Org.c_str(),
		    "O_FORMAT", Type.c_str(), 
		    "U_FORMAT", Type.c_str(), "COMPRESS", "BASIC2", NULL);
    break;
  default:
    throw Exception("Unrecognized compression type");
  }    
  if(status != 1)
    throw VicarException(status, "Call to zvopen failed for file " + fname_);
  set_type(Type);
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Initialize type_ from the string.
//-----------------------------------------------------------------------

void VicarFile::set_type(const std::string& Type) 
{
  if(Type =="BYTE")
    type_ = VICAR_BYTE;
  else if(Type =="HALF")
    type_ = VICAR_HALF;
  else if(Type =="FULL")
    type_ = VICAR_FULL;
  else if(Type =="REAL")
    type_ = VICAR_FLOAT;
  else if(Type =="DOUB")
    type_ = VICAR_DOUBLE;
  else
    throw Exception("Unrecognized type");
}

//-----------------------------------------------------------------------
/// Create a new VICAR file with the given size. Use the VICAR Name
/// and Instance input (so for example, "INP" and 2 is the second INP
/// file passed to a VICAR program. 
//-----------------------------------------------------------------------

VicarFile::VicarFile(int Instance, int Number_line, int Number_sample,
		     int Number_band,
		     const std::string& Type,
		     const std::string& Name, 
		     const std::string& Org,
		     compression Compress)
  : force_area_pixel_(false),
    unit_(-1), number_line_(Number_line), number_sample_(Number_sample),
    number_band_(Number_band),
    access_(WRITE)
{
#ifdef HAVE_VICAR_RTL
  if(Org == "BIP")
    throw Exception("Note that BIP doesn't seem to currently work. This hasn't been worth fixing, so we'll need to fix it only if you actually need BIP");
  int status = zvunit(&unit_, const_cast<char*>(Name.c_str()), Instance, 
		      NULL);
  fname_ = Name + " Instance: " + boost::lexical_cast<std::string>(Instance);
  if(status != 1)
    throw VicarException(status, "Call to zvunit failed for file " + fname_);
  switch(Compress) {
  case NONE:
    status = zvopen(unit(), "OP", "WRITE", "U_NL", Number_line, "U_NS",
		    Number_sample, "O_FORMAT", Type.c_str(), 
		    "U_NB", Number_band, "U_ORG", Org.c_str(),
		    "U_FORMAT", Type.c_str(), 
		    NULL);
    break;
  case BASIC:
    status = zvopen(unit(), "OP", "WRITE", "U_NL", Number_line, "U_NS",
		    Number_sample, "O_FORMAT", Type.c_str(), 
		    "U_NB", Number_band, "U_ORG", Org.c_str(),
		    "U_FORMAT", Type.c_str(), "COMPRESS", "BASIC", 
		    NULL);
    break;
  case BASIC2:
    status = zvopen(unit(), "OP", "WRITE", "U_NL", Number_line, "U_NS",
		    Number_sample, "O_FORMAT", Type.c_str(), 
		    "U_NB", Number_band, "U_ORG", Org.c_str(),
		    "U_FORMAT", Type.c_str(), "COMPRESS", "BASIC2", 
		    NULL);
    break;
  default:
    throw Exception("Unrecognized compression type");
  }    

  if(status != 1)
    throw VicarException(status, "Call to zvopen failed for file " + fname_);
  set_type(Type);
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Open a file, using the VICAR Name and Instance input (so for
/// example, "INP" and 2 is the second INP file passed to a VICAR program.
//-----------------------------------------------------------------------

VicarFile::VicarFile(int Instance, access_type Access, 
		     const std::string& Name)
  : force_area_pixel_(false), unit_(-1), access_(Access)
{
#ifdef HAVE_VICAR_RTL
  int status = zvunit(&unit_, const_cast<char*>(Name.c_str()), Instance, 
		      NULL);
  fname_ = Name + " Instance: " + boost::lexical_cast<std::string>(Instance);
  if(status != 1)
    throw VicarException(status, "Call to zvunit failed for file " + fname_);
  open_unit();
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Once a unit has been generated, open the file and process the
/// labels. 
//-----------------------------------------------------------------------

void VicarFile::open_unit()
{
#ifdef HAVE_VICAR_RTL
  std::string a = (access_ == READ ? "READ" :
		   (access_ == UPDATE ? "UPDATE" : "WRITE"));
  int status = zvopen(unit(), "OP", a.c_str(), NULL);
  if(status != 1)
    throw VicarException(status, "Call to zvopen failed for file " + fname_);

// Property is a nesting of labels. Because it is convenient, we store
// regular labels and nested labels together in label_type_ and
// label_nelement_. This is because other than the nesting and a
// slightly different call sequence, these get treated the same.

// First, get a list of all properties.
  int nprop = 1;
  int ulen = MAX_LABEL_KEY_SIZE + 1;
  std::vector<char> buf(ulen * nprop);
  int npropret;
  status = zlpinfo(unit(), &buf[0], &nprop, "NRET", &npropret, "ULEN", 
		   ulen, NULL);
  if(status != 1)
    throw VicarException(status, "Call to zlpinfo failed");
  nprop = (npropret > 0 ? npropret : 1);
  buf.resize(ulen * nprop);
  status = zlpinfo(unit(), &buf[0], &nprop, "ULEN", ulen, NULL);
  if(status != 1)
    throw VicarException(status, "Call to zlpinfo failed");
  std::vector<std::string> prop_list;
  for(int i = 0; i < nprop; ++i)
    prop_list.push_back(std::string(&buf[i * ulen]));
  prop_set_.insert(prop_list.begin(), prop_list.end());
  std::string prop = "";
  std::vector<std::string>::const_iterator next_prop = prop_list.begin();
  while(status ==1) {
    char key[MAX_LABEL_KEY_SIZE + 1];	
    char format[9];
    int maxlength, nelement;
    status = zlninfo(unit(), key, format, &maxlength, &nelement, NULL);
    if(status == 1) {		// Error handling after this loop
      std::string f(format);
      std::string k(key);
      if(k == "PROPERTY") 	// Starting a new property.
	prop = *next_prop++ + " ";
      else if(k =="TASK")	// End of property section
	prop = "";
      else {
	k = prop + k;
	if(f == "INT")
	  label_type_[k] = VicarFile::VICAR_INT;
	else if(f =="REAL")
	  label_type_[k] = VicarFile::VICAR_REAL;
	else if(f =="STRING") {
	  label_type_[k] = VicarFile::VICAR_STRING;
	  label_maxlength_[k] = maxlength;
	} else
	  throw Exception("Unrecognized label type " + f + " found in file " + 
			  fname_);
	label_nelement_[k] = nelement;
      }
    }
  }
  if(status != END_OF_LABEL)
    throw VicarException(status, "zlninfo failed for file " + fname_);
  number_line_ = label<int>("NL");
  number_sample_ = label<int>("NS");
  number_band_ = label<int>("NB");
  set_type(label<std::string>("FORMAT"));
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Destructor, closes file.
//-----------------------------------------------------------------------

VicarFile::~VicarFile()
{
  close();
}

//-----------------------------------------------------------------------
/// Close file.
//-----------------------------------------------------------------------

void VicarFile::close()
{
#ifdef HAVE_VICAR_RTL
  if(unit() != -1) {
    int status = zvclose(unit(), "CLOS_ACT", "FREE", NULL);
    if(status != 1 && status != FILE_NOT_OPEN) {
      VicarException e(status);
      e << "Call to zvclose failed for unit " << unit() << " file name " 
	<< file_name();
      throw e;
    }
    unit_ = -1;
  }
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Return true if the file has a GEOTIFF label in it, indicating it
/// has map information.
//-----------------------------------------------------------------------

bool VicarFile::has_map_info() const
{
  if(!(has_label("GEOTIFF MODELTRANSFORMATIONTAG") ||
       has_label("GEOTIFF MODELTIEPOINTTAG")))
     return false;
  // We *probably* have a map info, but VICAR can write out some
  // seriously mangled metadata. Try to actually read the map_info,
  // and if this fails we'll just say we don't have mapinfo
  try {
#ifdef HAVE_GDAL
    if(!map_info_.get())
      map_info_.reset(new MapInfo(vogr.from_vicar(*this)));
#else
  throw Exception("The current implementation of VicarFile::map_info uses the GDAL library, which was not found during the build of this library");
#endif
    return true;
  } catch(...) {
  }
  return false;
}

//-----------------------------------------------------------------------
/// Return true if the file has a NITF_CETAG label in it, indicating it
/// has RPC information.
//-----------------------------------------------------------------------

bool VicarFile::has_rpc() const
{
  return (has_label("GEOTIFF NITF_CETAG"));
}

//-----------------------------------------------------------------------
/// Read metadata for MapInfo.
//-----------------------------------------------------------------------

MapInfo VicarFile::map_info() const
{
  if(map_info_.get())
    return *map_info_;
  if(!has_map_info())
    throw Exception("Attempt to call map_info() on a file that doesn't have it.");
  // has_map_info has the side effect of setting up map_info, so we
  // can just return this now.
  return *map_info_;
}

//-----------------------------------------------------------------------
/// Set metadata for MapInfo.
//-----------------------------------------------------------------------

void VicarFile::map_info(const MapInfo& M)
{
  map_info_.reset(new MapInfo(M));
#ifdef HAVE_GDAL
  return vogr.to_vicar(M, *this);
#else
  throw Exception("The current implementation of VicarFile::map_info uses the GDAL library, which was not found during the build of this library");
#endif
}

//-----------------------------------------------------------------------
/// Delete a label from a file.
//-----------------------------------------------------------------------

void VicarFile::label_delete(const std::string& F, const std::string& Property)
{
#ifdef HAVE_VICAR_RTL
  int status;
  if(Property == "") {
    status = zldel(unit(), const_cast<char*>("SYSTEM"), 
		   const_cast<char*>(F.c_str()), NULL);
    if(status != 1 && status != CANNOT_FIND_KEY)
      throw VicarException(status, "Call to zldel failed");
  } else {
    status = zldel(unit(), const_cast<char*>("PROPERTY"),
		   const_cast<char*>(F.c_str()), 
		   const_cast<char*>("PROPERTY"), Property.c_str(), NULL);
    if(status != 1 && status != CANNOT_FIND_KEY && status != NO_SUCH_PROPERTY)
      throw VicarException(status, "Call to zldel failed");
  }
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Set the value of a label. If the label is already in the file, is 
/// is deleted and replaced with this new value. Otherwise, it is
/// simply added. Optionally the label can be part of a Property.
//-----------------------------------------------------------------------

void VicarFile::label_set(const std::string& F, 
			  int Val,
			  const std::string& Property)
{
#ifdef HAVE_VICAR_RTL
  int status;
  if(Property == "") {
    status = zldel(unit(), const_cast<char*>("SYSTEM"), 
		   const_cast<char*>(F.c_str()), NULL);
    if(status != 1 && status != CANNOT_FIND_KEY)
      throw VicarException(status, "Call to zldel failed");
    status = zladd(unit(), const_cast<char*>("SYSTEM"), 
		   const_cast<char*>(F.c_str()), 
		   &Val,
		   const_cast<char*>("FORMAT"), const_cast<char*>("INT"), NULL);
    if(status != 1)
      throw VicarException(status, "Call to zladd failed");
  } else {
    status = zldel(unit(), const_cast<char*>("PROPERTY"),
		   const_cast<char*>(F.c_str()), 
		   const_cast<char*>("PROPERTY"), Property.c_str(), NULL);
    if(status != 1 && status != CANNOT_FIND_KEY && status != NO_SUCH_PROPERTY)
      throw VicarException(status, "Call to zldel failed");
    status = zladd(unit(), const_cast<char*>("PROPERTY"), 
		   const_cast<char*>(F.c_str()), 
		   &Val,
		   const_cast<char*>("PROPERTY"), Property.c_str(),
		   const_cast<char*>("FORMAT"), const_cast<char*>("INT"), NULL);
    if(status != 1)
      throw VicarException(status, "Call to zladd failed");
  }
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Set the value of a label. If the label is already in the file, is 
/// is deleted and replaced with this new value. Otherwise, it is
/// simply added. Optionally the label can be part of a Property.
//-----------------------------------------------------------------------

void VicarFile::label_set(const std::string& F, 
			  float Val,
			  const std::string& Property)
{
#ifdef HAVE_VICAR_RTL
  int status;
  if(Property == "") {
    status = zldel(unit(), const_cast<char*>("SYSTEM"), 
		   const_cast<char*>(F.c_str()), NULL);
    if(status != 1 && status != CANNOT_FIND_KEY)
      throw VicarException(status,"Call to zldel failed");
    status = zladd(unit(), const_cast<char*>("SYSTEM"), 
		   const_cast<char*>(F.c_str()), 
		   &Val,
		   const_cast<char*>("FORMAT"), const_cast<char*>("REAL"), 
		   NULL);
    if(status != 1)
      throw VicarException(status,"Call to zladd failed");
  } else {
    status = zldel(unit(), const_cast<char*>("PROPERTY"), 
		   const_cast<char*>(F.c_str()), 
		   const_cast<char*>("PROPERTY"), Property.c_str(), NULL);
    if(status != 1 && status != CANNOT_FIND_KEY && status != NO_SUCH_PROPERTY)
      throw VicarException(status,"Call to zldel failed");
    status = zladd(unit(), const_cast<char*>("PROPERTY"), 
		   const_cast<char*>(F.c_str()), 
		   &Val,
		   const_cast<char*>("PROPERTY"), Property.c_str(),
		   const_cast<char*>("FORMAT"), const_cast<char*>("REAL"), 
		   NULL);
    if(status != 1)
      throw VicarException(status,"Call to zladd failed");
  }
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Set the value of a label. If the label is already in the file, is 
/// is deleted and replaced with this new value. Otherwise, it is
/// simply added. Optionally the label can be part of a Property.
//-----------------------------------------------------------------------

void VicarFile::label_set(const std::string& F, 
			  const std::string& Val,
			  const std::string& Property)
{
#ifdef HAVE_VICAR_RTL
  int status;
  if(Property == "") {
    status = zldel(unit(), const_cast<char*>("SYSTEM"), 
		   const_cast<char*>(F.c_str()), NULL);
    if(status != 1 && status != CANNOT_FIND_KEY)
      throw VicarException(status,"Call to zldel failed");
    status = zladd(unit(), const_cast<char*>("SYSTEM"), 
		   const_cast<char*>(F.c_str()), 
		   const_cast<char*>(Val.c_str()),
		   const_cast<char*>("FORMAT"), const_cast<char*>("STRING"), 
		   NULL);
    if(status != 1)
      throw VicarException(status,"Call to zladd failed");
  } else {
    status = zldel(unit(), const_cast<char*>("PROPERTY"), 
		   const_cast<char*>(F.c_str()), 
		   const_cast<char*>("PROPERTY"), Property.c_str(), NULL);
    if(status != 1 && status != CANNOT_FIND_KEY && status != NO_SUCH_PROPERTY)
      throw VicarException(status,"Call to zldel failed");
    status = zladd(unit(), const_cast<char*>("PROPERTY"), 
		   const_cast<char*>(F.c_str()), 
		   const_cast<char*>(Val.c_str()),
		   const_cast<char*>("PROPERTY"), Property.c_str(),
		   const_cast<char*>("FORMAT"), 
		   const_cast<char*>("STRING"), NULL);
    if(status != 1)
      throw VicarException(status,"Call to zladd failed");
  }
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Read metadata for Rpc.
//-----------------------------------------------------------------------

Rpc VicarFile::rpc() const
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
  res.error_bias = atof(label<string>("RPC_FIELD2",  g).c_str());
  res.error_random = atof(label<string>("RPC_FIELD3",  g).c_str());
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
/// Set metadata for Rpc.
//-----------------------------------------------------------------------

void VicarFile::rpc(const Rpc& V)
{
  if(V.rpc_type ==Rpc::RPC_B)
    label_set("NITF_CETAG", "RPC00B", "GEOTIFF");
  else if(V.rpc_type ==Rpc::RPC_A)
    label_set("NITF_CETAG", "RPC00A", "GEOTIFF");
  else 
    throw Exception("Unrecognized rpc type");
  label_set("RPC_FIELD1", "1", "GEOTIFF");
  label_set("RPC_FIELD2", to_s1(V.error_bias), "GEOTIFF");
  label_set("RPC_FIELD3", to_s1(V.error_random), "GEOTIFF");
  label_set("RPC_FIELD4", to_s1(V.line_offset), "GEOTIFF");
  label_set("RPC_FIELD5", to_s1(V.sample_offset), "GEOTIFF");
  label_set("RPC_FIELD6", to_s1(V.latitude_offset), "GEOTIFF");
  label_set("RPC_FIELD7", to_s1(V.longitude_offset), "GEOTIFF");
  label_set("RPC_FIELD8", to_s1(V.height_offset), "GEOTIFF");
  label_set("RPC_FIELD9", to_s1(V.line_scale), "GEOTIFF");
  label_set("RPC_FIELD10", to_s1(V.sample_scale), "GEOTIFF");
  label_set("RPC_FIELD11", to_s1(V.latitude_scale), "GEOTIFF");
  label_set("RPC_FIELD12", to_s1(V.longitude_scale), "GEOTIFF");
  label_set("RPC_FIELD13", to_s1(V.height_scale), "GEOTIFF");
  for(int i = 1; i <= 20; ++i) {
    std::string is = boost::lexical_cast<std::string>(i);
    label_set("RPC_FIELD14" + is, 
	      to_s2(V.line_numerator[i - 1]), "GEOTIFF");
    label_set("RPC_FIELD15" + is, 
	      to_s2(V.line_denominator[i - 1]), "GEOTIFF");
    label_set("RPC_FIELD16" + is, 
	      to_s2(V.sample_numerator[i - 1]), "GEOTIFF");
    label_set("RPC_FIELD17" + is, 
	      to_s2(V.sample_denominator[i - 1]), "GEOTIFF");
  }

  // VICAR expects NITF corners when it finds an RPC. We estimiate this
  // by finding the corners at the height offset of the RPC.
  SimpleDem d(V.height_offset);
  Geodetic g1 = V.ground_coordinate(ImageCoordinate(0, 0), d);
  Geodetic g2 = V.ground_coordinate(ImageCoordinate(0, number_sample() - 1), d);
  Geodetic g3 = V.ground_coordinate(ImageCoordinate(number_line() - 1, number_sample() - 1), d);
  Geodetic g4 = V.ground_coordinate(ImageCoordinate(number_line() - 1, 0), d);
  label_set("NITF_CORNERLAT1", to_s2(g1.latitude()), "GEOTIFF");
  label_set("NITF_CORNERLON1", to_s2(g1.longitude()), "GEOTIFF");
  label_set("NITF_CORNERLAT2", to_s2(g2.latitude()), "GEOTIFF");
  label_set("NITF_CORNERLON2", to_s2(g2.longitude()), "GEOTIFF");
  label_set("NITF_CORNERLAT3", to_s2(g3.latitude()), "GEOTIFF");
  label_set("NITF_CORNERLON3", to_s2(g3.longitude()), "GEOTIFF");
  label_set("NITF_CORNERLAT4", to_s2(g4.latitude()), "GEOTIFF");
  label_set("NITF_CORNERLON4", to_s2(g4.longitude()), "GEOTIFF");
}

//-----------------------------------------------------------------------
/// Close and reopen the file. Vicar is odd about reading to the end
/// of the file, and we sometimes need to reopen it to clear end of
/// file status.
//-----------------------------------------------------------------------

void VicarFile::reopen_file() const 
{
#ifdef HAVE_VICAR_RTL
  int status = zvclose(unit(), NULL);
  if(status != 1 && status != FILE_NOT_OPEN) {
    VicarException e(status);
    e << "Call to zvclose failed for unit " << unit() << " file name " 
      << file_name();
    throw e;
  }
  // Use UPDATE access for both WRITE and UPDATE, since you can't
  // reopen a file for WRITE.
  std::string a = (access_ == READ ? "READ" : "UPDATE");
  status = zvopen(unit(), "OP", a.c_str(), NULL);
  if(status != 1)
    throw VicarException(status, "Call to zvopen failed for file " + fname_);
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This is a thin wrapper around the RTL function zvread. If the
/// VICAR library is installed then we just forward to that library,
/// otherwise we throw an exception saying that it isn't available.
//-----------------------------------------------------------------------

int VicarFile::zvreadw(void* buffer, int Line, int Band) const
{
#ifdef HAVE_VICAR_RTL
  int status = zvread(unit(), buffer, "LINE", Line, "BAND", Band, 
		      "NBANDS", 1, NULL);

// Vicar is odd about reading to the end of file. If we get this
// error, then try closing and reopening file and then doing the read.

  if(status ==END_OF_FILE) {
    reopen_file();
    status = zvread(unit(), buffer, "LINE", Line, "BAND", Band, 
		    "NBANDS", 1, NULL);
  }
  return status;
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This is a thin wrapper around the RTL function zvread. If the
/// VICAR library is installed then we just forward to that library,
/// otherwise we throw an exception saying that it isn't available.
//-----------------------------------------------------------------------

int VicarFile::zvreadw(void* buffer, int Band) const
{
#ifdef HAVE_VICAR_RTL
  int status = zvread(unit(), buffer, "BAND", Band, "NBANDS", 1, NULL);

// Vicar is odd about reading to the end of file. If we get this
// error, then try closing and reopening file and then doing the read.

  if(status ==END_OF_FILE) {
    reopen_file();
    status = zvread(unit(), buffer, "BAND", Band, "NBANDS", 1, NULL);
  }
  return status;
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This is a thin wrapper around the RTL function zvwrit. If the
/// VICAR library is installed then we just forward to that library,
/// otherwise we throw an exception saying that it isn't available.
//-----------------------------------------------------------------------

int VicarFile::zvwritw(void* buffer, int Band)
{
#ifdef HAVE_VICAR_RTL
  return zvwrit(unit(), buffer, "BAND", Band, "NBANDS", 1, NULL);
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This is a thin wrapper around the RTL function zvwrit. If the
/// VICAR library is installed then we just forward to that library,
/// otherwise we throw an exception saying that it isn't available.
//-----------------------------------------------------------------------

int VicarFile::zvwritw(void* buffer, int Line, int Band)
{
#ifdef HAVE_VICAR_RTL
  return zvwrit(unit(), buffer, "LINE", Line, "BAND", Band, NULL);
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This is a thin wrapper around the RTL function zlget. If the
/// VICAR library is installed then we just forward to that library,
/// otherwise we throw an exception saying that it isn't available.
//-----------------------------------------------------------------------

int VicarFile::zlgetw(int unit, const char* type, const char* key, char* value)
{
#ifdef HAVE_VICAR_RTL
  return zlget(unit, const_cast<char*>(type), 
	       const_cast<char*>(key), value, NULL);
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This is a thin wrapper around the RTL function zlget. If the
/// VICAR library is installed then we just forward to that library,
/// otherwise we throw an exception saying that it isn't available.
//-----------------------------------------------------------------------

int VicarFile::zlgetsh(int unit, const char* key, char* value)
{
#ifdef HAVE_VICAR_RTL
  int status = VicarFile::zlgetw(unit, "SYSTEM", const_cast<char*>(key), value);
  if(status ==CANNOT_FIND_KEY)
    status = VicarFile::zlgetw(unit, "HISTORY", const_cast<char*>(key), value);
  return status;
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This is a thin wrapper around the RTL function zlget. If the
/// VICAR library is installed then we just forward to that library,
/// otherwise we throw an exception saying that it isn't available.
//-----------------------------------------------------------------------

int VicarFile::zlgetsh(int unit, const char* key, char* value, int ulen)
{
#ifdef HAVE_VICAR_RTL
  int status = VicarFile::zlgetw(unit, "SYSTEM", const_cast<char*>(key), 
				 value, ulen);
  if(status ==CANNOT_FIND_KEY)
    status = VicarFile::zlgetw(unit, "HISTORY", const_cast<char*>(key), 
			       value, ulen);
  return status;
#else
  throw VicarNotAvailableException();
#endif
}


//-----------------------------------------------------------------------
/// This is a thin wrapper around the RTL function zlget. If the
/// VICAR library is installed then we just forward to that library,
/// otherwise we throw an exception saying that it isn't available.
//-----------------------------------------------------------------------

int VicarFile::zlgetw(int unit, const char* type, const char* key, 
		      char* value, const char* prop)
{
#ifdef HAVE_VICAR_RTL
  return zlget(unit, const_cast<char*>(type), const_cast<char*>(key), 
	       value, "PROPERTY", prop, NULL);
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This is a thin wrapper around the RTL function zlget. If the
/// VICAR library is installed then we just forward to that library,
/// otherwise we throw an exception saying that it isn't available.
//-----------------------------------------------------------------------

int VicarFile::zlgetw(int unit, const char* type, const char* key, 
		      char* value, int ulen)
{
#ifdef HAVE_VICAR_RTL
  return zlget(unit, const_cast<char*>(type), const_cast<char*>(key), 
	       value, "ULEN", ulen, NULL);
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This is a thin wrapper around the RTL function zlget. If the
/// VICAR library is installed then we just forward to that library,
/// otherwise we throw an exception saying that it isn't available.
//-----------------------------------------------------------------------

int VicarFile::zlgetw(int unit, const char* type, const char* key, 
		      char* value, int ulen, const char* prop)
{
#ifdef HAVE_VICAR_RTL
  return zlget(unit, const_cast<char*>(type), const_cast<char*>(key), 
	       value, "ULEN", ulen, "PROPERTY", prop, NULL);
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This is a thin wrapper around the RTL function zvzinit. If the
/// VICAR library is installed then we just forward to that library,
/// otherwise we throw an exception saying that it isn't available.
//-----------------------------------------------------------------------

int VicarFile::zvzinitw(int argc, char *argv[])
{
#ifdef HAVE_VICAR_RTL
  return zvzinit(argc, argv);
#else
  throw VicarNotAvailableException();
#endif
  
}

//-----------------------------------------------------------------------
/// Return true if vicar functionality is available, otherwise return
/// false. We have vicar functionality of the library was configured
/// to use the VICAR library.
//-----------------------------------------------------------------------

bool VicarFile::vicar_available()
{
#ifdef HAVE_VICAR_RTL
  return true;
#else
  return false;
#endif
}
