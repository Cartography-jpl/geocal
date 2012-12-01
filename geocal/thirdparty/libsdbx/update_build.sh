#!/bin/bash
#
#  LibRoadnav
#  $Id: update_build.sh 1836 2008-12-21 09:32:48Z virtuald $
#
#  Copyright (c) 2008 Dustin Spicuzza <dustin@virtualroadside.com>
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
#
# I'm tired of accidentally screwing up the build system. This will try
# to remedy that... this only works on the primary files of course
#
# This updates the makefiles with the list of CPP/H/ETC files in the directories, 
# and then tries to do the task of updating the VS project files with the same 
# information. It uses XML, so it was easy enough to figure out what to 
# tell sed. :) 
#

RETVAL=""

MKOUTFILE="Makefile.am"
VSMKOUTFILE=sdbx.vcproj

##################################################
# These are used to update Makefiles
##################################################

# first arg is start of line, second arg is content, third is filename
function replace_file_content {
	echo -e "Modifying\t`pwd`/$MKOUTFILE..."
	cat $3 | awk "
		BEGIN {
			X=0
		}
		{
			if ( X == 1 ) {
				if ( \$0!~/.*\\\\/ ) {
					X=0
					print \"$1 \\\\\n\" \"$2\"
				}
			} else if ( \$0~/$1/ ) {
				X=1
			} else {
				print \$0
			}
		}" > "$3.tmp" && mv "$3.tmp" "$3"
}


# first arg is list of items
function get_list {
	local ret
	local last
	local i
	
	ret=""
	last=""
	
	for i in $1; do 
		if [ "$last" != "" ]; then
			ret="$ret$last\\\\\n"
		fi
		last="\t$i"
	done
	
	RETVAL="$ret$last"
}

# first arg is a dir to prepend, second arg is a list of files
function prepend_dir {

	local i

	# then ensure they have valid paths
	for i in $2; do
		items="$items $1/$i"
	done
	
	RETVAL=$items
}

# arg is the directory to go through
function get_source_files {
	local items=""
	local subdirs=""
	local i
	local j
	
	cd $1
	
	echo -e "Parsing \t`pwd`..."
	
	# get items for directories below us
	for i in `ls`; do
		if [ -d $i ]; then
			if [ "$subdirs" == "" ]; then
				subdirs=$i
			else
				subdirs="$subdirs $i"
			fi
			
			# get the files
			get_source_files "$i" "$2../"
			
			prepend_dir $i "$RETVAL"
		fi
	done
	
	# get items for our directory
	for i in "*.cpp *.h *.xpm *.png"; do
		for j in `ls $i 2> /dev/null`; do
			items="$items $j"
		done
	done
	
	if [ "$1" != "." ]; then
		cd ..
	fi
	
	RETVAL="$items"
}


# go to the root directory
OLDDIR=`pwd`
cd `dirname $0`

# rebuild the include directory

get_source_files "sdbx" "."
prepend_dir "sdbx" "$RETVAL"
get_list "$RETVAL"
	
replace_file_content "nobase_include_HEADERS =" "$RETVAL" "Makefile.am"



##################################################
# These are used to update VS project files
##################################################

#arg is filtername, list of items, MS-styled prefix
function vs_create_filter {
	local i
	local ret
	
	# special cases to make the rest of the code shorter
	if [ "$1" = "src Headers" ]; then
		ret="\t\t<Filter Name=\"Header Files\" Filter=\"h;hpp;hxx;hm;inl\">\r\n"
		ret="$ret\t\t\t<File RelativePath=\"..\\\\config.h\"><\\/File>\r\n"
	elif [ "$1" = "src" ]; then
		ret="\t\t<Filter Name=\"Source Files\" Filter=\"cpp;c;cxx;rc;def;r;odl;idl;hpj;bat\">\r\n"
	else
		ret="\t\t<Filter Name=\"$1\">\r\n"
	fi
	
	for i in $2; do
		ret="$ret\t\t\t<File RelativePath=\"$3$i\"><\\/File>\r\n"
	done
	ret="$ret\t\t<\\/Filter>\r\n"
	RETVAL=$ret
}

# arg is name of path, 2nd arg is path relative to vs dir MS-style
function vs_scan_cpp {
	local i
	local ret
	local cppfiles 
	local hfiles
	local filter
	
	ret=""
	cppfiles=""
	hfiles=""
	filter=""
	
	cd $1
	
	# scan any directories first
	for i in `ls`; do
		if [ -d $i ]; then
			vs_scan_cpp "$i" "$2\\\\$i"
			ret="$ret$RETVAL"
		fi
	done
	
	hfiles=`ls *.h 2> /dev/null`
	if [ "$hfiles" != "" ]; then
		vs_create_filter "$1 Headers" "`ls *.h`" "$2\\\\"
		ret="$ret$RETVAL"
	fi
	
	cppfiles=`ls *.cpp 2> /dev/null`
	if [ "$cppfiles" != "" ]; then
		vs_create_filter "$1" "`ls *.cpp`" "$2\\\\"
		ret="$ret$RETVAL"
	fi

	cd ..
	
	RETVAL=$ret
}

function vs_replace_file {
	echo -e "Modifying\t`pwd`/$2..."
	# see http://www.ilfilosofo.com/blog/2008/04/26/sed-multi-line-search-and-replace/
	sed -ne "1h;1!H;\${;g;s/<Files>.*<\\/Files>/<Files>\r\n$1\t<\\/Files>\r\n/g;p;}" $2 > $2.tmp
	mv $2.tmp $2
}

if [ -d "./sdbx" ]; then

	vs_scan_cpp sdbx "..\\\\..\\\\sdbx"
	vs_string="$vs_string$RETVAL"
	
	# then sed it in 
	vs_replace_file "$vs_string" win32/MSVC8LIB/$VSMKOUTFILE
	vs_replace_file "$vs_string" win32/MSVC9LIB/$VSMKOUTFILE
fi

##################################################
# This are used to fix svn properties and such
##################################################

function fixsvn 
{
	cd $1

	for i in `ls`; do
	
		if [ -d "$i" ]; then
			fixsvn $i
		fi
	done

	for i in `ls *.cpp *.c *.h *.hpp 2> /dev/null`; do
		dos2unix $i
		svn propdel svn:executable $i
		svn propset svn:eol-style "native" $i
		svn propset svn:keywords "Id" $i
	done
	
	for i in `ls *.am *.ac *.in *.m4 2> /dev/null`; do
		dos2unix $i
		svn propdel svn:executable $i
		svn propset svn:eol-style "LF" $i
	done
	
	
	for i in `ls *.sh configure 2> /dev/null`; do
		dos2unix $i
		svn propset svn:executable ON $i
		svn propset svn:eol-style "LF" $i
		svn propset svn:keywords "Id" $i
	done
	
	for i in `ls *.vcproj 2> /dev/null`; do
		unix2dos $i
		svn propdel svn:executable
		svn propset svn:eol-style "CRLF" $i
	done
	
	cd ..
}

if [[ "$1" != "nosvn" && -d "./sdbx" ]]; then
	fixsvn .
fi

cd $OLDDIR
