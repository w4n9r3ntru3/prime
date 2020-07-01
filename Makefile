
CXX = g++-9
CXXFLAGS = -rdynamic -std=c++11 -Wall -Wextra -Werror -flto -g
TARGETS = main.o Cell.o Grid.o MasterCell.o Chip.o Node.o Tree.o MergeTree.o union_find.o router3d.o cost_function.o
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
