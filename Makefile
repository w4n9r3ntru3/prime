
CXX = g++
CXXFLAGS = -O3 -std=c++11 -Wall -Wextra
TARGETS = main.o Cell.o Grid.o MasterCell.o PrimeMan.o TreeNet.o Node.o safe_assert.o
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
