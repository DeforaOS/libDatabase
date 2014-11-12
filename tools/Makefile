TARGETS	= $(OBJDIR)client
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
CC	= cc
CPPFLAGSF= -I ../include
CPPFLAGS=
CFLAGSF	= -W `pkg-config --cflags libSystem`
CFLAGS	= -Wall -g -O2
LDFLAGSF= `pkg-config --libs libSystem` -L ../src -Wl,-rpath,../src -lDatabase
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

client_OBJS = $(OBJDIR)client.o
client_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
client_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)client: $(client_OBJS)
	$(CC) -o $(OBJDIR)client $(client_OBJS) $(client_LDFLAGS)

$(OBJDIR)client.o: client.c
	$(CC) $(client_CFLAGS) -o $(OBJDIR)client.o -c client.c

clean:
	$(RM) -- $(client_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)

uninstall:

.PHONY: all clean distclean install uninstall
