/* $Id$ */
/* Copyright (c) 2013-2020 Pierre Pronchery <khorben@defora.org> */
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
#include <stdio.h>
#include <System.h>
#include "Database.h"

#ifndef PROGNAME_DATABASE
# define PROGNAME_DATABASE	"database"
#endif


/* database */
/* private */
/* types */
typedef struct _DatabaseFile
{
	FILE * fp;
	int first;
	unsigned int rows;
} DatabaseFile;


/* prototypes */
static int _database(char const * engine, char const * cfile,
		char const * section);
static Database * _database_load(char const * engine, char const * cfile,
		char const * section);
static int _database_print(void * data, int argc, char ** argv,
		char ** columns);
static int _database_query(char const * engine, char const * cfile,
		char const * section, int queryc, char * const queryv[]);

static int _usage(void);


/* functions */
/* database */
static int _database(char const * engine, char const * cfile,
		char const * section)
{
	DatabaseFile database;
	Database * db;
	char buf[BUFSIZ];

	if((db = _database_load(engine, cfile, section)) == NULL)
	{
		error_print(PROGNAME_DATABASE);
		return 2;
	}
	database.fp = stdout;
	while(fgets(buf, sizeof(buf), stdin) != NULL)
	{
		database.first = 1;
		database.rows = 0;
		/* XXX it may not have picked a complete line */
		if(database_query(db, buf, _database_print, &database) != 0)
		{
			error_print(PROGNAME_DATABASE);
			continue;
		}
		fprintf(database.fp, "(%u rows)\n", database.rows);
	}
	database_delete(db);
	return 0;
}


/* database_load */
static Database * _database_load(char const * engine, char const * cfile,
		char const * section)
{
	Database * db;
	Config * config;

	if(cfile == NULL)
		section = NULL;
	if((config = config_new()) == NULL)
		return NULL;
	if(cfile != NULL && config_load(config, cfile) != 0)
		db = NULL;
	else
		db = database_new(engine, config, section);
	config_delete(config);
	return db;
}


/* database_print */
static int _database_print(void * data, int argc, char ** argv, char ** columns)
{
	DatabaseFile * database = data;
	int i;

	database->rows++;
	if(argc == 0)
		return 0;
	if(database->first != 0)
	{
		database->first = 0;
		for(i = 0; i < argc; i++)
			fprintf(database->fp, "|%s", columns[i]);
		fputs("|\n", database->fp);
	}
	for(i = 0; i < argc; i++)
		fprintf(database->fp, "|%s", argv[i]);
	fputs("|\n", database->fp);
	return 0;
}


/* database_query */
static int _database_query(char const * engine, char const * cfile,
		char const * section, int queryc, char * const queryv[])
{
	int ret = 0;
	DatabaseFile database;
	Database * db;
	int i;

	if((db = _database_load(engine, cfile, section)) == NULL)
	{
		error_print(PROGNAME_DATABASE);
		return 2;
	}
	database.fp = stdout;
	for(i = 0; i < queryc; i++)
	{
		database.first = 1;
		database.rows = 0;
		if((ret = database_query(db, queryv[i], _database_print,
						&database)) != 0)
		{
			error_print(PROGNAME_DATABASE);
			break;
		}
		printf("(%u rows)\n", database.rows);
	}
	database_delete(db);
	return ret;
}


/* usage */
static int _usage(void)
{
	fputs("Usage: " PROGNAME_DATABASE
			" -d engine [-C configuration [-S section]][query...]\n"
"  -d	Database engine to load\n"
"  -C	Connection file to load\n"
"  -S	Section of the connection file to use\n",
stderr);
	return 1;
}


/* public */
/* functions */
/* main */
int main(int argc, char * argv[])
{
	int o;
	char const * engine = NULL;
	char const * cfile = NULL;
	char const * section = NULL;

	while((o = getopt(argc, argv, "C:d:S:")) != -1)
		switch(o)
		{
			case 'C':
				cfile = optarg;
				break;
			case 'd':
				engine = optarg;
				break;
			case 'S':
				section = optarg;
				break;
			default:
				return _usage();
		}
	if(engine == NULL)
		return _usage();
	else if(optind < argc)
		return _database_query(engine, cfile, section,
				argc - optind, &argv[optind]);
	return (_database(engine, cfile, section) == 0) ? 0 : 2;
}
