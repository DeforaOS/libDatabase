TARGETS	= $(OBJDIR)pylint.log $(OBJDIR)tests.log
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

$(OBJDIR)pylint.log: pylint.sh
	./pylint.sh -P "$(PREFIX)" -- "$(OBJDIR)pylint.log"

$(OBJDIR)tests.log: python.sh tests.sh
	./tests.sh -P "$(PREFIX)" -- "$(OBJDIR)tests.log"

clean:
	$(RM) -- $(pylint.log_OBJS) $(tests.log_OBJS)
	./pylint.sh -c -P "$(PREFIX)" -- "pylint.log"
	./tests.sh -c -P "$(PREFIX)" -- "tests.log"

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
