template <typename T>
class BoundSet {
 public:
  BoundSet(size_t maxN)
    : N(maxN) {
    elements = new T[N];
    size = 0;
  }
  ~BoundSet() {
    delete [] elements;
  }
  // BoundSet(const BoundSet& other)
  // BoundSet& operator=(const BoundSet& other)

  void add(const T& element);
  bool member(const T& query_element) const;
  void remove(const T& element);

  // joining the sets
  BoundSet<T> operator+(const BoundSet<T>& right) const;
  BoundSet<T>& operator+=(const BoundSet<T>& right);

  // comparing the sets
  bool operator==(const BoundSet<T>& right) const;
  bool operator!=(const BoundSet<T>& right) const;

 private:
  T* elements;
  const size_t N;
  size_t size;
};

template <typename T>
BoundSet<T> BoundSet<T>::operator+(const BoundSet& right) const {
  BoundSet<T> result(N + right.N);
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
  size_t duplicates = 0;
  for (size_t j = 0; j < right.size; ++j) {
    if (member(right.elements[j])) ++duplicates;
  }

  if (size + right.size - duplicates > N) {
    std::cout << "Not enough capacity!" << std::endl;
    // throw an exception or signal an error
  }

  for (size_t j = 0; j < right.size; ++j) {
    if (!member(right.elements[j])) {
      add(right.elements[j]);
    }
  }

  return *this;
}

