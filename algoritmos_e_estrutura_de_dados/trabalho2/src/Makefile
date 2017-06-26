# ------------------------------------------------
# Generic Makefile
#
# Author: yanick.rochon@gmail.com
# Date  : 2011-08-10
#
# Changelog :
#   2010-11-05 - first version
#   2011-08-10 - added structure : sources, objects, binaries
#                thanks to http://stackoverflow.com/users/128940/beta
#   2017-04-24 - changed order of linker params
# ------------------------------------------------

# project name (generate executable with this name)
TARGET   = university

# change these to proper directories where each file should be
SRCDIR   = src
INCDIR   = include
OBJDIR   = obj
BINDIR   = bin

CC       = gcc
# compiling flags here
CFLAGS   = -std=c11 -Wall -I $(INCDIR)

LINKER   = gcc

# linking flags here
LFLAGS   = -Wall -I$(INCDIR) -lm

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f

$(BINDIR)/$(TARGET): $(OBJECTS)
	@echo "$(LINKER) $(DEBUG) $(OBJECTS) $(LFLAGS) -o $@"
	@$(LINKER) $(DEBUG) $(OBJECTS) $(LFLAGS) -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@echo "$(CC) $(DEBUG) $(CFLAGS) -c $< -o $@"
	@$(CC) $(DEBUG) $(CFLAGS) -c $< -o $@

debug: DEBUG = -g3

debug: $(BINDIR)/$(TARGET)

.PHONY: clean
clean:
	@echo "$(rm) $(OBJECTS)"
	@$(rm) $(OBJECTS)

.PHONY: remove
remove: clean
	@echo "$(rm) $(BINDIR)/$(TARGET)"
	@$(rm) $(BINDIR)/$(TARGET)
