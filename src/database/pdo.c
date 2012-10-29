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
#include <System.h>
#include "database.h"
#include "../../config.h"

#ifndef PREFIX
# define PREFIX		"/usr/local"
#endif
#ifndef LIBDIR
# define LIBDIR		PREFIX "/lib"
#endif


/* PDO */
/* private */
/* types */
typedef struct _DatabasePlugin
{
	Plugin * plugin;
	DatabasePluginDefinition * dplugin;
	DatabasePlugin * database;
} PDO;

typedef struct _DatabaseStatement
{
	DatabaseStatement * statement;
} PDOStatement;


/* protected */
/* prototypes */
/* plug-in */
static PDO * _pdo_init(Config * config, char const * section);
static void _pdo_destroy(PDO * pdo);

static int _pdo_get_last_id(PDO * pdo);

static int _pdo_query(PDO * pdo, char const * query, DatabaseCallback callback,
		void * data);

static PDOStatement * _pdo_prepare_new(PDO * pdo, char const * query);
static void _pdo_prepare_delete(PDO * pdo, PDOStatement * statement);
static int _pdo_prepare_query(PDO * pdo, PDOStatement * statement,
		DatabaseCallback callback, void * data, va_list args);


/* public */
/* variables */
DatabasePluginDefinition database =
{
	"PDO",
	NULL,
	_pdo_init,
	_pdo_destroy,
	_pdo_get_last_id,
	_pdo_query,
	_pdo_prepare_new,
	_pdo_prepare_delete,
	_pdo_prepare_query
};


/* private */
/* functions */
/* _pdo_init */
static PDO * _pdo_init(Config * config, char const * section)
{
	PDO * pdo;
	char const * dsn;
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
	{
		backend = "sqlite3";
		section = "database::sqlite3";
		/* XXX may fail */
		config_set(config, section, "filename",
				&dsn[sizeof(sqlite3) - 1]);
	}
	else
		/* XXX report error */
		return NULL;
	if((pdo = object_new(sizeof(*pdo))) == NULL)
	{
		config_delete(config);
		return NULL;
	}
	pdo->database = NULL;
	if((pdo->plugin = plugin_new(LIBDIR, PACKAGE, "database", backend))
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


/* _pdo_destroy */
static void _pdo_destroy(PDO * pdo)
{
	if(pdo->database != NULL)
		pdo->dplugin->destroy(pdo->database);
	if(pdo->plugin != NULL)
		plugin_delete(pdo->plugin);
	object_delete(pdo);
}


/* accessors */
/* _pdo_get_last_id */
static int _pdo_get_last_id(PDO * pdo)
{
	return pdo->dplugin->get_last_id(pdo->database);
}


/* useful */
/* _pdo_prepare_new */
static PDOStatement * _pdo_prepare_new(PDO * pdo, char const * query)
{
	PDOStatement * statement;

	if((statement = object_new(sizeof(*statement))) == NULL)
		return NULL;
	if((statement->statement = pdo->dplugin->prepare_new(pdo->database,
					query)) == NULL)
	{
		object_delete(statement);
		return NULL;
	}
	return statement;
}


/* _pdo_prepare_delete */
static void _pdo_prepare_delete(PDO * pdo, PDOStatement * statement)
{
	pdo->dplugin->prepare_delete(pdo->database, statement->statement);
	object_delete(statement);
}


/* _pdo_prepare_query */
static int _pdo_prepare_query(PDO * pdo, PDOStatement * statement,
		DatabaseCallback callback, void * data, va_list args)
{
	return pdo->dplugin->prepare_query(pdo->database, statement->statement,
			callback, data, args);
}


/* _pdo_query */
static int _pdo_query(PDO * pdo, char const * query, DatabaseCallback callback,
		void * data)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, query);
#endif
	return pdo->dplugin->query(pdo->database, query, callback, data);
}
