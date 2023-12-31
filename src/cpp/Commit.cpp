#include "headers/Utils.h"
#include "headers/Commit.h"

Commit::Commit(const std::string& message) : _message(message) {
    std::stringstream ss;
    auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    ss << std::hex << timestamp;
    this->_hash = getHashFrom(this->_message + ss.str());

    this->_path = fs::path(Constants::SYVC::COMMITS_FOLDER_PATH + "/" + this->_hash);
    this ->_messageFilePath =  fs::path(this->_path / Constants::SYVC::COMMIT_MESSAGE_FILE_NAME);
    this ->_diffFolderPath = fs::path(this->_path / Constants::SYVC::COMMIT_DIFF_FOLDER_NAME);
}

std::vector<fs::path> Commit::_getStaged() {
    std::vector<fs::path> staged;
    for (const auto& path : fs::directory_iterator(Constants::SYVC::STAGE_FOLDER_PATH)) {
        staged.push_back(fs::path(path));
    }
    return staged;
}

void Commit::_saveMessage() {
    std::ofstream messageFile(this->_messageFilePath);
    messageFile << this->_message;
    messageFile.close();
}

void Commit::_createDiffFolder() {
    if (!fs::create_directory(this->_diffFolderPath)) {
        logError("Failed to create diff folder for commit with hash: " << this->_hash);
        return;
    }
}

void Commit::_createMessageFile() {
    std::ofstream messageFile(this->_messageFilePath);
    if (!messageFile.is_open()) {
        logError("Failed to create message file for commit with hash: " << this->_hash);
        return;
    }
}

void Commit::_clearStaged() {
    removeFolderContentsRecursively(Constants::SYVC::STAGE_FOLDER_PATH);
}

void Commit::_saveDiff() {
    copyFolderContentsRecursively(fs::path(Constants::SYVC::STAGE_FOLDER_PATH), this->_diffFolderPath);
}

const std::string& Commit::getMessage() const {
    return this->_message;
}

const std::string& Commit::getHash() const {
    return this->_hash;
}

void Commit::save() {
    std::vector<fs::path> staged = this->_getStaged(); 
    if (staged.empty()) {
        logError("Nothing to commit, no staged files!");
        return;
    }

    fs::create_directory(this->_path);

    
    this->_createDiffFolder();
    this->_createMessageFile();

    this->_saveMessage();
    this->_saveDiff();
    this->_clearStaged();

    printLine("Committed changes with message: '" << this->_message << "' and hash: '" << this->_hash << "'");
}

std::string Commit::loadCommitMessage(const std::string& commitHash) {
    std::ifstream commitFile(Constants::SYVC::COMMITS_FOLDER_PATH + "/" + commitHash + "/" + Constants::SYVC::COMMIT_MESSAGE_FILE_NAME);
    
    if (!commitFile.is_open()) {
        return "Error: Unable to load commit message for hash " + commitHash + "!";
    }

    std::ostringstream messageStream;
    messageStream << commitFile.rdbuf();
    commitFile.close();

    return messageStream.str();
}
