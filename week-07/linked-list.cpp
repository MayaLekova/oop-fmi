#include <iostream>

struct Element {
  // Element(int _data = 0, Element* _next = nullptr)

  int data;
  Element* next = nullptr;
};

class LinkedList {
 public:
  LinkedList();

  bool IsEmpty() const {
    return (first == nullptr);
  }

  void AddToStart(int num);
  void AddAfter(int num, Element* previous);  // O(1)
  bool AddAfter(int num, int prevNum);        // O(n)

  const Element* Find(int num) const;         // O(n)
  Element* Find(int num);                     // O(n)

  void Print() const;

 private:
  Element* first;
};

LinkedList::LinkedList() {
  first = nullptr;
}

void LinkedList::AddToStart(int num) {
  Element* newElem = new Element{num, first}; // constructor
  first = newElem;
}

void LinkedList::AddAfter(int num, Element* previous) {
  if (previous == nullptr) return;

  Element* newElem = new Element{num, previous->next};
  previous->next = newElem;
}

// returns if `prevNum` was found
// num: 4
// prevNum: 3
bool LinkedList::AddAfter(int num, int prevNum) {
  Element* found = Find(prevNum);   // Element{3}
  if (found) {
    Element* newElem = new Element{num, found->next};   // new Element(num, found->next)
    found->next = newElem;
    return true;
  }
  return false;
}


void LinkedList::Print() const {
  Element* current = first;
  // for (Element* current = first; current != nullptr; current = current->next)
  while(current != nullptr) {
    std::cout << current->data << " ";
    current = current->next;
  }
  std::cout << std::endl;
}

const Element* LinkedList::Find(int num) const {
  const Element* current = first;
  while (current != nullptr) {
    if (current->data == num) return current;
    current = current->next;
  }
  return nullptr;
}

Element* LinkedList::Find(int num) {
  Element* current = first;
  while (current != nullptr) {
    if (current->data == num) return current;
    current = current->next;
  }
  return nullptr;
}

int main() {
  LinkedList numbers;
  numbers.AddToStart(5);
  numbers.AddToStart(3);
  numbers.AddToStart(2);

  const Element* found = numbers.Find(3);
  if (found != nullptr) {
    std::cout << "Element was found" << std::endl;
  } else {
    std::cout << "Element NOT found" << std::endl;
  }

  numbers.Print();
}
