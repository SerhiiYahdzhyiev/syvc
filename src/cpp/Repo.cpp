#include "headers/Utils.h"
#include "headers/Repo.h"
#include "headers/Commit.h"

void Repo::_updateHead(const std::string& lastCommitHash) {
    std::ofstream headFile(Constants::HEAD_PATH, std::ios::trunc);
    headFile << lastCommitHash;
}

void Repo::_createConfigFile() {
    std::string currentDirectoryName = fs::current_path().stem().string();
    std::ofstream configFile(Constants::CONFIG_PATH);
    if (!configFile.is_open()) {
        std::cerr << "Error creating configuration file.\n";
        return;
    }

    configFile << "repository_name = " << currentDirectoryName << "\n";
    configFile << "default_branch = main\n";

    configFile.close();
}

void Repo::_createTrackedFile() {
    std::ofstream trackedFile(Constants::TRACKED_PATH);
    if (!trackedFile.is_open()) {
        std::cerr << "Error creating tracking file.\n";
        return;
    }
    
    trackedFile.close();
}

void Repo::_createHeadFile() {
    std::ofstream headFile(Constants::TRACKED_PATH);
    if (!headFile.is_open()) {
        std::cerr << "Error creating head file.\n";
        return;
    }
    
    headFile.close();
}

void Repo::_createStageFolder() {
    if (!fs::create_directory(Constants::COMMITS_FOLDER_PATH)) {
        std::cerr << "Error creating staging directory.\n";
        return;
    }
}

void Repo::_createCommitsFolder() {
    if (!fs::create_directory(Constants::COMMITS_FOLDER_PATH)) {
        std::cerr << "Error creating commits directory.\n";
        return;
    }
}

void Repo::_createMetaFolder() {
    if (!fs::create_directory(Constants::REPO_META_FOLDER_NAME)) {
        std::cerr << "Error creating repository directory.\n";
        return;
    }
}
bool Repo::_checkDiff(const std::string& path) {
    //TODO: Realize
    return true;
}

bool Repo::_isTracked(const std::string& path) {
    std::vector<fs::path> tracked = this->_getTrackedPathList();

    if (std::find(tracked.begin(), tracked.end(), path) != tracked.end()) {
        return true;
    }

    return false; 
}

bool Repo::_inStaged(const std::string& path) {
    fs::path stagedFilePath = fs::path(Constants::STAGE_FOLDER_PATH);

    return fs::exists(stagedFilePath);
}

std::vector<fs::path> Repo::_getTrackedPathList() {
    std::vector<fs::path> pathList;
    std::ifstream trackedFile(Constants::TRACKED_PATH);
    
    if (!trackedFile.is_open()) {
        std::cerr << "Error reading tracking file.\n";
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
    if (fs::exists(Constants::REPO_META_FOLDER_NAME)) {
        std::cerr << "Repository already initialized.\n";
        return;
    }

    this->_createMetaFolder();
    
    this->_createStageFolder();
    this->_createCommitsFolder();
    
    this->_createConfigFile();
    this->_createTrackedFile();
    this->_createHeadFile();

    std::cout << "Initialized empty repository.\n";
}

void Repo::add(const std::string& path) {
    if (!fs::exists(path)) {
        std::cerr << "Error: Path '" << path << "' does not exist.\n";
        return;
    } else if (fs::is_directory(path)) {
        fs::directory_iterator it(path);
        const fs::directory_iterator end;
        while (it != end) {
            this->add(path + it->path().string());
            ++it;
        }
    } 
    
    fs::path repoPath = fs::path(Constants::REPO_META_FOLDER_NAME);
    fs::path newTrackedFilePath = fs::path(repoPath / Constants::STAGE_FOLDER_NAME / fs::path(path).filename());
    
    if (this->_inStaged(path) || this->_isTracked(path)) {
        bool hasDiff = this->_checkDiff(path);
        if (hasDiff) {
            fs::copy(path, newTrackedFilePath, fs::copy_options::update_existing);
        } else {
            std::cerr << "No changes to stage in the file: " << path << "\n";
        }
    } else {
        if (!this->_isTracked(path)) {
            std::ofstream trackedFile(Constants::TRACKED_PATH, std::ios::app);
            trackedFile << newTrackedFilePath << "\n";

            fs::copy(path, newTrackedFilePath, fs::copy_options::update_existing);
    }

    std::cout << "Staged '" << path << "'\n";
    }
}

void Repo::remove(const std::string& path) {
    if (!fs::exists(path)) {
        std::cerr << "Error: Path '" << path << "' does not exist.\n";
        return;
    } else if (fs::is_directory(path)) {
        fs::directory_iterator it(path);
        const fs::directory_iterator end;
        while (it != end) {
            this->remove(path + it->path().string());
            ++it;
        }
    } 
    fs::path repoPath = fs::path(Constants::REPO_META_FOLDER_NAME);
    fs::path stagedFilePath = fs::path(repoPath / Constants::STAGE_FOLDER_NAME / fs::path(path).filename());

    if (!this->_inStaged(path)) {
        std::cerr << "Error: File '" << path << "' is not staged.\n";
    }

    fs::remove(stagedFilePath);

    std::cout << "Unstaged  '" << path << "'\n";
}

void Repo::commit(const std::string& message) {
    Commit commit(message);
    commit.save();
    this->_updateHead(commit.getHash());
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
