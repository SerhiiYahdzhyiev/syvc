cmake_minimum_required(VERSION 3.12)

project(syvc)

set(CMAKE_CXX_STANDARD 17)

# Add executable
add_executable(syvc src/main.cpp src/cpp/Repo.cpp src/cpp/FileTracker.cpp src/cpp/Commit.cpp src/cpp/Utils.cpp)

# Add include directories
target_include_directories(syvc PRIVATE src)
