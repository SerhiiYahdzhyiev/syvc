#pragma once

class Repo {
public:
    void init();
    void add(const std::string& filename);
    void remove(const std::string& filename);
    void commit(const std::string& message);
    void displayCommitLog() const;
};
