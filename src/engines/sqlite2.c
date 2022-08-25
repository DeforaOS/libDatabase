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



#include <stdarg.h>
#include <stdlib.h>
#ifdef DEBUG
# include <stdio.h>
#endif
#include <string.h>
#include <sqlite.h>
#include <System.h>
#include "Database/engine.h"


/* SQLite2 */
/* private */
/* types */
typedef struct _DatabaseEngine
{
	sqlite * handle;
} SQLite2;

typedef struct _DatabaseEngineStatement
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

static SQLite2Statement * _sqlite2_statement_new(SQLite2 * pgsql,
		char const * query);
static void _sqlite2_statement_delete(SQLite2 * pgsql, SQLite2Statement * statement);
static int _sqlite2_statement_query(SQLite2 * sqlite,
		SQLite2Statement * statement, DatabaseCallback callback,
		void * data, va_list args);


/* public */
/* variables */
DatabaseEngineDefinition database =
{
	"SQLite2",
	NULL,
	_sqlite2_init,
	_sqlite2_destroy,
	_sqlite2_get_last_id,
	_sqlite2_query,
	_sqlite2_statement_new,
	_sqlite2_statement_delete,
	_sqlite2_statement_query
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
/* _sqlite2_statement_new */
static SQLite2Statement * _sqlite2_statement_new(SQLite2 * sqlite,
		char const * query)
{
	SQLite2Statement * statement;

	if((statement = object_new(sizeof(*statement))) == NULL)
		return NULL;
	/* XXX this version of SQLite2 doesn't support prepared statements */
	statement->query = string_new(query);
	return statement;
}


/* _sqlite2_statement_delete */
static void _sqlite2_statement_delete(SQLite2 * sqlite, SQLite2Statement * statement)
{
	string_delete(statement->query);
	object_delete(statement);
}


/* _sqlite2_statement_query */
static int _sqlite2_statement_query(SQLite2 * sqlite,
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
