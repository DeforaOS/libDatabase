SUBDIRS	= engines
TARGETS	= libDatabase.a libDatabase.so.0.0 libDatabase.so.0 libDatabase.so
PREFIX	= /usr/local
DESTDIR	=
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF= -I ../include
CPPFLAGS=
CFLAGSF	= -W -fPIC `pkg-config --cflags libSystem`
CFLAGS	= -Wall -g -O2 -pedantic
LDFLAGS	= `pkg-config --libs libSystem`
AR	= ar
RANLIB	= ranlib
CCSHARED= $(CC) -shared
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: subdirs $(TARGETS)

subdirs:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE)) || exit; done

libDatabase_OBJS = database.o
libDatabase_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
libDatabase_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

libDatabase.a: $(libDatabase_OBJS)
	$(AR) -rc libDatabase.a $(libDatabase_OBJS)
	$(RANLIB) libDatabase.a

libDatabase.so.0.0 libDatabase.so.0 libDatabase.so: $(libDatabase_OBJS)
	$(CCSHARED) -o libDatabase.so.0.0 -Wl,-soname,libDatabase.so.0 $(libDatabase_OBJS) $(libDatabase_LDFLAGS)
	$(LN) -s -- libDatabase.so.0.0 libDatabase.so.0
	$(LN) -s -- libDatabase.so.0.0 libDatabase.so

database.o: database.c ../include/Database/database.h
	$(CC) $(libDatabase_CFLAGS) -c database.c

clean:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) clean) || exit; done
	$(RM) -- $(libDatabase_OBJS)

distclean:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) distclean) || exit; done
	$(RM) -- $(libDatabase_OBJS)
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) install) || exit; done
	$(MKDIR) $(DESTDIR)$(LIBDIR)
	$(INSTALL) -m 0644 libDatabase.a $(DESTDIR)$(LIBDIR)/libDatabase.a
	$(INSTALL) -m 0755 libDatabase.so.0.0 $(DESTDIR)$(LIBDIR)/libDatabase.so.0.0
	$(LN) -s -- libDatabase.so.0.0 $(DESTDIR)$(LIBDIR)/libDatabase.so.0
	$(LN) -s -- libDatabase.so.0.0 $(DESTDIR)$(LIBDIR)/libDatabase.so

uninstall:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) uninstall) || exit; done
	$(RM) -- $(DESTDIR)$(LIBDIR)/libDatabase.a
	$(RM) -- $(DESTDIR)$(LIBDIR)/libDatabase.so.0.0
	$(RM) -- $(DESTDIR)$(LIBDIR)/libDatabase.so.0
	$(RM) -- $(DESTDIR)$(LIBDIR)/libDatabase.so

.PHONY: all subdirs clean distclean install uninstall
