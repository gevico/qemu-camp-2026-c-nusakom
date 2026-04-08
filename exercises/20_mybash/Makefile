PWD := $(shell pwd)

CC = gcc
INC = -I$(PWD)/include

OBJDIR := $(PWD)/obj
BINDIR := $(PWD)/bin

.PHONY: all mysed myfile mywc mytrans mybash clean

all: mybash

mysed:
	@mkdir -p $(OBJDIR)/mysed
	$(CC) -Wall $(INC) -I$(PWD)/src/mysed -c src/mysed/mysed.c -o $(OBJDIR)/mysed/mysed.o
	ar rcs $(OBJDIR)/libmysed.a $(OBJDIR)/mysed/mysed.o

myfile:
	@mkdir -p $(OBJDIR)/myfile
	$(CC) -Wall $(INC) -I$(PWD)/src/myfile -c src/myfile/myfile.c -o $(OBJDIR)/myfile/myfile.o
	ar rcs $(OBJDIR)/libmyfile.a $(OBJDIR)/myfile/myfile.o

mywc:
	@mkdir -p $(OBJDIR)/mywc
	$(CC) -Wall $(INC) -I$(PWD)/src/mywc -c src/mywc/mywc.c -o $(OBJDIR)/mywc/mywc.o
	ar rcs $(OBJDIR)/libmywc.a $(OBJDIR)/mywc/mywc.o

mytrans:
	@mkdir -p $(OBJDIR)/mytrans
	$(CC) -Wall $(INC) -I$(PWD)/src/mytrans -c src/mytrans/myhash.c -o $(OBJDIR)/mytrans/myhash.o
	$(CC) -Wall $(INC) -I$(PWD)/src/mytrans -c src/mytrans/mytrans.c -o $(OBJDIR)/mytrans/mytrans.o
	ar rcs $(OBJDIR)/libmytrans.a $(OBJDIR)/mytrans/mytrans.o $(OBJDIR)/mytrans/myhash.o

mybash: mysed myfile mywc mytrans
	@mkdir -p $(OBJDIR)/mybash $(BINDIR)
	$(CC) -Wall $(INC) -c src/mybash/main.c -o $(OBJDIR)/mybash/main.o
	$(CC) -o $(BINDIR)/mybash $(OBJDIR)/mybash/main.o \
		-L$(OBJDIR) -lmysed -lmywc -lmytrans -lmyfile
	cp $(BINDIR)/mybash ./mybash

clean:
	rm -rf $(OBJDIR) $(BINDIR) mybash
