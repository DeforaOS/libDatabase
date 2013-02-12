/* $Id$ */
/* Copyright (c) 2013 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Database libDatabase */
/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */



#include <Python.h>
#include "Database.h"


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

	if((database = PyCapsule_GetPointer(self, _libdatabase_database_name))
			== NULL)
		return NULL;
	if(!PyArg_ParseTuple(args, ""))
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

	if((database = PyCapsule_GetPointer(self, _libdatabase_database_name))
			== NULL)
		return NULL;
	if(!PyArg_ParseTuple(args, "s", &query))
		return NULL;
	/* FIXME implement the callbacks one way or another */
	ret = database_query(database, query, NULL, NULL);
	return Py_BuildValue("i", ret);
}
