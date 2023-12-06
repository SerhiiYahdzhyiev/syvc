#include "headers/Utils.h"

std::string getHashFrom(const std::string& input) {
  std::size_t hash = Constants::FNV_OFFSET_BASIS;

  for (char ch : input) {
      hash ^= static_cast<std::size_t>(ch);
      hash *= Constants::FNV_PRIME;
  }

  std::stringstream stream;

  stream << hash;

  return stream.str();
}

Command parseCommand(const std::string& command) {
  static const std::map<std::string, Command> stringToCommand = {
    {"init", INIT},
    {"add", ADD},
    {"rm", REMOVE},
    {"log", LOG},
    {"status", STATUS},
    {"commit", COMMIT},
    {"revert", REVERT},
    {"help", HELP}
  };

  auto it = stringToCommand.find(command);
  if (it != stringToCommand.end()) {
    return it->second;
  } else {
    return UNKNOWN;
  }
}

void printHelp() {
  std::cout << "syvc - Simple Version Control System\n\n";
  std::cout << "Usage:\n";
  std::cout << "  syvc <command>\n\n";
  std::cout << "Commands:\n";
  std::cout << "  init                  Initialize a new syvc repository\n";
  std::cout << "  add <file>            Add a file to the syvc repository\n";
  std::cout << "  rm <file>             Remove a file from the syvc repository\n";
  std::cout << "  log                   Display the commit log\n";
  std::cout << "  status                Show the status of the syvc repository\n";
  std::cout << "  commit <message>      Commit changes to the syvc repository\n";
  std::cout << "  revert                Revert changes in the syvc repository\n";
  std::cout << "  help                  Display this help information\n";
}

void printError(const std::string& message) {
  std::cerr << message << "\n";
  std::cerr << "Type 'syvc help' for more information";
}
