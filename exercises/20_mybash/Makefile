PWD := $(shell pwd)

CC = gcc
CFLAGS = -Wall -I$(PWD)/include

OBJDIR := $(PWD)/obj
BINDIR := $(PWD)/bin

SUBDIRS := mysed myfile mywc mytrans

all: mybash

$(SUBDIRS):
	@mkdir -p $(OBJDIR)/$@
	$(CC) $(CFLAGS) -c src/$@/$@.c -o $(OBJDIR)/$@/$@.o
	$(CC) $(CFLAGS) -c src/$@/myhash.c -o $(OBJDIR)/$@/myhash.o 2>/dev/null || true
	ar rcs $(OBJDIR)/lib$@.a $(OBJDIR)/$@/$@.o $(OBJDIR)/$@/myhash.o 2>/dev/null || \
	ar rcs $(OBJDIR)/lib$@.a $(OBJDIR)/$@/$@.o

mybash: $(SUBDIRS)
	@mkdir -p $(OBJDIR)/mybash $(BINDIR)
	$(CC) $(CFLAGS) -c src/mybash/main.c -o $(OBJDIR)/mybash/main.o
	$(CC) $(CFLAGS) -o $(BINDIR)/mybash $(OBJDIR)/mybash/main.o \
		-L$(OBJDIR) -lmysed -lmywc -lmytrans -lmyfile
	cp $(BINDIR)/mybash ./mybash

clean:
	rm -rf $(OBJDIR) $(BINDIR) mybash

.PHONY: all $(SUBDIRS) mybash clean
