/*
 * This file is part of StrRoster project
 *
 * Scenario: build (write) a [str <-> id] dict offline, and serve (read) online.
 * 
 * StrRoster is a software tool which user can translate between string and ID
 * This tool will generate a series of consecutive ID from 1 for strings (0 for not exist)
 *
 * For simpily explainition, you can imagine here implemented an unordered_map<string, int>
 * ID is auto increment
 * If we have 4 strings: hello world foo bar
 * We wil have a function Str2ID, which will convert string to ID
 *   hello -> 1
 *   world -> 2
 *   foo   -> 3
 *   bar   -> 4
 * We also have a function ID2Str, which convert ID to string
 *   1 -> hello
 *   2 -> world
 *   3 -> foo
 *   4 -> bar
 *
 * Provide dump/load functions, which is super convinent for persistent
 *
 * Warning:
 *   1. We don't consider big endian or little endian of the machine
 *      Please load data generated from the same architecture to avoid confusion
 *   2. We don't support multi-thread for writing
 */
#ifndef STR_ROSTER_STR_ROSTER_H_
#define STR_ROSTER_STR_ROSTER_H_

#include <cstdint>
#include <string>
#include <vector>

#include "HashFunctions.h"

namespace StrRosterNS {

class StrRoster {
 public:
  /**
   * Ctor
   *
   * Init a database based on the estimated count
   *
   * @param estimated_count estimated count of strings
   */
  explicit StrRoster(size_t estimated_count);

  /**
   * Ctor
   *
   * Init a database based on an existing data
   *
   * @param data_folder Which contains the existing data
   *        load it to create a StrRoster which can serve immediately
   */
  explicit StrRoster(std::string data_folder);

  /**
   * Dtor
   *
   * Destructor to release all the possessed resources
   */
  ~StrRoster();

  /**
   * Store data to a folder
   *
   * @param data_folder Folder which will used to store the data
   * @return true if succeed, false others
   */
  bool Dump(std::string data_folder);

  /**
   * Restore data from a folder
   *
   * @param data_folder Folder which stored the data
   * @return true if succeed, false others
   */
  bool Load(std::string data_folder);

  /**
   * Add a string to our dict, in memory only, won't change data on disk
   *
   * @param str The string which we want to add
   * @return ID for that str
   */
  uint32_t AddStrToDict(std::string str);

  /**
   * Given a string, find its corresponding ID
   *
   * @param str The string which we want to get the ID for
   * @return ID for that str
   */
  uint32_t Str2ID(std::string str);

  /**
   * Given a ID, find its corresponding string and return if it succeed
   *
   * In some case, mpty string can't tell it's empty or not succeed
   * Ideally this function is unnecessary, just in case someont need it
   *
   * @param id Integer which can be used to retrive a string
   * @param str Out variable to store retrived string if succeed
   * @return succeed or not
   */
  bool ID2Str(uint32_t id, std::string& str);

  /**
   * Given a ID, find its corresponding string
   *
   * @param id Integer which can be used to retrive a string
   * @return string for that id
   */
  std::string ID2Str(uint32_t id);

 private:
  // variables
  static HASH_FUNCTION_PTR hash_functions_[6];
  uint32_t size_;
  uint32_t estimated_count_;
  uint32_t table_size_;
  uint32_t* hash_table_;
  std::vector<char> str_data_;
  std::vector<uint32_t> index_;

  // functions
  uint32_t internal_add(std::string& str, uint32_t hash_value);
};

}  // namespace StrRosterNS
#endif  // STR_ROSTER_STR_ROSTER_H_
