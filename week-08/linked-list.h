#ifndef LINKED_LIST_H
#define LINKED_LIST_H

template <typename D>
struct Element {
  // Element(int _data = 0, Element* _next = nullptr)

  D data;
  Element<D>* next = nullptr;
};

template <typename D>
class LinkedList {
 public:
  LinkedList();

  bool IsEmpty() const {
    return (first == nullptr);
  }

  void AddToStart(D data);
  void AddAfter(D data, Element<D>* previous);  // O(1)
  bool AddAfter(D data, D prevData);            // O(n)

  const Element<D>* Find(D query) const;         // O(n)
  Element<D>* Find(D query);                     // O(n)

  void Print() const;

 private:
  Element<D>* first;
};

#include "linked-list.hpp"

#endif  // LINKED_LIST_H
