/* $Id$ */
/* Copyright (c) 2015-2019 Pierre Pronchery <khorben@defora.org> */
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
