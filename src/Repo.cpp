#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "Repo.h"
#include "Commit.h"
#include <iostream>
#include <fstream>

#ifdef __cpp_lib_filesystem
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

void Repo::init() {
    std::string currentDirectoryName = fs::current_path().stem().string();

    if (fs::exists(".syvc")) {
        std::cerr << "Repository already initialized.\n";
        return;
    }

    if (!fs::create_directory(".syvc")) {
        std::cerr << "Error creating repository directory.\n";
        return;
    }

    std::ofstream configFile(".syvc/config.txt");
    if (!configFile.is_open()) {
        std::cerr << "Error creating configuration file.\n";
        return;
    }

    configFile << "repository_name = " << currentDirectoryName << "\n";
    configFile << "default_branch = main\n";

    fs::create_directory(".syvc/main");

    std::cout << "Initialized empty repository '" << currentDirectoryName << "'.\n";
}

void Repo::add(const std::string& filename) {
    if (!fs::exists(filename)) {
        std::cerr << "Error: File '" << filename << "' does not exist.\n";
        return;
    }

    fs::path repoPath = ".syvc";

    fs::create_directory(repoPath / "stage");

    fs::copy_file(filename, repoPath / "stage" / fs::path(filename).filename());

    std::cout << "Added '" << filename << "' to the staging area.\n";
}

void Repo::remove(const std::string& filename) {
    fs::path repoPath = ".syvc";
    fs::path stagedFilePath = repoPath / "stage" / fs::path(filename).filename();

    if (!fs::exists(stagedFilePath)) {
        std::cerr << "Error: File '" << filename << "' is not staged.\n";
        return;
    }

    fs::remove(stagedFilePath);

    std::cout << "Removed '" << filename << "' from the staging area.\n";
}

void Repo::commit(const std::string& message) {
    Commit commit(message);
    for (const auto& filename : fs::directory_iterator(".syvc/stage")) {
        commit.addFile(filename.path().filename().string());
    }
    commit.createCommit();
}
