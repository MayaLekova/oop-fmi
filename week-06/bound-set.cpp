#include <cstdlib>
#include <iostream>

// решение на задачата за множество с максимален брой елементи:
// https://drive.google.com/file/d/1hTyLu9E0hXLnmllrkZkcYqLtVlamYspi/view?usp=sharing

template <typename T>
class BoundSet {
 public:
  BoundSet(size_t N)
    : maxN(N) {
    elements = new T[maxN];
    size = 0;
  }
  ~BoundSet() {
    delete [] elements;
  }
  BoundSet(const BoundSet& other);
  BoundSet& operator=(const BoundSet& other);

  void add(const T& new_element);
  bool member(const T& query_element) const;
  void remove(const T& element);

  // оператори за сливане на множества
  BoundSet<T> operator+(const BoundSet<T>& right) const;
  BoundSet<T>& operator+=(const BoundSet<T>& right);

  // оператори за сравнение
  bool operator==(const BoundSet<T>& right) const;
  bool operator!=(const BoundSet<T>& right) const;

  // само за дебъгване, по-просто би било да имаме функция `print`
  template <typename S>
  friend std::ostream& operator<<(std::ostream& output_stream, const BoundSet<S>& bound_set);

 private:
  BoundSet();   // "скриваме" конструктора по подразбиране, така че обекти
                // от клас `BoundSet` да не могат да бъдат създавани без да
                // се подаде `maxN` за капацитет.

  // методи за поддръжка на елементите в контейнера
  void init_from(const BoundSet<T>& other);
  void deinit();

  // метод за докладване на недостъчен капацитет.
  // Като упражнение, може да го подаваме като функционален обект
  // при създаване на класа, а това да е само имплементация по подразбиране.
  void signal_error();

  T* elements = nullptr;
  const size_t maxN;  // капацитет
  size_t size;        // реално запълнения брой елементи
};

template <typename T>
BoundSet<T>::BoundSet(const BoundSet<T>& other)
  : maxN(other.maxN)
  , size(other.size)
{
  init_from(other);
}

template <typename T>
BoundSet<T>& BoundSet<T>::operator=(const BoundSet<T>& other) {
  if (&other == this) return *this;

  deinit();

  maxN = other.maxN;
  size = other.size;
  init_from(other);
}

template <typename T>
void BoundSet<T>::init_from(const BoundSet<T>& other) {
  elements = new T[maxN];
  for (size_t i = 0; i < size; ++i) {
    elements[i] = other.elements[i];
  }
}

template <typename T>
void BoundSet<T>::deinit() {
  if (!elements) {
    return;
  }
  delete [] elements;

  elements = nullptr;
}

template <typename T>
void BoundSet<T>::signal_error() {
  // тук бихме могли да хвърлим изключение (няколко теми по-нататък)
  // или да сигнализираме грешката по друг начин.
  std::cout << "Not enough capacity!" << std::endl;
}

template <typename T>
BoundSet<T> BoundSet<T>::operator+(const BoundSet& right) const {
  BoundSet<T> result(maxN + right.maxN);
  for (size_t i = 0; i < size; ++i) {
    result.add(elements[i]);
  }
  for (size_t j = 0; j < right.size; ++j) {
    if (!result.member(right.elements[j])) {
      result.add(right.elements[j]);
    }
  }

  return result;
}

template <typename T>
BoundSet<T>& BoundSet<T>::operator+=(const BoundSet& right) {
  // преброяваме дубликатите (елементите, които са вече в множеството)
  size_t duplicates = 0;
  for (size_t j = 0; j < right.size; ++j) {
    if (member(right.elements[j])) ++duplicates;
  }

  // проверка дали наличният капацитет ще ни е достатъчен
  if (size + right.size - duplicates > maxN) {
    signal_error();
    return *this;
  }

  // добавяме елементите без тези, които са вече в множеството
  for (size_t j = 0; j < right.size; ++j) {
    if (!member(right.elements[j])) {
      add(right.elements[j]);
    }
  }

  return *this;
}

template <typename T>
void BoundSet<T>::add(const T& new_element) {
  if (size + 1 > maxN) {
    signal_error();
    return;
  }

  elements[size] = new_element;
  ++size;
}

template <typename T>
bool BoundSet<T>::member(const T& query_element) const {
  // само едно срещане на елемента е достатъчно, няма нужда да
  // претърсваме цялото множество 
  for (size_t i = 0; i < size; ++i) {
    if (elements[i] == query_element) return true;
  }

  return false;
}

template <typename T>
void BoundSet<T>::remove(const T& element) {
  size_t found = size;
  for (size_t i = 0; i < size; ++i) {
    if (elements[i] == element) {   // елементът е намерен
      found = i;                    // отбелязваме къде
      break;
    }
  }

  // за да изтрием намерения елемент, ще го заменим с последния
  // (ако има такъв) и ще намалим размера на множеството
  if (found < size) {
    if (size > 1) {
      elements[found] = elements[size - 1];
    }
    --size;
  }
  // алтернативно можем да преместим всички елементи след намерения
  // с една позиция наляво; текущата имплементация е по-оптимизирана.
}

template <typename T>
std::ostream& operator<<(std::ostream& output_stream, const BoundSet<T>& bound_set) {
  output_stream << "("; 
  for (size_t i = 0; i < bound_set.size; ++i) {
    output_stream << bound_set.elements[i] << ", ";
  }
  output_stream << ")" << std::endl;

  return output_stream;
}

int main() {
  BoundSet<int> primes(10);
  primes.add(2);
  primes.add(3);
  primes.add(5);
  primes.add(7);

  std::cout << "  Primes: " << primes;

  // тестваме добавяне на 10 нечетни числа;
  // сумарното множество трябва да е с по-голям капацитет 
  BoundSet<int> odds(10);
  for (int i = 0; i < 10; ++i) {
    odds.add(i * 2 + 1);
  }
  std::cout << "  Odds: " << odds;

  BoundSet<int> against_all_odds = primes + odds;
  std::cout << "  Set disjunction: " << against_all_odds;

  // тестваме добавяне на естествените числа към оригиналното множество;
  // без дубликати, капацитетът би трябвало да е достатъчен.
  BoundSet<int> naturals(10);
  for (int i = 1; i <= 10; ++i) {
    naturals.add(i);
  }

  primes += naturals;
  std::cout << "  Primes and natural numbers: " << primes << std::endl;

  // добавяне на още един или няколко елемента
  // би трябвало да предизвика грешка;
  std::cout << "  adding an element out of the capacity:" << std::endl;
  primes.add(42);
  std::cout << "  adding an set out of the capacity:" << std::endl;
  primes += odds;

  // и оригиналното множество все още би трябвало да е в използваем вид:
  std::cout << "  The set should still be usable:" << std::endl;
  std::cout << primes;
  
  return 0;
}
