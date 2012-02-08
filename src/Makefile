##
## Makefile for JAMaster Server
##
## If you have any suggested changes, please submit a pull request :)
##

# Compiler to use
CC = g++

CFLAGS = -fpermissive -fwritable-strings
LDFLAGS =

# Directory for intermediate files (obj files)
OBJDIR = ../obj

# Directory to output the executable to
OUTDIR = ../bin

# Filename for the output executable
EXECUTABLE = jams

### OBJECT FILES #################################
# Classes
OBJECTS = Command.o NetAdr.o Q3OobMsg.o InfoString.o

# Static files
OBJECTS += net.o oob_cmds.o

# Jumbled mess
OBJECTS += config.o jams.o main.o rcon.o Server.o utils.o

# Platform-specific headers
OBJECTS += platform_win32.o platform_posix.o

# Libraries
OBJECTS += cJSON.o
##################################################

# The target depends on the output directory
$(EXECUTABLE): $(OBJECTS) | $(OUTDIR)
	$(CC) $(LDFLAGS) $(patsubst %,$(OBJDIR)/%,$(OBJECTS)) -o $(OUTDIR)/$@

# All the objects depend on the object directory
$(OBJECTS): | $(OBJDIR)

.c.o .cpp.o:
	$(CC) $(CFLAGS) -c $< -o $(OBJDIR)/$@

# Create the intermediate directory if it doesn't exist
$(OBJDIR):
	mkdir $(OBJDIR)

# Create the output directory if it doesn't exist
$(OUTDIR):
	mkdir $(OUTDIR)

# Clean out the contents of the intermediate and output directories
.PHONY: clean
clean:
	rm -rf $(OBJDIR)/*
	rm -rf $(OUTDIR)/*
