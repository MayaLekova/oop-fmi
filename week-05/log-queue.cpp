#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <functional>
#include <iostream>

float random_float(float max = 1.0f) {
  float r = static_cast<float>(rand()) /
            static_cast<float>(RAND_MAX);
  return r * max;
}

template <typename T>
class DynamicArray {
 public:
  DynamicArray(size_t initial_size = 0);
  DynamicArray(const DynamicArray& other);
  ~DynamicArray();

  // mutators (both read- and write-)
  DynamicArray& operator=(const DynamicArray& other);
  void add(const T& element);

  size_t size() const {
    return filled_size;
  }

  // accessors (read-only); safe => performs bounds checks
  T& element_at(size_t index);
  const T& element_at(size_t index) const;

  // alternative to element_at - operator[]
  // unsafe => only asserts that index is in bounds
  T& operator[](size_t index);
  const T& operator[](size_t index) const;

 private:
  void grow(size_t new_size);
  void init(size_t size, size_t capacity);
  void copy_from(const DynamicArray<T>& other);
  void deinit();

  static constexpr unsigned int growth_factor = 1.6;

  static constexpr T INVALID_ELEMENT = T();

  T* elements;
  size_t capacity, filled_size;
};

// template <typename T>
// T DynamicArray<T>::INVALID_ELEMENT = T();

template <typename T>
DynamicArray<T>::DynamicArray(size_t initial_size) {
  init(initial_size, initial_size * growth_factor);
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other) {
  init(other.filled_size, other.capacity);
  copy_from(other);
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
  deinit();
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& other) {
  if (this == &other) return *this;   // avoid re-assigning the same object

  // Clean-up existing data.
  deinit();
  
  // Re-allocate as needed and copy data from the other object.
  init(other.filled_size, other.capacity);
  copy_from(other);

  return *this;
}

template <typename T>
void DynamicArray<T>::add(const T& element) {
  size_t new_size = ++filled_size;
  if (new_size * growth_factor > capacity) {
    grow(new_size);
  }

  elements[new_size - 1] = element;
}

template <typename T>
T& DynamicArray<T>::element_at(size_t index) {
  if (index < filled_size) {
    return this->operator[](index);
  }
  // if we use exceptions, we should throw one here
  // otherwise we can return the static INVALID_ELEMENT
  return const_cast<T&>(INVALID_ELEMENT);
}

template <typename T>
const T& DynamicArray<T>::element_at(size_t index) const {
  return const_cast<DynamicArray<T>*>(this)->element_at(index);
}

template <typename T>
T& DynamicArray<T>::operator[](size_t index) {
  assert(index < filled_size);
  return elements[index];
}

// private

template <typename T>
void DynamicArray<T>::grow(size_t new_size) {
  size_t new_capacity = new_size * growth_factor;
  T* new_elements = new T[new_capacity];

  // copy the old elements
  for (size_t i = 0; i < filled_size; ++i) {
    new_elements[i] = elements[i];
  }

  delete elements;  // Don't call the elements' destructors here!
  elements = new_elements;

  filled_size = new_size;
  capacity = new_capacity;
}

template <typename T>
void DynamicArray<T>::init(size_t size, size_t capacity) {
  this->capacity = capacity;
  this->filled_size = size;

  elements = new T[capacity];
}

template <typename T>
void DynamicArray<T>::copy_from(const DynamicArray<T>& other) {
  for (size_t i = 0; i < other.size(); ++i) {
    elements[i] = other.elements[i];
  }
}

template <typename T>
void DynamicArray<T>::deinit() {
  delete [] elements;
  elements = nullptr;
}

template <typename T>
using Operation = std::function<T(T)>;

template <typename T>
void map(DynamicArray<T>& array, Operation<T> op) {
  for (size_t i = 0; i < array.size(); ++i) {
    T& current = array.element_at(i);
    current = op(current);
  }
}

template <typename T>
using Predicate = std::function<bool(T)>;

template <typename T>
DynamicArray<T> filter(const DynamicArray<T>& array, Predicate<T> pred) {
  DynamicArray<T> filtered;

  for (size_t i = 0; i < array.size(); ++i) {
    const T& current = array.element_at(i);
    if (pred(current)) {
      filtered.add(current);
    }
  }

  return filtered;
}

void read_sensors(size_t size, DynamicArray<float>& out_temps) {
  for (size_t i = 0; i < size; ++i) {
    out_temps.add(random_float(45));
  }
}

void print_sensors(const DynamicArray<float>& temps) {
  for (size_t i = 0; i < temps.size(); ++i) {
    std::cout << temps.element_at(i) << " ";
  }
  std::cout << std::endl;
}

float decrease_by_3(float t) {
  return t - 3;
}

int main() {
  srand(static_cast<unsigned>(time(0)));

  DynamicArray<float> temperatures;
  read_sensors(100, temperatures);

  std::cout << "Valid temp (within bounds): " << temperatures.element_at(42) << std::endl;  
  std::cout << "Invalid temp (out of bounds): " << temperatures.element_at(142) << std::endl;

  std::cout << "Tempteratures - raw reads:" << std::endl;
  print_sensors(temperatures);

  std::cout << " ---------------------------- " << std::endl;

  // We have to call `map<float`> explicitly, as type T(T) (where T = float)
  // can't be deduced directly from the lambda.
  map<float>(temperatures, [](float x) -> float{ return x + 10; });

  // Here we know the prototype of the std::function, so `map`
  // can be called directly, without having to write `map<float>`.
  std::function<float(float)> decrease = decrease_by_3;
  map(temperatures, decrease);

  std::cout << "Tempteratures - after applying +10 and -3:" << std::endl;
  print_sensors(temperatures);

  std::cout << " ---------------------------- " << std::endl;

  DynamicArray<float> warmer_than_25;
  warmer_than_25 = filter<float>(temperatures, [](float x){ return x > 25; });

  std::cout << "Tempteratures > 25 deg - after filtering:" << std::endl;
  print_sensors(warmer_than_25);

  std::cout << " ---------------------------- " << std::endl;

  return 0;
}
