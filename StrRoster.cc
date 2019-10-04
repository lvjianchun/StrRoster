#include "StrRoster.h"

#include <cstring>
#include <cstdio>
#include <iostream>

#include "FileUtil.h"
#include "HashFunctions.h"

#define HASH_RETRY_NUM 26

namespace StrRosterNS {

HASH_FUNCTION_PTR StrRoster::hash_functions_[6] = {BKDRHash, APHash, DJBHash, JSHash, RSHash, SDBMHash};

StrRoster::StrRoster(size_t estimated_count) {
  estimated_count_ = estimated_count;
  table_size_ = estimated_count < 65536 ? estimated_count * 4 : estimated_count / 2 * 5;
  hash_table_ = new uint32_t[table_size_];
  memset(hash_table_, 0, table_size_);
  index_.reserve(estimated_count + 100);
  index_.push_back(0);
}

StrRoster::StrRoster(std::string data_folder) {
  Load(data_folder);
  estimated_count_ = index_.size();
}

StrRoster::~StrRoster() {
  delete[] hash_table_;
}

bool StrRoster::Dump(std::string data_folder) {
  CheckAndAddSeperator(data_folder);
  if (!CreateFolderIfNotExist(data_folder.c_str())) {
    std::cerr << "Folder '" << data_folder << "' can not be crteated" << std::endl;
    return false;
  }

  std::string hash_table_filename = data_folder + "hash_table.data";
  WriteFile(hash_table_filename.c_str(), reinterpret_cast<uint8_t*>(hash_table_), sizeof(uint32_t) * table_size_);
  std::string str_data_filename = data_folder + "str_data.data";
  WriteFile(str_data_filename.c_str(), reinterpret_cast<uint8_t*>(&str_data_[0]), str_data_.size());
  std::string index_filename = data_folder + "index.data";
  WriteFile(index_filename.c_str(), reinterpret_cast<uint8_t*>(&index_[0]), sizeof(uint32_t) * index_.size());
}

bool StrRoster::Load(std::string data_folder) {
  CheckAndAddSeperator(data_folder);
  if (!CheckFolderExist(data_folder.c_str())) {
    std::cerr << "Folder '" << data_folder << "' does not exist" << std::endl;
    return false;
  }
  // load hash_table 
  std::string hash_table_filename = data_folder + "hash_table.data";
  uint32_t hash_table_file_size = GetFileSize(hash_table_filename.c_str());
  table_size_ = hash_table_file_size / sizeof(uint32_t);
  hash_table_ = new uint32_t[table_size_];
  uint32_t hash_table_read_size = ReadFile(hash_table_filename.c_str(),
      reinterpret_cast<uint8_t*>(hash_table_), hash_table_file_size);
  std::cout << "table_size: " << table_size_ << std::endl;

  // load str_data
  std::string str_data_filename = data_folder + "str_data.data";
  uint32_t str_data_file_size = GetFileSize(str_data_filename.c_str());
  str_data_.resize(str_data_file_size);
  uint32_t str_data_read_size = ReadFile(str_data_filename.c_str(),
      reinterpret_cast<uint8_t*>(&str_data_[0]), str_data_file_size);
  std::cout << "str_data_file_size: " << str_data_file_size << std::endl;

  // load index
  std::string index_filename = data_folder + "index.data";
  uint32_t index_file_size = GetFileSize(index_filename.c_str());
  index_.resize(index_file_size / sizeof(uint32_t));
  uint32_t index_read_size = ReadFile(index_filename.c_str(),
      reinterpret_cast<uint8_t*>(&index_[0]), index_file_size);
  std::cout << "index size: " << index_.size() << std::endl;
}

uint32_t StrRoster::AddStrToDict(std::string str) {
  // Try to see if we have already had that str or not
  uint32_t id = Str2ID(str);
  if (id != 0) return id;

  // If the str not exist, add it
  // Try all the hash functions to see if we can solve confliction
  uint32_t hash_value = 0;
  for(HASH_FUNCTION_PTR hash : hash_functions_) {
    hash_value = hash(str.c_str()) % table_size_;
    if (hash_table_[hash_value] == 0) {
      return InternalAdd(str, hash_value);
    }
  }
  // After try all the hash functions, still conflict
  // Try to solve conflict by adding numbers
  int base_hash_value = hash_functions_[0](str.c_str()) % table_size_;
  for (int i = 1; i <= HASH_RETRY_NUM; ++i) {
    int t = i * i;
    hash_value = (base_hash_value + i * i) % table_size_;
    if (hash_table_[hash_value] == 0) {
      return InternalAdd(str, hash_value);
    }
  }
  std::cout << "Error: After trying " << HASH_RETRY_NUM + 6
    << " times, still can't solve conflict for '"
    << str << "', please increase estimated_count (which now is "
    << estimated_count_ <<") and try again." << std::endl;
  return 0;
}

uint32_t StrRoster::Str2ID(std::string str) {
  uint32_t hash_value = 0;
  uint32_t id = 0;
  // Try all the hash functions to see if str is there
  for(HASH_FUNCTION_PTR hash : hash_functions_) {
    hash_value = hash(str.c_str()) % table_size_;
    id = hash_table_[hash_value];
    if (id == 0) {
      return 0;
    } else if (str == ID2Str(id)) {
      return id;
    }
  }
  // After try all the hash functions, we still can't find the str
  // continue try to find it use same method as AddStrToDict
  int base_hash_value = hash_functions_[0](str.c_str()) % table_size_;
  for (int i = 1; i <= HASH_RETRY_NUM; ++i) {
    int t = i * i;
    hash_value = (base_hash_value + i * i) % table_size_;
    id = hash_table_[hash_value];
    if (id == 0) {
      return 0;
    } else if (str == ID2Str(id)) {
      return id;
    }
  }
  return 0;
}

bool StrRoster::ID2Str(uint32_t id, std::string* p_str) {
  // Caller's responsibility to clear str by themselves.
  if (id <= 0 || id >= index_.size()) return false;
  uint32_t l = index_[id-1];
  uint32_t r = index_[id];
  for (int i = l; i < r; ++i) {
    p_str->push_back(str_data_[i]);
  }
}

std::string StrRoster::ID2Str(uint32_t id) {
  std::string str;
  ID2Str(id, &str);
  return str;
}

uint32_t StrRoster::InternalAdd(std::string& str, uint32_t hash_value) {
  hash_table_[hash_value] = index_.size();
  for (auto c : str) {
    str_data_.push_back(c);
  }
  index_.push_back(str_data_.size());
  return index_.size()-1;
}

}  // namespace StrRosterNS
