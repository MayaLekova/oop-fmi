#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdint>

// Базов клас за външен ресурс (asset) в даден engine.
class Asset {
 public:
  virtual void LoadFile(const std::string& filepath, bool binaryMode) = 0;  
};

class Texture : public Asset {
 public:
  // virtual
  void LoadFile(const std::string& filepath, bool binaryMode);
  void WriteFile(const std::string& filepath, bool binaryMode);

 private:
  // Можем да си дефинираме наш тип за стойност на една компонента
  //   (червено, синьо, зелено) от даден цвят. Две ползи от това:
  //  1. даваме семантика на типа, т.е. показваме значението му
  //  2. при нужда да поддържаме цветове в други цветови пространства (напр.
  //     с компоненти - реални числа от 0 до 1, вместо цели от 0 до 255),
  //     ще променим кода на по-малко места.
  using color_t = uint8_t;

  struct Pixel {
    color_t R;  // red
    color_t G;  // green
    color_t B;  // blue
    float A;    // alpha
  };

  unsigned int width, height;
  // 5000 пиксела по всяко измерение
  static constexpr unsigned int MAX_TEXTURE_SIZE = 5000;
  Pixel** pixel_data;
};

void Texture::LoadFile(const std::string& filepath, bool binaryMode) {
  std::ios::openmode flags = std::ios::in;
  if (binaryMode) {
    flags |= std::ios::binary;
  }

  std::ifstream file(filepath, flags);
  if (!file.is_open()) {
    std::cout << "Texture can't be loaded" << std::endl;
    return;
  } else {
    std::cout << "Loaded texture with name " << filepath << std::endl;
  }

  // зареждаме данните от файла;
  file >> width >> height;

  // проверяваме дали w/h са смислени, напр. <= 5000
  assert(width <= MAX_TEXTURE_SIZE);
  assert(height <= MAX_TEXTURE_SIZE);

  // записваме ги като пиксели
  color_t color_value;
  // трябва да проверим дали pixel_data вече има записани данни!
  // pixel_data = new Pixel*[width * height];
  // алтернативно:
  pixel_data = new Pixel*[height];
  // и след това във външния цикъл заделяме цял ред с

  for (size_t i = 0; i < height; ++i) {
    //
    pixel_data[i] = new Pixel[width];
    for (size_t j = 0; j < width; ++j) {
      file >> color_value;
      Pixel p = {color_value, 0, 0, 0};
      pixel_data[i][j] = p;
    }
  }
}

void Texture::WriteFile(const std::string& filepath, bool binaryMode) {
  std::ios::openmode flags = std::ios::out;
  if (binaryMode) {
    flags |= std::ios::binary;
  }
  std::ofstream out_file(filepath, flags);

  if (!out_file.is_open()) {
    std::cout << "Texture file can't be written" << std::endl;
    return;
  } else {
    std::cout << "Starting to write texture with name " << filepath << std::endl;
  }

  // записваме заглавната част на файла
  out_file << width << " " << height << "\n";

  // записваме пикселите
  for (size_t i = 0; i < height; ++i) {
    // запис в двоичен режим
    if (binaryMode) {
      out_file.write(reinterpret_cast<const char*>(pixel_data[i]), width * sizeof(Pixel));
      continue;
    }

    // запис в текстов режим
    for (size_t j = 0; j < width; ++j) {
      if (!binaryMode) {
        out_file << pixel_data[i][j].R << " ";
      }
    }
    out_file << "\n";
  }
  out_file << "\n"; 
}

class Effect : public Asset {

};

int main() {
  Texture t;
  t.LoadFile("red.txt", false);
  t.WriteFile("other_red_text.txt", false);
  t.WriteFile("other_red_binary.bin", true);
}
