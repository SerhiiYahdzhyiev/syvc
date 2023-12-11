#pragma once

class Repo {
public:
    void init();
    void add(const std::string& path);
    void remove(const std::string& path);
    void commit(const std::string& message);
    void revert(const std::string& commitHash);
    void displayCommitLog();
private:
    std::vector<fs::path> _getTrackedPathList();
    std::vector<std::string> _getCommitHashesHistory();

    void _createConfigFile();
    void _createTrackedFile();
    void _createHeadFile();

    void _createMetaFolder();
    void _createStageFolder();
    void _createCommitsFolder();

    void _updateHead(const std::string& lastCommitHash);
    void _updateHead(std::vector<std::string>* newHashes);

    void _removeCommits(std::vector<std::string>* commitHashesToRemove);
    void _removeCommit(const std::string& hash);

    bool _isTracked(const std::string& path);
    bool _inStaged(const std::string& path);
};
