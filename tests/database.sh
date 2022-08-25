#!/bin/sh
#$Id$
#Copyright (c) 2016 Pierre Pronchery <khorben@defora.org>
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
DEBUG="_debug"
DEVNULL="/dev/null"
PROGNAME="database.sh"
#executables
RM="rm -f"
SQLITE2="sqlite"
SQLITE3="sqlite3"


#functions
#debug
_debug()
{
	echo "$@" 1>&2
	"$@"
	ret=$?
	#ignore errors when the command is not available
	[ $ret -eq 127 ]					&& return 0
	return $ret
}


#error
_error()
{
	echo "$PROGNAME: $@" 1>&2
	return 2
}


#usage
_usage()
{
	echo "Usage: $PROGNAME [-P prefix] target" 1>&2
	return 1;
}


#main
clean=0
while getopts "cP:" name; do
	case "$name" in
		c)
			clean=1
			;;
		P)
			#XXX ignored for compatibility
			;;
		?)
			_usage
			exit $?
			;;
	esac
done
shift $((OPTIND - 1))
if [ $# -ne 1 ]; then
	_usage
	exit $?
fi
target="$1"
database="${target#$OBJDIR}"

[ "$clean" -ne 0 ] && exit 0

case "$database" in
	sqlite2.db)
		$RM -- "$target"				|| exit 2
		#XXX run twice to ensure there are no false positives
		echo .read "sqlite2.sql" | $DEBUG $SQLITE2 "$target" > "$DEVNULL" 2>&1
		echo .read "sqlite2.sql" | $DEBUG $SQLITE2 "$target"
		res=$?
		if [ $res -ne 0 ]; then
			_error "$target: Error $res"
			exit $?
		fi
		;;
	sqlite3.db)
		$RM -- "$target"				|| exit 2
		#XXX run twice to ensure there are no false positives
		echo .read "sqlite3.sql" | $DEBUG $SQLITE3 "$target" > "$DEVNULL" 2>&1
		echo .read "sqlite3.sql" | $DEBUG $SQLITE3 "$target"
		res=$?
		if [ $res -ne 0 ]; then
			_error "$target: Error $res"
			exit $?
		fi
		;;
	*)
		_error "$database: Unknown database target"
		exit 2
		;;
esac
