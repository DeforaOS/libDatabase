/* $Id$ */
/* Copyright (c) 2016-2022 Pierre Pronchery <khorben@defora.org> */
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



#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <System.h>
#include "Database.h"

#ifndef PROGNAME_SQLITE3
# define PROGNAME_SQLITE3	"sqlite3"
#endif


/* sqlite3 */
/* private */
/* prototypes */
static int _sqlite3(void);

static int _usage(void);


/* functions */
/* sqlite3 */
static int _sqlite3(void)
{
	Config * config;
	Database * db;
	String * filename;
	char const * p;

	if((config = config_new()) == NULL)
	{
		error_print(PROGNAME_SQLITE3);
		return -1;
	}
	if((p = getenv("OBJDIR")) != NULL)
		filename = string_new_append(p, "/sqlite3.db", NULL);
	else
		filename = string_new("sqlite3.db");
	if(filename == NULL
			|| config_set(config, NULL, "filename", filename) != 0)
	{
		error_print(PROGNAME_SQLITE3);
		string_delete(filename);
		config_delete(config);
		return -1;
	}
	string_delete(filename);
	db = database_new("sqlite3", config, NULL);
	config_delete(config);
	if(db == NULL)
		return 2;
	if(database_query(db, "SELECT * FROM \"table\"", NULL, NULL) != 0)
	{
		error_print(PROGNAME_SQLITE3);
		database_delete(db);
		return 2;
	}
	database_delete(db);
	return 0;
}


/* usage */
static int _usage(void)
{
	fputs("Usage: " PROGNAME_SQLITE3 "\n", stderr);
	return 1;
}


/* public */
/* functions */
/* main */
int main(int argc, char * argv[])
{
	(void) argv;

	if(argc != 1)
		return _usage();
	return (_sqlite3() == 0) ? 0 : 2;
}
