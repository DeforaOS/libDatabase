#!/bin/sh
#$Id$
#Copyright (c) 2014-2016 Pierre Pronchery <khorben@defora.org>
#This file is part of DeforaOS Database libDatabase
#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, version 3 of the License.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses/>.



#variables
[ -f "../config.sh" ] && . "../config.sh"
[ -n "$PREFIX" ] || PREFIX="/usr/local"
[ -n "$LIBDIR" ] || LIBDIR="$PREFIX/lib"
PKG_CONFIG_PATH="$PWD/../data:$LIBDIR/pkgconfig"
PYTHONDIR="../src/python"
#executables
CONFIGURE="configure -v"
MAKE="make"


[ -n "$OBJDIR" ] && PKG_CONFIG_PATH="${OBJDIR}../data:$LIBDIR/pkgconfig"
(cd "$PYTHONDIR" &&
	$CONFIGURE &&
	PKG_CONFIG_PATH="$PKG_CONFIG_PATH" $MAKE clean all)
