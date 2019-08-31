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



#ifndef LIBDATABASE_SRC_DATABASE_H
# define LIBDATABASE_SRC_DATABASE_H

# include "Database/database.h"
# include "Database/engine.h"


/* Database */
/* protected */
/* functions */
DatabaseEngineStatement * database_engine_statement_new(Database * database,
		char const * query);
void database_engine_statement_delete(Database * database,
		DatabaseEngineStatement * statement);

/* useful */
int database_engine_statement_query(Database * database,
		DatabaseEngineStatement * statement,
		DatabaseCallback callback, void * data, va_list args);

#endif /* !LIBDATABASE_SRC_DATABASE_H */
