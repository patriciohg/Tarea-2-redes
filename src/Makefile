CC=gcc
CXX=g++
RM=rm -f

BIN=../Server

INCLUDES=-I includes
LDFLAGS=
LDLIBS=

CXXFLAGS=-std=c++14 -Wall

DIR_OBJ=objs

SRCS=$(wildcard *.cc)

OBJS=$(patsubst %.cc,$(DIR_OBJ)/%.o,$(SRCS))


all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDLIBS) $(INCLUDES)

$(DIR_OBJ)/%.o: %.cc
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(LDLIBS) $(LDFLAGS) $(INCLUDES)

clean:
	$(RM) $(OBJS)
	$(RM) *~

distclean: clean
	$(RM) $(BIN)

.PHONY: all clean distclean
	
