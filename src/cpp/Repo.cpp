#include "headers/Utils.h"
#include "headers/Repo.h"
#include "headers/Commit.h"

void Repo::init() {
    std::string currentDirectoryName = fs::current_path().stem().string();

    if (fs::exists(Constants::REPO_META_FOLDER_NAME)) {
        std::cerr << "Repository already initialized.\n";
        return;
    }

    if (!fs::create_directory(Constants::REPO_META_FOLDER_NAME)) {
        std::cerr << "Error creating repository directory.\n";
        return;
    }

    std::ofstream configFile(Constants::CONFIG_PATH);
    if (!configFile.is_open()) {
        std::cerr << "Error creating configuration file.\n";
        return;
    }

    configFile << "repository_name = " << currentDirectoryName << "\n";
    configFile << "default_branch = main\n";

    fs::create_directory(Constants::COMMITS_FOLDER_PATH);

    std::cout << "Initialized empty repository '" << currentDirectoryName << "'.\n";
}

void Repo::add(const std::string& filename) {
    if (!fs::exists(filename)) {
        std::cerr << "Error: File '" << filename << "' does not exist.\n";
        return;
    }

    fs::path repoPath = Constants::REPO_META_FOLDER_NAME;

    fs::create_directory(repoPath / Constants::STAGE_FOLDER_NAME);

    fs::copy_file(filename, repoPath / Constants::STAGE_FOLDER_NAME / fs::path(filename).filename());

    std::cout << "Added '" << filename << "' to the staging area.\n";
}

void Repo::remove(const std::string& filename) {
    fs::path repoPath = Constants::REPO_META_FOLDER_NAME;
    fs::path stagedFilePath = repoPath / Constants::STAGE_FOLDER_NAME / fs::path(filename).filename();

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

void Repo::displayCommitLog() const {
    std::cout << "Commit Log:\n";

    for (const auto& entry : fs::directory_iterator(Constants::COMMITS_FOLDER_PATH)) {
        if (entry.is_directory()) {
            std::string commitHash = entry.path().filename().string();
            std::string commitMessage = Commit::loadCommitMessage(commitHash);

            std::cout << "[" << commitHash << "] " << commitMessage << "\n";
        }
    }
}
