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



#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <libpq-fe.h>
#include <System.h>
#include "database.h"


/* PgSQL */
/* private */
/* types */
typedef struct _DatabasePlugin
{
	PGconn * handle;
	Oid last;
} PgSQL;

typedef struct _DatabaseStatement
{
	PGresult * res;
	char * query;
} PgSQLStatement;


/* prototypes */
/* useful */
static int _pgsql_process(PgSQL * pgsql, PGresult * res,
		DatabaseCallback callback, void * data);


/* protected */
/* prototypes */
/* plug-in */
static PgSQL * _pgsql_init(Config * config, char const * section);
static void _pgsql_destroy(PgSQL * pgsql);

static int _pgsql_get_last_id(PgSQL * pgsql);

static int _pgsql_query(PgSQL * pgsql, char const * query,
		DatabaseCallback callback, void * data);

static PgSQLStatement * _pgsql_prepare_new(PgSQL * pgsql, char const * query);
static void _pgsql_prepare_delete(PgSQL * pgsql, PgSQLStatement * statement);
static int _pgsql_prepare_query(PgSQL * pgsql, PgSQLStatement * statement,
		DatabaseCallback callback, void * data, va_list args);


/* public */
/* variables */
DatabasePluginDefinition database =
{
	"PostgreSQL",
	NULL,
	_pgsql_init,
	_pgsql_destroy,
	_pgsql_get_last_id,
	_pgsql_query,
	_pgsql_prepare_new,
	_pgsql_prepare_delete,
	_pgsql_prepare_query
};


/* protected */
/* functions */
/* plug-in */
/* pgsql_init */
static void _init_append(char * buf, size_t size, char const * variable,
		char const * value);

static PgSQL * _pgsql_init(Config * config, char const * section)
{
	PgSQL * pgsql;
	char buf[256] = "";

	if((pgsql = object_new(sizeof(*pgsql))) == NULL)
		return NULL;
	pgsql->last = InvalidOid;
	/* build the connection string */
	_init_append(buf, sizeof(buf), "user", config_get(config, section,
				"username"));
	_init_append(buf, sizeof(buf), "password", config_get(config, section,
				"password"));
	_init_append(buf, sizeof(buf), "dbname", config_get(config, section,
				"database"));
	_init_append(buf, sizeof(buf), "host", config_get(config, section,
				"hostname"));
	_init_append(buf, sizeof(buf), "port", config_get(config, section,
				"port"));
	/* connect to the database */
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() \"%s\"\n", __func__, buf);
#endif
	if((pgsql->handle = PQconnectdb(buf)) == NULL
			|| PQstatus(pgsql->handle) != CONNECTION_OK)
	{
		error_set_code(1, "%s", (pgsql->handle != NULL)
				? PQerrorMessage(pgsql->handle)
				: "Unable to obtain the connection string");
		_pgsql_destroy(pgsql);
		return NULL;
	}
	return pgsql;
}

static void _init_append(char * buf, size_t size, char const * variable,
		char const * value)
{
	size_t len;

	if(value == NULL)
		return;
	len = strlen(buf);
	snprintf(&buf[len], size - len, "%s%s=%s", (len > 0) ? " " : "",
			variable, value);
}


/* pgsql_destroy */
static void _pgsql_destroy(PgSQL * pgsql)
{
	if(pgsql->handle != NULL)
		PQfinish(pgsql->handle);
	object_delete(pgsql);
}


/* accessors */
/* pgsql_get_last_id */
static int _pgsql_get_last_id(PgSQL * pgsql)
{
	/* FIXME use currval() of the relevant instead */
	if(pgsql->last == InvalidOid)
		return -1;
	return pgsql->last;
}


/* useful */
/* pgsql_prepare_new */
static void _prepare_new_adapt(char * q);

static PgSQLStatement * _pgsql_prepare_new(PgSQL * pgsql,
		char const * query)
{
	PgSQLStatement * statement;
	char * q;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, query);
#endif
	if((statement = object_new(sizeof(*statement))) == NULL)
		return NULL;
	statement->query = strdup(query);
	statement->res = NULL;
	q = strdup(query);
	if(statement->query == NULL || q == NULL)
	{
		free(q);
		_pgsql_prepare_delete(pgsql, statement);
		return NULL;
	}
	/* adapt the statement for PostgreSQL */
	_prepare_new_adapt(q);
	if((statement->res = PQprepare(pgsql->handle, query, q, 0, NULL))
			== NULL
			|| PQresultStatus(statement->res) != PGRES_COMMAND_OK)
	{
		error_set_code(1, "%s", PQerrorMessage(pgsql->handle));
		_pgsql_prepare_delete(pgsql, statement);
		return NULL;
	}
	return statement;
}

