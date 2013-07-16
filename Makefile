PACKAGE	= libDatabase
VERSION	= 0.0.0
SUBDIRS	= data doc include src tools
RM	= rm -f
LN	= ln -f
TAR	= tar -czvf


all: subdirs

subdirs:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE)) || exit; done

clean:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) clean) || exit; done

distclean:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) distclean) || exit; done

dist:
	$(RM) -r -- $(PACKAGE)-$(VERSION)
	$(LN) -s -- . $(PACKAGE)-$(VERSION)
	@$(TAR) $(PACKAGE)-$(VERSION).tar.gz -- \
		$(PACKAGE)-$(VERSION)/data/Makefile \
		$(PACKAGE)-$(VERSION)/data/libDatabase.pc.in \
		$(PACKAGE)-$(VERSION)/data/pkgconfig.sh \
		$(PACKAGE)-$(VERSION)/data/project.conf \
		$(PACKAGE)-$(VERSION)/doc/Makefile \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc.sh \
		$(PACKAGE)-$(VERSION)/doc/project.conf \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/Makefile \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/libDatabase-docs.xml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/project.conf \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/Makefile \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/Database.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/database.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/libDatabase-unused.sgml \
		$(PACKAGE)-$(VERSION)/doc/gtkdoc/tmpl/project.conf \
		$(PACKAGE)-$(VERSION)/include/Database.h \
		$(PACKAGE)-$(VERSION)/include/Makefile \
		$(PACKAGE)-$(VERSION)/include/project.conf \
		$(PACKAGE)-$(VERSION)/include/Database/database.h \
		$(PACKAGE)-$(VERSION)/include/Database/engine.h \
		$(PACKAGE)-$(VERSION)/include/Database/Makefile \
		$(PACKAGE)-$(VERSION)/include/Database/project.conf \
		$(PACKAGE)-$(VERSION)/src/database.c \
		$(PACKAGE)-$(VERSION)/src/Makefile \
		$(PACKAGE)-$(VERSION)/src/python/project.conf \
		$(PACKAGE)-$(VERSION)/src/python/Makefile \
		$(PACKAGE)-$(VERSION)/src/python/libDatabase.c \
		$(PACKAGE)-$(VERSION)/src/python/libDatabase.py \
		$(PACKAGE)-$(VERSION)/src/project.conf \
		$(PACKAGE)-$(VERSION)/src/engines/pdo.c \
		$(PACKAGE)-$(VERSION)/src/engines/pgsql.c \
		$(PACKAGE)-$(VERSION)/src/engines/sqlite2.c \
		$(PACKAGE)-$(VERSION)/src/engines/sqlite3.c \
		$(PACKAGE)-$(VERSION)/src/engines/template.c \
		$(PACKAGE)-$(VERSION)/src/engines/Makefile \
		$(PACKAGE)-$(VERSION)/src/engines/project.conf \
		$(PACKAGE)-$(VERSION)/tools/client.c \
		$(PACKAGE)-$(VERSION)/tools/Makefile \
		$(PACKAGE)-$(VERSION)/tools/project.conf \
		$(PACKAGE)-$(VERSION)/Makefile \
		$(PACKAGE)-$(VERSION)/config.h \
		$(PACKAGE)-$(VERSION)/config.sh \
		$(PACKAGE)-$(VERSION)/project.conf
	$(RM) -- $(PACKAGE)-$(VERSION)

install:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) install) || exit; done

uninstall:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) uninstall) || exit; done

.PHONY: all subdirs clean distclean dist install uninstall
