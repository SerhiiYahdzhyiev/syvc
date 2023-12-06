#pragma once

namespace Constants {
  const std::string REPO_META_FOLDER_NAME = ".syvc";
  const std::string CONFIG_PATH = ".syvc/config.txt";
  const std::string COMMITS_FOLDER_PATH = ".syvc/commits";
  const std::string STAGE_FOLDER_NAME = "stage";
  const std::string STAGE_FOLDER_PATH = ".syvc/stage";
  const std::string COMMIT_MESSAGE_FILE_NAME = "message.txt";
  const std::size_t FNV_OFFSET_BASIS = 14695981039346656037ull;
  const std::size_t FNV_PRIME = 1099511628211ull;
}
