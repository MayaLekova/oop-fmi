#ifndef LINKED_LIST_H   // #ifdef
#define LINKED_LIST_H

// горните две директиви са т.нар. "include guards" (щитове),
// които да ни предпазят от двойно дефиниране на класа
// и неговите методи, ако някой "include-не" (включи)
// два пъти header файла.

template <typename D>
struct Element {
  D data;
  Element* next = nullptr;
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

  const Element<D>* Find(D data) const;         // O(n)
  Element<D>* Find(D data);                     // O(n)

  const Element<D>& First() const;              // O(1)

  void Print() const;

 private:
  Element<D>* first;
};

// Тук включваме имплементацията на шаблонните методи.
// Правим го след дефиницията на класа, за да се виждат
// дефинициите от самите имплементации. 
#include "linked-list.hpp"

// Ако класът ни не е шаблонен, дефиницията му ще отиде
// в header файл (.h), а имплементацията - в .cpp файл,
// който ще подаваме на самия компилатор заедно с основния
// файл за програмата (main.cpp или както се нарича файла
// с `main` функцията).

// По правило:
//  1. вклюваме с #include .h | .hpp файловете
//  2. компилираме (построяваме до изпълним код)
//     .cpp файловете

#endif  // LINKED_LIST_H
