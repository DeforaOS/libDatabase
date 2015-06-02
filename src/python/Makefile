MKDIR	= mkdir -m 0755 -p
INSTALL	= install
RM	= rm -f
TARGETS	= $(OBJDIR)_libDatabase.so
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF= -I ../../include
CPPFLAGS=
CFLAGSF	= -W `pkg-config --cflags python-2.7 libDatabase` -fPIC
CFLAGS	= -Wall -g -O2
LDFLAGSF= `pkg-config --libs python-2.7 libDatabase`
LDFLAGS	= -L.. -L$(LIBDIR) -Wl,-rpath,$(LIBDIR)
AR	= ar
RANLIB	= ranlib
CCSHARED= $(CC) -shared
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

_libDatabase_OBJS = $(OBJDIR)libDatabase.o
_libDatabase_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
_libDatabase_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)_libDatabase.so: $(_libDatabase_OBJS)
	$(CCSHARED) -o $(OBJDIR)_libDatabase.so $(_libDatabase_OBJS) $(_libDatabase_LDFLAGS)

$(OBJDIR)libDatabase.o: libDatabase.c
	$(CC) $(_libDatabase_CFLAGS) -o $(OBJDIR)libDatabase.o -c libDatabase.c

clean:
	$(RM) -- $(_libDatabase_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages
	$(INSTALL) -m 0755 $(OBJDIR)_libDatabase.so $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/_libDatabase.so
	$(MKDIR) $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages
	$(INSTALL) -m 0644 libDatabase.py $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/libDatabase.py

uninstall:
	$(RM) -- $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/_libDatabase.so
	$(RM) -- $(DESTDIR)$(PREFIX)/lib/python2.7/site-packages/libDatabase.py

.PHONY: all clean distclean install uninstall
