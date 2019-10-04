#include "FileUtil.h"

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#define PATH_SEPERATER '/'

namespace StrRosterNS {

void CheckAndAddSeperator(std::string& folder) {
  if (folder.back() != PATH_SEPERATER) {
    folder.push_back(PATH_SEPERATER);
  }
}

bool CheckFolderExist(const char* folder) {
  struct stat info;
  int res = stat(folder, &info);
  if (res == 0 && (info.st_mode & S_IFDIR)) {
    return true;
  }
  return false;
}

bool CreateFolderIfNotExist(const char* folder) {
  if (CheckFolderExist(folder)) {
    return true;
  }
  mkdir(folder, 0755);
  if (CheckFolderExist(folder)) {
    return true;
  }
  return false;
}

void WriteFile(const char* file_name, uint8_t* data, size_t bytes) {
	FILE* file = fopen(file_name, "wb");
  fwrite(data, sizeof(uint8_t), bytes, file);
  fclose(file);
}

size_t GetFileSize(const char* file_name) {
  size_t file_size = 0;
  FILE *p_file = fopen(file_name, "rb");
  if (p_file == NULL) {
    std::cerr << "File '" << file_name << "' read error" << std::endl;
    return 0;
  }

  // obtain file size:
  fseek (p_file , 0 , SEEK_END);
  file_size = ftell(p_file);

  fclose (p_file);
  return file_size;
}

size_t ReadFile(const char* file_name, uint8_t* buffer, size_t bytes) {
  size_t read_size = 0;
  FILE * p_file = fopen(file_name, "rb");
  if (p_file == NULL) {
    std::cerr << "File '" << file_name << "' read error" << std::endl;
    return 0;
  }

  // read the file into the buffer:
  read_size = fread(buffer, 1, bytes, p_file);
  if (read_size != bytes) {
    std::cerr << "File '" << file_name << "' read size " << read_size
      << ", but desired size is " << bytes << std::endl;
  }

  fclose(p_file);
  return read_size;
}

}  // namespace StrRosterNS
