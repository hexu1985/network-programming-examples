CXX = g++
CXXFLAGS = -Wall -std=c++11 -I${HOME}/local/boost/include
LDFLAGS = -L${HOME}/local/boost/lib
LDLIBS = #-lboost_system

SOURCES = $(shell ls *.cpp)
PROGRAMS = $(patsubst %.cpp,%,$(SOURCES))

.PHONY: all 
all: $(PROGRAMS)

$(PROGRAMS): %: %.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o $(PROGRAMS)

