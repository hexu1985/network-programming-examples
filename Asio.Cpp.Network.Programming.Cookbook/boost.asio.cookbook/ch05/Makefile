
OS ?= $(shell uname -s)

RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -std=c++11
INCLUDES = 
LDFLAGS = -lboost_filesystem -lboost_thread -lboost_system -lpthread -lcrypto -lssl
LDPATH =

ifeq ($(OS), Windows_NT)
	LDFLAGS = -lboost_thread-mt -lboost_system-mt -lpthread -lwsock32 -lws2_32 -lcrypto -lssl
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
