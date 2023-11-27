#include "Repo.h"
#include <iostream>

void Repo::init() {
    std::cout << "Initialized empty repository.\n";
}

void Repo::add(const std::string& filename) {
    std::cout << "Added '" << filename << "' to the staging area.\n";
}

void Repo::remove(const std::string& filename) {
    std::cout << "Removed '" << filename << "' from the staging area.\n";
}

void Repo::commit() {
    std::cout << "Committed changes.\n";
}
