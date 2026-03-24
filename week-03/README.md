# Член-функции. Указателят this. Static.
## Член-функции
Член-функциите са функции, дефинирани като част от даден клас. 
```c++
#include <iostream>

struct Student {
    char name[100];
    unsigned age;

    void setAge(unsigned new_age) {
        this = ...; // compile-time error (assignment to 'this' - anachronism)
        age = new_age;
    }

    void print() const {
        std::cout << "I am " << name << " - " << age << " years old.";
    }
};

int main() {
    Student st{ "Ivan", 25 };
    st.print();
}
```
Както се вижда член-функцията *print* може да бъде достъпена през обект от тип Student по същия начин, по който се достъпват и член-данните. Това, което се случва в действителност е,
че член-функцията *print* се превежда по време на компилация като външна функция с параметър указател към Student, който се казва **this** (по-долу ще говорим по-подробно за това).
</br> Кодът би изглеждал така:
```c++
struct Student {
    char name[100];
    unsigned age;
};

void Student::setAge(Student* const this, unsigned new_age) {
    this->age = new_age;
}

void Student::print(const Student* const this) const {
    std::cout << "I am " << this->name << " - " << this->age << " years old.";
}
```
## Указателят this
Както казахме по-горе, всяка член-функции в действителност е нормална външна функция с един допълнителен параметър - указателят **this**. Този указател сочи към конкретния
обект, от който е извикана дадената функция. Както се вижда, указателят this винаги е константен в смисъла, че не можем да променяме накъде сочи той. Тоест не можем да направим следното:
```c++
struct Student {
    char name[100];
    unsigned age;

    void setAge(unsigned new_age) {
        this = ...; // compile-time error (assignment to 'this' - anachronism)
        age = new_age;
    }

    void print() const {
        std::cout << "I am " << name << " - " << age << " years old.";
    }
};
```
В интерес на истината анахронизмът, за който става въпрос в текста на грешката е, че в най-старите стандарти на C++ подобно присвояване всъщност е било възможно, но е било премахнато от стандарта на
езика впоследствие.

