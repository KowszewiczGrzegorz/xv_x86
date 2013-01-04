
ifdef CROSS
	CC=arm-unknown-linux-gnueabi-g++
else
	CC=g++
endif

CFLAGS=-W -Wall -Werror -lm -O3 -std=c++11
LDFLAGS=
EXEC=xv25_test 
OBJECTS=main xv25 pointsLibrary odometry webApi hash

OBJS = $(OBJECTS:%=$(OBJDIR)/%.o)

ARCH=x86
TOPDIR=..
SRCDIR = $(TOPDIR)/src
OBJDIR = $(TOPDIR)/obj/$(ARCH)
BINDIR = $(TOPDIR)/bin/$(ARCH)

ifdef NO_COLOR
	GREEN=
	RED=
	WHITE=
else
	GREEN:=\033[01;32m
	RED:=\033[01;31m
	WHITE:=\033[00m
endif

ifdef VERBOSE
	V=
else
	V=@
endif

all: $(BINDIR)/$(EXEC)

$(BINDIR)/$(EXEC): $(OBJS)
	$(V)test -d $(BINDIR) || mkdir -p $(BINDIR)
	@printf "$(GREEN)[BIN]$(WHITE) $@ \n"; 
	$(V)$(CC) -o $@ $^ $(LDFLAGS) 

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(V)test -d $(OBJDIR) || mkdir -p $(OBJDIR)
	@printf "$(GREEN)[CXX]$(WHITE) $<\n"; 
	$(V)$(CC) -c $< $(CFLAGS) -o $@

.PHONY: clean mrproper

clean:
	@printf "$(RED)[clean]$(WHITE)\n";
	$(V)rm -rf $(TOPDIR)/obj

mrproper: clean
	$(V)printf "$(RED)[mrproper]$(WHITE)\n";
	$(V)rm -rf $(TOPDIR)/bin
