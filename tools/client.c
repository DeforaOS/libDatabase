/* $Id$ */
/* Copyright (c) 2013 Pierre Pronchery <khorben@defora.org> */
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
#include <stdio.h>
#include <System.h>
#include "Database.h"

#ifndef PROGNAME
# define PROGNAME	"client"
#endif


/* client */
/* private */
/* types */
typedef struct _Client
{
	FILE * fp;
	int first;
	int rows;
} Client;


/* prototypes */
static int _client(char const * engine, char const * cfile,
		char const * section);

static int _usage(void);


/* functions */
/* client */
static int _client_print(void * data, int argc, char ** argv, char ** columns);

static int _client(char const * engine, char const * cfile,
		char const * section)
{
	Client client;
	Config * config;
	Database * db;
	char buf[BUFSIZ];

	if(cfile == NULL)
		section = NULL;
	if((config = config_new()) == NULL)
	{
		error_print(PROGNAME);
		return -1;
	}
	if((cfile != NULL && config_load(config, cfile) != 0)
			|| (db = database_new(engine, config, section)) == NULL)
	{
		error_print(PROGNAME);
		config_delete(config);
		return 2;
	}
	client.fp = stdout;
	while(fgets(buf, sizeof(buf), stdin) != NULL)
	{
		client.first = 1;
		client.rows = 0;
		/* XXX it may not have picked a complete line */
		if(database_query(db, buf, _client_print, &client) != 0)
		{
			error_print(PROGNAME);
			continue;
		}
		fprintf(client.fp, "(%u rows)\n", client.rows);
	}
	database_delete(db);
	return 0;
}

static int _client_print(void * data, int argc, char ** argv, char ** columns)
{
	Client * client = data;
	int i;

	client->rows++;
	if(argc == 0)
		return 0;
	if(client->first != 0)
	{
		client->first = 0;
		for(i = 0; i < argc; i++)
			fprintf(client->fp, "|%s", columns[i]);
		fputs("|\n", client->fp);
	}
	for(i = 0; i < argc; i++)
		fprintf(client->fp, "|%s", argv[i]);
	fputs("|\n", client->fp);
	return 0;
}


/* usage */
static int _usage(void)
{
	fputs("Usage: " PROGNAME " -d engine [-C configuration [-S section]]\n"
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
	if(engine == NULL || optind != argc)
		return _usage();
	return (_client(engine, cfile, section) == 0) ? 0 : 2;
}
