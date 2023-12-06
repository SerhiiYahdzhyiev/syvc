#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>

#include "headers/Constants.h"
#include "headers/Command.h"

namespace fs = std::filesystem;

std::string getHashFrom(const std::string& input);
void printHelp();
void printError(const std::string& message);
Command parseCommand(const std::string& command);
