TARGETS	= pgsql.so sqlite2.so sqlite3.so template.so
PREFIX	= /usr/local
DESTDIR	=
LIBDIR	= $(PREFIX)/lib
CC	?= cc
CPPFLAGSF?=
CPPFLAGS?=
CFLAGSF	= -W -fPIC
CFLAGS	= -Wall -g -O2 -pedantic
AR	?= ar
RANLIB	?= ranlib
CCSHARED?= $(CC) -shared
RM	?= rm -f
LN	?= ln -f
MKDIR	?= mkdir -p
INSTALL	?= install


all: $(TARGETS)

pgsql_OBJS = pgsql.o
pgsql_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) -I `pg_config --includedir` $(CFLAGSF) $(CFLAGS)
pgsql_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) -L `pg_config --libdir` -Wl,-rpath,`pg_config --libdir` `pg_config --libs` -lpq

pgsql.so: $(pgsql_OBJS)
	$(CCSHARED) -o pgsql.so $(pgsql_OBJS) $(pgsql_LDFLAGS)

sqlite2_OBJS = sqlite2.o
sqlite2_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS) `pkg-config --cflags sqlite`
sqlite2_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) `pkg-config --libs sqlite`

sqlite2.so: $(sqlite2_OBJS)
	$(CCSHARED) -o sqlite2.so $(sqlite2_OBJS) $(sqlite2_LDFLAGS)

sqlite3_OBJS = sqlite3.o
sqlite3_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS) `pkg-config --cflags sqlite3`
sqlite3_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) `pkg-config --libs sqlite3`

sqlite3.so: $(sqlite3_OBJS)
	$(CCSHARED) -o sqlite3.so $(sqlite3_OBJS) $(sqlite3_LDFLAGS)

template_OBJS = template.o
template_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
template_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

template.so: $(template_OBJS)
	$(CCSHARED) -o template.so $(template_OBJS) $(template_LDFLAGS)

pgsql.o: pgsql.c database.h
	$(CC) $(pgsql_CFLAGS) -c pgsql.c

sqlite2.o: sqlite2.c database.h
	$(CC) $(sqlite2_CFLAGS) -c sqlite2.c

sqlite3.o: sqlite3.c database.h
	$(CC) $(sqlite3_CFLAGS) -c sqlite3.c

template.o: template.c database.h
	$(CC) $(template_CFLAGS) -c template.c

clean:
	$(RM) -- $(pgsql_OBJS) $(sqlite2_OBJS) $(sqlite3_OBJS) $(template_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Database/database
	$(INSTALL) -m 0644 -- pgsql.so $(DESTDIR)$(LIBDIR)/Database/database/pgsql.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Database/database
	$(INSTALL) -m 0644 -- sqlite2.so $(DESTDIR)$(LIBDIR)/Database/database/sqlite2.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Database/database
	$(INSTALL) -m 0644 -- sqlite3.so $(DESTDIR)$(LIBDIR)/Database/database/sqlite3.so

uninstall:
	$(RM) -- $(DESTDIR)$(LIBDIR)/Database/database/pgsql.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Database/database/sqlite2.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Database/database/sqlite3.so

.PHONY: all clean distclean install uninstall
