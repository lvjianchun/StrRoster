#include "HashFunctions.h"

// BKDR Hash Function
uint32_t BKDRHash(const char *str) {
  uint32_t seed = 131; // 31 131 1313 13131 131313 etc..
  uint32_t hash = 0;
 
  while (*str) {
    hash = hash * seed + (*str++);
  }
 
  return (hash & 0x7FFFFFFF);
}

// AP Hash Function
uint32_t APHash(const char *str) {
  uint32_t hash = 0;
  int i;
 
  for (i=0; *str; i++) {
    if ((i & 1) == 0) {
      hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
    } else {
      hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
    }
  }
 
  return (hash & 0x7FFFFFFF);
}
 
// DJB Hash Function
uint32_t DJBHash(const char *str) {
  uint32_t hash = 5381;
 
  while (*str) {
    hash += (hash << 5) + (*str++);
  }
 
  return (hash & 0x7FFFFFFF);
}
 
// JS Hash Function
uint32_t JSHash(const char *str) {
  uint32_t hash = 1315423911;
 
  while (*str) {
    hash ^= ((hash << 5) + (*str++) + (hash >> 2));
  }
 
  return (hash & 0x7FFFFFFF);
}
 
// RS Hash Function
uint32_t RSHash(const char *str) {
  uint32_t b = 378551;
  uint32_t a = 63689;
  uint32_t hash = 0;
 
  while (*str) {
    hash = hash * a + (*str++);
    a *= b;
  }
 
  return (hash & 0x7FFFFFFF);
}
 
// SDB Hash Function
uint32_t SDBMHash(const char *str) {
  uint32_t hash = 0;
 
  while (*str) {
    // equivalent to: hash = 65599*hash + (*str++);
    hash = (*str++) + (hash << 6) + (hash << 16) - hash;
  }
 
  return (hash & 0x7FFFFFFF);
}
 
