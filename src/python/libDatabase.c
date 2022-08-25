/* $Id$ */
/* Copyright (c) 2013-2015 Pierre Pronchery <khorben@defora.org> */
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



#include <Python.h>
#include "../../include/Database.h"


/* libDatabase */
/* private */
/* constants */
static char const _libdatabase_database_name[] = "libDatabase::Database";


/* prototypes */
/* Database */
static PyObject * _libdatabase_database_new(PyObject * self, PyObject * args);
static void _libdatabase_database_delete(PyObject * self);

static PyObject * _libdatabase_database_get_last_id(PyObject * self,
		PyObject * args);

static PyObject * _libdatabase_database_query(PyObject * self, PyObject * args);


/* variables */
static PyMethodDef _libdatabase_methods[] =
{
	{ "database_new", _libdatabase_database_new, METH_VARARGS,
		"Instantiates a Database object." },
	{ "database_get_last_id", _libdatabase_database_get_last_id,
		METH_VARARGS, "Obtain the ID of the latest row inserted." },
	{ "database_query", _libdatabase_database_query, METH_VARARGS,
		"Perform a query on the Database object." },
	{ NULL, NULL, 0, NULL }
};


/* public */
/* prototypes */
PyMODINIT_FUNC init_libDatabase(void);


/* functions */
/* init_libDatabase */
PyMODINIT_FUNC init_libDatabase(void)
{
	Py_InitModule("_libDatabase", _libdatabase_methods);
}


/* private */
/* functions */
/* Database */
/* libdatabase_database_new */
static PyObject * _libdatabase_database_new(PyObject * self, PyObject * args)
{
	Database * database;
	char const * engine;
	char const * section;

	if(!PyArg_ParseTuple(args, "ss", &engine, &section))
		return NULL;
	/* FIXME obtain the Config object */
	if((database = database_new(engine, config_new(), section)) == NULL)
		return NULL;
	return PyCapsule_New(database, _libdatabase_database_name,
			_libdatabase_database_delete);
}


/* libdatabase_database_delete */
static void _libdatabase_database_delete(PyObject * self)
{
	Database * database;

	if((database = PyCapsule_GetPointer(self, _libdatabase_database_name))
			== NULL)
		return;
	database_delete(database);
}


/* libdatabase_database_get_last_id */
static PyObject * _libdatabase_database_get_last_id(PyObject * self,
		PyObject * args)
{
	Database * database;
	long long ret;

	if(!PyArg_ParseTuple(args, "O", &self))
		return NULL;
	if((database = PyCapsule_GetPointer(self, _libdatabase_database_name))
			== NULL)
		return NULL;
	ret = database_get_last_id(database);
	return Py_BuildValue("L", ret);
}


/* libdatabase_database_query */
static PyObject * _libdatabase_database_query(PyObject * self, PyObject * args)
{
	Database * database;
	char const * query;
	int ret;

	if(!PyArg_ParseTuple(args, "Os", &self, &query))
		return NULL;
	if((database = PyCapsule_GetPointer(self, _libdatabase_database_name))
			== NULL)
		return NULL;
	/* FIXME implement the callbacks one way or another */
	ret = database_query(database, query, NULL, NULL);
	return Py_BuildValue("i", ret);
}
