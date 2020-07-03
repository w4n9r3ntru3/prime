// FIXME
// this class is diabled because of missing parts

#ifdef BUCKET

/***********************************************************************

  FileName    [BucketList.h]

  Author      [Yang Chien Yi]

  This file defines the data structure "bucket list".

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

// FIXME raw pointers everywhere

class Node {
   public:
    // Constructor and destructor
    Node(const int& id) : _id(id), _prev(0), _next(0) {}
    ~Node() {}

    // Basic access methods
    int getId() const { return _id; }
    Node* getPrev() const { return _prev; }
    Node* getNext() const { return _next; }

    // Set functions
    void setId(const int& id) { _id = id; }
    void setPrev(Node* prev) { _prev = prev; }
    void setNext(Node* next) { _next = next; }

   private:
    int _id;      // id of the node (indicating the cell)
    Node* _prev;  // pointer to the previous node
    Node* _next;  // pointer to the next node
};

class BucketList {
   public:
    BucketList() : _Pmax(0), _maxGain(0), _iter(0) {}
    ~BucketList() {}

    void init(int Pmax) {
        _Pmax = Pmax;
        _list.reserve(2 * Pmax + 1);
        _maxGain = -Pmax;
    }

    void clean() {
        _list.clear();
        for (int i = 0, n = 2 * _Pmax + 1; i < n; ++i) {
            _list.push_back(0);
        }
    }

    // access function
    int getMaxGain() const { return _maxGain; }
    void summary() {
        std::cerr << std::endl << _iter << std::endl;
        std::cerr << "max gain: " << _maxGain << std::endl;
        for (int i = 0; i < 2 * _Pmax + 1; ++i) {
            std::cerr << i - _Pmax << ": ";
            Node* node = _list[i];
            while (node) {
                std::cerr << node->getId() << " ";
                node = node->getNext();
            }
            std::cerr << std::endl;
        }
        ++_iter;
    }

    // modifier
    Node* getTop() {
        Node* node = _list[_maxGain + _Pmax];
        Node* next = node->getNext();
        _list[_maxGain + _Pmax] = next;
        // the last in the bucket
        if (next == 0) {
            do {
                --_maxGain;
            } while (_list[_maxGain + _Pmax] == 0 &&
                     _maxGain > -_Pmax);  // find the new max gain
        } else {
            next->setPrev(0);
        }
        return node;
    }

    void insert(Node* node, int gain) {
        if (gain > _maxGain) {
            _maxGain = gain;
        }
        Node* origin = _list[gain + _Pmax];  // original node
        _list[gain + _Pmax] = node;
        node->setPrev(0);
        node->setNext(origin);  // connect
        if (origin) {
            origin->setPrev(node);  // connect
        }
    }

    void dec(Node* node, int gain) {
        Node* prev = node->getPrev();
        Node* next = node->getNext();
        if (prev) {
            prev->setNext(next);
        } else {
            _list[gain + _Pmax] = next;  // first in the bucket
        }
        if (next) {
            next->setPrev(prev);
        }
        // the last in the bucket
        if (prev == 0 && next == 0) {
            _list[gain + _Pmax] = 0;
            if (gain == _maxGain)
                --_maxGain;
        }
        insert(node, gain - 1);
    }

    void inc(Node* node, int gain) {
        if (gain == _maxGain) {
            ++_maxGain;
        }
        Node* prev = node->getPrev();
        Node* next = node->getNext();
        if (prev) {
            prev->setNext(next);
        } else {
            _list[gain + _Pmax] = next;  // first in the bucket
        }
        if (next) {
            next->setPrev(prev);
        }
        // the last in the bucket
        if (prev == 0 && next == 0) {
            _list[gain + _Pmax] = 0;
        }
        insert(node, gain + 1);
    }

   private:
    int _Pmax;
    std::vector<Node*> _list;
    int _maxGain;
    int _iter;
};

#endif
