#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "Commit.h"
#include "Utils.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

#ifdef __cpp_lib_filesystem
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

Commit::Commit(const std::string& message) : message(message) {
    std::stringstream ss;
    auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    ss << std::hex << timestamp;
    hash = Utils::calculateSHA1(message + ss.str());
}

const std::string& Commit::getMessage() const {
    return message;
}

const std::string& Commit::getHash() const {
    return hash;
}

void Commit::addFile(const std::string& filename) {
    stagingArea.push_back(filename);
}

void Commit::createCommit() {
    fs::path commitPath = ".syvc/main/" + hash;

    fs::create_directory(commitPath);

    for (const auto& filename : stagingArea) {
        fs::copy_file(".syvc/stage/" + fs::path(filename).filename().string(), commitPath / fs::path(filename).filename().string());
    }

    std::ofstream commitFile(commitPath / "commit_message.txt");
    commitFile << message;
    commitFile.close();

    std::cout << "Committed changes with message: '" << message << "' and hash: '" << hash << "'\n";
}
