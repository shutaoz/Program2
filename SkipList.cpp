//
// Created by Shutao Zhang on 5/1/21.
//

#include "SkipList.h"
#include <random>
#include <cassert>

using namespace std;

SkipList::SkipList(int maxLevels) : maxLevels_{maxLevels}
{
    heads_ = new SkipListNode*[maxLevels_];
    tails_ = new SkipListNode*[maxLevels_];
    for (int i=0; i<maxLevels_; i++){
        heads_[i] = tails_[i] = nullptr;
    }
}

SkipList::~SkipList()
{
    delete [] heads_;
    delete [] tails_;
    heads_ = nullptr;
    tails_ = nullptr;
    maxLevels_ = 0;
}

bool SkipList::insert(int item)
{
    // find insertions points
    vector<SkipListNode*> insertionPoints = find(item);

    // check if item already in list
    if (insertionPoints[0] != nullptr && insertionPoints[0]->data_ == item) {
        return false;
    }

    bool insertNextLevel = true;
    int currentLevel = 0;
    SkipListNode* lowerLevelNode = nullptr;
    while (insertNextLevel && currentLevel < maxLevels_) {
        SkipListNode* newNode = new SkipListNode(item);

        //Insertion into list: special cases
        if (insertionPoints[currentLevel] == nullptr) {
            // 1. empty list
            if (tails_[currentLevel] == nullptr) {
                heads_[currentLevel] = newNode;
                tails_[currentLevel] = newNode;
                newNode->prev_ = nullptr;
                newNode->next_ = nullptr;
            } else {
                // 2. insertion at end of list
                tails_[currentLevel]->next_ = newNode;
                newNode->prev_ = tails_[currentLevel];
                tails_[currentLevel] = newNode;
                newNode->next_ = nullptr;
            }
        } else {
            // Insertion into list: general case
            addBefore(newNode, insertionPoints[currentLevel], currentLevel);
        }

        // Do level pointers
        newNode->downLevel_ = lowerLevelNode;
        newNode->upLevel_ = nullptr;
        if (lowerLevelNode != nullptr) {
            lowerLevelNode->upLevel_ = newNode;
        }

        insertNextLevel = alsoHigher();
        lowerLevelNode = newNode;
        currentLevel++;
    }

    return true;
}


bool SkipList::erase(int item)
{
    vector<SkipListNode*> insertionPoints = find(item);

    if (insertionPoints[0] == nullptr || insertionPoints[0]->data_ != item) {
        return false;
    }

    for (int level=0; level < maxLevels_; level++)
    {
        SkipListNode* temp = insertionPoints[level];

        if (temp == nullptr || temp->data_ != item) {
            break;
        }

        if (temp->prev_ == nullptr) {
            heads_[level] = temp->next_;
        } else {
            temp->prev_->next_ = temp->next_;
        }
        if (temp->next_ == nullptr) {
            tails_[level] = temp->prev_;
        } else {
            temp->next_->prev_ = temp->prev_;
        }
        temp->prev_ = temp->next_ = temp->upLevel_ = temp->downLevel_ = nullptr;
        delete temp;
    }
    return true;
}


bool SkipList::contains(int item) const
{
    // Not using find, as it does not have a speedup
    SkipListNode* curNode = heads_[maxLevels_-1];
    for (int curLevel = maxLevels_-1; curLevel >=0; curLevel--)
    {
        while (curNode != nullptr && curNode->data_ < item)
        {
            curNode = curNode->next_;
        }
        // Case 1: we stopped and found the item
        if (curNode != nullptr && curNode->data_ == item) {
            return true;
        }
        // Case 2: we didn't find the item, which means we went too far
        if (curLevel == 0) continue;

        if (curNode == nullptr) {
            if (tails_[curLevel] == nullptr) {
                curNode = heads_[curLevel-1];
            } else {
                curNode = tails_[curLevel]->downLevel_;
            }
        } else {
            // We're one node too far at this level; need to continue
            // search from the preceding node.
            if (curNode->prev_ == nullptr) {
                // This node is the first node on the level; need to start
                // at beginning of next level
                curNode = heads_[curLevel-1];
            } else {
                // Otherwise, back up one node and go down
                curNode = curNode->prev_->downLevel_;
            }
        }
    }
    return false;
}

