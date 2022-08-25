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



#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <libpq-fe.h>
#include <System.h>
#include "Database/engine.h"


/* PgSQL */
/* private */
/* types */
typedef struct _DatabaseEngine
{
	PGconn * handle;
	Oid last;
} PgSQL;

typedef struct _DatabaseEngineStatement
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

static int64_t _pgsql_get_last_id(PgSQL * pgsql);

static int _pgsql_query(PgSQL * pgsql, char const * query,
		DatabaseCallback callback, void * data);

static PgSQLStatement * _pgsql_statement_new(PgSQL * pgsql, char const * query);
static void _pgsql_statement_delete(PgSQL * pgsql, PgSQLStatement * statement);
static int _pgsql_statement_query(PgSQL * pgsql, PgSQLStatement * statement,
		DatabaseCallback callback, void * data, va_list args);


/* public */
/* variables */
DatabaseEngineDefinition database =
{
	"PostgreSQL",
	NULL,
	_pgsql_init,
	_pgsql_destroy,
	_pgsql_get_last_id,
	_pgsql_query,
	_pgsql_statement_new,
	_pgsql_statement_delete,
	_pgsql_statement_query
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
static int64_t _pgsql_get_last_id(PgSQL * pgsql)
{
	/* FIXME use currval() of the relevant sequence instead */
	if(pgsql->last == InvalidOid)
		return -1;
	return pgsql->last;
}


/* useful */
/* pgsql_statement_new */
static void _statement_new_adapt(char * q);

static PgSQLStatement * _pgsql_statement_new(PgSQL * pgsql,
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
		_pgsql_statement_delete(pgsql, statement);
		return NULL;
	}
	/* adapt the statement for PostgreSQL */
	_statement_new_adapt(q);
	if((statement->res = PQprepare(pgsql->handle, query, q, 0, NULL))
			== NULL
			|| PQresultStatus(statement->res) != PGRES_COMMAND_OK)
	{
		error_set_code(1, "%s", PQerrorMessage(pgsql->handle));
		free(q);
		_pgsql_statement_delete(pgsql, statement);
		return NULL;
	}
	free(q);
	return statement;
}

static void _statement_new_adapt(char * query)
{
	int i;
	char * p;
	int j;
	size_t len;

	/* FIXME this only works for up to 9 arguments */
	for(i = 0; (p = strchr(query, ':')) != NULL; i++)
	{
		*(p++) = '$';
		*(p++) = '1' + i;
		for(j = 0; isalpha((unsigned char)p[j]) || p[j] == '_'; j++);
		len = strlen(p) + 1;
		memmove(p, &p[j], len - j);
	}
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s() => \"%s\"\n", __func__, query);
#endif
}


/* pgsql_statement_delete */
static void _pgsql_statement_delete(PgSQL * pgsql,
		PgSQLStatement * statement)
{
	(void) pgsql;

	PQclear(statement->res);
	free(statement->query);
	object_delete(statement);
}


/* pgsql_statement_query */
static int _pgsql_statement_query(PgSQL * pgsql, PgSQLStatement * statement,
		DatabaseCallback callback, void * data, va_list args)
{
	int ret = 0;
	size_t cnt;
	int type;
#ifdef DEBUG
	char const * name;
#endif
	int l;
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
#ifdef DEBUG
		name = va_arg(args, char const *);
#else
		va_arg(args, char const *);
#endif
		if((p = realloc(v, sizeof(*v) * (cnt + 1))) == NULL)
		{
			ret = -error_set_code(1, "%s", strerror(errno));
			break;
		}
		v = p;
		v[cnt] = NULL;
		switch(type)
		{
			case DT_NULL:
				if((v[cnt] = va_arg(args, void *)) != NULL)
					ret = -error_set_code(1, "%s",
							strerror(EINVAL));
				break;
			case DT_INTEGER:
				l = va_arg(args, int);
				snprintf(buf, sizeof(buf), "%d", l);
				if((v[cnt] = strdup(buf)) == NULL)
					ret = -error_set_code(1, "%s",
							strerror(errno));
				break;
			case DT_TIMESTAMP:
				t = va_arg(args, time_t);
				if(gmtime_r(&t, &tm) == NULL
						|| strftime(buf, sizeof(buf),
							"%Y-%m-%d %H:%M:%S",
							&tm) == 0
						|| (v[cnt] = strdup(buf))
						== NULL)
					ret = -error_set_code(1, "%s",
							strerror(errno));
				break;
			case DT_VARCHAR:
				s = va_arg(args, char const *);
				if(s == NULL)
					ret = -error_set_code(1, "%s",
							strerror(EINVAL));
				else if((v[cnt] = strdup(s)) == NULL)
					ret = -error_set_code(1, "%s",
							strerror(errno));
				break;
			default:
				ret = -error_set_code(1, "%s (%d)",
						"Unsupported type", type);
				break;
		}
#ifdef DEBUG
		fprintf(stderr, "DEBUG: %s() %lu: type %u, \"%s\": \"%s\"\n",
				__func__, cnt + 1, type, name, v[cnt]);
#endif
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
	ret = _pgsql_process(pgsql, res, callback, data);
	PQclear(res);
	for(i = 0; i < cnt; i++)
		free(v[i]);
	free(v);
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
	if((n = PQntuples(res)) == 0)
	{
		/* call the callback with no values */
		if(callback != NULL)
			callback(data, cnt, NULL, columns);
	}
	else if(callback != NULL)
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
