#ifndef ION_LOCK_TABLE_H
#define ION_LOCK_TABLE_H

#include "ion/storage.h"
#include <cstdint>

namespace Ion {

class LockTable {
  static constexpr char   k_locktableName[] = "lockv.cfg";
  static constexpr size_t k_maxEntries = 64;

public:
  typedef uint8_t lockv_t;
  static constexpr size_t k_locktableEntrySize = sizeof(lockv_t) + sizeof(uint32_t);

  static lockv_t getLockv(InternalStorage::Record r);
  static bool isLockable(InternalStorage::Record r);

  static bool makeLockable(InternalStorage::Record r);
  static bool makeNotLockable(InternalStorage::Record r);

protected:
  static char * lockvPtr(InternalStorage::Record r);
  static char * createLockv(InternalStorage::Record r);
};

}

#endif /* ION_LOCK_TABLE_H */
