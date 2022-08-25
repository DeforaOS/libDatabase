/* $Id$ */
/* Copyright (c) 2016-2022 Pierre Pronchery <khorben@defora.org> */
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