static void _prepare_new_adapt(char * query)
{
	int i;
	char * p;
	int j;
	size_t len;
	int c;

	/* FIXME this only works for up to 9 arguments */
	for(i = 0; (p = strchr(query, ':')) != NULL; i++)
	{
		*(p++) = '$';
		*(p++) = '1' + i;
		for(j = 0; isalpha((c = p[j])) || p[j] == '_'; j++);
		len = strlen(p) + 1;
		memmove(p, &p[j], len - j);
	}
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() => \"%s\"\n", __func__, query);
#endif
}


/* pgsql_prepare_delete */
static void _pgsql_prepare_delete(PgSQL * pgsql,
		PgSQLStatement * statement)
{
	PQclear(statement->res);
	free(statement->query);
	object_delete(statement);
}


/* pgsql_prepare_query */
static int _pgsql_prepare_query(PgSQL * pgsql, PgSQLStatement * statement,
		DatabaseCallback callback, void * data, va_list args)
{
	int ret = 0;
	size_t cnt;
	int type;
	char const * name;
	long l;
	char buf[32];
	time_t t;
	struct tm tm;
	char const * s;
	char ** v = NULL;
	char ** p;
	size_t i;
	PGresult * res;

	/* FIXME this assumes the same order as in the prepared statement */
	for(cnt = 0; ret == 0 && (type = va_arg(args, int)) != -1; cnt++)
	{
		name = va_arg(args, char const *);
		if((p = realloc(v, sizeof(*v) * (cnt + 1))) == NULL)
		{
			ret = -error_set_code(1, "%s", strerror(errno));
			break;
		}
		v = p;
		v[cnt] = NULL;
		switch(type)
		{
			case DT_INTEGER:
				l = va_arg(args, long);
				snprintf(buf, sizeof(buf), "%ld", l);
				v[cnt] = strdup(buf);
				break;
			case DT_TIMESTAMP:
				t = va_arg(args, time_t);
				if(gmtime_r(&t, &tm) == NULL)
					break;
				if(strftime(buf, sizeof(buf), "%Y-%m-%d"
							" %H:%M:%S", &tm) == 0)
					break;
				v[cnt] = strdup(buf);
				break;
			case DT_VARCHAR:
				s = va_arg(args, char const *);
				v[cnt] = strdup(s);
				break;
			default:
				errno = ENOSYS;
				break;
		}
		if(v[cnt] == NULL)
			ret = -error_set_code(1, "%s", strerror(errno));
	}
	if(ret != 0)
	{
		for(i = 0; i < cnt; i++)
			free(v[i]);
		free(v);
		return ret;
	}
	pgsql->last = InvalidOid;
	res = PQexecPrepared(pgsql->handle, statement->query, cnt, v, NULL,
			NULL, 0);
	for(i = 0; i < cnt; i++)
		free(v[i]);
	free(v);
	ret = _pgsql_process(pgsql, res, callback, data);
	PQclear(res);
	return ret;
}


/* pgsql_query */
static int _pgsql_query(PgSQL * pgsql, char const * query,
		DatabaseCallback callback, void * data)
{
	int ret;
	PGresult * res;

#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, query);
#endif
	pgsql->last = InvalidOid;
	if((res = PQexec(pgsql->handle, query)) == NULL)
	{
		error_set_code(1, "%s", PQerrorMessage(pgsql->handle));
		PQclear(res);
		return -1;
	}
	ret = _pgsql_process(pgsql, res, callback, data);
	PQclear(res);
	return ret;
}


/* private */
/* functions */
/* useful */
/* pgsql_process */
static int _pgsql_process(PgSQL * pgsql, PGresult * res,
		DatabaseCallback callback, void * data)
{
	char ** columns;
	char ** fields;
	size_t cnt;
	size_t i;
	int n;
	int j;

	if(PQresultStatus(res) == PGRES_COMMAND_OK)
	{
		pgsql->last = PQoidValue(res);
		return 0;
	}
	else if(PQresultStatus(res) != PGRES_TUPLES_OK)
	{
		error_set_code(1, "%s", PQerrorMessage(pgsql->handle));
		return -1;
	}
	cnt = PQnfields(res);
	columns = malloc(sizeof(*columns) * cnt); /* XXX may fail */
	fields = malloc(sizeof(*fields) * cnt); /* XXX may fail */
	/* obtain the names of the columns */
	for(i = 0; i < cnt; i++)
	{
		columns[i] = PQfname(res, i);
#ifdef DEBUG
		fprintf(stderr, "DEBUG: %s() %zu/%zu \"%s\"\n", __func__, i + 1,
				cnt, columns[i]);
#endif
	}
	n = PQntuples(res);
	for(j = 0; j < n; j++)
	{
		/* obtain the values of the fields */
		for(i = 0; i < cnt; i++)
			fields[i] = PQgetvalue(res, j, i);
		/* call the callback */
		callback(data, cnt, fields, columns);
	}
	free(fields);
	free(columns);
	return 0;
}
