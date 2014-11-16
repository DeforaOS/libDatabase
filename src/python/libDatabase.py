#!/usr/bin/env python2.7
#$Id$
#Copyright (c) 2013-2014 Pierre Pronchery <khorben@defora.org>
#This file is part of DeforaOS Database libDatabase
#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU Lesser General Public License as published by
#the Free Software Foundation, version 3 of the License.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU Lesser General Public License for more details.
#
#You should have received a copy of the GNU Lesser General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses/>.



import _libDatabase


#Database
class Database:
    def __init__(self, engine, section):
        self.database = _libDatabase.database_new(engine, section)

    def getLastId(self):
        return database.database_get_last_id(self.database)

    def query(self, query):
        return database.database_query(self.database, query)
