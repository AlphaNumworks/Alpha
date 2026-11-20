#ifndef ION_LOCK_TABLE_H
#define ION_LOCK_TABLE_H

#include "ion/storage.h"
#include <cstdint>

namespace Ion {

class LockTable {
  static constexpr char   k_locktableName[] = "lockv.cfg";

public:
  typedef uint8_t lockv_t;
  static constexpr size_t k_locktableEntrySize = sizeof(lockv_t) + sizeof(uint32_t);

  static lockv_t getLockv(InternalStorage::Record r);
  static bool isLockable(InternalStorage::Record r);

  static bool makeLockable(InternalStorage::Record r);
  static bool makeNotLockable(InternalStorage::Record r);

  static void remove(InternalStorage::Record r);
  static void rename(InternalStorage::Record oldRecord, InternalStorage::Record newRecord);

  static bool isTable(InternalStorage::Record r);
  static InternalStorage::Record::Data tableValue();
  static bool isLockableIn(InternalStorage::Record::Data table, InternalStorage::Record r);

protected:
  static bool isLocked();
  static char * entryPtr(InternalStorage::Record::Data table, uint32_t crc);
  static char * lockvPtr(InternalStorage::Record r);
  static char * createLockv(InternalStorage::Record r);
};

}

#endif /* ION_LOCK_TABLE_H */
