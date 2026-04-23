#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

// Функция, която принтира на стандартния изход съдържанието
// на текстов файл с име `filename`, четейки по "думи".
// Връща `false`, ако файлът не е намерен или нямаме достъп
// за четене до него. 
bool read_file_by_words(const std::string& filename) {
  std::ifstream file(filename, std::ios::in);
  if (!file.is_open()) {
    std::cout << "Файлът не е намерен или нямаме права за четене.";
    return false;
  }

  std::string line;
  while(!file.eof()) {
    file >> line;
    std::cout << "-- " << line << " --\n";
  }
  file.close();

  return true;
}

bool read_file_by_lines(const std::string& filename) {
  std::ifstream file(filename, std::ios::in);
  if (!file.is_open()) {
    std::cout << "Файлът не е намерен или нямаме права за четене.";
    return false;
  }

  // `line` играе роля на буфер за един ред от файла
  std::string line;
  while(std::getline(file, line)) {
    std::istringstream input_stream(line);
    int grade, count;
    char c;
    input_stream >> grade >> c >> count;
    std::cout << "Броят ученици, получили оценка " << grade <<
     " е: " << count << std::endl;
  }

  file.close();
  return true;
}

int main() {
  std::cout << "Четем файла по символи: " << std::endl;
  bool result = read_file_by_words("text_files/aria-labels.md");
  
  std::cout << "\n==================\n";
  std::cout << "Четем файла по редове: " << std::endl;
  result = read_file_by_lines("text_files/grades.txt");

  return 0;
}
