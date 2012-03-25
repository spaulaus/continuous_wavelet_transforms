#!/bin/make
SHELL=/bin/bash

vpath %.cpp src/
vpath %.h include/

CINCLUDEDIRS = -Iinclude

CXX = g++
CXXFLAGS += -Wall $(CINCLUDEDIRS)
LDLIBS += -lsqlite3 -ldl -lpthread
c++SrcSuf = cpp

#Define Objects
SIGNALGENERATORO = SignalGenerator.o
#MOTHERWAVELETGENERATOR = MotherWaveletGenerator.o
MAINO = cwt.o

#List Objects
OBJS = $(MAINO) $(SIGNALGENERATORO) 
#$(MOTHERWAVELETGENERATOR)

PROGRAM = cwt

.SUFFIXES: .$(c++SrcSuf)

.phony: all clean
all: $(PROGRAM)

clean: 
	@echo "Cleaning..."
	@rm -f $(OBJS) $(PROGRAM) *~ src/*~ include/*~

$(PROGRAM): $(OBJS) 
	$(CXX) $(LDLIBS) $^ -o $@