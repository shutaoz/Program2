//
// Created by Shutao Zhang on 5/2/21.
//

#include <cstdlib>

using namespace std;

#include "SquareContainer.h"

// Prefix form of "next" operator
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
T& SquareContainer<T>::iterator::operator*()
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
bool SquareContainer<T>::iterator::operator==(const iterator& rhs) const
{
    return (theContainer==rhs.theContainer) && (x==rhs.x) && (y==rhs.y);
}

template <typename T>
bool SquareContainer<T>::iterator::operator!=(const iterator& rhs) const
{
    return (theContainer!=rhs.theContainer) || (x!=rhs.x) || (y!=rhs.y);
}
