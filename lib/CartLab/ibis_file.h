#ifndef IBIS_FILE_H
#define IBIS_FILE_H
#include "vicar_file.h"
#include <boost/type_index.hpp>

namespace GeoCal {
  class IbisFile;
/****************************************************************//**
  Base class for all IBIS columns.
*******************************************************************/

class IbisColumnBase : public boost::noncopyable, 
		       public Printable<IbisColumnBase> {
public:
  virtual ~IbisColumnBase() {}
  
//-----------------------------------------------------------------------
/// Flush data to disk.
//-----------------------------------------------------------------------

  virtual void flush() {};

//-----------------------------------------------------------------------
/// Flag indicating if the column has been updated. If the file has
/// access_type of WRITE, then this defaults to true. If the file has 
/// access_type of UPDATE, then we only write data out if this has
/// been marked true. So if you modify data and want it written out,
/// you need to set this flag.
//-----------------------------------------------------------------------

  bool update;

//-----------------------------------------------------------------------
/// IbisFile that this column is for.
//-----------------------------------------------------------------------

  const IbisFile& ibis_file() const {return f;}

//-----------------------------------------------------------------------
/// Column index. This is 0-based rather than the 1-based used by some
/// VICAR routines.
//-----------------------------------------------------------------------
  
  int column_index() const {return index;}

//-----------------------------------------------------------------------
/// Size in bytes of column element. This is mostly only interesting
/// for the VICAR_ASCII types, since the other types are just the size
/// of the underlying type.
//-----------------------------------------------------------------------
  
  int size_byte() const {return size_byte_;}
  virtual void print(std::ostream& Os) const = 0;
protected:
  IbisColumnBase(IbisFile& F, int Index, int Size_byte);
  IbisFile& f;
  int index;
  int size_byte_;
};

/****************************************************************//**
  This class handles reading and writing a single column of a IBIS
  file.

  If you update a column and want the results to be written out, 
  then you need to mark the "updated" flag as true, otherwise 
  whatever changes are made won't be written to disk.
*******************************************************************/

template<class T> class IbisColumn : public IbisColumnBase {
public:
  virtual ~IbisColumn() {}
  virtual void flush();
  std::vector<T> data;
  virtual void print(std::ostream& Os) const;
private:
  // Only IbisFile can call this.
  IbisColumn(IbisFile& F, int Index, int Size_byte);
  friend class IbisFile;
};

/****************************************************************//**
  This provides read and write for a IBIS file. IBIS is VICAR format
  used for tabulating raster format data - basically it is a table.
  This is used to interact with existing VICAR routines.

  The ASCII column type is a bit of a special case. For IBIS files,
  the ASCII column type is a fixed number of characters. We access
  this a std::string type. If you attempt to write a string that is
  too large for a column, then we silently truncate the string.
  Alternatively, we could have triggered an error in that case, but
  after thinking about it truncating seemed like the better choice. We
  can revisit this decision if it becomes clear that triggering an
  error would be better.

  As an implementation detail, the class VicarFile and IbisFile need
  to coordinate in translating file names to unit numbers since the
  same underlying Vicar routine zvunit is used by both. We arbitrarily
  have VicarFile handle this, and IbisFile accesses VicarFile when it
  needs to do this.
*******************************************************************/
class IbisFile : public Printable<IbisFile> {
public:
//-----------------------------------------------------------------------
/// Type of access.
//-----------------------------------------------------------------------

  enum access_type {READ, WRITE, UPDATE};

//-----------------------------------------------------------------------
/// Type of data in file. Note that we don't currently support complex
/// we can add that if we end up needing it.
//-----------------------------------------------------------------------

  enum data_type {VICAR_BYTE, VICAR_HALF, VICAR_FULL, VICAR_FLOAT,
		  VICAR_DOUBLE, VICAR_ASCII};

  IbisFile(const std::string& Fname, access_type Access = READ);
  IbisFile(const std::string& Fname, int Number_row, 
	   const std::vector<std::string>& Format, 
	   const std::string& Organization = "COLUMN");
  virtual ~IbisFile();
  
//-----------------------------------------------------------------------
/// Access type of file.
//-----------------------------------------------------------------------

  access_type access() const {return access_;}

  void close();

//-----------------------------------------------------------------------
/// Data type for given column. Note that this is 0-based rather than
/// 1-based. 
//-----------------------------------------------------------------------

  data_type column_data_type(int I) const 
  {
    range_check(I, 0, number_col());
    return col_type[I];
  }

//-----------------------------------------------------------------------
/// Give access to the given column.
//-----------------------------------------------------------------------

  template<class T> const IbisColumn<T>& column(int Index) const
  {
    range_check(Index, 0, number_col());
    const IbisColumn<T>* res = 
      dynamic_cast<const IbisColumn<T>*>(col[Index].get());
    if(!res) {
      Exception e;
      e << "We were trying to get a IbisColumn of type "
	<< boost::typeindex::type_id<T>().pretty_name()
	<< " but the column index " << Index << " "
	<< "has type code data_type of "
	<< column_data_type(Index);
      throw e;
    }
    return *res;
  }

//-----------------------------------------------------------------------
/// Give access to the given column.
//-----------------------------------------------------------------------

