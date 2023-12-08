#pragma once

class Repo {
public:
    void init();
    void add(const std::string& path);
    void remove(const std::string& path);
    void commit(const std::string& message);
    void displayCommitLog() const;
private:
    std::vector<fs::path> _getTrackedPathList();

    void _createConfigFile();
    void _createTrackedFile();
    void _createHeadFile();

    void _createMetaFolder();
    void _createStageFolder();
    void _createCommitsFolder();

    void _updateHead(const std::string& lastCommitHash);

    bool _isTracked(const std::string& path);
    bool _inStaged(const std::string& path);
    bool _checkDiff(const std::string& path);
};
