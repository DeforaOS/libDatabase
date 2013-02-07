TARGETS	= pdo.so pgsql.so sqlite2.so sqlite3.so template.so
PREFIX	= /usr/local
DESTDIR	=
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF= -I ../../include
CPPFLAGS=
CFLAGSF	= -W -fPIC
CFLAGS	= -Wall -g -O2 -pedantic
AR	= ar
RANLIB	= ranlib
CCSHARED= $(CC) -shared
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

pdo_OBJS = pdo.o
pdo_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS) `pkg-config --cflags libSystem`
pdo_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) `pkg-config --cflags libSystem`

pdo.so: $(pdo_OBJS)
	$(CCSHARED) -o pdo.so $(pdo_OBJS) $(pdo_LDFLAGS)

pgsql_OBJS = pgsql.o
pgsql_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) -I `pg_config --includedir` $(CFLAGSF) $(CFLAGS)
pgsql_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) -L `pg_config --libdir` -Wl,-rpath,`pg_config --libdir` `pg_config --libs` -lpq

pgsql.so: $(pgsql_OBJS)
	$(CCSHARED) -o pgsql.so $(pgsql_OBJS) $(pgsql_LDFLAGS)

sqlite2_OBJS = sqlite2.o
sqlite2_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS) `pkg-config --cflags sqlite libSystem`
sqlite2_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) `pkg-config --libs sqlite libSystem`

sqlite2.so: $(sqlite2_OBJS)
	$(CCSHARED) -o sqlite2.so $(sqlite2_OBJS) $(sqlite2_LDFLAGS)

sqlite3_OBJS = sqlite3.o
sqlite3_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS) `pkg-config --cflags sqlite3 libSystem`
sqlite3_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) `pkg-config --libs sqlite3 libSystem`

sqlite3.so: $(sqlite3_OBJS)
	$(CCSHARED) -o sqlite3.so $(sqlite3_OBJS) $(sqlite3_LDFLAGS)

template_OBJS = template.o
template_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
template_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

template.so: $(template_OBJS)
	$(CCSHARED) -o template.so $(template_OBJS) $(template_LDFLAGS)

pdo.o: pdo.c ../../include/Database/database.h ../../config.h
	$(CC) -D PREFIX=\"$(PREFIX)\" $(pdo_CFLAGS) -c pdo.c

pgsql.o: pgsql.c ../../include/Database/database.h
	$(CC) $(pgsql_CFLAGS) -c pgsql.c

sqlite2.o: sqlite2.c ../../include/Database/database.h
	$(CC) $(sqlite2_CFLAGS) -c sqlite2.c

sqlite3.o: sqlite3.c ../../include/Database/database.h
	$(CC) $(sqlite3_CFLAGS) -c sqlite3.c

template.o: template.c ../../include/Database/database.h
	$(CC) $(template_CFLAGS) -c template.c

clean:
	$(RM) -- $(pdo_OBJS) $(pgsql_OBJS) $(sqlite2_OBJS) $(sqlite3_OBJS) $(template_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Database/engine
	$(INSTALL) -m 0644 -- pdo.so $(DESTDIR)$(LIBDIR)/Database/engine/pdo.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Database/engine
	$(INSTALL) -m 0644 -- pgsql.so $(DESTDIR)$(LIBDIR)/Database/engine/pgsql.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Database/engine
	$(INSTALL) -m 0644 -- sqlite2.so $(DESTDIR)$(LIBDIR)/Database/engine/sqlite2.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Database/engine
	$(INSTALL) -m 0644 -- sqlite3.so $(DESTDIR)$(LIBDIR)/Database/engine/sqlite3.so

uninstall:
	$(RM) -- $(DESTDIR)$(LIBDIR)/Database/engine/pdo.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Database/engine/pgsql.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Database/engine/sqlite2.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Database/engine/sqlite3.so

.PHONY: all clean distclean install uninstall
