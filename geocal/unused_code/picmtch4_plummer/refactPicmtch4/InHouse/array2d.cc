/*===========================================================================
=                                                                           =
=                                  Array2d                                  =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                    MISR
                             GeoCal Processing
                                                                        
            Copyright 2006, California Institute of Technology.         
                           ALL RIGHTS RESERVED.                         
                 U.S. Government Sponsorship acknowledged.              

!C++=========================================================================

!CLASS NAME: Array2d

!ABSTRACT:

   This contains support routines for the class Array2d

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/
#ifndef ARRAY2D_CC
#define ARRAY2D_CC
#include "array2d.h"            // Definition of class

//***************************************************************************
// Definition of function Array2d::Array2d(size_type num_row,
// size_type num_col). 
//***************************************************************************

template<class T>
GeoCal::GeoCalCore::Array2d<T>::Array2d(
size_type	   Num_row,	// Number of rows in array.
size_type	   Num_col	// Number of columns in array.
)
:num_col_(Num_col), num_row_(Num_row), v_(Num_row * Num_col)
{
  range_min_check(num_row_, (size_type) 0);
  range_min_check(num_col_, (size_type) 0);
}

template<class T>
GeoCal::GeoCalCore::Array2d<T>::Array2d(
size_type	   Num_row,	// Number of rows in array.
size_type	   Num_col,	// Number of columns in array.
const T&	   C		// Value to use in initialization.
)
:num_col_(Num_col), num_row_(Num_row), v_(Num_row * Num_col, C)
{
  range_min_check(num_row_, (size_type) 0);
  range_min_check(num_col_, (size_type) 0);
}

//***************************************************************************
// Definition of function void Array2d::resize(size_type num_row, size_type
// num_col, T c = T()).
//***************************************************************************

template <class T>
void GeoCal::GeoCalCore::Array2d<T>::resize(
size_type	   Num_row,	// Number of row in array.
size_type	   Num_col,	// Number of columns in array.
const T&	   C		// Value to use in filling array.
)
{
  range_min_check(Num_row, (size_type) 0);
  range_min_check(Num_col, (size_type) 0);
  v_.resize(Num_row * Num_col, C);
  num_row_ = Num_row;
  num_col_ = Num_col;
}


template <class T>
void GeoCal::GeoCalCore::Array2d<T>::resize(
size_type	   Num_row,	// Number of row in array.
size_type	   Num_col	// Number of columns in array.
)
{
  range_min_check(Num_row, (size_type) 0);
  range_min_check(Num_col, (size_type) 0);
  v_.resize(Num_row * Num_col);
  num_row_ = Num_row;
  num_col_ = Num_col;
}

//***************************************************************************
// Definition of function void Array2d::subset(size_type Lstart, size_type
// Sstart, size_type Number_line, size_type Number_sample, Array2d<T>&
// Res) const. 
//***************************************************************************

template <class T>
void GeoCal::GeoCalCore::Array2d<T>::subset(
size_type          Lstart,      // First line number to extract.
size_type          Sstart,      // First sample to extract.
size_type          Number_line, // Number of lines to extract.
size_type          Number_sample,
                                // Number of samples to extract.
Array2d<T>&        Res          // Resulting subset.
) const
{
  range_min_check(Lstart, (size_type) 0);
  range_min_check(Sstart, (size_type) 0);
  range_min_check(Number_line, (size_type) 0);
  range_min_check(Number_sample, (size_type) 0);
  range_max_check(Lstart + Number_line, number_row() + 1);
  range_max_check(Sstart + Number_sample, number_col() + 1);
  Res.resize(Number_line, Number_sample);
  iterator i = Res.begin();
  for(size_type l = Lstart; l < Lstart + Number_line; l++) {
    const_iterator beg = begin() + (l * number_col() + Sstart);
    i = std::copy(beg, beg + Number_sample, i);
  }
}
#endif
