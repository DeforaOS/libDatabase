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



#include <stdio.h>
#include <System.h>
#include "Database.h"


int main(void)
{
	Config * config;
	Database * db;
	char buf[BUFSIZ];

	/* FIXME implement a usage screen */
	if((config = config_new()) == NULL
			|| (db = database_new("pgsql", config, NULL)) == NULL)
	{
		error_print("client");
		return 2;
	}
	while(fgets(buf, sizeof(buf), stdin) != NULL)
	{
		/* XXX it may not have picked a complete line */
		if(database_query(db, buf, NULL, NULL) != 0)
		{
			error_print("client");
			continue;
		}
		/* FIXME implement printing the results */
	}
	database_delete(db);
	return 0;
}
