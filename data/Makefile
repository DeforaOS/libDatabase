TARGETS	= $(OBJDIR)libDatabase.pc
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

$(OBJDIR)libDatabase.pc: libDatabase.pc.in ../config.sh
	./pkgconfig.sh -P "$(PREFIX)" -- "$(OBJDIR)libDatabase.pc"

clean:
	$(RM) -- $(libDatabase.pc_OBJS)
	./pkgconfig.sh -c -P "$(PREFIX)" -- "libDatabase.pc"

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	./pkgconfig.sh -P "$(DESTDIR)$(PREFIX)" -i -- "$(OBJDIR)libDatabase.pc"

uninstall:
	./pkgconfig.sh -P "$(DESTDIR)$(PREFIX)" -u -- "libDatabase.pc"

.PHONY: all clean distclean install uninstall
