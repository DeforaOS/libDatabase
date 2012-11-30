/* $Id$ */
/* Copyright (c) 2012 Pierre Pronchery <khorben@defora.org> */
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



#include <stdarg.h>
#include <stdlib.h>
#ifdef DEBUG
# include <stdio.h>
#endif
#include <string.h>
#include <sqlite.h>
#include <System.h>
#include "database.h"


/* SQLite2 */
/* private */
/* types */
typedef struct _DatabasePlugin
{
	sqlite * handle;
} SQLite2;

typedef struct _DatabaseStatement
{
	char * query;
} SQLite2Statement;


/* protected */
/* prototypes */
/* plug-in */
static SQLite2 * _sqlite2_init(Config * config, char const * section);
static void _sqlite2_destroy(SQLite2 * pgsql);

static int64_t _sqlite2_get_last_id(SQLite2 * pgsql);

static int _sqlite2_query(SQLite2 * pgsql, char const * query,
		DatabaseCallback callback, void * data);

static SQLite2Statement * _sqlite2_prepare_new(SQLite2 * pgsql,
		char const * query);
static void _sqlite2_prepare_delete(SQLite2 * pgsql, SQLite2Statement * statement);
static int _sqlite2_prepare_query(SQLite2 * sqlite,
		SQLite2Statement * statement, DatabaseCallback callback,
		void * data, va_list args);


/* public */
/* variables */
DatabasePluginDefinition database =
{
	"SQLite2",
	NULL,
	_sqlite2_init,
	_sqlite2_destroy,
	_sqlite2_get_last_id,
	_sqlite2_query,
	_sqlite2_prepare_new,
	_sqlite2_prepare_delete,
	_sqlite2_prepare_query
};


/* private */
/* functions */
/* _sqlite2_init */
static SQLite2 * _sqlite2_init(Config * config, char const * section)
{
	SQLite2 * sqlite;
	char const * name;
	char * error = NULL;

	if((sqlite = object_new(sizeof(*sqlite))) == NULL)
		return NULL;
	sqlite->handle = NULL;
	if((name = config_get(config, section, "filename")) != NULL
			&& (sqlite->handle = sqlite_open(name, 0, &error))
			== NULL)
	{
		error_set_code(1, "%s: %s", name, (error != NULL) ? error
				: "Unknown error");
		free(error);
	}
	/* check for errors */
	if(sqlite->handle == NULL)
	{
		_sqlite2_destroy(sqlite);
		return NULL;
	}
	return sqlite;
}


/* _sqlite2_destroy */
static void _sqlite2_destroy(SQLite2 * sqlite)
{
	if(sqlite->handle != NULL)
		sqlite_close(sqlite->handle);
	object_delete(sqlite);
}


/* accessors */
/* _sqlite2_get_last_id */
static int64_t _sqlite2_get_last_id(SQLite2 * sqlite)
{
	/* FIXME really implement */
	return -1;
}


/* useful */
/* _sqlite2_prepare_new */
static SQLite2Statement * _sqlite2_prepare_new(SQLite2 * sqlite,
		char const * query)
{
	SQLite2Statement * statement;

	if((statement = object_new(sizeof(*statement))) == NULL)
		return NULL;
	/* XXX this version of SQLite2 doesn't support prepared statements */
	statement->query = string_new(query);
	return statement;
}


/* _sqlite2_prepare_delete */
static void _sqlite2_prepare_delete(SQLite2 * sqlite, SQLite2Statement * statement)
{
	string_delete(statement->query);
	object_delete(statement);
}


/* _sqlite2_prepare_query */
static int _sqlite2_prepare_query(SQLite2 * sqlite,
		SQLite2Statement * statement, DatabaseCallback callback,
		void * data, va_list args)
{
	int type = -1;
	char const * name;
	char const * s;
	char * query = NULL;
	sqlite_vm * vm;
	const char * tail = NULL;
	char * error = NULL;

	/* FIXME really implement */
	while((type = va_arg(args, int)) != -1)
	{
		name = va_arg(args, char const *);
		switch(type)
		{
			case DT_VARCHAR:
				s = va_arg(args, char const *);
				/* FIXME implement */
				break;
		}
	}
	query = statement->query; /* XXX remove once really implemented */
	if(sqlite_compile(sqlite->handle, query, &tail, &vm, &error)
			!= SQLITE_OK)
	{
		error_set_code(1, "%s", error);
		free(error);
		return -1;
	}
	/* ignore errors */
	if(sqlite_finalize(vm, &error) != SQLITE_OK)
		free(error);
	return 0;
}


/* _sqlite2_query */
static int _sqlite2_query(SQLite2 * sqlite, char const * query,
		DatabaseCallback callback, void * data)
{
	int ret;
	char * error = NULL;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, query);
#endif
	ret = (sqlite_exec(sqlite->handle, query, callback, data, &error)
			== SQLITE_OK) ? 0 : -error_set_code(1, "%s",
				(error != NULL) ? error : "Unknown error");
	if(error != NULL)
		free(error);
	return ret;
}
