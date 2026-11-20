#include "ion.h"
#include "ion/storage.h"
#include <ion/lock_table.h>

namespace Ion
{

    bool LockTable::makeLockableByFullname(const char * fullname)
    {
        char * lock_ptr  = lockvPtrByFullname(fullname);

        if (lock_ptr == nullptr) {
            return false;
        }
        (*(lockv_t *)lock_ptr) |= 1u;
        return true;
    }

    bool LockTable::makeLockableByNameAndExtension(const char * name, const char * extension)
    {
        char * lock_ptr  = lockvPtrByNameAndExtension(name, extension);

        if (lock_ptr == nullptr) {
            return false;
        }
        (*(lockv_t *)lock_ptr) |= 1u;
        return true;
    }

    bool LockTable::makeNotLockableByFullname(const char * fullname)
    {
        char * lock_ptr  = lockvPtrByFullname(fullname);

        if (lock_ptr == nullptr) {
            return false;
        }
        (*(lockv_t *)lock_ptr) &= ~(lockv_t)(1u);
        return true;
    }

    bool LockTable::makeNotLockableByNameAndExtension(const char * name, const char * extension)
    {
        char * lock_ptr  = lockvPtrByNameAndExtension(name, extension);

        if (lock_ptr == nullptr) {
            return false;
        }
        (*(lockv_t *)lock_ptr) &= ~(lockv_t)(1u);
        return true;
    }

    LockTable::lockv_t LockTable::getLockvByFullname(const char *fullname)
    {
        char * lock_ptr = lockvPtrByFullname(fullname);

        if (lock_ptr == nullptr) {
            return 0;
        }
        return *(lockv_t *)lock_ptr;
    }

    LockTable::lockv_t LockTable::getLockvByNameAndExtension(const char * name, const char * extension)
    {
        char * lock_ptr = lockvPtrByNameAndExtension(name, extension);

        return (lock_ptr == nullptr) ? 0 : *(lockv_t *)lock_ptr;
    }

    bool LockTable::isLockableByFullname(const char *fullname)
    {
        return (getLockvByFullname(fullname) & (1u)) == 1;
    }

    bool LockTable::isLockableByNameAndExtension(const char * name, const char * extension)
    {
        return (getLockvByNameAndExtension(name, extension) & (1u)) == 1;
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

    char * LockTable::lockvPtrByNameAndExtension(const char * name, const char * extension)
    {
        InternalStorage::Record r = Ion::Storage::sharedStorage()->recordNamed("lockv.cfg");

        if (r.isNull()) {
            return nullptr;
        }

        InternalStorage::Record::Data v = r.value();
        uint32_t crc = InternalStorage::Record(name, extension).fullNameCRC32();
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
