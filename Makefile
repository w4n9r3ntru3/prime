
CXXC = g++
CXXCFLAGS = -std=c++11 -Wall -Wextra -flto -Iinclude -O3
TARGETS = src/main.o \
		  src/core/Cell.o src/core/Grid.o src/core/MasterCell.o src/core/Chip.o \
		  src/grad/ConjugateGradient.o \
		  src/tree/Node.o src/tree/Tree.o src/tree/MergeTree.o src/tree/union_find.o \
		  src/router/router3d.o src/router/cost_function.o \
		  src/quad/QuadUtil.o src/quad/QuadNode.o src/quad/QuadTree.o src/quad/QuadForest.o src/quad/QuadExt.o

BINARY = prime
FLAGS =

.PHONY: all clean

all:
	$(MAKE) clean
	$(MAKE) $(BINARY) -j

$(BINARY): $(TARGETS)
	$(CXXC) $(CXXCFLAGS) $? -o $(BINARY) $(FLAGS)

%.o: %.cpp
	$(CXXC) -c $(CXXCFLAGS) $< -o $@ $(FLAGS)

clean:
	rm -f $(BINARY) $(TARGETS)
