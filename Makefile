CC		= gcc
PROG		= rot
CFLAGS		= -Wall
# SRC		= $(wildcard shell/*.c) $(wildcard src/*.c)
# BINS		:= $(SRC:%.c=%.o)
BUILDDIR	= build/
SHELL		= /bin/bash
DESTDIR		= /usr/local/bin

ifneq ($(DEBUG),)
	CFLAGS := $(CFLAGS) -g
endif

.PHONY: clean
all: $(PROG)

$(PROG): # $(BINS)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(PROG).c $^ -o $(BUILDDIR)$@

%.o: %.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $? -o $@

clean:
	git clean -Xfq
	rm -rf build/$(PROG)

install:
	cp $(BUILDDIR)$(PROG) $(DESTDIR)/