#include "geocal_config.h"
#include "ibis_file.h"
#include "geocal_serialize_support.h"
#ifdef HAVE_VICAR_RTL
#include "ibisfile.h"		// Vicar P1 routines for IBIS access.
#endif
#include <boost/foreach.hpp>

// Prototype is not declared in zvproto.h
extern "C" {
int v2_deactivate_a_unit(int unit);
}

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void IbisFile::save(Archive & ar, const unsigned int version) const
{
  GEOCAL_GENERIC_BASE(IbisFile);
  ar & GEOCAL_NVP_(access) & GEOCAL_NVP_(fname);
}

template<class Archive>
void IbisFile::load(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(IbisFile);
  ar & GEOCAL_NVP_(access) & GEOCAL_NVP_(fname);
  unit_ = -1;
  unit_ = VicarFile::file_name_to_unit(fname_);
  unit_open();
  read_metadata();
  create_col();
}

GEOCAL_SPLIT_IMPLEMENT(IbisFile);
#endif

//-----------------------------------------------------------------------
/// This is a thin wrapper around the p1 function IBISColumnRead. If the
/// VICAR library is installed then we just forward to that library,
/// otherwise we throw an exception saying that it isn't available.
//-----------------------------------------------------------------------

int IbisFile::IBISColumnReadw(int ibis_id, char *buffer , int column, 
			      int srow, int nrows)
{
#ifdef HAVE_VICAR_RTL
  return IBISColumnRead(ibis_id, buffer, column, srow, nrows);
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This is a thin wrapper around the p1 function IBISColumnWrite. If the
/// VICAR library is installed then we just forward to that library,
/// otherwise we throw an exception saying that it isn't available.
//-----------------------------------------------------------------------

int IbisFile::IBISColumnWritew(int ibis_id, char *buffer, int column, 
			      int srow, int nrows)
{
#ifdef HAVE_VICAR_RTL
  return IBISColumnWrite(ibis_id, buffer, column, srow, nrows);
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This is a thin wrapper around the p1 function IBISColumnSet. If the
/// VICAR library is installed then we just forward to that library,
/// otherwise we throw an exception saying that it isn't available.
//-----------------------------------------------------------------------

int IbisFile::IBISColumnSetw(int ibis_id, char* name, void* value,
			     int column)
{
#ifdef HAVE_VICAR_RTL
  return IBISColumnSet(ibis_id, name, value, column);
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Constructor. The column index is 0 based.
//-----------------------------------------------------------------------

IbisColumnBase::IbisColumnBase(IbisFile& F, int Index, int Size_byte)
  : f(F), index(Index), size_byte_(Size_byte)
{
  if(f.access() == IbisFile::WRITE)
    update = true;
  else
    update = false;
}

//-----------------------------------------------------------------------
/// Open given file for read or update.
///
/// Note for UPDATE, we do *not* write out updated data unless you
/// mark the column or entire file as updated. This is done to prevent
/// writing out unchanged data. You can use the function
/// "mark_updated" to do this.
//-----------------------------------------------------------------------

IbisFile::IbisFile(const std::string& Fname, access_type Access)
: access_(Access),
  fname_(Fname),
  unit_(-1)
{
  unit_ = VicarFile::file_name_to_unit(Fname);
  unit_open();
  read_metadata();
  create_col();
}

//-----------------------------------------------------------------------
/// Create a new IBIS file. We pass in the number of rows and the
/// format for each column. The format is given as a list of strings,
/// with each string being one of the format types taken by the IBIS
/// library - "BYTE", "HALF", "FULL", "REAL", "DOUB", "Ann" (e.g.,
/// "A10" for 10 character ASCII). The organization of the file can be
/// given as "COLUMN" or "ROW".
//-----------------------------------------------------------------------

IbisFile::IbisFile(const std::string& Fname, int Number_row, 
		   const std::vector<std::string>& Format, 
		   const std::string& Organization)
: access_(WRITE),
  fname_(Fname),
  unit_(-1),
  nrow(Number_row)
{
#ifdef HAVE_VICAR_RTL
  ncol = (int) Format.size();

  // Massage Format to put it into the form expected by IBISFileOpen.
  std::vector<char> fmt(IFMT_SIZE * ncol, '\0');
  for(int i = 0; i < ncol; ++i) {
    if(Format[i].size() + 1 > IFMT_SIZE)
      throw Exception("Format string is too long");
    std::copy(Format[i].begin(), Format[i].end(), fmt.begin() + IFMT_SIZE * i);
  }

  unit_ = VicarFile::file_name_to_unit(Fname);
  unit_open(&fmt[0], Organization.c_str());
  read_metadata();
  create_col();
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Close a file. You don't normally need to call this directly, it
/// is done by the destructor. But it is useful to have for use by python.
//-----------------------------------------------------------------------

void IbisFile::close() 
{
#ifdef HAVE_VICAR_RTL
  flush();
  if(ibis_fh_ != -1) {
    int status = IBISFileClose(ibis_fh_, NULL);
    if(status != 1) {
      VicarException e(status);
      e << "Call to IBISFileClose failed for unit " << unit() << " file name "
	<< file_name();
      throw e;
    }
  }
  ibis_fh_ = -1;
  if(unit_ != -1)
    v2_deactivate_a_unit(unit_); // Can't do this through IBIS close
				// like we can for a vicar file.
  unit_ = -1;
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Read the column data type. Note that the column number is the
/// standard C 0-based, not the 1-based numbering used by some of the
/// VICAR routines.
//-----------------------------------------------------------------------

IbisFile::data_type IbisFile::read_column_data_type(int I) const
{
#ifdef HAVE_VICAR_RTL
  range_check(I, 0, number_col());
  char tname[I2_MAX_TYPE_NAME];
  // +1 is because this is 1-based rather than 0-based.
  int status = IBISColumnGet(ibis_fh(), const_cast<char*>(ICOLUMN_FORMAT), 
			     tname, I + 1);
  if(status != 1)
    throw VicarException(status, "Call to IBISColumnGet failed for file " + 
			 fname_);
  if(tname == VicarType::vicar_type<VicarByte>())
    return VICAR_BYTE;
  if(tname == VicarType::vicar_type<VicarHalf>())
    return VICAR_HALF;
  if(tname == VicarType::vicar_type<VicarFull>())
    return VICAR_FULL;
  if(tname == VicarType::vicar_type<VicarFloat>())
    return VICAR_FLOAT;
  if(tname == VicarType::vicar_type<VicarDouble>())
    return VICAR_DOUBLE;
  if(tname[0] == 'A')
    return VICAR_ASCII;
  VicarException e(status);
  e << "Call to IBISColumnGet returned an unrecognized type '" << tname 
    << "' for file " << fname_ << " column " << I;
  throw e;
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Read the column size.
//-----------------------------------------------------------------------

int IbisFile::read_column_size(int I) const
{
#ifdef HAVE_VICAR_RTL
  range_check(I, 0, number_col());
  int res;
  // +1 is because this is 1-based rather than 0-based.
  int status = IBISColumnGet(ibis_fh(), const_cast<char*>(ICOLUMN_SIZE), 
			     (char*) &res, I + 1);
  if(status != 1)
    throw VicarException(status, "Call to IBISColumnGet failed for file " + 
			 fname_);
  return res;
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

IbisFile::~IbisFile()
{
  close();
}

//-----------------------------------------------------------------------
/// Open a file once the unit number has been assigned.
//-----------------------------------------------------------------------

void IbisFile::unit_open(const char* Format, const char* Org) const
{
#ifdef HAVE_VICAR_RTL
  const char *a = (access_ == READ ? IMODE_READ :
		   (access_ == UPDATE ? IMODE_UPDATE : IMODE_WRITE));
  int status = IBISFileOpen(unit_, &ibis_fh_, const_cast<char*>(a), ncol, nrow, 
			    const_cast<char*>(Format), const_cast<char*>(Org));
  if(status != 1)
    throw VicarException(status, "Call to IBISFileOpen failed for file " + 
			 fname_);
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Read the metadata in the file once it has been opened.
//-----------------------------------------------------------------------

void IbisFile::read_metadata()
{
#ifdef HAVE_VICAR_RTL
  int status = IBISFileGet(ibis_fh(), const_cast<char*>(IFILE_NR), &nrow, 
			   1, 1, 0);
  if(status != 1)
    throw VicarException(status, "Call to IBISFileGet failed for file " + 
			 fname_);
  status = IBISFileGet(ibis_fh(), const_cast<char*>(IFILE_NC), &ncol, 1, 1, 0);
  if(status != 1)
    throw VicarException(status, "Call to IBISFileGet failed for file " + 
			 fname_);
#else
  throw VicarNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// Fill in col with IbisColumn data.
//-----------------------------------------------------------------------

void IbisFile::create_col()
{
  col.resize(number_col());
  col_type.resize(number_col());
  for(int i = 0; i < number_col(); ++i) {
    col_type[i] = read_column_data_type(i);
    int bs = read_column_size(i);
    switch(col_type[i]) {
    case VICAR_BYTE:
      col[i].reset(new IbisColumn<VicarByte>(*this, i, bs));
      break;
    case VICAR_HALF:
      col[i].reset(new IbisColumn<VicarHalf>(*this, i, bs));
      break;
    case VICAR_FULL:
      col[i].reset(new IbisColumn<VicarFull>(*this, i, bs));
      break;
    case VICAR_FLOAT:
      col[i].reset(new IbisColumn<VicarFloat>(*this, i, bs));
      break;
    case VICAR_DOUBLE:
      col[i].reset(new IbisColumn<VicarDouble>(*this, i, bs));
      break;
    case VICAR_ASCII:
      col[i].reset(new IbisColumn<std::string>(*this, i, bs));
      break;
    default:
      throw Exception("This shouldn't be able to happen");
    }
  }
}

//-----------------------------------------------------------------------
/// IBIS file handle number used in IBIS calls
//-----------------------------------------------------------------------

int IbisFile::ibis_fh() const 
{
  if(ibis_fh_ ==-1)
    unit_open();
  return ibis_fh_; 
}

//-----------------------------------------------------------------------
/// Flush data to disk, including any changes. This is automatically
/// called by the destructor.
//-----------------------------------------------------------------------

void IbisFile::flush()
{
#ifdef HAVE_VICAR_RTL
  if(access() == READ)
    return;			// Nothing to do
  typedef boost::shared_ptr<IbisColumnBase> c_ref;
  BOOST_FOREACH(c_ref& i, col) {
    i->flush();
  }
  if(ibis_fh_ != -1) {
    int status = IBISFileClose(ibis_fh_, NULL);
    if(status != 1) {
      VicarException e(status);
      e << "Call to IBISFileClose failed for unit " << unit() << " file name "
	<< file_name();
      throw e;
    }
  }

  // Even if we opened this with WRITE, after flushing we are updating
  // an existing file.
  
  access_ = UPDATE;
  ibis_fh_ = -1;
#else
  throw VicarNotAvailableException();
#endif
}


//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

void IbisFile::print(std::ostream& Os) const
{
  Os << "IBIS File\n"
     << "  File:       " << file_name() << "\n"
     << "  Number row: " << number_row() << "\n"
     << "  Number col: " << number_col() << "\n";
}

//-----------------------------------------------------------------------
// Special cases for std::string columns
//-----------------------------------------------------------------------

template<> IbisColumn<std::string>::IbisColumn(IbisFile& F, int Index, 
					       int Size_byte)
: IbisColumnBase(F, Index, Size_byte),
  data(F.number_row())
{
#ifdef HAVE_VICAR_RTL
  if(F.access() != IbisFile::WRITE) {
    std::vector<char> d(f.number_row() * size_byte(), '\0');
    int status = IBISColumnRead(f.ibis_fh(), &d[0],
				index + 1, 0, f.number_row());
    for(int i = 0; i < f.number_row(); ++i) {
      std::string s(d.begin() + i * size_byte(), 
		    d.begin() + (i + 1) * size_byte());
      data[i] = s.substr(0, s.find_first_of('\0'));
    }
    if(status != 1)
      throw VicarException(status, "IBISColumnRead call failed");
  }
#else
  throw VicarNotAvailableException();
#endif
}

template<> void IbisColumn<std::string>::flush()
{
#ifdef HAVE_VICAR_RTL
  if(f.access() == IbisFile::WRITE ||
     (f.access() == IbisFile::UPDATE &&
      update)) {
    if((int) data.size() != f.number_row())
      throw Exception("data.size() is no longer f.number_row()");
    std::vector<char> d(f.number_row() * size_byte(), '\0');
    for(int i = 0; i < f.number_row(); ++i) {
      // -1 To allow space for trailing '\0'. I'm not sure if this is
      // required by IBIS or not, but it seems safest to do.
      std::string trunc = data[i].substr(0, size_byte() - 1);
      std::copy(trunc.begin(), trunc.end(), d.begin() + size_byte() * i);
    }
    int status = IBISColumnWrite(f.ibis_fh(), &d[0], 
				 index + 1, 0, f.number_row());
    if(status != 1)
      throw VicarException(status, "IBISColumnWrite call failed");
    update = false;
  }
#else
  throw VicarNotAvailableException();
#endif
}

template<> void IbisColumn<std::string>::print(std::ostream& Os) const
{
  Os << "IBIS Column\n"
     << "  File:       " << f.file_name() << "\n"
     << "  Index:      " << index << "\n"
     << "  Number row: " << f.number_row() << "\n"
     << "  Type:       " << "A" << size_byte() << "\n";
}

//-----------------------------------------------------------------------
/// IBIS has support for  casting a column type to and from a
/// different type for reading and writing. This changes whatever the
/// native type is to the given type. Note that this is fixed. Also
/// anything done with the previous type column is lost, so you should
/// call this before changing values in an IBIS column.
//-----------------------------------------------------------------------

void IbisFile::set_column_type(int i, data_type dt)
{
  std::string val;
  switch(dt) {
  case VICAR_BYTE:
    val = "BYTE";
    break;
  case VICAR_HALF:
    val = "HALF";
    break;
  case VICAR_FULL:
    val = "FULL";
    break;
  case VICAR_FLOAT:
    val = "REAL";
    break;
  case VICAR_DOUBLE:
    val = "DOUB";
    break;
  case VICAR_ASCII:
    throw Exception("We don't support changing the type to ASCII");
    break;
  default:
    throw Exception("This shouldn't be able to happen");
  }
  
  int status = IbisFile::IBISColumnSetw(ibis_fh(), "U_FORMAT",
					const_cast<char*>(val.c_str()),
					i + 1);
  if(status != 1)
    throw VicarException(status, "IBISColumnSet call failed");
  switch(dt) {
  case VICAR_BYTE:
    col[i].reset(new IbisColumn<VicarByte>(*this, i, 1));
    break;
  case VICAR_HALF:
    col[i].reset(new IbisColumn<VicarHalf>(*this, i, 2));
    break;
  case VICAR_FULL:
    col[i].reset(new IbisColumn<VicarFull>(*this, i, 4));
    break;
  case VICAR_FLOAT:
    col[i].reset(new IbisColumn<VicarFloat>(*this, i, 4));
    break;
  case VICAR_DOUBLE:
    col[i].reset(new IbisColumn<VicarDouble>(*this, i, 8));
    break;
  case VICAR_ASCII:
    throw Exception("We don't support changing the type to ASCII");
    break;
  default:
    throw Exception("This shouldn't be able to happen");
  }
}
