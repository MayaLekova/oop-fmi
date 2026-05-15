#include <string>
#include <iostream>
#include <map>

using std::string;

// Базов абстрактен клас за въвеждане и изпълнение на команда от клавиатурата.
class Command {
 public:
  // Конструкторът на базовия клас не може да бъде виртуален,
  // защото конструкторът инициализира т.нар. "виртуална таблица",
  // а виртуалното извикване има нужда тя вече да е създадена,
  // за да се пренасочи към правилния метод на наследника.
  Command(const string& name)
    : name_(name) {}

  // Когато използваме полиморфизъм, задължително трябва да направим
  // деструктора на базовия клас виртуален, за да гарантираме, че
  // когато се разрушават обекти от класовете-наследници, ще се извика
  // правилният деструктор и ще се освободи целият обект.
  virtual ~Command() {}

  // `execute` е чисто виртуална (чрез `= 0` като атрибут в края).
  // По този начин задължаваме наследяващите класове
  // да имплементират конкретен алгоритъм за изпълнение на командата.
  virtual void execute(const string& runtime_params) = 0;

  // Всяка команда може да "докладва" какво прави и как се използва
  // чрез функцията `describe`.
  virtual string describe() const = 0;

  // `name` няма нужда да бъде имплементирана по различен начин
  // в наследниците, затова може да не бъде виртуална.
  const string& name() const {
    return name_;
  }
 
 private:
  string name_;
};

// Клас за регистър на команди, който реализира шаблона "сингълтън"
// (singleton), т.е. в нашата програма ще ползваме една инстанция
// на този клас от всички налични .cpp файлове.
//
// Шаблонът включва:
//  1. Публичен метод за достъп до инстанцията, в случая - `getInstance`.
//  2. Скрита имплементация на конструктора по подразбиране
//     (в `private` секцията).
//  3. Забраняваме копиращия конструктор и оператора за присвояване,
//     в по-модерните версии на езика - чрез анотацията `= delete`.

class CommandRegistry {
 public:
  bool RegisterCommand(const string& name);
  void ExecuteCommand(const string& name, const string& runtime_params);

  // 1. Публичният метод за достъп:
  static CommandRegistry& getInstance();

  // Помощен клас итератор, за обхождане на командите в регистъра.
  class Iterator {
   public:
    void init(const CommandRegistry* registry_) {
      // при инициализация запазваме началото на колекцията (std::map)
      // от команди.
      current = registry_->commands.begin();
      registry = registry_;
    }
    bool has_next() const {
      return current != registry->commands.end();
    }
    const Command& get() {
      // iterator обекта на `std::map` съдържа двойка (`std::pair`)
      // от ключа и стойността на записите. В случая стойността е
      // от тип указател към `Command`.
      Command* result = current->second;
      return *result;
    }
    void next() {
      current++;
    }

   private:
    // нашият итератор ще пази докъде е стигнал при обхождането
    // на регистръра с команди, катко използваме предоставения
    // от `std::map` структурата клас за итератор (нещо като
    // "умен" указател, специфичен за тази структура).
    std::map<string, Command*>::const_iterator current;
    const CommandRegistry* registry;
  };

  ~CommandRegistry() {
    // `CommandRegistry` се грижи да освободи паметта за заделените команди.
    std::map<string, Command*>::iterator cmd = commands.begin();
    for (; cmd != commands.end(); ++cmd) {
      delete cmd->second;
    }
  }

 private:
  // 2. Скритият конструктор по подразбиране.
  CommandRegistry() {}

  // 3. Забранените копиращ конструктор и оператор за присвояване.
  CommandRegistry(const CommandRegistry& other) = delete;
  CommandRegistry& operator=(const CommandRegistry& other) = delete;

  // В курса по СДП ще учим по-подробно за съответствията (mapping),
  // каквито са например хеш-маповете и само-балансиращите се дървета.
  std::map<string, Command*> commands;
};

// Вариант за имплементация на `singleton`, при който инстанцията
// се менажира (създава, освобождава) локално от метода за достъп до нея.
CommandRegistry& CommandRegistry::getInstance() {
  static CommandRegistry local_instance_;
  return local_instance_;
}

// Конкретните имплементации на командите биха могли да са изнесени
// в отделни файлове.
class FileCommand : public Command {
 public:
  // С помощта на инициализиращи списък можем да извикаме и конструктора
  // на базовия клас. Ако не дефинираме обаче конструктор на наследника,
  // той ще получи само конструктор по подразбиране (а ние искаме да
  // предаваме параметъра `name` за име на командата).
  FileCommand(const string& name)
    : Command(name) {}

