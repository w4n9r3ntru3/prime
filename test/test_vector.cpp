#include <iostream>

#include "safe_vector.h"

void test_const(const safe::vector<int> vec) {
    auto result = vec[1];
    std::cout << result.ok() << "\n";

    if (result.ok()) {
        std::cout << result.field() << "\n";
    }
}

int main(int argc, char const* argv[]) {
    auto vec = safe::vector<int>();

    vec.push_back(3);
    vec.push_back(9);

    auto result = vec[1];
    std::cout << result.ok() << "\n";

    if (result.ok()) {
        std::cout << result.field() << "\n";
        result.field() = -1;
    }

    test_const(vec);

    return 0;
}
