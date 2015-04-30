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



#ifndef LIBDATABASE_DATABASE_ENGINE_H
# define LIBDATABASE_DATABASE_ENGINE_H

# include <stdarg.h>
# include <stdint.h>
# include <System.h>
# include "database.h"
# include "result.h"


/* DatabaseEngine */
/* public */
/* types */
typedef struct _DatabaseEngine DatabaseEngine;

typedef struct _DatabaseEngineDefinition
{
	char const * name;
	char const * description;
	/* essential */
	DatabaseEngine * (*init)(Config * config, char const * section);
	void (*destroy)(DatabaseEngine * engine);
	/* accessors */
	int64_t (*get_last_id)(DatabaseEngine * engine);
	/* useful */
	int (*query)(DatabaseEngine * engine, char const * query,
			DatabaseCallback callback, void * data);
	DatabaseResult * (*query_result)(DatabaseEngine * engine,
			char const * query);
	/* prepared statements */
	DatabaseStatement * (*prepare_new)(DatabaseEngine * engine,
			char const * query);
	void (*prepare_delete)(DatabaseEngine * engine,
			DatabaseStatement * statement);
	int (*prepare_query)(DatabaseEngine * engine,
			DatabaseStatement * statement,
			DatabaseCallback callback, void * data,
			va_list args);
	DatabaseResult * (*prepare_query_result)(DatabaseEngine * engine,
			DatabaseStatement * statement,
			va_list args);
} DatabaseEngineDefinition;

#endif /* !LIBDATABASE_DATABASE_ENGINE_H */
