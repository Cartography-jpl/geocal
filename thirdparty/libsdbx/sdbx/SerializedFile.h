/*
 *  SDBX
 *	$Id: SerializedFile.h 1841 2009-01-04 18:38:58Z virtuald $
 *
 *  Copyright (c) 2008 - 2009 Dustin Spicuzza <dustin@virtualroadside.com>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
#ifndef SERIALIZEDFILE_H
#define SERIALIZEDFILE_H
 
#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/ffile.h>


#include "Serialization.h"

#include "support/UniqueId.h"
#include "support/utility.h"

#ifdef USE_MSVC_MEMORY_TRACING
	// this enables advanced memory leak detection
	#include <wx/msw/msvcrt.h>
#endif

#define SET_BAD_ERROR( code )	\
	m_lastError = code;				\
	assert(0 && #code)

 
/**
	\class SerializedFile
	\brief Loads/Saves arrays of fixed-size objects to/from a file
	
	@note Notice that it says 'fixed-size' objects. This is not designed
	to do variable-length serialization.
	
	The object being stored in the external vector can be anything, but must 
	have the following members:
	
	struct T {
		explicit T(const unsigned char * const buffer);
		void Serialize(unsigned char * buffer);
		enum { record_size };
	};
	
	This allows optimized serialization of the items being stored, especially
	if the compiler inlines the operation. It is important to keep the relative
	ordering of the class variables in mind when you implement serialization
	constructors (otherwise GCC will complain, among other things). An example 
	implementation looks like this:
	
	struct SerializedInt {
	
		int item1;
		int item2;
	
		/// serialization constructor
		explicit SerializedInt(const unsigned char * const buffer) :
			DESERIALIZE(int, 0, item1),
			DESERIALIZE(int, sizeof(int), item2)
		{}
		
		/// serializer function
		void Serialize(unsigned char * buffer) const
		{
			SERIALIZE(int, 0, item1);
			SERIALIZE(int, sizeof(int), item2);
		}
		
		/// serialization typedef
		enum { record_size = sizeof(int)*2 };
	};
	
	There are also generic Serialization routines that you can use if you
	are serializing containers of T. See Serialization.h for more info.

	At the moment, this may only load/save 2^32 objects at once. However,
	it uses 64-bit object IDs. 
	
	@tparam start_offset	This is the offset that ID 0 begins at. This
							is so you can have data before the serialized
							items. If you use this, you *must* fill this
							(and flush the file) before serializing any 
							items!
	
*/
template <typename T, wxFileOffset start_offset = 0 >
class SerializedFile {
public:

	// typedefs
	typedef uint64_t size_type;
	typedef size_type IdType;

	// error codes
	enum SerializedFileError {
		ERR_NONE = 0,	//! no error
		ERR_BAD_SEEK,	//! error while seeking
		ERR_BAD_WRITE,	//! error while writing
		ERR_BAD_READ	//! error while reading	
	};
	
	
	/**
		\brief Opens the file
		
		@param filename		Filename to open
		@param buffer_size	Size (in records) of the buffer (defaults to 4096)
		
	*/
	SerializedFile(const wxFileName &filename, size_t buffer_size = 4096) :
		m_filename(filename),
		m_buffer_valid(false), m_dirty(false), m_lastError(ERR_NONE),
		m_buffer_size(record<T>::size * buffer_size)
	{
		// ensure that this is as big as expected
		wxASSERT_MSG(sizeof(wxFileOffset) >= 8, wxT("Your compiler does not support 64-bit offsets"));
		
		// create a new file if it doesn't exist, otherwise open existing file
		if (!wxFileExists(filename.GetFullPath()))
			m_file.Open(filename.GetFullPath(), wxT("wb+"));
		else
			m_file.Open(filename.GetFullPath(), wxT("rb+"));

		m_buffer = new unsigned char[m_buffer_size];
		m_buffer_end = &m_buffer[m_buffer_size];
		memset(m_buffer, 0, m_buffer_size);
		
		assert(m_buffer);
	}
	
	~SerializedFile()
	{
		delete [] m_buffer;
		m_file.Close();
	}
	
	//! deletes the file this is associated with
	bool Delete() 
	{
		m_file.Close();
		return wxRemoveFile(m_filename.GetFullPath());
	}
	
	//! Check to see if the file is initialized properly
	bool IsOpened() const
	{
		return m_file.IsOpened();
	}
	

	
	//! Returns the file handle underneath this class
	wxFFile* GetFFile()
	{
		return &m_file;
	}
	
	//! Returns the most recent error code
	SerializedFileError GetErrorCode() const 
	{ 
		return m_lastError; 
	}
	
	wxString GetErrorString() const
	{
		switch (m_lastError) {
			case ERR_BAD_SEEK:
				return _("Error while seeking");
			case ERR_BAD_WRITE:
				return _("Error while writing");
			case ERR_BAD_READ:
				return _("Error while reading");
			default:
				break;
		}
		return wxT("");
	}
	
