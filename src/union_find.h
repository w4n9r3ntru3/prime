// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <iostream>
#include <memory>
#include <sstream>

// TODO handle union find and merging tree

// Merging Tree Node
template <typename T>
class Node {
   public:
    // constructor
    Node(T&& data) : data(std::move(data)), left(nullptr), right(nullptr) {}
    Node(T&& data, Node& left, Node& right)
        : data(std::move(data)), left(&left), right(&right) {}
    ~Node() {
        delete left;
        delete right;
    }

   private:
    // fields
    T data;
    Node *left, *right;
};