#include "linked-list.h"
// Тук включваме дефиницията (интерфейса) на класа.

#include <iostream>

template <typename D>
LinkedList<D>::LinkedList() {
  first = nullptr;
}

template <typename D>
void LinkedList<D>::AddToStart(D num) {
  Element<D>* newElem = new Element<D>{num, first};
  first = newElem;
}

template <typename D>
void LinkedList<D>::AddAfter(D num, Element<D>* previous) {
  if (previous == nullptr) return;

  Element<D>* newElem = new Element<D>{num, previous->next};
  previous->next = newElem;
}

template <typename D>
bool LinkedList<D>::AddAfter(D num, D prevNum) {
  Element<D>* found = Find(prevNum);
  if (found) {
    Element<D>* newElem = new Element<D>{num, found->next};
    found->next = newElem;
    return true;
  }
  return false;
}

template <typename D>
const Element<D>& LinkedList<D>::First() const {
  // Ако списъкът е празен, можем да връщаме референция
  // към някакъв "празен" (нулев) елемент
  // или да хвърляме изключение.
  // if (first == nullptr) return DUMMY;

  return *first;  // Валидно е само ако списъкът не е празен!
}

template <typename D>
void LinkedList<D>::Print() const {
  Element<D>* current = first;
  while(current != nullptr) {
    std::cout << current->data << " ";
    current = current->next;
  }
  std::cout << std::endl;
}

template <typename D>
const Element<D>* LinkedList<D>::Find(D data) const {
  return const_cast<LinkedList<D>*>(this);
}

template <typename D>
Element<D>* LinkedList<D>::Find(D data) {
  Element<D>* current = first;
  while (current != nullptr) {
    if (current->data == data) return current;
    current = current->next;
  }
  return nullptr;
}
