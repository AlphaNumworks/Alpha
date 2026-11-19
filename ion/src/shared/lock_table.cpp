#include "ion.h"
#include "ion/storage.h"
#include <ion/lock_table.h>

namespace Ion
{

    LockTable::lockv_t LockTable::getLockvByFullname(char *buffer, const char *fullname)
    {
        char * lock_ptr = lockvPtrByFullname(buffer, fullname);

        if (lock_ptr == nullptr) {
            return 0;
        }
        return *(lockv_t *)lock_ptr;
    }

    char * LockTable::lockvPtrByFullname(char *buffer, const char *fullname)
    {
        InternalStorage::Record r = Ion::Storage::sharedStorage()->recordNamed("lock.meta");

        if (r.isNull()) {
            return nullptr;
        }

        InternalStorage::Record::Data v = r.value();
        uint32_t crc = InternalStorage::Record(fullname).fullNameCRC32();
        for (char * p = (char*)v.buffer; p < (char*)v.buffer + v.size; p += k_locktableEntrySize) {
            uint32_t otherCRC;
            memcpy(&otherCRC, p, sizeof(uint32_t));
            if (otherCRC == crc) {
                return p + sizeof(uint32_t);
            }
        }
        return nullptr;
    }

}
