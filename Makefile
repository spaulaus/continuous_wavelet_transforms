#!/bin/make
SHELL=/bin/bash

#Define the virtual paths
vpath %.cpp src/
vpath %.hpp inc/
vpath %.o obj/

#Set some of the compile options
CXX = g++
CXXFLAGS = -fPIC -g -Wall $(CINCLUDEDIRS)
LDLIBS = -ldl -lpthread -lSignalGenerator -lGnuplotPipes -lMotherWaveletGenerator
CINCLUDEDIRS = -Iinc
c++SrcSuf = cpp

#Set the name of the program to be compiled
PROGRAM = example
VERSION = $(shell git describe --abbrev=0 --tags)

#Define Objects
MAINO = main.o
CWTO  = Cwt.o

PROGRAM = cwt

#Make the object list and prefix the object directory
OBJS = $(MAINO) $(CWTO)
OBJDIR = obj
OBJS_W_DIR = $(addprefix $(OBJDIR)/,$(OBJS))

.SUFFIXES: .$(c++SrcSuf)

all: $(OBJS_W_DIR) $(PROGRAM)

$(OBJS_W_DIR): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

$(PROGRAM): $(OBJS_W_DIR)
	$(CXX) $(CXXFLAGS) $(LDLIBS) $^ -o $@

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean so
clean: 
	@echo "Cleaning..."
	@rm -f $(OBJDIR)/* $(PROGRAM) *~ src/*~ include/*~
