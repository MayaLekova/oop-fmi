#include <string>
#include <map>
#include <iostream>

using std::string;

class Command {
 public:
  Command(const string& name, const string& param)
    : name_(name)
    , param_(param) {}

  // задължаваме наследяващите класове да имплементират конкретен
  // алгоритъм за изпълнение на командата.
  virtual void execute(const string& runtime_params) const = 0;
  // runtime_params би могло да бъде указател към структура

 private:
  const string name_;
  const string param_;
};

class FileCommand : public Command {
 public:
  FileCommand(const string& name, const string& param)
    : Command(name, param) {
      // какви други параметри могат да са специфични за файловите команди?
    }
};

// TODO: да го направим Singleton
class CommandRegistry {
 public:
  void AddCommand(const string& name, Command* command);
  void ExecuteCommand(const string& name, const string& runtime_params);

 private:
  std::map<string, Command*> registry;
};

void CommandRegistry::AddCommand(const string& name, Command* command) {
  // бихме могли да поверим създаването на командите и на този метод
  registry[name] = command;
}

void CommandRegistry::ExecuteCommand(const string& name, const string& runtime_params) {
  auto command = registry.find(name);
  if (command != registry.end()) {
    command->second->execute(runtime_params);
  } else {
    std::cout << "Command not found: " << name << std::endl;
  }
}

int main() {
  CommandRegistry cm;
  cm.AddCommand("LoadFile", ...);
  
  return 0;
}
