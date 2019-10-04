#ifndef STR_ROSTER_FILE_UTIL_H_
#define STR_ROSTER_FILE_UTIL_H_
#include <string>

namespace StrRosterNS {

void CheckAndAddSeperator(std::string& folder);
bool CheckFolderExist(const char* folder);
bool CreateFolderIfNotExist(const char* folder);
void WriteFile(const char* file_name, uint8_t* data, size_t bytes);
size_t GetFileSize(const char* file_name);
size_t ReadFile(const char* file_name, uint8_t* buffer, size_t bytes);

}  // namespace StrRosterNS

#endif // STR_ROSTER_FILE_UTIL_H_
