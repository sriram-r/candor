#include "runtime.h"
#include "heap.h" // Heap

#include <stdint.h> // uint32_t
#include <assert.h> // assert
#include <string.h> // strncmp
#include <sys/types.h> // size_t

namespace dotlang {

char* RuntimeAllocate(Heap* heap, uint32_t bytes) {
  return heap->new_space()->Allocate(bytes);
}

char* RuntimeLookupProperty(char* obj, char* key) {
  Heap::HeapTag key_tag = static_cast<Heap::HeapTag>(
      *reinterpret_cast<uint64_t*>(key));
  uint64_t mask = *reinterpret_cast<uint64_t*>(obj + 8);

  // Skip tag+size
  char* map = *reinterpret_cast<char**>(obj + 16) + 16;

  switch (key_tag) {
   case Heap::kTagString:
    {
      uint32_t* hash_addr = reinterpret_cast<uint32_t*>(key + 8);
      uint32_t hash = *hash_addr;
      uint32_t length = *reinterpret_cast<uint32_t*>(key + 16);
      char* value = key + 24;

      // If hash wasn't computed yet - compute it now
      if (hash == 0) {
        hash = ComputeHash(value, length);
        *hash_addr = hash;
      }

      // Do circular lookup into map, rehash if we returned to the start point
      uint32_t start = hash & mask;
      uint32_t index = start;
      for (; index != start - sizeof(void*); index += sizeof(void*)) {
        if (index > mask) {
          index = 0;
        }

        if (*reinterpret_cast<uint64_t*>(map + index) == 0) break;
        if (RuntimeCompare(*reinterpret_cast<char**>(map + index), key) == 0) {
          break;
        }
      }

      // If we hadn't looped - return slot's address
      if (index != ((start - sizeof(void*)) & mask)) {
        // Insert key if needed
        if (*reinterpret_cast<uint64_t*>(map + index) == 0) {
          *reinterpret_cast<char**>(map + index) = key;
        }
        return map + index + mask + sizeof(void*);
      }
    }
    break;
   default:
    assert(0 && "Not implemented yet");
  }

  return 0;
}


size_t RuntimeCompare(char* lhs, char* rhs) {
  Heap::HeapTag lhs_tag = static_cast<Heap::HeapTag>(
      *reinterpret_cast<uint64_t*>(lhs));
  Heap::HeapTag rhs_tag = static_cast<Heap::HeapTag>(
      *reinterpret_cast<uint64_t*>(rhs));

  if (lhs_tag != Heap::kTagString) assert(0 && "Not implemented yet");
  if (rhs_tag != Heap::kTagString) assert(0 && "Not implemented yet");

  uint64_t lhs_length = *reinterpret_cast<uint64_t*>(lhs + 16);
  uint64_t rhs_length = *reinterpret_cast<uint64_t*>(rhs + 16);

  return lhs_length < rhs_length ? -1 :
         lhs_length > rhs_length ? 1 :
         strncmp(lhs + 24, rhs + 24, lhs_length);
}

} // namespace dotlang
