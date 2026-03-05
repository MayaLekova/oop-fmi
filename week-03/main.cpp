/*
1. Да реализираме числов интервал - клас Interval,
  който съдържа следните данни:

  - две числа lower, upper - краища на отворения интервал (lower, upper);
  Казваме, че един интервал е валиден, когато lower <= upper.
  - статична член-данна INVALID_INTERVAL от тип Interval, която ще
  ползваме за отбелязване на невалидни интервали.
*/

// глобална променлива
int my_var = 42;

class Interval {
 public:
  Interval();
  Interval(int a, int b);

  void init(int a, int b);
  bool isValid() const {
    return lower <= upper;
  }

  Interval merge(const Interval& other);

  static Interval INVALID_INTERVAL;
  static int counter;

 private:
  int lower;
  int upper;
};

// инициализиране на статичните член-данни на класа
Interval Interval::INVALID_INTERVAL = Interval();
int Interval::counter = 0;

Interval::Interval() {
  lower = 0;
  upper = 0;
}

Interval::Interval(int a, int b) {
  init(a, b);
}

void Interval::init(int a, int b) {
  if (a <= b) {
    lower = a;
    upper = b;
  } else {
    lower = b;
    upper = a;
  }
}

Interval Interval::merge(const Interval& other) {
  // this->lower === lower
  int lowest = (lower < other.lower ? lower : other.lower);
  int highest = (upper > other.upper ? upper : other.upper);

  return Interval(lowest, highest);
}

// пример за друг клас, ComplexNumber с разделени декларация и дефиниция
// class ComplexNumber {
//   void Interval();
// };

// void ComplexNumber::Interval() {
//    ..
// }

/*
Да реализираме следните операции към него:

  - конструктор по подразбиране, който инициализира upper и lower с 0;
  - член-функция (метод) init, която приема две числа A и B и връща обект от тип
  Interval, който представлява интервал с краища числата A и B.
  - метод isValid, който проверява дали даден интервал е валиден.
*/

/*
  - метод `merge`, който приема два интервала и връща тяхното обединение.
  *забележка:* това обединение винаги съществува. 
  - метод `intersect`, който приема два интервала и връща сечението между тях.
  Ако сечението е празен интервал, трябва да връща невалидния интервал.
*/

/*
2. Да направим класа Interval шаблонен по типа на данните. Да проверим
  валидността му, като го използваме за цели и дробни числа.
*/

// дефинициите на член-функциите на класа Interval, когато
// започнем да го правим шаблонен:
// template <typename T>
// Interval<T> Interval<T>::merge(const Interval<T>& other) {...}

/*
3. Да реализираме структура Date, която съдържа следните данни:
  
  - три естествени числа year, month, day
  - глобален operator<, който приема две дати и връща булева
  стойност - дали първата дата предхожда втората. Ако някоя от датите е
  невалидна, връща false, ако съвпадат - също.
*/

struct Date {
  // TODO
};

bool operator<(Date a, Date b) {
  // TODO
  return false;
}

/*
4. Да демонстрираме използването на класа Interval с току-що създадената
  структура за дата.
*/

int main() {
  return 0;
}
