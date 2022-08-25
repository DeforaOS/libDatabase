/* $Id$ */
/* Copyright (c) 2019 Pierre Pronchery <khorben@defora.org> */
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


/* useful */
/* databasestatement_query */
int databasestatement_query(DatabaseStatement * statement,
		DatabaseCallback callback, void * data, ...)
{
	int ret;
	va_list args;

	va_start(args, data);
	ret = databasestatement_queryv(statement, callback, data, args);
	va_end(args);
	return ret;
}


/* databasestatement_queryv */
int databasestatement_queryv(DatabaseStatement * statement,
		DatabaseCallback callback, void * data, va_list args)
{
	return database_engine_statement_query(statement->database,
			statement->statement, callback, data, args);
}
