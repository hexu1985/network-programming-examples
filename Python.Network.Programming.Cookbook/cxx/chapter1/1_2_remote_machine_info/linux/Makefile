CXX = g++
CXXFLAGS = -Wall -std=c++11 -fsanitize=address -fno-omit-frame-pointer
LDFLAGS = 
LDLIBS = -lasan

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

