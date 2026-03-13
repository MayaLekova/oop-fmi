#include <cstddef>

template <typename T>
class DynamicArray {
 public:
  DynamicArray(size_t initial_size = 0);
  DynamicArray(const DynamicArray& other);
  ~DynamicArray();

  // mutators (both read- and write-)
  DynamicArray& operator=(const DynamicArray& other);
  void add(const T& element);

  // accessors (read-only)
  T& element_at(size_t index);
  const T& element_at(size_t index) const;

 private:
  void grow(size_t new_size);
  void init(size_t size, size_t capacity);
  void deinit();

  static constexpr unsigned int growth_factor = 1.6;

  T* elements;
  size_t capacity, filled_size;
};

template <typename T>
DynamicArray<T>::DynamicArray(size_t initial_size) {
  init(initial_size, initial_size * growth_factor);
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other) {
  init(other.filled_size, other.capacity);
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
  deinit();
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& other) {
  if (this == &other) return *this;   // avoid re-assigning the same object

  deinit();
  
  init(other.filled_size, other.capacity);
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
void DynamicArray<T>::deinit() {
  delete [] elements;
  elements = nullptr;
}

int main() {
  return 0;
}
