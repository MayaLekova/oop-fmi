#include "../week-07/linked-list.h"

#include <string>
#include <iostream>

// Абстрактен клас `Animal`, представящ едно животно с неговото име.
// Методът `likes` е "чисто виртуален" (pure virtual), което
// прави невъзможно директното създаване на обекти от клас `Animal`.
// Ще създаваме обекти от наследниците - Sheep, Dog, ...
class Animal {
 public:
  Animal(const std::string& _name)
   : name(_name) {}

  virtual std::string speak() const;
  virtual bool likes(const std::string& food) const = 0;

 private:
  std::string name;
};

// имплементация по подразбиране - абстрактното животно
// не знае как да говори
std::string Animal::speak() const {
  return "Не мога да говоря =(";
}

class Sheep : public Animal {
 public:
  Sheep(const std::string& _name)
    : Animal(_name) {
    }

  // ключова дума `virtual` ще укаже на компилатора да търси
  // метод със същата сигнатура в (някой от) базовите класове
  // и да "презапише" имплементацията във виртуалната таблица
  // за наследника (Sheep).
  virtual std::string speak() const override;
  virtual bool likes(const std::string& food) const override;
};

class Dog : public Animal {
 public:
  Dog(const std::string& _name)
    : Animal(_name) {
  }

  virtual bool likes(const std::string& food) const override {
    return true;
  }
};

// virtual
std::string Sheep::speak() const {
  return "Искам да изям Генчо!";
}

// virtual
bool Sheep::likes(const std::string& food) const {
  if (food == "grass") return true;
  if (food == "lamb") return false; // hell no
  if (food == "мартеничка") return true;
  return false;
}

int main() {
  LinkedList<Animal*> zoo;
  zoo.AddToStart(new Sheep("Генчо"));
  std::cout << zoo.First().data->speak() << std::endl;

  zoo.AddToStart(new Dog("Шаро"));
  std::cout << zoo.First().data->speak() << std::endl;
  
  // За следващия път:
  // 1. Трябва да освободим паметта от zoo
  // 2. Да си направим итератор, който да дава последователен
  //    достъп до елементите на списъка.

  return 0;
}
