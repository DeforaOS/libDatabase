TARGETS	= libDatabase.pc
PREFIX	= /usr/local
DESTDIR	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -p
INSTALL	= install


all: $(TARGETS)

libDatabase.pc: libDatabase.pc.in ../config.sh
	./pkgconfig.sh -P "$(PREFIX)" -- "libDatabase.pc"

clean:
	$(RM) -- $(libDatabase.pc_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	./pkgconfig.sh -P "$(DESTDIR)$(PREFIX)" -i -- "libDatabase.pc"

uninstall:
	./pkgconfig.sh -P "$(DESTDIR)$(PREFIX)" -u -- "libDatabase.pc"

.PHONY: all clean distclean install uninstall
