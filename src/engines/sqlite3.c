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



#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sqlite3.h>
#include <System.h>
#include "Database/engine.h"


/* SQLite3 */
/* private */
/* types */
typedef struct _DatabaseEngine
{
	sqlite3 * handle;
} SQLite3;

typedef struct _DatabaseStatement
{
	sqlite3_stmt * stmt;
} SQLite3Statement;


/* protected */
/* prototypes */
/* plug-in */
static SQLite3 * _sqlite3_init(Config * config, char const * section);
static void _sqlite3_destroy(SQLite3 * sqlite3);

static int64_t _sqlite3_get_last_id(SQLite3 * sqlite3);

static int _sqlite3_query(SQLite3 * sqlite3, char const * query,
		DatabaseCallback callback, void * data);

static SQLite3Statement * _sqlite3_prepare_new(SQLite3 * sqlite3,
		char const * query);
static void _sqlite3_prepare_delete(SQLite3 * sqlite3,
		SQLite3Statement * statement);
static int _sqlite3_prepare_query(SQLite3 * sqlite3,
		SQLite3Statement * statement, DatabaseCallback callback,
		void * data, va_list args);


/* public */
/* variables */
DatabaseEngineDefinition database =
{
	"SQLite3",
	NULL,
	_sqlite3_init,
	_sqlite3_destroy,
	_sqlite3_get_last_id,
	_sqlite3_query,
	NULL,
	_sqlite3_prepare_new,
	_sqlite3_prepare_delete,
	_sqlite3_prepare_query,
	NULL
};


/* private */
/* functions */
/* _sqlite3_init */
static SQLite3 * _sqlite3_init(Config * config, char const * section)
{
	SQLite3 * sqlite3;
	char const * name;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, section);
#endif
	if((sqlite3 = object_new(sizeof(*sqlite3))) == NULL)
		return NULL;
	sqlite3->handle = NULL;
	if((name = config_get(config, section, "filename")) != NULL
			&& sqlite3_open(name, &sqlite3->handle) != SQLITE_OK)
		error_set_code(1, "%s: %s", name, (sqlite3->handle != NULL)
				? sqlite3_errmsg(sqlite3->handle)
				: "Unknown error");
	if(sqlite3->handle == NULL)
	{
		_sqlite3_destroy(sqlite3);
		return NULL;
	}
	return sqlite3;
}


/* _sqlite3_destroy */
static void _sqlite3_destroy(SQLite3 * sqlite3)
{
	if(sqlite3->handle != NULL)
		sqlite3_close(sqlite3->handle);
	object_delete(sqlite3);
}


/* accessors */
/* _sqlite3_get_last_id */
static int64_t _sqlite3_get_last_id(SQLite3 * sqlite3)
{
	/* XXX returns an int64_t */
	return sqlite3_last_insert_rowid(sqlite3->handle);
}


/* useful */
/* _sqlite3_prepare_new */
static SQLite3Statement * _sqlite3_prepare_new(SQLite3 * sqlite3,
		char const * query)
{
	SQLite3Statement * statement;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, query);
#endif
	if((statement = object_new(sizeof(*statement))) == NULL)
		return NULL;
	if(sqlite3_prepare_v2(sqlite3->handle, query, -1, &statement->stmt,
				NULL) != SQLITE_OK)
	{
		error_set_code(1, "%s", sqlite3_errmsg(sqlite3->handle));
		object_delete(statement);
		return NULL;
	}
	return statement;
}


/* _sqlite3_prepare_delete */
static void _sqlite3_prepare_delete(SQLite3 * sqlite3,
		SQLite3Statement * statement)
{
	/* XXX ignore errors */
	sqlite3_finalize(statement->stmt);
	object_delete(statement);
}


