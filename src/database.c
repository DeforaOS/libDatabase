/* $Id$ */
/* Copyright (c) 2012-2019 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Database libDatabase */
/* All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */



#include <string.h>
#include <System.h>
#include "Database/database.h"
#include "Database/engine.h"
#include "database.h"
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


/* useful */
/* database_query */
int database_query(Database * database, char const * query,
		DatabaseCallback callback, void * data)
{
	return database->dplugin->query(database->database, query, callback,
			data);
}


/* protected */
/* functions */
/* database_engine_statement_new */
DatabaseEngineStatement * database_engine_statement_new(Database * database,
		char const * query)
{
	return database->dplugin->statement_new(database->database, query);
}


/* database_engine_statement_delete */
void database_engine_statement_delete(Database * database,
		DatabaseEngineStatement * statement)
{
	database->dplugin->statement_delete(database->database, statement);
}


/* useful */
/* database_engine_statement_query */
int database_engine_statement_query(Database * database,
		DatabaseEngineStatement * statement,
		DatabaseCallback callback, void * data, va_list args)
{
	return database->dplugin->statement_query(database->database, statement,
			callback, data, args);
}


