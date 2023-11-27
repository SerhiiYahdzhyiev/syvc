cmake_minimum_required(VERSION 3.12)

project(syvc)

set(CMAKE_CXX_STANDARD 11)

# Add executable
add_executable(syvc src/main.cpp src/Repo.cpp src/FileTracker.cpp src/Commit.cpp src/Utils.cpp)

# Add include directories
target_include_directories(syvc PRIVATE src)
