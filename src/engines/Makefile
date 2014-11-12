TARGETS	= $(OBJDIR)pdo.so $(OBJDIR)pgsql.so $(OBJDIR)sqlite2.so $(OBJDIR)sqlite3.so $(OBJDIR)template.so
OBJDIR	=
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

pdo_OBJS = $(OBJDIR)pdo.o
pdo_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS) `pkg-config --cflags libSystem`
pdo_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) `pkg-config --cflags libSystem`

$(OBJDIR)pdo.so: $(pdo_OBJS)
	$(CCSHARED) -o $(OBJDIR)pdo.so $(pdo_OBJS) $(pdo_LDFLAGS)

pgsql_OBJS = $(OBJDIR)pgsql.o
pgsql_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) -I `pg_config --includedir` $(CFLAGSF) $(CFLAGS) `pkg-config --cflags libSystem`
pgsql_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) `pkg-config --cflags libSystem` -L `pg_config --libdir` -Wl,-rpath,`pg_config --libdir` -lpq

$(OBJDIR)pgsql.so: $(pgsql_OBJS)
	$(CCSHARED) -o $(OBJDIR)pgsql.so $(pgsql_OBJS) $(pgsql_LDFLAGS)

sqlite2_OBJS = $(OBJDIR)sqlite2.o
sqlite2_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS) `pkg-config --cflags sqlite libSystem`
sqlite2_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) `pkg-config --libs sqlite libSystem`

$(OBJDIR)sqlite2.so: $(sqlite2_OBJS)
	$(CCSHARED) -o $(OBJDIR)sqlite2.so $(sqlite2_OBJS) $(sqlite2_LDFLAGS)

sqlite3_OBJS = $(OBJDIR)sqlite3.o
sqlite3_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS) `pkg-config --cflags sqlite3 libSystem`
sqlite3_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) `pkg-config --libs sqlite3 libSystem`

$(OBJDIR)sqlite3.so: $(sqlite3_OBJS)
	$(CCSHARED) -o $(OBJDIR)sqlite3.so $(sqlite3_OBJS) $(sqlite3_LDFLAGS)

template_OBJS = $(OBJDIR)template.o
template_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS) `pkg-config --cflags libSystem`
template_LDFLAGS = $(LDFLAGSF) $(LDFLAGS) `pkg-config --libs libSystem`

$(OBJDIR)template.so: $(template_OBJS)
	$(CCSHARED) -o $(OBJDIR)template.so $(template_OBJS) $(template_LDFLAGS)

$(OBJDIR)pdo.o: pdo.c ../../include/Database/database.h ../../config.h
	$(CC) -D PREFIX=\"$(PREFIX)\" $(pdo_CFLAGS) -o $(OBJDIR)pdo.o -c pdo.c

$(OBJDIR)pgsql.o: pgsql.c ../../include/Database/database.h
	$(CC) $(pgsql_CFLAGS) -o $(OBJDIR)pgsql.o -c pgsql.c

$(OBJDIR)sqlite2.o: sqlite2.c ../../include/Database/database.h
	$(CC) $(sqlite2_CFLAGS) -o $(OBJDIR)sqlite2.o -c sqlite2.c

$(OBJDIR)sqlite3.o: sqlite3.c ../../include/Database/database.h
	$(CC) $(sqlite3_CFLAGS) -o $(OBJDIR)sqlite3.o -c sqlite3.c

$(OBJDIR)template.o: template.c ../../include/Database/database.h
	$(CC) $(template_CFLAGS) -o $(OBJDIR)template.o -c template.c

clean:
	$(RM) -- $(pdo_OBJS) $(pgsql_OBJS) $(sqlite2_OBJS) $(sqlite3_OBJS) $(template_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Database/engine
	$(INSTALL) -m 0755 $(OBJDIR)pdo.so $(DESTDIR)$(LIBDIR)/Database/engine/pdo.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Database/engine
	$(INSTALL) -m 0755 $(OBJDIR)pgsql.so $(DESTDIR)$(LIBDIR)/Database/engine/pgsql.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Database/engine
	$(INSTALL) -m 0755 $(OBJDIR)sqlite2.so $(DESTDIR)$(LIBDIR)/Database/engine/sqlite2.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Database/engine
	$(INSTALL) -m 0755 $(OBJDIR)sqlite3.so $(DESTDIR)$(LIBDIR)/Database/engine/sqlite3.so

uninstall:
	$(RM) -- $(DESTDIR)$(LIBDIR)/Database/engine/pdo.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Database/engine/pgsql.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Database/engine/sqlite2.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Database/engine/sqlite3.so

.PHONY: all clean distclean install uninstall
