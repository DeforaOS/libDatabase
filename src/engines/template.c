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


/* Template */
/* private */
/* types */
typedef struct _DatabaseEngine
{
	void * handle;
} Template;

typedef struct _DatabaseEngineStatement
{
	char * query;
} TemplateStatement;


/* protected */
/* prototypes */
/* plug-in */
static Template * _template_init(Config * config, char const * section);
static void _template_destroy(Template * pgsql);

static int64_t _template_get_last_id(Template * pgsql);

static int _template_query(Template * pgsql, char const * query,
		DatabaseCallback callback, void * data);

static TemplateStatement * _template_statement_new(Template * pgsql,
		char const * query);
static void _template_statement_delete(Template * pgsql, TemplateStatement * statement);
static int _template_statement_query(Template * template,
		TemplateStatement * statement, DatabaseCallback callback,
		void * data, va_list args);


/* public */
/* variables */
DatabaseEngineDefinition database =
{
	"Template",
	NULL,
	_template_init,
	_template_destroy,
	_template_get_last_id,
	_template_query,
	_template_statement_new,
	_template_statement_delete,
	_template_statement_query
};


/* private */
/* functions */
/* template_init */
static Template * _template_init(Config * config, char const * section)
{
	Template * template;

	if((template = object_new(sizeof(*template))) == NULL)
		return NULL;
	template->handle = NULL;
	return template;
}


/* template_destroy */
static void _template_destroy(Template * template)
{
	object_delete(template);
}


/* accessors */
/* template_get_last_id */
static int64_t _template_get_last_id(Template * template)
{
	/* FIXME really implement */
	return -1;
}


/* useful */
/* template_statement_new */
static TemplateStatement * _template_statement_new(Template * template,
		char const * query)
{
	TemplateStatement * statement;

	if((statement = object_new(sizeof(*statement))) == NULL)
		return NULL;
	statement->query = string_new(query);
	return statement;
}


/* template_statement_delete */
static void _template_statement_delete(Template * template, TemplateStatement * statement)
{
	string_delete(statement->query);
	object_delete(statement);
}


/* template_statement_query */
static int _template_statement_query(Template * template,
		TemplateStatement * statement, DatabaseCallback callback,
		void * data, va_list args)
{
	int type = -1;
	char const * name;
	char const * s;
	void * p;

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
			default:
				p = va_arg(args, void *);
				break;
		}
	}
	return -1;
}


/* template_query */
static int _template_query(Template * template, char const * query,
		DatabaseCallback callback, void * data)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, query);
#endif
	return -1;
}
