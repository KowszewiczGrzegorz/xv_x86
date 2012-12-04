CC=g++
CFLAGS=-W -Wall -Werror -lm -O3
LDFLAGS=
EXEC=xv25_test 
OBJECTS=main xv25 pointsLibrary odometry webApi hash

ARCH=x86
TOPDIR=..
SRCDIR = $(TOPDIR)/src
OBJDIR = $(TOPDIR)/obj/$(ARCH)
BINDIR = $(TOPDIR)/bin/$(ARCH)

GREEN:=\033[01;32m
RED:=\033[01;31m
WHITE:=\033[00m

OBJS = $(OBJECTS:%=$(OBJDIR)/%.o)

V=@

all: $(BINDIR)/$(EXEC)

$(BINDIR)/$(EXEC): $(OBJS)
	$(V)test -d $(BINDIR) || mkdir -p $(BINDIR)
	$(V)printf "$(GREEN)[BIN]$(WHITE) $@ \n"; 
	$(V)$(CC) -o $@ $^ $(LDFLAGS) 

$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	$(V)test -d $(OBJDIR) || mkdir -p $(OBJDIR)
	$(V)printf "$(GREEN)[CXX]$(WHITE) $<\n"; 
	$(V)$(CC) -c $< $(CFLAGS) -o $@

.PHONY: clean mrproper

clean:
	$(V)printf "$(RED)[clean]$(WHITE)\n";
	$(V)rm -rf $(TOPDIR)/obj

mrproper: clean
	$(V)printf "$(RED)[mrproper]$(WHITE)\n";
	$(V)rm -rf $(TOPDIR)/bin
