#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <vector>

#include "headers/Constants.h"

namespace fs = std::filesystem;

std::string getHashFrom(const std::string& input);
