#!/bin/sh
#$Id$
#Copyright (c) 2014 Pierre Pronchery <khorben@defora.org>
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
#executables
MAKE="make"


(cd ../src/python && $MAKE clean all)