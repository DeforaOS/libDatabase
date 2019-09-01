/* $Id$ */
/* Copyright (c) 2019 Pierre Pronchery <khorben@defora.org> */
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



#ifndef LIBDATABASE_DATABASE_STATEMENT_H
# define LIBDATABASE_DATABASE_STATEMENT_H

# include "database.h"


/* DatabaseStatement */
/* public */
/* types */
typedef struct _DatabaseStatement DatabaseStatement;


/* public */
/* functions */
DatabaseStatement * databasestatement_new(Database * database,
		char const * query);
void databasestatement_delete(DatabaseStatement * statement);

/* accessors */
Database * databasestatement_get_database(DatabaseStatement * statement);

/* useful */
int databasestatement_query(DatabaseStatement * statement,
		DatabaseCallback callback, void * data, ...);

#endif /* !LIBDATABASE_DATABASE_STATEMENT_H */
