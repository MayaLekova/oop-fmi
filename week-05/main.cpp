#include <cassert>
#include <iostream>
#include <functional>

template <typename T>
class DynamicArray {
 public:
  DynamicArray(size_t initial_capacity = 20);
  DynamicArray(const DynamicArray& other);
  ~DynamicArray();
  DynamicArray& operator=(const DynamicArray& other);

  size_t size() const {
    return filled_size;
  }

  void add(const T& element);
  T& element_at(size_t index);
  const T& element_at(size_t index) const;

 private:
  // Allocates the memory for the dynamic array
  void init(size_t size, size_t cap);
  // Copy the contents of the array
  void copy_elements(const DynamicArray& other);
  // Clean-up existing elements
  void deinit();
  void grow();

  static const int growth_factor = 2;
  static constexpr T INVALID_ELEMENT = T();

  T* elements;
  size_t filled_size, capacity;
};

template <typename T>
DynamicArray<T>::DynamicArray(size_t initial_capacity)
  : capacity(initial_capacity),
    filled_size(0),
    elements(nullptr)
{
  init(capacity, filled_size);
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& other) {
  init(other.capacity, other.filled_size);
  // copy the actual elements
  copy_elements(other);
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
  deinit();
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& other) {
  if (this == &other) {
    return *this;
  }

  deinit();
  init(other.capacity, other.filled_size);
  copy_elements(other);

  return *this;
}

template <typename T>
void DynamicArray<T>::add(const T& element) {
  if (filled_size + 1 <= capacity) {
    elements[filled_size] = element;
    ++filled_size;
    return;
  }

  // We grow the array
  grow();
  // Add the new element
  elements[filled_size] = element;
  ++filled_size;
}

template <typename T>
T& DynamicArray<T>::element_at(size_t index) {
  if (index < filled_size) {
    return elements[index];
  }

  return const_cast<T&>(INVALID_ELEMENT);
}

template <typename T>
const T& DynamicArray<T>::element_at(size_t index) const {
  if (index < filled_size) {
    return elements[index];
  }

  return INVALID_ELEMENT;
}

// private

template <typename T>
void DynamicArray<T>::init(size_t cap, size_t size) {
  filled_size = size;
  capacity = cap;
  elements = new T[capacity];
}

template <typename T>
void DynamicArray<T>::copy_elements(const DynamicArray& other) {
  assert(elements != nullptr);

  // Copy the elements using operator=, *NOT* strcpy
  for (size_t i = 0; i < other.filled_size; ++i) {
    elements[i] = other.elements[i];
  }
}

template <typename T>
void DynamicArray<T>::deinit() {
  delete [] elements;
  elements = nullptr;
  filled_size = 0;
  capacity = 0;
}

template <typename T>
void DynamicArray<T>::grow() {
  size_t new_capacity = capacity * growth_factor;
  // Allocate larger array
  T* new_elements = new T[new_capacity];

  // Copy the old elements in the new memory
  for (size_t i = 0; i < filled_size; ++i) {
    new_elements[i] = elements[i];
  }
  // Clean-up the old elements
  delete [] elements;
  // Store the new array pointer in `this` object
  elements = new_elements;
  capacity = new_capacity;
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

float random_float(float max = 1.0f) {
  float r = static_cast<float>(rand()) /
            static_cast<float>(RAND_MAX);
  return r * max;
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

float increase_by_20(float x) {
  return x + 20;
}

int main() {
  DynamicArray<float> temperatures;
  // write something to `temperatures`
  read_sensors(100, temperatures);

  std::cout << temperatures.size() << std::endl;
  print_sensors(temperatures);

  map<float>(temperatures, increase_by_20);
  std::cout << "---------------" << std::endl;
  print_sensors(temperatures);

  map<float>(temperatures, [](float x) -> float{ return x - 10; });
  std::cout << "---------------" << std::endl;
  print_sensors(temperatures);

  std::function<float(float)> decreaser = [](float x) -> float{ return x - 10; };
  map(temperatures, decreaser);

  return 0;
}
