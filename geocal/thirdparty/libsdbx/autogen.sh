#!/bin/sh
# Run this to generate all the initial makefiles, etc.

srcdir=`dirname $0`

(aclocal && 
autoconf && 
automake --add-missing --copy && 
echo "Build update OK. Running configure") || 
	(echo "ERROR: The build files could not be updated" && exit 1)

echo Running $srcdir/configure $conf_flags "$@" ...
	$srcdir/configure $conf_flags "$@" \
	&& echo Now type \`make\' to compile $PKG_NAME
  
exit $?
