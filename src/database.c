/* $Id$ */
/* Copyright (c) 2012-2015 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Database libDatabase */
/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */



#include <string.h>
#include <System.h>
#include "Database.h"
#include "../config.h"

#ifndef PREFIX
# define PREFIX		"/usr/local"
#endif
#ifndef LIBDIR
# define LIBDIR		PREFIX "/lib"
#endif


/* Database */
/* private */
/* types */
struct _Database
{
	Plugin * plugin;
	DatabaseEngineDefinition * dplugin;
	DatabaseEngine * database;
};


/* public */
/* functions */
/* database_new */
Database * database_new(char const * engine, Config * config,
		char const * section)
{
	Database * database;

	if((database = object_new(sizeof(*database))) == NULL)
		return NULL;
	memset(database, 0, sizeof(*database));
	if((database->plugin = plugin_new(LIBDIR, "Database", "engine",
					engine)) == NULL
			|| (database->dplugin = plugin_lookup(database->plugin,
					"database")) == NULL
			|| (database->database = database->dplugin->init(config,
					section)) == NULL)
	{
		database_delete(database);
		return NULL;
	}
	return database;
}


/* database_delete */
void database_delete(Database * database)
{
	if(database->plugin != NULL)
	{
		if(database->dplugin != NULL
				&& database->dplugin->destroy != NULL
				&& database->database != NULL)
			database->dplugin->destroy(database->database);
		plugin_delete(database->plugin);
	}
	object_delete(database);
}


/* accessors */
/* database_get_last_id */
int64_t database_get_last_id(Database * database)
{
	return database->dplugin->get_last_id(database->database);
}


/* database_prepare_new */
DatabaseStatement * database_prepare_new(Database * database,
		char const * query)
{
	return database->dplugin->prepare_new(database->database, query);
}


/* database_prepare_delete */
void database_prepare_delete(Database * database, DatabaseStatement * statement)
{
	database->dplugin->prepare_delete(database->database, statement);
}


/* database_prepare_query */
int database_prepare_query(Database * database, DatabaseStatement * statement,
		DatabaseCallback callback, void * data, ...)
{
	int ret;
	va_list ap;

	va_start(ap, data);
	ret = database->dplugin->prepare_query(database->database, statement,
			callback, data, ap);
	va_end(ap);
	return ret;
}


/* database_query */
int database_query(Database * database, char const * query,
		DatabaseCallback callback, void * data)
{
	return database->dplugin->query(database->database, query, callback,
			data);
}
