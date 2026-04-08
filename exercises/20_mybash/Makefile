PWD     := $(shell pwd)
CC      := gcc
CFLAGS  := -Wall -I$(PWD)/include
OBJDIR  := $(PWD)/obj
BINDIR  := $(PWD)/bin

SUBDIRS := mysed myfile mywc mytrans

# 每个子模块的库路径
LIB_mysed   := $(OBJDIR)/libmysed.a
LIB_myfile  := $(OBJDIR)/libmyfile.a
LIB_mywc    := $(OBJDIR)/libmywc.a
LIB_mytrans := $(OBJDIR)/libmytrans.a
ALL_LIBS    := $(LIB_mysed) $(LIB_myfile) $(LIB_mywc) $(LIB_mytrans)

# 传给子模块的链接命令（顶层统一管理）
LINKCMD := $(CC) -o $(BINDIR)/mybash $(OBJDIR)/mybash/main.o \
	-L$(OBJDIR) -lmysed -lmywc -lmytrans -lmyfile

.PHONY: all $(SUBDIRS) mybash clean

all: mybash

# 构建各子模块静态库
$(LIB_mysed):
	@mkdir -p $(OBJDIR)
	$(MAKE) -C src/mysed OBJDIR=$(OBJDIR) CC=$(CC) CFLAGS="$(CFLAGS) -I$(PWD)/src/mysed"

$(LIB_myfile):
	@mkdir -p $(OBJDIR)
	$(MAKE) -C src/myfile OBJDIR=$(OBJDIR) CC=$(CC) CFLAGS="$(CFLAGS) -I$(PWD)/src/myfile"

$(LIB_mywc):
	@mkdir -p $(OBJDIR)
	$(MAKE) -C src/mywc OBJDIR=$(OBJDIR) CC=$(CC) CFLAGS="$(CFLAGS) -I$(PWD)/src/mywc"

$(LIB_mytrans):
	@mkdir -p $(OBJDIR)
	$(MAKE) -C src/mytrans OBJDIR=$(OBJDIR) CC=$(CC) CFLAGS="$(CFLAGS) -I$(PWD)/src/mytrans"

# 构建 mybash 可执行文件
mybash: $(ALL_LIBS)
	@mkdir -p $(OBJDIR)/mybash $(BINDIR)
	$(CC) $(CFLAGS) -c src/mybash/main.c -o $(OBJDIR)/mybash/main.o
	$(CC) -o $(BINDIR)/mybash $(OBJDIR)/mybash/main.o \
		-L$(OBJDIR) -lmysed -lmywc -lmytrans -lmyfile
	cp $(BINDIR)/mybash ./mybash

clean:
	rm -rf $(OBJDIR) $(BINDIR) mybash
