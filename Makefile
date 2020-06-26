
CXX = g++
CXXFLAGS = -O3 -std=c++11 -Wall -Wextra -Werror -flto
TARGETS = main.o Cell.o Grid.o MasterCell.o PrimeMan.o Node.o Tree.o MergeTree.o
BINARY = prime
FLAGS =

.PHONY: all clean

all:
	$(MAKE) clean
	$(MAKE) $(BINARY) -j

$(BINARY): $(TARGETS)
	$(CXX) $(CXXFLAGS) $? -o $(BINARY) $(FLAGS)

%.o: src/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ $(FLAGS)

clean:
	rm -f $(BINARY) $(TARGETS)
