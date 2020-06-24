#include <Node.h>
#include <safe.h>
#include <stdlib.h>
#include <union_find.h>

#include <iostream>
#include <memory>
#include <utility>

int main(int argc, char const* argv[]) {
    constexpr unsigned size = 1000;
    auto nodes = safe::vector<TreeNode>();
    auto pairs = safe::vector<std::pair<unsigned, unsigned>>();

    for (unsigned i = 0; i < size; ++i) {
        nodes.push_back(std::move(TreeNode(i)));
    }

    for (unsigned i = 0; i < size - 1; ++i) {
        for (unsigned j = i + 1; j < size; ++j) {
            pairs.push_back(std::make_pair(i, j));
        }
    }

    constexpr unsigned MAGIC = size * size;

    for (unsigned i = 0; i < MAGIC; ++i) {
        unsigned a = rand() % pairs.size(), b = rand() % pairs.size();
        std::swap(pairs[a], pairs[b]);
    }
    for (auto iter = pairs.begin(); iter != pairs.end(); ++iter) {
        std::cout << "[" << iter->first << ", " << iter->second << "] ";
    }
    std::cout << "\n";

    union_find(nodes, pairs);

    std::cout << nodes << "\n";

    return 0;
}
