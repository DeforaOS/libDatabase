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
#include <System.h>
#include "Database/engine.h"
#include "../../config.h"

/* constants */
#ifndef PREFIX
# define PREFIX		"/usr/local"
#endif
#ifndef LIBDIR
# define LIBDIR		PREFIX "/lib"
#endif


/* PDO */
/* private */
/* types */
typedef struct _DatabaseEngine
{
	Plugin * plugin;
	DatabaseEngineDefinition * dplugin;
	DatabaseEngine * database;
} PDO;

typedef struct _DatabaseEngineStatement
{
	DatabaseEngineStatement * statement;
} PDOStatement;


/* protected */
/* prototypes */
/* plug-in */
static PDO * _pdo_init(Config * config, char const * section);
static void _pdo_destroy(PDO * pdo);

static int64_t _pdo_get_last_id(PDO * pdo);

static int _pdo_query(PDO * pdo, char const * query, DatabaseCallback callback,
		void * data);

static PDOStatement * _pdo_statement_new(PDO * pdo, char const * query);
static void _pdo_statement_delete(PDO * pdo, PDOStatement * statement);
static int _pdo_statement_query(PDO * pdo, PDOStatement * statement,
		DatabaseCallback callback, void * data, va_list args);


/* public */
/* variables */
DatabaseEngineDefinition database =
{
	"PDO",
	NULL,
	_pdo_init,
	_pdo_destroy,
	_pdo_get_last_id,
	_pdo_query,
	_pdo_statement_new,
	_pdo_statement_delete,
	_pdo_statement_query
};


/* private */
/* functions */
/* pdo_init */
static char const * _init_pgsql(char const * dsn, Config * config,
		char const ** section);
static char const * _init_sqlite3(char const * dsn, Config * config,
		char const ** section);

static PDO * _pdo_init(Config * config, char const * section)
{
	PDO * pdo;
	char const * dsn;
	char const pgsql[] = "pgsql:";
	char const sqlite3[] = "sqlite:";
	char const * backend = NULL;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, section);
#endif
	if((dsn = config_get(config, section, "dsn")) == NULL
			|| (config = config_new()) == NULL)
		return NULL;
	/* FIXME implement more backends */
	if(strncmp(dsn, sqlite3, sizeof(sqlite3) - 1) == 0)
		backend = _init_sqlite3(&dsn[sizeof(sqlite3) - 1], config,
				&section);
	else if(strncmp(dsn, pgsql, sizeof(pgsql) - 1) == 0)
		backend = _init_pgsql(&dsn[sizeof(pgsql) - 1], config,
				&section);
	else
		/* XXX report error */
		return NULL;
	if((pdo = object_new(sizeof(*pdo))) == NULL)
	{
		config_delete(config);
		return NULL;
	}
	pdo->database = NULL;
	if((pdo->plugin = plugin_new(LIBDIR, "Database", "engine", backend))
			== NULL
			|| (pdo->dplugin = plugin_lookup(pdo->plugin,
					"database")) == NULL
			|| (pdo->database = pdo->dplugin->init(config, section))
			== NULL)
	{
		config_delete(config);
		_pdo_destroy(pdo);
		return NULL;
	}
	config_delete(config);
	return pdo;
}

static char const * _init_pgsql(char const * dsn, Config * config,
		char const ** section)
{
	char * p;
	char const * name;
	char * r;
	char const * value;

	if((p = strdup(dsn)) == NULL)
		return NULL;
	*section = "database::pgsql";
	/* parse the DSN */
	for(name = p, r = p; *r != '\0'; name = r)
	{
		if((r = strchr(name, '=')) == NULL)
			break; /* XXX detect incomplete parsing */
		*r = '\0';
		value = ++r;
		if((r = strchr(value, ';')) != NULL)
			*(r++) = '\0';
		else
			r = strchr(value, '\0');
#ifdef DEBUG
		fprintf(stderr, "DEBUG: %s=%s\n", name, value);
#endif
		if(strcmp(name, "user") == 0)
			name = "username";
		else if(strcmp(name, "dbname") == 0)
			name = "database";
		if(config_set(config, *section, name, value) != 0)
		{
			r = NULL;
			break;
		}
	}
	free(p);
	if(r == NULL)
		return NULL;
	return "pgsql";
}

static char const * _init_sqlite3(char const * dsn, Config * config,
		char const ** section)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() %s=%s\n", __func__, "filename", dsn);
#endif
	*section = "database::sqlite3";
	if(config_set(config, *section, "filename", dsn) != 0)
		return NULL;
	return "sqlite3";
}


/* pdo_destroy */
static void _pdo_destroy(PDO * pdo)
{
	if(pdo->database != NULL)
		pdo->dplugin->destroy(pdo->database);
	if(pdo->plugin != NULL)
		plugin_delete(pdo->plugin);
	object_delete(pdo);
}


/* accessors */
/* pdo_get_last_id */
static int64_t _pdo_get_last_id(PDO * pdo)
{
	return pdo->dplugin->get_last_id(pdo->database);
}


/* useful */
/* pdo_statement_new */
static PDOStatement * _pdo_statement_new(PDO * pdo, char const * query)
{
	PDOStatement * statement;

	if((statement = object_new(sizeof(*statement))) == NULL)
		return NULL;
	if((statement->statement = pdo->dplugin->statement_new(pdo->database,
					query)) == NULL)
	{
		object_delete(statement);
		return NULL;
	}
	return statement;
}


/* pdo_statement_delete */
static void _pdo_statement_delete(PDO * pdo, PDOStatement * statement)
{
	pdo->dplugin->statement_delete(pdo->database, statement->statement);
	object_delete(statement);
}


/* pdo_statement_query */
static int _pdo_statement_query(PDO * pdo, PDOStatement * statement,
		DatabaseCallback callback, void * data, va_list args)
{
	return pdo->dplugin->statement_query(pdo->database,
			statement->statement, callback, data, args);
}


/* pdo_query */
static int _pdo_query(PDO * pdo, char const * query, DatabaseCallback callback,
		void * data)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, query);
#endif
	return pdo->dplugin->query(pdo->database, query, callback, data);
}
