#include <string>

#include "linked-list.h"

using std::string;

class Animal {
 public:
  Animal(const string& _name)
    : name(_name)
  {}


  virtual string speak() const = 0;
  virtual bool likes(const string& food) const = 0;

 private:
  string name;
};

class Dog : public Animal {
 public:
  Dog(const string& _name)
    : Animal(_name) {}

  virtual string speak() const override {
    return "Bark!";
  }
  virtual bool likes(const string& food) const override {
    if (food == "meat") return true;
    if (food == "slippers") return true;
    return false;
  }
};

int main() {
  LinkedList<Animal*> zoo;

  zoo.AddToStart(new Dog("Шаро"));
  zoo.AddToStart(new Dog("Зиги"));
  zoo.AddToStart(new Dog("Скорострел"));

  return 0;
}