	/**
		\brief Gets the last ID contained in the file
		
		@warning Do not call this often, the value isn't cached and 
		the Length() operation can take awhile.. 
	*/
	IdType GetLastId() const
	{
		assert(m_file.IsOpened());
		wxFileOffset len = m_file.Length();
		
		if (len < start_offset)
			return 0;
		
		return (len - start_offset) / record<T>::size;
	}
	
	
	void SetBufferSize(const size_t buffer_size)
	{
		size_t new_size = record<T>::size * buffer_size;
	
		// assume that the copy isn't worth it
		if (new_size != m_buffer_size)
		{
			m_buffer_size = new_size;
			
			delete [] m_buffer;
			m_buffer = new unsigned char[new_size];
			m_buffer_end = &m_buffer[new_size];
			memset(m_buffer, 0, m_buffer_size);
			m_buffer_valid = false;
		}
	}
	
	/**
		\brief Load objects from the file into an array
		
		@param start_id 	starting id of the object array
		@param count		number of items to load/initialize
		@return 			number of items load
	*/
	size_type LoadObjects(const IdType &start_id, const size_t count, T * dst)
	{
		size_t i = 0;
		wxFileOffset bytes_read;
		unsigned char * buf_ptr;

		IdType first_read_id = start_id;
		
		assert(m_file.IsOpened());
		assert(dst);

		if (m_dirty)
		{
			m_file.Flush();
			m_dirty = false;
		}
	
		// first, see if the start_id is in the current page
		// TODO: It could be faster if we tried to detect if the range
		// fell *within* our range, and load it that way..
		if (m_buffer_valid && m_buf_start_id <= start_id && m_buf_end_id > start_id)
		{
			// reset this: this is safe, since we checked the bounds
			buf_ptr = &m_buffer[(start_id - m_buf_start_id) * record<T>::size];

			// copy out the current page
			do
			{
				dst[i++] = T(record<T>::deserialize(buf_ptr));
			}
			while ((buf_ptr += record<T>::size) < m_buffer_end && i < count);

			first_read_id += i;
		}

		// always seek -- its not guaranteed that the file pointer
		// will be where we want it. worst case, extra function call
		if (i < count)
		{		
			// this shouldn't happen, but if it does...
			if (!m_file.Seek(start_offset + first_read_id * record<T>::size))
			{
				SET_BAD_ERROR( ERR_BAD_SEEK );
				return 0;
			}
				
			m_buf_end_id = first_read_id;
		}
		
		m_lastError = ERR_NONE;
		
		while (i < count)
		{
			m_buf_start_id = m_buf_end_id;
		
			// read the items page by page
			bytes_read = m_file.Read(m_buffer, m_buffer_size);

			// ensure that it actually read something
			if (bytes_read == wxInvalidOffset || !bytes_read)
			{
				SET_BAD_ERROR( ERR_BAD_READ );
				break;
			}

			// calculate the end id
			m_buf_end_id = m_buf_start_id +  bytes_read / record<T>::size;
			
			// reset this
			buf_ptr = m_buffer;
		
			// copy out the current page
			do
			{
				dst[i++] = T(record<T>::deserialize(buf_ptr));
			}
			while ((buf_ptr += record<T>::size) < m_buffer_end && i < count);
		}
		
		// let it be known that this is indeed valid
		m_buffer_valid = true;
		
		return i;
	}
	
	
	/**
		\brief Writes a specified number of objects from an array to file
		
		@note Be sure to check that the specified number of objects was saved.
		Use GetErrorCode() if the numbers do not match
		
		@param start_id 	starting id of the object array
		@param count		number of items to save
		@return 			number of items saved
	*/
	size_t SaveObjects(const IdType &start_id, const size_t count, const T * const src)
	{
		size_type i = 0;
		unsigned char * buf_ptr;
		
		size_t to_write;
		
		assert(m_file.IsOpened());
		assert(src);
	
		// this shouldn't happen, but if it does...
		if (!m_file.Seek(start_offset + start_id * record<T>::size))
		{
			SET_BAD_ERROR( ERR_BAD_SEEK );
			return 0;
		}
		
		// partial validation of the buffer
		m_lastError = ERR_NONE;
		m_buffer_valid = true;
		m_buf_end_id = start_id;
		
		do
		{
			// set these up
			m_buf_start_id = m_buf_end_id;
			buf_ptr = m_buffer;
		
			// setup the current page
			do
			{
				record<T>::serialize(src[i++], buf_ptr);
			}
			while ((buf_ptr += record<T>::size) < m_buffer_end && i < count);
			
			// calculate write size
			to_write = (size_t)(buf_ptr - m_buffer);
			
			// adjust end id
			m_buf_end_id += to_write / record<T>::size;
			
			// write the buffer
			if (m_file.Write(m_buffer, to_write) != to_write)
			{
				// adjust the count and exit -- invalidate the buffer
				i -= to_write/record<T>::size;
				m_buffer_valid = false;
				SET_BAD_ERROR( ERR_BAD_WRITE );
				break;
			}
		}
		while (i < count);
		
		// set the dirty flag
		m_dirty = true;

		return i;
	}

private:
	SerializedFile();
	DISALLOW_COPY_AND_ASSIGN(SerializedFile);

	wxFFile m_file;
	wxFileName m_filename;
	
	// disk buffer stuff
	bool m_buffer_valid;
	bool m_dirty;
	
	// error code
	SerializedFileError m_lastError;
	
	IdType m_buf_start_id;
	IdType m_buf_end_id;
	
	unsigned char * m_buffer;
	unsigned char * m_buffer_end;
	size_t m_buffer_size;	
};

#undef SET_BAD_ERROR

#endif
 