#include "../headers/Utils.h"
#include "../headers/Repo.h"
#include "../headers/Commit.h"

void Repo::_removeCommit(const std::string& hash) {
    std::string commitFolderPath = Constants::SYVC::COMMITS_FOLDER_PATH + "/" + hash;
    removeFolderContentsRecursively(commitFolderPath);
}

void Repo::_removeCommits(std::vector<std::string>* commitHashesToRemove) {
    for (const std::string& hash: *commitHashesToRemove) {
        this->_removeCommit(hash);
    }
}

void Repo::_updateHead(const std::string& lastCommitHash) {
    std::ofstream headFile(Constants::SYVC::HEAD_PATH, std::ios::app);
    headFile << lastCommitHash << std::endl;
    headFile.close();
}

void Repo::_updateHead(std::vector<std::string>* newHashes) {
    std::ofstream headFile(Constants::SYVC::HEAD_PATH, std::ios::trunc);
    for (const std::string& line : *newHashes) {
        headFile << line << std::endl;
    }
    headFile.close();
}

void Repo::_createConfigFile() {
    std::string currentDirectoryName = fs::current_path().stem().string();
    std::ofstream configFile(Constants::SYVC::CONFIG_PATH);
    if (!configFile.is_open()) {
        logError("Error creating configuration file.");
        return;
    }

    configFile << "repository_name = " << currentDirectoryName << "\n";
    configFile << "default_branch = main\n";

    configFile.close();
}

void Repo::_createTrackedFile() {
    std::ofstream trackedFile(Constants::SYVC::TRACKED_PATH);
    if (!trackedFile.is_open()) {
        logError("Error creating tracking file.");
        return;
    }
    
    trackedFile.close();
}

void Repo::_createHeadFile() {
    std::ofstream headFile(Constants::SYVC::HEAD_PATH);
    if (!headFile.is_open()) {
        logError("Error creating head file.");
        return;
    }
    
    headFile.close();
}

void Repo::_createStageFolder() {
    if (!fs::create_directory(Constants::SYVC::STAGE_FOLDER_PATH)) {
        logError("Error creating staging directory.");
        return;
    }
}

void Repo::_createCommitsFolder() {
    if (!fs::create_directory(Constants::SYVC::COMMITS_FOLDER_PATH)) {
        logError("Error creating commits directory.");
        return;
    }
}

void Repo::_createMetaFolder() {
    if (!fs::create_directory(Constants::SYVC::REPO_META_FOLDER_NAME)) {
        logError("Error creating repository directory.");
        return;
    }
}

bool Repo::_isTracked(const std::string& path) {
    std::vector<fs::path> tracked = this->_getTrackedPathList();

    if (std::find(tracked.begin(), tracked.end(), path) != tracked.end()) {
        return true;
    }

    return false; 
}

bool Repo::_inStaged(const std::string& path) {
    fs::path stagedFilePath = fs::path(Constants::SYVC::STAGE_FOLDER_PATH);

    return fs::exists(stagedFilePath);
}

std::vector<std::string> Repo::_getCommitHashesHistory() {
    std::vector<std::string> hashes;
    std::ifstream headFile(Constants::SYVC::HEAD_PATH);
    
    if (!headFile.is_open()) {
        logError("Error reading head file.");
        return hashes;
    }
    
    std::string path;
    while (std::getline(headFile, path)) {
        hashes.push_back(path);
    }

    headFile.close();
    return hashes;
}

std::vector<fs::path> Repo::_getTrackedPathList() {
    std::vector<fs::path> pathList;
    std::ifstream trackedFile(Constants::SYVC::TRACKED_PATH);
    
    if (!trackedFile.is_open()) {
        logError("Error reading tracking file.");
        return pathList;
    }

    std::string path;
    while (std::getline(trackedFile, path)) {
        pathList.push_back(fs::path(path));
    }

    trackedFile.close();
    return pathList;
}

void Repo::init() {
    if (fs::exists(Constants::SYVC::REPO_META_FOLDER_NAME)) {
        logError("Repository already initialized.");
        return;
    }

    this->_createMetaFolder();
    
    this->_createStageFolder();
    this->_createCommitsFolder();
    
    this->_createConfigFile();
    this->_createTrackedFile();
    this->_createHeadFile();

    printLine("Initialized empty repository.");
}

void Repo::add(const std::string& path) {
    if (!fs::exists(path)) {
        logError("Error: Path '" << path << "' does not exist.");
        return;
    } else if (fs::is_directory(path)) {
        fs::directory_iterator it(path);
        const fs::directory_iterator end;
        while (it != end) {
            this->add(it->path().string());
            ++it;
        }
        return;
    } 
    
    fs::path repoPath = fs::path(Constants::SYVC::REPO_META_FOLDER_NAME);
    fs::path newTrackedFilePath = fs::path(repoPath / Constants::SYVC::STAGE_FOLDER_NAME / fs::path(path));

    fs::create_directories(newTrackedFilePath.parent_path());

    if (!this->_isTracked(path)) {
        std::ofstream trackedFile(Constants::SYVC::TRACKED_PATH, std::ios::app);
        trackedFile << path << "\n";
    }
    
    fs::copy(path, newTrackedFilePath, fs::copy_options::update_existing);

    printLine("Staged '" << path << "'");
}

void Repo::remove(const std::string& path) {
    if (!fs::exists(path)) {
        logError("Error: Path '" << path << "' does not exist.");
        return;
    } else if (fs::is_directory(path)) {
        fs::directory_iterator it(path);
        const fs::directory_iterator end;
        while (it != end) {
            this->remove(path + it->path().string());
            ++it;
        }
    } 
    fs::path repoPath = fs::path(Constants::SYVC::REPO_META_FOLDER_NAME);
    fs::path stagedFilePath = fs::path(repoPath / Constants::SYVC::STAGE_FOLDER_NAME / fs::path(path).filename());

    if (!this->_inStaged(path)) {
        logError("Error: File '" << path << "' is not staged.");
    }

    fs::remove(stagedFilePath);

    printLine("Unstaged  '" << path << "'");
}

void Repo::commit(const std::string& message) {
    Commit commit(message);
    commit.save();
    this->_updateHead(commit.getHash());
}

void Repo::revert(const std::string& commitHash) {
    std::vector<std::string> hashes = this->_getCommitHashesHistory();
    
    auto it = std::find(hashes.begin(), hashes.end(), commitHash);

    if (it == hashes.end()) {
        logError("Failed to find commit with hash " << commitHash << " in repo's history"); 
    } else {
        std::vector<std::string> commitHashesToRemove(++it, hashes.end());
        std::vector<std::string> newHashes(hashes.begin(), it);
        
        this->_removeCommits(&commitHashesToRemove);
        this->_updateHead(&newHashes);

        fs::path sourcePath = fs::path(Constants::SYVC::COMMITS_FOLDER_PATH + "/" + commitHash + "/" + Constants::SYVC::COMMIT_DIFF_FOLDER_NAME);
        fs::path destPath = fs::path(".");
        
        copyFolderContentsRecursively(sourcePath, destPath);
    }
}

void Repo::displayCommitLog() {
    printLine("Commit Log:");

    std::vector<std::string> history = this->_getCommitHashesHistory();

    for (const std::string& hash : history) { 
        std::string message = Commit::loadCommitMessage(hash);
        printLine(hash << " " << message);
    }
}
