//
// Created by Shutao Zhang on 5/1/21.
//

#pragma once

#include <vector>
#include <iostream>
#include <string>

using namespace std;

class SkipList {
public:
    // default SkipList has max levels of 1, just one doubly-linked list
    // Q: What does the keyword "explicit" do?
    explicit SkipList(int maxLevels = 1);
    // destructor
    virtual ~SkipList();
    // return true if successfully added, no duplicates
    bool insert(int item);
    // item deletion; return true if successfully removed
    bool erase(int item);
    // return true if found in SkipList
    bool contains(int item) const;

    friend ostream& operator<<(ostream& os, const SkipList& list);

private:
    struct SkipListNode {
        // Convenience constructor to create node, set its data, and set all pointers to nullptr
        explicit SkipListNode(int data);
        // data for SNode
        int data_;
        // link to next at same level
        SkipListNode* next_;
        // link to previous at same level
        SkipListNode* prev_;
        // link up one level
        SkipListNode* upLevel_;
        // link down one level
        SkipListNode* downLevel_;
    };
    // maximum # of levels of SkipList, levels are 0 to maxLevels-1
    int maxLevels_;
    // array of maxLevels_ SkipListNode pointers as head pointers. For example,
    // if maxLevels_ == 2, we'd have Heads[0] and Heads[1]. Dynamically allocated
    // by constructor.
    SkipListNode** heads_;
    // array of maxLevels_ SkipListNode pointers as tail pointers.
    SkipListNode** tails_;
    // given a pointer to a SkipListNode, place it before the given nextNode
    void addBefore(SkipListNode* newNode, SkipListNode* nextNode, int level);
    // return true 50% of time,
    // each node has a 50% chance of being at higher level
    bool alsoHigher() const;

    vector<SkipListNode*> find(int item) const;
    string levelToString(int level) const;
};

ostream& operator<<(ostream& os, const SkipList& list);
