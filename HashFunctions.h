#ifndef STR_ROSTER_HASH_FUNCTIONS_H_
#define STR_ROSTER_HASH_FUNCTIONS_H_
#include <cstdint>

namespace StrRosterNS {

uint32_t BKDRHash(const char *str);
uint32_t APHash(const char *str);
uint32_t DJBHash(const char *str);
uint32_t JSHash(const char *str);
uint32_t RSHash(const char *str);
uint32_t SDBMHash(const char *str);

}  // namespace StrRosterNS

#endif // STR_ROSTER_HASH_FUNCTIONS_H_
