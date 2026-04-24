#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

// Функция, която принтира на стандартния изход съдържанието
// на текстов файл с име `filename`, четейки по "думи" (tokens).
// Огражда всяка дума се огражда с квадратни скоби [] за по-нагледно.
// Връща `false`, ако файлът не е намерен или нямаме достъп
// за четене до него. 
bool read_file_by_words(const std::string& filename) {
  std::ifstream file(filename, std::ios::in);
  if (!file.is_open()) {
    std::cout << "Файлът не е намерен или нямаме права за четене.";
    return false;
  }

  std::string token;

  // Проверяваме дали файлът е отворен успешно
  while(!file.eof()) {
    file >> token;
    std::cout << "[ " << token << " ]\n";
  }
  file.close();

  return true;
}

// Помощна структура, в която ще прочитаме данните от един ред
// във формат "6: 10"
struct Histogram {
  int grade;
  unsigned int count;
};

// Фунцкия, която чете файл по редове и извежда на конзолата
// всеки ред с дължината му. Ако може да прочете хистограма от реда
// извежда данните от нея по начин: "Оценка 6 имат 20 ученика."
bool read_file_by_lines(const std::string& filename) {

  // Отваряме файла за четене:
  std::ifstream file(filename, std::ios::in);

  // Можем да комбинираме няколко възможни флага, вкл. за писане (`in`),
  // за добавяне (`app`), за двоичен режим на работа (`binary`) и други:
  // std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::out);
  // https://en.cppreference.com/cpp/io/ios_base/openmode

  // Проверяваме дали файлът е отворен успешно
  if (!file.is_open()) {
    std::cout << "грешка при четене на файла;"
      " нямаме права или не е намерен" << std::endl;
    return false;
  }

  // Докато не сме стигнали края на файла:
  while (!file.eof()) {
    // Друга проверка, която проверява и флаговете за грешки, би била:
    // [file.good()](https://en.cppreference.com/cpp/io/basic_ios/good)
    // Възможните флагове, които могат да се включат по време на работа
    // с файла или закачения за него поток (stream), за да сигнализират
    // грешки или достигнат край на файла:
    // https://en.cppreference.com/cpp/io/basic_ios/rdstate

    std::string temp_line;
    std::getline(file, temp_line);

    Histogram h;
    char c;

    std::istringstream input_stream(temp_line);
    std::cout << "---> прочетен ред: \"" << temp_line << "\"; дължина: " << temp_line.length() << std::endl;
    if (temp_line.length() != 0) {
      input_stream >> h.grade >> c >> h.count;

      // Ако искаме да вземем конкретен символ, например двоеточието ':'
      const char* raw_string = temp_line.c_str();
      char colon = raw_string[1];  // символ с индекс 1

      std::cout << "Оценка " << h.grade << " имат " << h.count << " ученика." << std::endl;

    }
  }

  file.close();
  return true;
}

int main() {
  // Четем файла по символи.
  bool result = read_file_by_words("text_files/aria-labels.md");
  std::cout << "\n==================\n";

  // Четем файла по редове.
  result = read_file_by_lines("text_files/grades.txt");
  return 0;
}
