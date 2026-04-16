#include <iostream>

#include "linked-list.h"

template <typename D>
LinkedList<D>::LinkedList() {
  first = nullptr;
}

template <typename D>
void LinkedList<D>::AddToStart(D data) {
  Element<D>* newElem = new Element<D>{data, first}; // constructor
  first = newElem;
}

template <typename D>
void LinkedList<D>::AddAfter(D data, Element<D>* previous) {
  if (previous == nullptr) return;

  Element<D>* newElem = new Element<D>{data, previous->next};
  previous->next = newElem;
}

template <typename D>
bool LinkedList<D>::AddAfter(D data, D prevData) {
  Element<D>* found = Find(prevData);
  if (found) {
    Element<D>* newElem = new Element<D>{data, found->next};   // new Element(num, found->next)
    found->next = newElem;
    return true;
  }
  return false;
}


template <typename D>
void LinkedList<D>::Print() const {
  Element<D>* current = first;
  // for (Element* current = first; current != nullptr; current = current->next)
  while(current != nullptr) {
    std::cout << current->data << " ";
    current = current->next;
  }
  std::cout << std::endl;
}

template <typename D>
const Element<D>* LinkedList<D>::Find(D query) const {
  return const_cast<LinkedList<D>*>(this);
}

template <typename D>
Element<D>* LinkedList<D>::Find(D query) {
  Element<D>* current = first;
  while (current != nullptr) {
    if (current->data == query) return current;
    current = current->next;
  }
  return nullptr;
}

int test_linked_list() {
  LinkedList<int> numbers;
  numbers.AddToStart(5);
  numbers.AddToStart(3);
  numbers.AddToStart(2);

  const Element<int>* found = numbers.Find(3);
  if (found != nullptr) {
    std::cout << "Element was found" << std::endl;
  } else {
    std::cout << "Element NOT found" << std::endl;
  }

  numbers.Print();

  return 0;
}
