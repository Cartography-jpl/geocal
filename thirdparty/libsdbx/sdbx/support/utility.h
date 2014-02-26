/*
 *  SDBX
 *  Utility.h
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

#ifndef SDBX_SUPPORT_UTILITY_H
#define SDBX_SUPPORT_UTILITY_H


// from the google C++ Style Guide:
// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&);               \
	void operator=(const TypeName&)
#endif
  
#ifndef NO_DEFAULT_CONSTRUCTOR
#define NO_DEFAULT_CONSTRUCTOR(TypeName) \
	TypeName()
#endif
  
#ifndef DELETE_IF_NOT_NULL
#define DELETE_IF_NOT_NULL(var) if (var) delete var; var = NULL
#endif

#ifndef WARN_IF_UNUSED
#ifdef __GNUC__
	// this is pretty neat -- it warns you if you don't
	// use the return value of a function. GCC only
	#define WARN_IF_UNUSED __attribute__ ((warn_unused_result))
#else
	#define WARN_IF_UNUSED
#endif
#endif

#endif