void SkipList::addBefore(SkipListNode* newNode, SkipListNode* nextNode, int level)
{
    newNode->next_ = nextNode;
    newNode->prev_ = nextNode->prev_;
    // Special case: insert at front of list
    if (newNode->prev_ == nullptr) {
        heads_[level] = newNode;
    } else {
        newNode->prev_->next_ = newNode;
    }
    nextNode->prev_ = newNode;
}



bool SkipList::alsoHigher() const
{
    static default_random_engine generator;    // generator
    static bernoulli_distribution coin(0.5);  // distribution
    return coin(generator);
}

vector<SkipList::SkipListNode*> SkipList::find(int item) const
{
    vector<SkipListNode*> nextNodes(maxLevels_, nullptr);
    SkipListNode* curNode = heads_[maxLevels_-1];
    for (int curLevel = maxLevels_-1; curLevel >=0; curLevel--)
    {
        while (curNode != nullptr && curNode->data_ < item)
        {
            curNode = curNode->next_;
        }
        nextNodes[curLevel] = curNode;
        // Move curNode. If we're at level 0, then no need to do this
        if (curLevel == 0) continue;

        // if we reached a nullptr, then the previous level could be
        // empty, or the item could be greater than all items in the
        // previous level.
        if (curNode == nullptr) {
            if (tails_[curLevel] == nullptr) {
                curNode = heads_[curLevel-1];
            } else {
                curNode = tails_[curLevel]->downLevel_;
            }
        } else {
            // We're one node too far at this level; need to continue
            // search from the preceding node.
            if (curNode->prev_ == nullptr) {
                // This node is the first node on the level; need to start
                // at beginning of next level
                curNode = heads_[curLevel-1];
            } else {
                // Otherwise, back up one node and go down
                curNode = curNode->prev_->downLevel_;
            }
        }
    }

    return nextNodes;
}

string SkipList::levelToString(int level) const
{
    string theString;

    assert(level < maxLevels_);
    for (SkipListNode* curNode = heads_[level];
         curNode != nullptr;
         curNode = curNode->next_)
    {
        theString += to_string(curNode->data_);
        if (curNode->next_ != nullptr) {
            theString += ", ";
        }
    }
    if (heads_[level] == nullptr) {
        theString = "empty";
    }

    return theString;
}
template <typename T>
typename SquareContainer<T>::iterator
SquareContainer<T>::iterator::operator++()
{
    // Don't do anything if we're already past the end
    if (y < theContainer->ySize) {
        if (++x >= theContainer->xSize) {
            x = 0;
            y++;
        }
    }
    return *this;
}


// Postfix form of "next"
template <typename T>
typename SquareContainer<T>::iterator
SquareContainer<T>::iterator::operator++(int)
{
    iterator returnValue = *this;

    // Don't do anything if we're already past the end
    if (y < theContainer->ySize) {
        if (++x >= theContainer->xSize) {
            x = 0;
            y++;
        }
    }
    return returnValue;
}

// Returns the current item
template <typename T>
T& SkipList<T>::iterator::operator*()
{
    // If we're past the end, it's really an error. You could throw an
    // exception, if you like. Make sure you document the iterator's
    // behavior in these circumstances. Here, we just crap out.
    if (y >= theContainer->ySize) {
        cerr << "Attempt to dereference bad iterator; x=" << x
             << ", y=" << y << endl;
        exit(EXIT_FAILURE);
    } else
        return theContainer->contents[x][y];
}

// Comparison operators
template <typename T>
bool SkipList<T>::iterator::operator==(const iterator& rhs) const
{
    return (theContainer==rhs.theContainer) && (x==rhs.x) && (y==rhs.y);
}

template <typename T>
bool SkipList<T>::iterator::operator!=(const iterator& rhs) const
{
    return (theContainer!=rhs.theContainer) || (x!=rhs.x) || (y!=rhs.y);
}


SkipList::SkipListNode::SkipListNode(int data) : data_(data)
{
    next_ = prev_ = upLevel_ = downLevel_ = nullptr;
}

ostream& operator<<(ostream& os, const SkipList& list)
{
    for (int curLevel = list.maxLevels_-1; curLevel >=0; curLevel--)
    {
        os << "Level: " << curLevel << " -- "
           << list.levelToString(curLevel) << endl;
    }

    return os;
}