/* _sqlite3_prepare_query */
static int _sqlite3_prepare_query(SQLite3 * sqlite3,
		SQLite3Statement * statement, DatabaseCallback callback,
		void * data, va_list args)
{
	int ret = 0;
	int type;
	char const * name;
	int i;
	int cnt;
	int argc;
	char ** argv;
	char ** columns;
	char ** p;
	int l;
	time_t t;
	struct tm tm;
	char buf[32];
	char const * s;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s()\n", __func__);
#endif
	sqlite3_reset(statement->stmt);
	while(ret == 0 && (type = va_arg(args, int)) != -1)
	{
		name = va_arg(args, char const *);
		if((i = sqlite3_bind_parameter_index(statement->stmt, name))
				== 0)
		{
			ret = -error_set_code(1, "%s", "Unknown parameter");
			break;
		}
		switch(type)
		{
			case DT_NULL:
				if((s = va_arg(args, void *)) != NULL)
					ret = -error_set_code(1, "%s",
							strerror(EINVAL));
				else
					sqlite3_bind_null(statement->stmt, i);
				break;
			case DT_INTEGER:
				l = va_arg(args, int);
#ifdef DEBUG
				fprintf(stderr, "DEBUG: %s() %s=\"%ld\"\n",
						__func__, name, l);
#endif
				if(sqlite3_bind_int(statement->stmt, i, l)
						== SQLITE_OK)
					break;
				ret = -error_set_code(1, "%s", sqlite3_errmsg(
							sqlite3->handle));
				break;
			case DT_TIMESTAMP:
				t = va_arg(args, time_t);
				if(gmtime_r(&t, &tm) == NULL)
					break;
				if(strftime(buf, sizeof(buf), "%Y-%m-%d"
							" %H:%M:%S", &tm) == 0)
				{
					ret = -error_set_code(1, "%s", strerror(
								errno));
					break;
				}
				if(sqlite3_bind_text(statement->stmt, i, buf,
							-1, SQLITE_TRANSIENT)
						== SQLITE_OK)
					break;
				ret = -error_set_code(1, "%s", sqlite3_errmsg(
							sqlite3->handle));
				break;
			case DT_VARCHAR:
				if((s = va_arg(args, char const *)) == NULL)
				{
					ret = -error_set_code(1, "%s",
							strerror(EINVAL));
					break;
				}
#ifdef DEBUG
				fprintf(stderr, "DEBUG: %s() %s=\"%s\" (%d)\n",
						__func__, name, s, i);
#endif
				if(sqlite3_bind_text(statement->stmt, i, s, -1,
							SQLITE_STATIC)
						== SQLITE_OK)
					break;
				ret = -error_set_code(1, "%s", sqlite3_errmsg(
							sqlite3->handle));
				break;
			default:
				ret = -error_set_code(1, "%s (%d)",
						"Unsupported type", type);
				break;
		}
	}
	if(ret != 0)
		return ret;
	/* return directly if there are no results */
	if((argc = sqlite3_column_count(statement->stmt)) == 0)
	{
		/* FIXME should really execute until done */
		if(sqlite3_step(statement->stmt) != SQLITE_DONE)
			ret = -error_set_code(1, "%s", sqlite3_errmsg(
						sqlite3->handle));
		return ret;
	}
	/* pre-allocate the results */
	argv = malloc(sizeof(*argv) * argc);
	columns = malloc(sizeof(*columns) * argc);
	if(argv == NULL || columns == NULL)
	{
		free(argv);
		free(columns);
		return -error_set_code(1, "%s", strerror(errno));
	}
	/* obtain the column names */
	for(i = 0; i < argc; i++)
		/* XXX may fail */
		columns[i] = strdup(sqlite3_column_name(statement->stmt, i));
	for(cnt = 0; (i = sqlite3_step(statement->stmt)) == SQLITE_ROW; cnt++)
	{
#ifdef DEBUG
		fprintf(stderr, "DEBUG: %s() cnt=%d\n", __func__, cnt);
#endif
		/* reset the result line */
		memset(argv, 0, sizeof(*argv) * argc);
		for(i = 0; (type = sqlite3_column_type(statement->stmt, i))
				!= SQLITE_NULL; i++)
		{
			switch(type)
			{
				case SQLITE_INTEGER:
					l = sqlite3_column_int(statement->stmt,
							i);
					snprintf(buf, sizeof(buf), "%d", l);
					s = buf;
					break;
				case SQLITE_TEXT:
					s = (char const *)sqlite3_column_text(
							statement->stmt, i);
					break;
				case SQLITE_BLOB:
				case SQLITE_FLOAT:
				default:
					/* FIXME really implement */
#ifdef DEBUG
					fprintf(stderr, "DEBUG: %s() i=%d\n",
							__func__, i);
#endif
					continue;
			}
			p = &argv[i];
			/* XXX check errors */
			*p = strdup(s);
#ifdef DEBUG
			fprintf(stderr, "DEBUG: %s() argv[%d]=\"%s\"\n",
					__func__, i, argv[i]);
#endif
		}
		/* call the callback */
		if(ret == 0 && callback != NULL)
			callback(data, argc, argv, columns);
		for(i = 0; i < argc; i++)
			free(argv[i]);
	}
	free(argv);
	/* call the callback with no values */
	if(cnt == 0 && callback != NULL)
		callback(data, argc, NULL, columns);
	for(l = 0; l < argc; l++)
		free(columns[l]);
	free(columns);
	if(ret == 0 && i != SQLITE_DONE)
		ret = -error_set_code(1, "%s", sqlite3_errmsg(sqlite3->handle));
	return ret;
}


/* _sqlite3_query */
int _sqlite3_query(SQLite3 * sqlite3, char const * query,
		DatabaseCallback callback, void * data)
{
	int ret;
	char * error = NULL;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, query);
#endif
	ret = (sqlite3_exec(sqlite3->handle, query, callback, data, &error)
			== SQLITE_OK) ? 0 : -error_set_code(1, "%s",
				(error != NULL) ? error : "Unknown error");
	if(error != NULL)
		sqlite3_free(error);
	return ret;
}
