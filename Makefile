
CXX = g++
CXXFLAGS = -O3 -std=c++11 -Wall -Wextra -Werror
TARGETS = main.o Cell.o Grid.o MasterCell.o PrimeMan.o Force.o TreeNet.o union_find.o
BINARY = prime
FLAGS =

.PHONY: all clean

all:
	$(MAKE) clean
	$(MAKE) $(BINARY) -j

dbg:
	g++ src/main.cpp src/Cell.cpp src/Grid.cpp src/MasterCell.cpp src/PrimeMan.cpp -o dbg -std=c++11 -g

$(BINARY): $(TARGETS)
	$(CXX) $(CXXFLAGS) $? -o $(BINARY) $(FLAGS)

%.o: src/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(FLAGS)

clean:
	rm -f $(BINARY) $(TARGETS)

clean_dbg:
	rm -f dbg