  template<class T> IbisColumn<T>& column(int Index)
  {
    range_check(Index, 0, number_col());
    IbisColumn<T>* res = dynamic_cast<IbisColumn<T>*>(col[Index].get());
    if(!res) {
      Exception e;
      e << "We were trying to get a IbisColumn of type "
	<< boost::typeindex::type_id<T>().pretty_name()
	<< " but the column index " << Index << " "
	<< "has type code data_type of "
	<< column_data_type(Index);
      throw e;
    }
    return *res;
  }

//-----------------------------------------------------------------------
/// Short cut for column<T>(j).data[i].
//-----------------------------------------------------------------------

  template<class T> const T& data(int i, int j) const
  { return column<T>(j).data[i];}

//-----------------------------------------------------------------------
/// Short cut for column<T>(j).data[i].
//-----------------------------------------------------------------------

  template<class T> T& data(int i, int j) 
  { return column<T>(j).data[i];}

//-----------------------------------------------------------------------
/// File name
//-----------------------------------------------------------------------
  
  const std::string& file_name() const { return fname_; }
  void flush();
  int ibis_fh() const;

//-----------------------------------------------------------------------
/// Mark whole file as updated. No need to do this if you open the
/// file as "WRITE", but for "UPDATE" we only update columns
/// explicitly marked as updated (so we don't write out unchanged
/// column). This routine marks all the columns as updated and needing
/// to be written to disk.
//-----------------------------------------------------------------------

  void mark_updated()
  {
    for(int i = 0; i < number_col(); ++i)
      col[i]->update = true;
  }

//-----------------------------------------------------------------------
/// Mark whole file as updated. No need to do this if you open the
/// file as "WRITE", but for "UPDATE" we only update columns
/// explicitly marked as updated (so we don't write out unchanged
/// column). This routine marks the specific columns as updated and needing
/// to be written to disk.
//-----------------------------------------------------------------------

  void mark_updated(int I)
  {
    range_check(I, 0, number_col());
    col[I]->update = true;
  }

//-----------------------------------------------------------------------
/// Number of rows in the file
//-----------------------------------------------------------------------

  int number_row() const {return nrow;}

//-----------------------------------------------------------------------
/// Number of columns in the file
//-----------------------------------------------------------------------

  int number_col() const {return ncol;}

  void set_column_type(int i, data_type dt);
  
//-----------------------------------------------------------------------
/// Unit number for VicarFile
//-----------------------------------------------------------------------

  int unit() const {return unit_;}
  virtual void print(std::ostream& Os) const;

  static int IBISColumnReadw(int ibis_id, char *buffer, int column, 
			     int srow, int nrows);
  static int IBISColumnWritew(int ibis_id, char *buffer, int column, 
			      int srow, int nrows);
  static int IBISColumnSetw(int ibis_id, char *name, void *value, int column);
protected:
  IbisFile() {}
private:
  access_type access_;
  std::string fname_;
  int unit_;			///< Unit number 
  mutable int ibis_fh_;		///< Used as a file handle unit number
				///by various IBIS routines.
  int nrow;
  int ncol;
  std::vector<boost::shared_ptr<IbisColumnBase> > col;
  std::vector<data_type> col_type;
  void unit_open(const char* Format = NULL, const char* Org = "COLUMN") const;
  void read_metadata();
  void create_col();
  data_type read_column_data_type(int I) const;
  int read_column_size(int I) const;
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
};

template<class T> IbisColumn<T>::IbisColumn(IbisFile& F, int Index, 
  int Size_byte)
  : IbisColumnBase(F, Index, Size_byte),
  data(F.number_row())
{
  if(F.access() != IbisFile::WRITE) {
    int status = IbisFile::IBISColumnReadw(f.ibis_fh(), 
					   reinterpret_cast<char*>(&data[0]),
					   index + 1, 0, f.number_row());
    if(status != 1)
      throw VicarException(status, "IBISColumnRead call failed");
  }
}

//-----------------------------------------------------------------------
/// Flush data to disk.
//-----------------------------------------------------------------------

template<class T> void IbisColumn<T>::flush() 
{
  if(f.access() == IbisFile::WRITE ||
     (f.access() == IbisFile::UPDATE &&
      update)) {
    if((int) data.size() != f.number_row())
      throw Exception("data.size() is no longer f.number_row()");
    int status = IbisFile::IBISColumnWritew(f.ibis_fh(), 
					    reinterpret_cast<char*>(&data[0]), 
					    index + 1, 0, f.number_row());
    if(status != 1)
      throw VicarException(status, "IBISColumnWrite call failed");
    update = false;
  }
}

//-----------------------------------------------------------------------
/// Print to stream
//-----------------------------------------------------------------------

template<class T> void IbisColumn<T>::print(std::ostream& Os) const
{
  Os << "IBIS Column\n"
     << "  File:       " << f.file_name() << "\n"
     << "  Index:      " << index << "\n"
     << "  Number row: " << f.number_row() << "\n"
     << "  Type:       " << VicarType::vicar_type<T>() << "\n";
}

// std::string type is a special case.

template<> IbisColumn<std::string>::IbisColumn(IbisFile& F, int Index, 
					       int Size_byte);
template<> void IbisColumn<std::string>::flush();
template<> void IbisColumn<std::string>::print(std::ostream& Os) const;


}
GEOCAL_EXPORT_KEY(IbisFile);
#endif

