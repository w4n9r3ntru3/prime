// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <execinfo.h>
#include <stdlib.h>

#include <iostream>
namespace safe {
void assert(bool argument) {
    if (argument) {
        return;
    }

    constexpr size_t CALL_STACK_SIZE = 128;
    void* callstack[CALL_STACK_SIZE];
    int frames = backtrace(callstack, CALL_STACK_SIZE);
    char** stacktrace = backtrace_symbols(callstack, frames);
    for (size_t i = 0; i < CALL_STACK_SIZE; ++i) {
        std::cout << stacktrace[i] << "\n";
    }
    free(stacktrace);
    exit(1);
}

}  // namespace safe