### Константни член-функции
Член-функцията setAge няма ключовата дума const преди тялото ѝ. Как тогава и в нея не можем да променяме накъде сочи указателя this? </br>
Отговорът е прост - ключовата дума const преди тялото на член-функция определя това дали можем да модифицираме член-данните на един обект. Тоест, поглеждайки примера от по-горе:
```c++
#include <iostream>

struct Student {
    char name[100];
    unsigned age;

    void setAge(unsigned new_age) {
        // тази член-функция не е константа, което означава, че МОЖЕМ да променяме член-данните
        age = new_age;
    }

    void print() const {
        std::cout << "I am " << name << " - " << age << " years old.";
        // член-функцията е константна, което означава, че действително ще изглежда като void print(const Student* const this) {...}
        // тоест в нея НЕ МОЖЕМ да променяме член-данните
        // това дава компилационна грешка
        age = 35;
    }
};
```
Константния указател към обект (и константната референция) дават достъп само до константните член-функции и всички член-данни, но забраняват промяната им по всякакъв начин.
От друга страна, неконстантните указатели и референции дават достъп до всички член-функции (без значение константни или не) и всички член-данни, позволявайки промяната им.
```c++
struct Student {
    char name[100];
    unsigned age;

    void setAge(unsigned new_age) {
        age = new_age;
    }

    void print() const {
        std::cout << "I am " << name << " - " << age << " years old.";
    }
};

void f(const Student& s) {
    s.setAge(123); // compile-time error (setAge is not const)
    s.print(); // ok

    s.age = 123; // compile-time error (reference is const)
}

void g(Student& s) {
    s.setAge(123); // ok
    s.print(); // ok

    s.age = 123; // ok
}
```
## Static
В C++ можем да използваме ключовата дума **static** по 4 начина:
### Статична член-данна
Това е споделена член-данна от всички обекти от даден тип. В действителност това е еквивалентно на това да имаме глобална променлива, "скрита" от нашия клас. Това е различно от нестатичните член-данни, които са различни променливи за всеки един обект.
```c++
#include <iostream>

struct Student {
    static unsigned c;
    unsigned age;
};
unsigned Student::c = 0;

int main()
{
    Student st;
    st.c++; // 1
    st.age = 123;
    Student st2;
    st2.c++; // 2
    st2.age = 55;

    std::cout << st.c << " " << st2.c << std::endl; // 2 2
    std::cout << st.age << " " << st2.age << std::endl; // 123 55
}
```
Например тук статичната член-данна *c* е споделена от всички обекти от тип Student, тоест всяка промяна върху *c* от някой обект ще бъде видима и в останалите обекти от същия тип. </br>
Също така, задължително е да инициализираме статичната член-данна с някаква стойност. Не можем да направим това директно на същия ред като при нестатичните член-данни и затова инициализацията е по-особена откъм синтаксис.
### Статична член-функция
Припомняме, че всяка член-функция представлява действително външна функция с допълнителен параметър указателя към обекта, който я извиква. Затова, за да извикаме дадена член-функция, трябва да имаме конкретен обект,
от който да направим това. 
Има обаче член-функции, които могат да бъдат викани и без конкретен обект, тоест не са свързани с конкретен обект. Това са статичните член-функции:
```c++
struct Student {
    char name[100];
    unsigned age;

    void print() const {
        std::cout << "I am " << name << " - " << age << " years old.";
    }

    static void static_print() {
        std::cout << "I am a statically printed student";
    }
};
```
В действителност са външни функции, но без указател this:
```c++
struct Student {
    char name[100];
    unsigned age;
};

void Student::print(const Student* const this) {
    std::cout << "I am " << this->name << " - " << this->age << " years old.";
}

void Student::static_print() {
    std::cout << "I am a statically printed student";
}
```
От статични член-функции не можем да викаме нестатични член-функции (тъй като те изискват конкретен обект за извикването си), но пък можем да викаме статични член-функции.
Също можем да достъпваме статични член-данни, но не и нестатични такива.
```c++
struct Student {
    char name[100];
    unsigned age; 
    static unsigned c;

    static void f() {
        g(); // ok (static member function)
        c++; // ok (static member variable)

        print(); // compile-time error (non-static member function)
        age = 123; // compile-time error (non-static member variable)
    }

    static void g() {

    }

    void print() const {
        std::cout << "I am " << name << " - " << age << " years old.";
    }
};
```
### Статична променлива във функция
Нека разгледаме следната програма:
```c++
struct Counter {
    unsigned c = 0;
};

void increase_and_print_counter() {
    Counter counter;
    counter.c++;
    std::cout << counter.c << std::endl;
}

int main() {
    increase_and_print_counter(); // 1
    increase_and_print_counter(); // 1
    increase_and_print_counter(); // 1
    increase_and_print_counter(); // 1
}
```
Ясно е, че всеки път ще се принтира числото 1, тъй като при всяко извикване на функцията *increase_and_print_counter*, се създава нова променлива в тялото ѝ, в която изначално
член-данната *c* има стойност 0, увеличаваме я и я принтираме.
</br>
Бихме искали обаче да принтираме 1, после 2, после 3 и накрая 4. Един вариант да направим това е като имаме статична член-данна, понеже ще бъде споделена от всеки обект от тип Counter
и така бихме могли да постигнем желания ефект:
```c++
#include <iostream>

struct Counter {
    static unsigned c;
};
unsigned Counter::c = 0;

void increase_and_print_counter() {
    Counter counter;
    counter.c++;
    std::cout << counter.c << std::endl;
}

int main() {
    increase_and_print_counter(); // 1
    increase_and_print_counter(); // 2
    increase_and_print_counter(); // 3
    increase_and_print_counter(); // 4
}
```
Сега възниква следния проблем - създаваме 4 обекта, въпреки че нямаме нужда и от четирите. Можем да създадем само един обект при първото извикване на функцията и той да се използва
при всяко извикване на функцията. Именно за това се ползват статичните променливи във функции. </br>
Добавяйки ключовата дума *static* пред декларацията на променливата counter, въвеждаме следното поведение - променливата се създава САМО един път, при първото извикване на функцията в програмата,
а при всяко следващо извикване се използва създаденият при първото извикване обект. Понеже обектът е един, член-данната *c* също е една, което позволява да се увеличи първо до 1, после до 2, след това до 3 и накрая до 4:
```c++
#include <iostream>

struct Counter {
    unsigned c = 0;
};

void increase_and_print_counter() {
    static Counter counter;
    counter.c++;
    std::cout << counter.c << std::endl;
}

int main() {
    increase_and_print_counter(); // 1
    increase_and_print_counter(); // 2
    increase_and_print_counter(); // 3
    increase_and_print_counter(); // 4
}
```
### Глобална статична променлива
Засега няма да разглеждаме глобалните статични променливи, тъй като поне до към момент на курса ще се използват по напълно същия начин като нестатичните такива. Разлика между двете се появява, когато програмата ни е разположена в множество файлове (с което ще се занимаваме по-нататък през семестъра).
