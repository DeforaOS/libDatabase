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


/* Template */
/* private */
/* types */
typedef struct _DatabasePlugin
{
	void * handle;
} Template;

typedef struct _DatabaseStatement
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

static TemplateStatement * _template_prepare_new(Template * pgsql,
		char const * query);
static void _template_prepare_delete(Template * pgsql, TemplateStatement * statement);
static int _template_prepare_query(Template * template,
		TemplateStatement * statement, DatabaseCallback callback,
		void * data, va_list args);


/* public */
/* variables */
DatabasePluginDefinition database =
{
	"Template",
	NULL,
	_template_init,
	_template_destroy,
	_template_get_last_id,
	_template_query,
	_template_prepare_new,
	_template_prepare_delete,
	_template_prepare_query
};


/* private */
/* functions */
/* _template_init */
static Template * _template_init(Config * config, char const * section)
{
	Template * template;

	if((template = object_new(sizeof(*template))) == NULL)
		return NULL;
	template->handle = NULL;
	return template;
}


/* _template_destroy */
static void _template_destroy(Template * template)
{
	object_delete(template);
}


/* accessors */
/* _template_get_last_id */
static int64_t _template_get_last_id(Template * template)
{
	/* FIXME really implement */
	return -1;
}


/* useful */
/* _template_prepare_new */
static TemplateStatement * _template_prepare_new(Template * template,
		char const * query)
{
	TemplateStatement * statement;

	if((statement = object_new(sizeof(*statement))) == NULL)
		return NULL;
	statement->query = string_new(query);
	return statement;
}


/* _template_prepare_delete */
static void _template_prepare_delete(Template * template, TemplateStatement * statement)
{
	string_delete(statement->query);
	object_delete(statement);
}


/* _template_prepare_query */
static int _template_prepare_query(Template * template,
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


/* _template_query */
static int _template_query(Template * template, char const * query,
		DatabaseCallback callback, void * data)
{
#ifdef DEBUG
	fprintf(stderr, "DEBUG: %s(\"%s\")\n", __func__, query);
#endif
	return -1;
}
