/* $Id$ */
/* Copyright (c) 2015-2019 Pierre Pronchery <khorben@defora.org> */
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
/* TODO:
 * - really implement pooling */



#define database pgsql_database
#include "pgsql.c"
#undef database


/* protected */
/* prototypes */
static PgSQL * _pgsqlpool_init(Config * config, char const * section);


/* public */
/* variables */
DatabaseEngineDefinition database =
{
	"PostgreSQL Pool",
	NULL,
	_pgsqlpool_init,
	_pgsql_destroy,
	_pgsql_get_last_id,
	_pgsql_query,
	_pgsql_statement_new,
	_pgsql_statement_delete,
	_pgsql_statement_query
};


/* functions */
/* plug-in */
/* pgsqlpool_init */
static PgSQL * _pgsqlpool_init(Config * config, char const * section)
{
	const char pgsqlpool[] = "database::pgsqlpool";
	const char pgsql[] = "database::pgsql";

	/* XXX default to the regular master if detected */
	if(string_compare(section, pgsqlpool) == 0)
		section = pgsql;
	return _pgsql_init(config, pgsql);
}
