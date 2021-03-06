/* $Id$ */
/* Copyright (c) 2012-2019 Pierre Pronchery <khorben@defora.org> */
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

# include <stdint.h>
# include <System.h>


/* Database */
/* public */
/* types */
typedef enum _DatabaseType
{
	DT_NULL = 0,
	DT_INTEGER,
	DT_TIMESTAMP,
	DT_VARCHAR
} DatabaseType;

typedef struct _Database Database;

typedef int (*DatabaseCallback)(void * data, int argc, char ** argv,
		char ** columns);


/* public */
/* functions */
Database * database_new(char const * engine, Config * config,
		char const * section);
void database_delete(Database * database);

/* accessors */
int64_t database_get_last_id(Database * database);

/* useful */
int database_query(Database * database, char const * query,
		DatabaseCallback callback, void * data);

#endif /* !LIBDATABASE_DATABASE_DATABASE_H */
