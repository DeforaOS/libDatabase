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



#include <string.h>
#include <System.h>
#include "Database/engine.h"
#include "Database/statement.h"
#include "database.h"


/* DatabaseStatement */
/* private */
/* types */
struct _DatabaseStatement
{
	Database * database;
	DatabaseEngineStatement * statement;
};


/* public */
/* functions */
/* databasestatement_new */
DatabaseStatement * databasestatement_new(Database * database,
		char const * query)
{
	DatabaseStatement * statement;

	if((statement = object_new(sizeof(*statement))) == NULL)
		return NULL;
	statement->database = database;
	if(database_engine_statement_new(database, query) != 0)
	{
		object_delete(statement);
		return NULL;
	}
	return statement;
}


/* databasestatement_delete */
void databasestatement_delete(DatabaseStatement * statement)
{
	database_engine_statement_delete(statement->database,
			statement->statement);
	object_delete(statement);
}


/* accessors */
Database * databasestatement_get_database(DatabaseStatement * statement)
{
	return statement->database;
}


/* databasestatement_query */
int databasestatement_query(DatabaseStatement * statement,
		DatabaseCallback callback, void * data, ...)
{
	int ret;
	va_list args;

	va_start(args, data);
	ret = database_engine_statement_query(statement->database,
			statement->statement, callback, data, args);
	va_end(args);
	return ret;
}
