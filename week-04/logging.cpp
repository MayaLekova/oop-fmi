#include <iostream>
#include <vector>

#include <cstring>

struct DateTime {
  using field = unsigned short;

  DateTime()
    : year(1980)
    , month(1)
    , day(1)
    , hour(0)
    , minute(0)
    , second(0)
  {}

  DateTime(field _year, field _month, field _day,
          field _hour, field _minute, field _second)
    : year(_year)
    , month(_month)
    , day(_day)
    , hour(_hour)
    , minute(_minute)
    , second(_second)
  {
    // year = _year;
    // month = _month;
    // ..
  }

  void increment() {
    if (second < 60) {
      ++second;
      return;
    }
    if (minute < 60) {
      ++minute;
      return;
    }
    if (hour < 24) {
      ++hour;
      return;
    }
    // TODO: normally we should use a mapping from month to the number
    // of days it contains. For simplicity, we only generates dates with
    // days in the range [1; 28].
    if (day < 28) {
      ++day;
      return;
    }
    if (month < 12) {
      ++month;
      return;
    }
    ++year;   // In the year 2525...
  }

  void print() const {
    std::cout << "[timestamp]";
  }

  field year;   // unsigned short year;
  field month;
  field day;

  field hour;
  field minute;
  field second;
};

// class that owns the memory for the string
class String {
 public:
  String(const char* msg = nullptr, size_t len = 0) {
    init(msg, len);
  }
  String(const String& other) {
    init(other.message, other.length);
  }
  // TODO: implement `operator=`
  ~String() {
    deinit();
  }

  void print() const {
    // `this` points to a const object
    if (!message) {
      std::cout << "NULL string";
      return;
    }
    std::cout << message;
  }

 private:
  // `init` relies on the object being empty; if it's already used,
  // `deinit` must be called first.
  void init(const char* msg, size_t len);
  // `deinit` frees the memory and nullifies the object.
  void deinit();

  char* message = nullptr;
  size_t length = 0;
};

void String::init(const char* msg, size_t len) {
  message = new char[len];
  strncpy(message, msg, len);  // strcpy
  length = len;
}

void String::deinit() {
  delete message;
  message = nullptr;
  length = 0;
}

enum class Severity {
  LOW,
  MODERATE,
  HIGH
};

void print_severity(Severity sev) {
  switch(sev) {
    case Severity::LOW:
      std::cout << " LOW ";
      break;
    case Severity::MODERATE:
      std::cout << " MODERATE ";
      break;
    case Severity::HIGH:
      std::cout << " HIGH ";
      break;
  }
}

class LogEntry {
 public:
  LogEntry()
  {}

  LogEntry(DateTime _timestamp, Severity _severity, const char* msg, size_t len)
    : timestamp(_timestamp)
    , severity(_severity)
    , message(msg, len)
  {
    // some more complex code in the constructor body;
    // the "simple" initialisation goes in the initialiser list
  }

  void print() const {
    timestamp.print();
    print_severity(severity);
    message.print();
    std::cout << std::endl;
  }

 private:
  DateTime timestamp;
  Severity severity;
  String message;
};

std::vector<LogEntry> log_queue;

void generate_all_the_logs(size_t logs_count) {
  DateTime timestamp;

  for (size_t i = 0; i < logs_count; ++i) {
    LogEntry le(timestamp, Severity::MODERATE, "OMG!", 4);
    log_queue.push_back(le);
    timestamp.increment();
  }
}

int main() {
  generate_all_the_logs(10000000);
  log_queue[42].print();

  return 0;
}
