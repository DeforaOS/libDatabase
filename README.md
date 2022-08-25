DeforaOS libDatabase
====================

About libDatabase
-----------------

libDatabase provides an abstraction layer for relational database management
systems (RDBMS) using SQL as the query language.

libDatabase depends on the DeforaOS libSystem library (version 0.4.3 or above),
which is found on the website for the DeforaOS Project:
<https://www.defora.org/>.


Compiling libDatabase
---------------------

libDatabase depends on the following components:

 * DeforaOS libSystem
 * An implementation of `make`
 * GTK-Doc for the API documentation
 * PostgreSQL's client libraries
 * SQLite 2
 * SQLite 3

With GCC, this should then be enough to compile and install libDatabase:

    $ make install

To install libDatabase in a dedicated directory, like `/path/to/libDatabase`:

    $ make PREFIX="/path/to/libDatabase" install


Generating Makefiles
--------------------

If necessary, it is possible to re-generate the Makefiles for the current
platform with `configure(1)` from the DeforaOS configure project, found at
<https://www.defora.org/os/project/16/configure>. The procedure is then as
follows:

    $ configure
    $ make

Please refer to the documentation of DeforaOS configure for further
instructions.

THe Makefiles provided target the NetBSD Operating System
<https://www.netbsd.org/> by default.


Python bindings
---------------

Part of libDatabase's API can also be accessed from within the Python
programming language. This binding is not compiled or installed by default.
After installing libDatabase as documented above, the Python interface can be
built and installed as follows:

    $ (cd src/python && make install)

Likewise, an alternate installation directory may be specified:

    $ (cd src/python && make PREFIX="/path/to/libDatabase" install)

Makefiles can be re-generated for the bindings as well:

    $ configure src/python


Distributing libDatabase
------------------------

DeforaOS libDatabase is subject to the terms of the 2-clause BSD license.
Please see the `COPYING` file for more information.


Caveats
-------

### Python bindings

The Python bindings are outdated and still assume Python 2.7.

