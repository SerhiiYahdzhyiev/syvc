#pragma once
#ifndef SYVC_REPO_H
#define SYVC_REPO_H

#include <string>

class Repo {
public:
    void init();
    void add(const std::string& filename);
    void remove(const std::string& filename);
    void commit(const std::string& message);
};

#endif
