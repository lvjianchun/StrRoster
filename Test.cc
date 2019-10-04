#include <iostream>

#include "StrRoster.h"

using namespace StrRosterNS;

void Add(StrRoster* sr, std::string str) {
  uint32_t id = sr->AddStrToDict(str);
  std::cout << "Add string '" << str << "', assigned id is: " << id << std::endl;
}

void SearchStr(StrRoster* sr, std::string str) {
  uint32_t id = sr->Str2ID(str);
  std::cout << "Search string '" << str << "', found id is: " << id << std::endl;
}

void SearchID(StrRoster* sr, uint32_t id) {
  std::string str;
  bool succ = sr->ID2Str(id, str);
  std::cout << "Search id '" << id << "', succeed:" << (succ ? "true" : "false") << ", corresponding string is: " << str << std::endl;
}

int main() {
  StrRoster sr(4);
  std::vector<std::string> test_data;
  test_data.push_back("hello");
  test_data.push_back("world");
  test_data.push_back("foo");
  test_data.push_back("bar");
  test_data.push_back("");

  for (std::string s : test_data) {
    Add(&sr, s);
  }
  for (std::string s : test_data) {
    SearchStr(&sr, s);
  }
  for (int i = 0; i <= test_data.size()+1; ++i) {
    SearchID(&sr, i);
  }
  for (std::string s : test_data) {
    SearchStr(&sr, s+"a");
  }
}