  virtual void execute(const string& runtime_params);
  virtual string describe() const;
};

void FileCommand::execute(const string& runtime_params) {
  // "име–на–файл режим–на–запис"
  // прочитаме име на файла от runtime_params
  // прочитаме или записваме файл
  std::cout << "Зареждаме файл с параметри: " << runtime_params << std::endl;
}

string FileCommand::describe() const {
  return "Команда за работа с файлове. Употреба: ...";
}

class HelpCommand : public Command {
 public:
  HelpCommand(const string& name)
    : Command(name) {}

  virtual void execute(const string& runtime_params);
  virtual string describe() const;
};

void HelpCommand::execute(const string& runtime_params) {
  // обръща се към регистъра
  // прочита всички команди
  // извиква `describe` и го принтира на конзолата
  CommandRegistry::Iterator cmd_iterator;
  cmd_iterator.init(&CommandRegistry::getInstance());

  std::cout << "Помощ за командите в системата:" << std::endl;
  std::cout << "------------------------------" << std::endl;
  while(cmd_iterator.has_next()) {
    const Command& command = cmd_iterator.get();
    std::cout << "   > " << command.name() << std::endl;
    std::cout << command.describe() << std::endl;
    cmd_iterator.next();
  }
  std::cout << "------------------------------" << std::endl;
}

string HelpCommand::describe() const {
  // One command to describe them all...
  return "Команда, която описва работата на всички команди.";
}

class ExitCommand : public Command {
 public:
  ExitCommand(const string& name)
    : Command(name) {}

  virtual void execute(const string& runtime_params);
  virtual string describe() const;
};

void ExitCommand::execute(const string& runtime_params) {
  std::cout << "Край на програмата." << std::endl;
  exit(0);
}

string ExitCommand::describe() const {
  return "Команда, която приключва изпълнението на програмата.";
}

// Имплементациите на `RegisterCommand` и `ExecuteCommand` са доста
// обемни и биха могли да бъдат изнесени в отделен .cpp файл.
bool CommandRegistry::RegisterCommand(const string& name) {
  if (name == "load") {
    Command* command = new FileCommand("load");
    commands["load"] = command;
  } else if (name == "help") {
    Command* command = new HelpCommand("help");
    commands["help"] = command;
  } else if (name == "end") {
    Command* command = new ExitCommand("end");
    commands["end"] = command;
  } else {
    std::cout << "Командата не е разпозната!" << std::endl;
    return false;
  }
  return true;
}

void CommandRegistry::ExecuteCommand(
    const string& name, const string& runtime_params) {
  // Опитваме да намерим команда с това име в регистъра.
  std::map<string, Command*>::iterator found = commands.find(name);
  if(found != commands.end()) {
    // Препращаме параметрите от `runtime_params` към изпълнението на
    // командата, в случай, че сме намерили такава.
    found->second->execute(runtime_params);
  } else {
    std::cout << "Командата не е разпозната!" << std::endl;
  }
}

int main() {
  // Достъпът до *singleton* инстанцията (единствената съществуваща
  // инстанция на класа `CommandRegistry`) се извършва чрез статичния
  // метод `getInstance`.
  CommandRegistry::getInstance().RegisterCommand("help");
  CommandRegistry::getInstance().RegisterCommand("load");
  CommandRegistry::getInstance().RegisterCommand("end");

  // Временна променлива, в която ще прочетем един ред от конзолата.
  string command_line, command_name, params;
  while(true) {
    // Извеждаме "подсказка" (prompt), че програмата чака вход.
    std::cout << "> ";
    // Прочитаме цял ред от конзолата (може да се използва и за файлови потоци):
    std::getline(std::cin, command_line);

    string::size_type space_position = command_line.find(' ');
    if (space_position == string::npos) {
      // Прочели сме команда без параметри.
      command_name = command_line;
    } else {
      // Разделяме прочетения ред по следния начин:
      // име-на-команда    параметър-1 параметър-2 ...
      // <command_name>    <params>
      //               ^-------------- <space_position>
      command_name = command_line.substr(0, space_position);
      params = command_line.substr(space_position + 1, command_line.length());
    }

    CommandRegistry::getInstance().ExecuteCommand(command_name, params);
  }

  return 0;
}
