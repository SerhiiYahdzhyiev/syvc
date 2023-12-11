#pragma once

class Commit {
public:
    Commit(const std::string& message);

    const std::string& getMessage() const;
    const std::string& getHash() const;

    void save();

    static std::string loadCommitMessage(const std::string& commitHash);
private:
    std::string _message;
    std::string _hash;

    fs::path _path;
    fs::path _messageFilePath;
    fs::path _diffFolderPath;

    std::vector<fs::path> _getStaged();

    void _clearStaged();
    void _saveMessage();
    void _saveDiff();

    void _createDiffFolder();
    void _createMessageFile();
};
