PREFIX	= /usr/local
DESTDIR	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install
INCLUDEDIR= $(PREFIX)/include


all:

clean:

distclean: clean

install:
	$(MKDIR) $(DESTDIR)$(INCLUDEDIR)/Database
	$(INSTALL) -m 0644 -- database.h $(DESTDIR)$(INCLUDEDIR)/Database/database.h
	$(MKDIR) $(DESTDIR)$(INCLUDEDIR)/Database
	$(INSTALL) -m 0644 -- engine.h $(DESTDIR)$(INCLUDEDIR)/Database/engine.h

uninstall:
	$(RM) -- $(DESTDIR)$(INCLUDEDIR)/Database/database.h
	$(RM) -- $(DESTDIR)$(INCLUDEDIR)/Database/engine.h

.PHONY: all clean distclean install uninstall
