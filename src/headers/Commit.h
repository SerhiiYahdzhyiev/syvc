#pragma once

class Commit {
public:
    Commit(const std::string& message);
    const std::string& getMessage() const;
    const std::string& getHash() const;
    void addFile(const std::string& filename);
    void createCommit();
    static std::string loadCommitMessage(const std::string& commitHash);

private:
    std::string message;
    std::string hash;
    std::vector<std::string> stagingArea;
};
