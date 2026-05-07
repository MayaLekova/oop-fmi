// вариант 1 - съхраняваме инстанцията като статична член-данна,
// по указател; създаваме я динамично
class Singleton {
 public:
  Singleton();
  Singleton& getInstance();

 private:
  Singleton(const Singleton& other) = delete;
  Singleton& operator=(const Singleton& other) = delete;

  static Singleton* instance;
};

// инициализация на статична член-данна на класа `Singleton`
Singleton* Singleton::instance = nullptr;

Singleton& Singleton::getInstance() {
  if (instance == nullptr) {
    // създаваме обекта при първото извикване на метода;
    // няма обаче кой да освободи паметта в края на прогрмата
    instance = new Singleton();
  }
  return *instance;
}
