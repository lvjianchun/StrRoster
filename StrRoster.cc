#include <cstring>
#include <iostream>

#include "StrRoster.h"
#include "HashFunctions.h"

#define HASH_RETRY_NUM 26

namespace StrRosterNS {

HASH_FUNCTION_PTR StrRoster::hash_functions_[6] = {BKDRHash, APHash, DJBHash, JSHash, RSHash, SDBMHash};

StrRoster::StrRoster(size_t estimated_count) {
  size_ = 0;
  estimated_count_ = estimated_count;
  table_size_ = estimated_count < 65536 ? estimated_count * 4 : estimated_count / 2 * 5;
  hash_table_ = new uint32_t[table_size_];
  memset(hash_table_, 0, table_size_);
  index_.reserve(estimated_count + 100);
  index_.push_back(0);
}

StrRoster::StrRoster(std::string data_folder) {
}

StrRoster::~StrRoster() {
  delete[] hash_table_;
}

bool StrRoster::Dump(std::string data_folder) {
}

bool StrRoster::Load(std::string data_folder) {
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
      return internal_add(str, hash_value);
    }
  }
  // After try all the hash functions, still conflict
  // Try to solve conflict by adding numbers
  int base_hash_value = hash_functions_[0](str.c_str()) % table_size_;
  for (int i = 1; i <= HASH_RETRY_NUM; ++i) {
    int t = i * i;
    hash_value = (base_hash_value + i * i) % table_size_;
    if (hash_table_[hash_value] == 0) {
      return internal_add(str, hash_value);
    }
  }
  std::cout << "After trying " << HASH_RETRY_NUM + 6
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

bool StrRoster::ID2Str(uint32_t id, std::string& str) {
  // Caller's responsibility to clear str by themselves.
  if (id <= 0 || id > size_) return false;
  uint32_t l = index_[id-1];
  uint32_t r = index_[id];
  for (int i = l; i < r; ++i) {
    str.push_back(str_data_[i]);
  }
}

std::string StrRoster::ID2Str(uint32_t id) {
  std::string str;
  ID2Str(id, str);
  return str;
}

uint32_t StrRoster::internal_add(std::string& str, uint32_t hash_value) {
  ++size_;
  hash_table_[hash_value] = size_;
  for (auto c : str) {
    str_data_.push_back(c);
  }
  index_.push_back(0);
  index_[size_] = str_data_.size();
  return size_;
}

}  // namespace StrRosterNS
