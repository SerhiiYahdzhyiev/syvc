#ifndef SYVC_COMMIT_H
#define SYVC_COMMIT_H

#include <string>
#include <vector>

class Commit {
public:
    Commit(const std::string& message);
    const std::string& getMessage() const;
    const std::string& getHash() const;
    void addFile(const std::string& filename);
    void createCommit();

private:
    std::string message;
    std::string hash;
    std::vector<std::string> stagingArea;

    // Helper function to generate SHA-256 hash
    std::string generateHash(const std::string& input) const;
};

#endif
