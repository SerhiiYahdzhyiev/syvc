#include "../headers/Utils.h"

std::string getHashFrom(const std::string& input) {
  std::size_t hash = Constants::FNV::OFFSET_BASIS;

  for (char ch : input) {
      hash ^= static_cast<std::size_t>(ch);
      hash *= Constants::FNV::PRIME;
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
  printLine("syvc - Simple Version Control System\n");
  printLine("Usage:");
  printLine("  syvc <command>\n");
  printLine("Commands:");
  printLine("  init                  Initialize a new syvc repository");
  printLine("  add <file>            Add a file to the syvc repository");
  printLine("  rm <file>             Remove a file from the syvc repository");
  printLine("  log                   Display the commit log");
  printLine("  commit <message>      Commit changes to the syvc repository");
  printLine("  revert                Revert changes in the syvc repository");
  printLine("  help                  Display this help information");
}

void printError(const std::string& message) {
  logError(message);
  logError("Type 'syvc help' for more information");
}

void removeFolderContentsRecursively(const std::string& path) {
    for (const auto& entry : fs::directory_iterator(path)) {
        try {
            if (fs::is_directory(entry.status())) {
                removeFolderContentsRecursively(entry.path().string());
            }

            fs::remove(entry.path());
        } catch (const std::exception& e) {
            logError("Error removing " << entry.path() << ": " << e.what());
            return;
        }
    }
}

void copyFolderContentsRecursively(const fs::path& source, const fs::path& destination) {
    try {
        if (!fs::exists(destination)) {
            logError("Failed to copy, destination folder does not exist!");
            return;
        }

        for (const auto& entry : fs::directory_iterator(source)) {
            const fs::path currentPath = entry.path();
            const fs::path newPath = destination / currentPath.filename();

            if (fs::is_directory(currentPath)) {
                fs::create_directories(newPath);
                copyFolderContentsRecursively(currentPath, newPath);
            } else {
                fs::copy(currentPath, newPath, fs::copy_options::overwrite_existing);
            }
        }
    } catch (const std::exception& e) {
        logError("Error copying directory: " << e.what());
    }
}
