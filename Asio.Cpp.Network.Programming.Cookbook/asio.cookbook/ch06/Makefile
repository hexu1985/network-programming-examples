
OS ?= $(shell uname -s)

RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -DASIO_STANDALONE 
INCLUDES = -I../../asio-1.12.2/include
LDFLAGS = -lpthread
LDPATH =

ifeq ($(OS), Windows_NT)
	LDFLAGS = -lwsock32 -lws2_32 #-lpthread 
endif

SOURCES = $(shell ls *.cpp)
PROGS = $(SOURCES:%.cpp=%)

all: $(PROGS)
	@echo "PROGS = $(PROGS)" 

clean:
	@echo "OS: $(OS)"
	$(RM) $(PROGS)

%: %.cpp
	$(CXX) -o $@ $(CXXFLAGS) $(INCLUDES) $^ $(LDFLAGS) $(LDPATH)
