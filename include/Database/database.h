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



#ifndef LIBDATABASE_DATABASE_DATABASE_H
# define LIBDATABASE_DATABASE_DATABASE_H

# include <stdarg.h>
# include <System.h>


/* Database */
/* private */
/* types */
typedef enum _DatabaseType
{
	DT_NULL = 0,
	DT_INTEGER,
	DT_TIMESTAMP,
	DT_VARCHAR
} DatabaseType;

typedef struct _Database Database;
typedef struct _DatabaseStatement DatabaseStatement;

typedef int (*DatabaseCallback)(void * data, int argc, char ** argv,
		char ** columns);

typedef struct _DatabasePlugin DatabasePlugin;

typedef struct _DatabasePluginDefinition
{
	char const * name;
	char const * description;
	/* essential */
	DatabasePlugin * (*init)(Config * config, char const * section);
	void (*destroy)(DatabasePlugin * plugin);
	/* accessors */
	int (*get_last_id)(DatabasePlugin * plugin);
	/* useful */
	int (*query)(DatabasePlugin * plugin, char const * query,
			DatabaseCallback callback, void * data);
	/* prepared statements */
	DatabaseStatement * (*prepare_new)(DatabasePlugin * plugin,
			char const * query);
	void (*prepare_delete)(DatabasePlugin * plugin,
			DatabaseStatement * statement);
	int (*prepare_query)(DatabasePlugin * plugin,
			DatabaseStatement * statement,
			DatabaseCallback callback, void * data,
			va_list args);
} DatabasePluginDefinition;


/* public */
/* functions */
Database * database_new(char const * engine, Config * config,
		char const * section);
void database_delete(Database * database);

/* accessors */
int database_get_last_id(Database * database);

/* useful */
DatabaseStatement * database_prepare_new(
		Database * database, char const * query);
void database_prepare_delete(Database * database,
		DatabaseStatement * statement);
int database_prepare_query(Database * database,
		DatabaseStatement * statement,
		DatabaseCallback callback, void * data, ...);

int database_query(Database * database, char const * query,
		DatabaseCallback callback, void * data);

#endif /* !LIBDATABASE_DATABASE_DATABASE_H */
