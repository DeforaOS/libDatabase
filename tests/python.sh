#!/bin/sh
#$Id$
#Copyright (c) 2014-2016 Pierre Pronchery <khorben@defora.org>
#This file is part of DeforaOS Database libDatabase
#All rights reserved.
#
#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions are
#met:
#
#1. Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#
#2. Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
#IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
#TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
#PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
#TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */



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
