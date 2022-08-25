/* $Id$ */
/* Copyright (c) 2012-2019 Pierre Pronchery <khorben@defora.org> */
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



#ifndef LIBDATABASE_DATABASE_ENGINE_H
# define LIBDATABASE_DATABASE_ENGINE_H

# include <stdarg.h>
# include <stdint.h>
# include <System.h>
# include "database.h"


/* DatabaseEngine */
/* public */
/* types */
typedef struct _DatabaseEngine DatabaseEngine;
typedef struct _DatabaseEngineStatement DatabaseEngineStatement;

typedef struct _DatabaseEngineDefinition
{
	char const * name;
	char const * description;

	/* essential */
	DatabaseEngine * (*init)(Config * config, char const * section);
	void (*destroy)(DatabaseEngine * engine);

	/* accessors */
	int64_t (*get_last_id)(DatabaseEngine * engine);

	/* useful */
	int (*query)(DatabaseEngine * engine, char const * query,
			DatabaseCallback callback, void * data);

	/* prepared statements */
	DatabaseEngineStatement * (*statement_new)(DatabaseEngine * engine,
			char const * query);
	void (*statement_delete)(DatabaseEngine * engine,
			DatabaseEngineStatement * statement);
	int (*statement_query)(DatabaseEngine * engine,
			DatabaseEngineStatement * statement,
			DatabaseCallback callback, void * data,
			va_list args);
} DatabaseEngineDefinition;

#endif /* !LIBDATABASE_DATABASE_ENGINE_H */
