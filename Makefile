
CXXC = g++
CXXCFLAGS = -std=c++11 -Wall -Wextra -flto -Iinclude -g -lstdc++
TARGETS = src/main.cpp \
		  src/core/Cell.cpp src/core/Grid.cpp src/core/MasterCell.cpp src/core/Chip.cpp \
		  src/grad/ConjugateGradient.cpp src/grad/analytical.cpp \
		  src/router/router3d.cpp src/router/cost_function.cpp \
		  src/quad/QuadUtil.cpp src/quad/QuadNode.cpp src/quad/QuadTree.cpp src/quad/QuadForest.cpp src/quad/Bounds.cpp \
		  src/core/mmapstream.cpp \

BINARY = prime
FLAGS =

.PHONY: all clean

all:
	$(MAKE) clean
	$(CXXC) $(CXXCFLAGS) -o $(BINARY) $(TARGETS) $(FLAGS)

clean:
	rm -f $(BINARY)
