#!/bin/make
SHELL=/bin/bash

vpath %.cpp src/
vpath %.hpp include/

CINCLUDEDIRS = -Iinclude

CXX = g++
CXXFLAGS += -Wall $(CINCLUDEDIRS)
LDLIBS += -ldl -lpthread -lmotherwavelet -lsignalgen
c++SrcSuf = cpp

#Define Objects
MAINO = ContinuousWaveletTransform.o

#List Objects
OBJS = $(MAINO)

PROGRAM = cwt

.SUFFIXES: .$(c++SrcSuf)

.phony: all clean
all: $(PROGRAM)

clean: 
	@echo "Cleaning..."
	@rm -f $(OBJS) $(PROGRAM) *~ src/*~ include/*~

$(PROGRAM): $(OBJS) 
	$(CXX) $(LDLIBS) $^ -o $@