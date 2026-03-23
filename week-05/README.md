# Указатели към функции, std::function и ламбда изрази

## Указател към функция

Функциите в C++ имат адрес в паметта, точно като обектите. Можем да пазим този адрес в **указател към функция**.

```c++
int add(int a, int b) {
    return a + b;
}

int main() {
    int (*fp)(int, int) = add; // указател към функция, приемаща два int и връщаща int
    std::cout << fp(3, 4); // 7
}
```

Синтаксисът е малко тромав, затова е удобно да ползваме `using`:
```c++
using BinaryOp = int(*)(int, int);

BinaryOp fp = add;
fp(3, 4); // 7
```

`using` дефинира псевдоним за тип — точно като `typedef`, но с по-четим синтаксис. Работи и за не-указателни типове:
```c++
using Name = std::string; // псевдоним за прост тип
using Matrix = int[4][4]; // псевдоним за масив
using Predicate = bool(*)(int); // указател към функция bool(int)
using Transform = int(*)(int); // указател към функция int(int)
```

Особено полезно е когато типът се ползва на много места, тъй като смяната на сигнатурата изисква промяна само на едно място:
```c++
using Comparator = bool(*)(int, int);

void sort(int* arr, int n, Comparator cmp);
int  findIf(int* arr, int n, Comparator cmp);
// ако сигнатурата се промени, редактираме само using-а
```

Указателите към функции са удобни, когато искаме да подаваме поведение като параметър:
```c++
using Transform = int(*)(int);

void map(int* arr, int n, Transform transform) {
    for (int i = 0; i < n; i++) {
        arr[i] = transform(arr[i]);
    }
}

int doubleIt(int x) { return x * 2; }

int main() {
    int arr[] = {1, 2, 3, 4};
    map(arr, 4, doubleIt); // {2, 4, 6, 8}
}
```

## Проблемът с указателите към функции — не можем да обхващаме локални или други променливи

Указателят към функция е просто адрес. Той не носи никакъв контекст, няма как да запомним локални променливи.

```c++
using Transform = int(*)(int);
void map(int* arr, int n, Transform transform);

int main() {
    int factor = 5;

    // НЕ се компилира
    map(arr, n, [](int x) { return x * factor; });
}
```

За да заобиколим това с обикновена функция, трябва да ползваме глобална или статична променлива, което не е добра практика.  
Решението са ламбда изразите.

```c++
using Transform = int(*)(int);
void map(int* arr, int n, Transform transform);

int main() {
    int factor = 5;

    map(arr, n, [factor](int x) { return x * factor; });
}
```

Това е начинът да обхванем променливи, които ни трябва. В този случай обаче пак няма да се компилира, понеже `[factor](int x) { return x * factor; }` вече става друг тип (std::function), който не може да се присвои към указател на функция.

## Ламбда изрази

Ламбда изразът е анонимна функция, която може да **capture-ва** (захваща) променливи от обграждащия scope.

```
[ capture ] ( params ) -> return_type { body }
```

```c++
std::function<int(int)> square = [](int x) { return x * x; };
std::cout << square(5); // 25
```

`return_type` може да се пропусне, компилаторът го извежда сам.

### Capture

Тук е ключовата разлика спрямо обикновените функции.

**Capture по стойност** — копира стойността в момента на дефиниране на ламбдата:
```c++
double taxRate = 0.2;

std::function<double(double)> priceWithTax = [taxRate](double price) {
    return price * (1 + taxRate);
};

priceWithTax(100.0); // 120.0

taxRate = 0.5;
priceWithTax(100.0); // все още 120.0, защото копирахме taxRate в дефиницията на ламбдата
```

**Capture по референция** — ламбдата вижда текущата стойност на променливата:
```c++
std::vector<std::string> log;

std::function<void(const std::string&)> addToLog = [&log](const std::string& msg) {
    log.push_back(msg);
};

addToLog("User logged in");
addToLog("File opened");
```

**Смесен capture** — полезен когато имаме множество променливи:
```c++
int minAge = 18;
int maxAge = 65;
std::string blockedName = "Admin";

std::function<bool(const Person&)> isEligible = [minAge, maxAge, &blockedName](const Person& p) {
    return p.age >= minAge && p.age <= maxAge && p.name != blockedName;
};
```

| Синтаксис | Значение |
|-----------|----------|
| `[x]`     | capture-ва `x` по стойност |
| `[&x]`    | capture-ва `x` по референция |
| `[=]`     | capture-ва всичко по стойност |
| `[&]`     | capture-ва всичко по референция |
| `[=, &x]` | всичко по стойност, но `x` по референция |
| `[this]`  | capture-ва текущия обект (в методи) |

**Относно `[&]` — ако ламбдата надживее scope-а на захванатата променлива, оставаме с висяща референция към нещо, което вече е изтрито, така че ще имаме undefined behavior!**

```c++
auto makeLambda() {
    int x = 42;
    return [&x]() { return x; }; // x е унищожен след return
}
```

### Ламбди и указатели към функции

Ламбда **без capture** може имплицитно да се конвертира до указател към функция:

```c++
int (*fp)(int) = [](int x) { return x * 2; };
```

```c++
int factor = 3;
int (*fp)(int) = [factor](int x) { return x * factor; }; // не се компилира - с capture клауза не може
```

## std::function

`std::function` (от `<functional>`) е wrapper, който може да съдържа **всичко callable (всичко което може да се извика с оператор() )** — функция, ламбда (дори с capture), функтор.

```c++
#include <functional>

std::function<int(int, int)> op;

op = [](int a, int b) { return a + b; };
op(3, 4); // 7

int factor = 5;
op = [factor](int a, int b) { return (a + b) * factor; }; // работи
op(3, 4); // 35
```

Сега `map` може да приема всичко, което е callable:
```c++
void map(int* arr, int n, std::function<int(int)> transform) {
    for (int i = 0; i < n; i++) {
        arr[i] = transform(arr[i]);
    }
}

int main() {
    int arr[] = {1, 2, 3, 4};
    int bonus = 10;
    map(arr, 4, [bonus](int x) { return x + bonus; }); // {11, 12, 13, 14}
}
```
