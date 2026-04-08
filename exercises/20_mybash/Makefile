PWD     := $(shell pwd)
CC      := gcc
INC     := -I$(PWD)/include \
           -I$(PWD)/src/mysed \
           -I$(PWD)/src/myfile \
           -I$(PWD)/src/mywc \
           -I$(PWD)/src/mytrans
CFLAGS  := -Wall $(INC)
OBJDIR  := $(PWD)/obj
BINDIR  := $(PWD)/bin

OBJS := \
    $(OBJDIR)/mysed.o \
    $(OBJDIR)/myfile.o \
    $(OBJDIR)/mywc.o \
    $(OBJDIR)/mytrans.o \
    $(OBJDIR)/myhash.o \
    $(OBJDIR)/main.o

.PHONY: all clean

all: mybash

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)

$(OBJDIR)/mysed.o:   src/mysed/mysed.c     | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/myfile.o:  src/myfile/myfile.c   | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/mywc.o:    src/mywc/mywc.c       | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/mytrans.o: src/mytrans/mytrans.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/myhash.o:  src/mytrans/myhash.c  | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/main.o:    src/mybash/main.c     | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

mybash: $(OBJS) | $(BINDIR)
	$(CC) -o $(BINDIR)/mybash $(OBJS)
	cp $(BINDIR)/mybash ./mybash

clean:
	rm -rf $(OBJDIR) $(BINDIR) mybash
