#include "ion.h"
#include "ion/storage.h"
#include <ion/lock_table.h>

namespace Ion
{

    LockTable::lockv_t LockTable::getLockvByFullname(const char *fullname)
    {
        char * lock_ptr = lockvPtrByFullname(fullname);

        if (lock_ptr == nullptr) {
            return 0;
        }
        return *(lockv_t *)lock_ptr;
    }

    bool LockTable::isLockableByFullname(const char *fullname)
    {
        return (getLockvByFullname(fullname) & (1u)) == 1;
    }

    char * LockTable::lockvPtrByFullname(const char *fullname)
    {
        InternalStorage::Record r = Ion::Storage::sharedStorage()->recordNamed("lockv.cfg");

        if (r.isNull()) {
            return nullptr;
        }

        InternalStorage::Record::Data v = r.value();
        uint32_t crc = InternalStorage::Record(fullname).fullNameCRC32();
        char * end = (char *)v.buffer + v.size;
        for (char * p = (char*)v.buffer; p + k_locktableEntrySize <= end; p += k_locktableEntrySize) {
            uint32_t otherCRC;
            memcpy(&otherCRC, p, sizeof(uint32_t));
            if (otherCRC == crc) {
                return p + sizeof(uint32_t);
            }
        }
        return nullptr;
    }

}
