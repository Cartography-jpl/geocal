/*===========================================================================
=                                                                           =
=                                  Array2d<T>                               =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                    MISR
                             GeoCal Processing
                                                                        
            Copyright 2006, California Institute of Technology.         
                           ALL RIGHTS RESERVED.                         
                 U.S. Government Sponsorship acknowledged.              

!C++=========================================================================

!CLASS NAME: Array2d<T>

!ABSTRACT:

   This is a general purpose 2D array.

!DERIVED FROM:


!DESCRIPTION:

   This is just a simple interface to a std::vector<T>, making it appear to
   be a 2D array. The macro range_check is used to do range
   checking. Range checking is done by default, it can be turned off
   if desired. See Err/range_check.h for details.

   Array is 0 based, and ordered by row, as is the normal C
   convention (e.g., begin() can be passed to routines expected a T*
   to a 2d array).

!PUBLIC MEMBERS:

   inline Array2d() 
     - This constructs an array with 0 rows and columns.

   Array2d(size_type num_row, size_type num_col)
     - This constructs an array with the given number of rows and columns. 

   Array2d(size_type num_row, size_type num_col, const T& C)
     - This constructs an array with the given number of rows and columns. 

   inline virtual ~Array2d()
     - Destructor.

   inline const_iterator begin() const
     - This is a iterator at the beginning of the array.

   inline iterator begin()
     - This is a iterator at the beginning of the array.

   typedef const_iterator
     - Constant iterator

   typedef const_reverse_iterator
     - Constant reverse iterator.

   inline const_iterator end() const
     - This is an iterator at the end of the array.

   inline iterator end()
     - This is an iterator at the end of the array.

   typedef iterator
     - Nonconst iterator.

   inline size_type number_col() const
     - This returns the number of columns in the array.

   inline size_type number_row() const
     - Returns number of rows in array.

   inline const_reference operator()(size_type i, size_type j) const
     - Access the (i, j) element of the array.

   inline reference operator()(size_type i, size_type j)
     - Access the (i, j) element of the array.

   inline const_reverse_iterator rbegin() const
     - Return a reverse iterator at the end of the array.

   inline reverse_iterator rbegin()
     - Return a reverse iterator at the end of the array.

   inline const_reverse_iterator rend() const
     - Return reverse iterator at beginning of array.

   inline reverse_iterator rend()
     - Return reverse iterator at beginning of array.

   void resize(size_type Num_row, size_type Num_col, const T& C)
     - Change the size of the array. Note that previous content of array is
       lost.

   void resize(size_type Num_row, size_type Num_col)
     - Change the size of the array. Note that previous content of array is
       lost.

   typedef reverse_iterator
     - Nonconst reverse iterator.

   void subset(size_type Lstart, size_type Sstart, size_type
     Number_line, size_type Number_sample, Array2d<T>& Res) const
     - Return a subset of the array.

!PROTECTED MEMBERS:

!PRIVATE MEMBERS:

   size_type num_col_
     - This is the number of columns in the array. It should always be >=0. 

   size_type num_row_
     - This is the number of rows in the array. It should always be >=0.

   std::vector<T> v_
     - This is the actual data of the array. It is stored row by row (e.g.,
       standard C convention).

!ASSOCIATED FUNCTIONS:

!ERRORS:

!KNOWN BUGS AND LIMITATIONS:

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <vector>		// Definition of vector<T>.

namespace GeoCal {
  namespace GeoCalCore {
    template <class T> class Array2d;
  }
}

//***************************************************************************
// Declaration of class Array2d.
//***************************************************************************

template <class T> class GeoCal::GeoCalCore::Array2d {
public:
  typedef typename std::vector<T>::const_iterator const_iterator;
  typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;
  typedef typename std::vector<T>::iterator iterator;
  typedef typename std::vector<T>::reverse_iterator reverse_iterator;
  typedef typename std::vector<T>::value_type value_type;
  typedef typename std::vector<T>::reference reference;
  typedef typename std::vector<T>::const_reference const_reference;
  typedef typename std::vector<T>::difference_type difference_type;
  typedef typename std::vector<T>::size_type size_type;
  inline	   Array2d();
  Array2d(size_type num_row, size_type num_col); 
  Array2d(size_type num_row, size_type num_col, const T& C); 
  inline virtual   ~Array2d();
  inline const_iterator   
		   begin() const;
  inline iterator 
		   begin();
  inline const_iterator   
		   end() const;
  inline iterator  end();
  inline size_type number_col() const;
  inline size_type number_row() const;
  inline const_reference  
		   operator()(size_type i, size_type j) const;
  inline reference operator()(size_type i, size_type j);
  inline const_reverse_iterator 
		   rbegin() const;
  inline reverse_iterator 
		   rbegin();
  inline const_reverse_iterator 
		   rend() const;
  inline reverse_iterator 
		   rend();
  void		   resize(size_type num_row, size_type num_col, const
			  T& C); 
  void		   resize(size_type num_row, size_type num_col);
  void		   subset(size_type Lstart, size_type Sstart,
			  size_type Number_line, size_type
			  Number_sample, Array2d<T>& Res) const; 
protected:
private:
  size_type num_col_;
  size_type num_row_;
  std::vector<T> v_;
};

//***************************************************************************
// Include source of inline functions.
//***************************************************************************

#include "array2d.icc"

//-----------------------------------------------------------------------
// Really shouldn't need this inclusion, but sometime SGI CC gets
// confused when doing template instantiation w/o this. In any case,
// it does no harm. 
//-----------------------------------------------------------------------

#include "array2d.cc"
#endif

