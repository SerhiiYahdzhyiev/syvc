#include "headers/Utils.h"
#include "headers/Commit.h"

Commit::Commit(const std::string& message) : message(message) {
    std::stringstream ss;
    auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    ss << std::hex << timestamp;
    hash = getHashFrom(message + ss.str());
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
    fs::path commitPath = Constants::COMMITS_FOLDER_PATH + "/" + hash;

    fs::create_directory(commitPath);

    for (const auto& filename : stagingArea) {
        fs::copy_file(Constants::STAGE_FOLDER_PATH + "/" + fs::path(filename).filename().string(), commitPath / fs::path(filename).filename().string());
    }

    std::ofstream commitFile(commitPath / Constants::COMMIT_MESSAGE_FILE_NAME);
    commitFile << message;
    commitFile.close();

    std::cout << "Committed changes with message: '" << message << "' and hash: '" << hash << "'\n";
}

std::string Commit::loadCommitMessage(const std::string& commitHash) {
    std::ifstream commitFile(Constants::COMMITS_FOLDER_PATH + "/" + commitHash + "/" + Constants::COMMIT_MESSAGE_FILE_NAME);
    
    if (!commitFile.is_open()) {
        return "Error: Unable to load commit message for hash " + commitHash + "!";
    }

    std::ostringstream messageStream;
    messageStream << commitFile.rdbuf();
    commitFile.close();

    return messageStream.str();
}
