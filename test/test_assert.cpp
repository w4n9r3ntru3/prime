#include <safe_assert.h>

void some_function() {
    safe::assert(0);
}

int main(int argc, char const* argv[]) {
    some_function();
    return 0;
}
