PREFIX	= /usr/local
DESTDIR	=
RM	?= rm -f
LN	?= ln -f
MKDIR	?= mkdir -p
INSTALL	?= install
INCLUDEDIR= $(PREFIX)/include


all:

clean:

distclean: clean

install:
	$(MKDIR) $(DESTDIR)$(INCLUDEDIR)/Database
	$(INSTALL) -m 0644 -- database.h $(DESTDIR)$(INCLUDEDIR)/Database/database.h

uninstall:
	$(RM) -- $(DESTDIR)$(INCLUDEDIR)/Database/database.h

.PHONY: all clean distclean install uninstall
