// MoveSemantics.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "smart-pointers.h"

int main()
{
    int* some_ints = new int[3];
    some_ints[0] = 42;
    some_ints[1] = 63;
    some_ints[2] = 1337;

    UniquePtr<int> ptr1(some_ints);
    std::cout << ptr1.Get() << std::endl;

    ptr1.Reset(some_ints + 2);
    std::cout << ptr1.Get() << std::endl;
}
