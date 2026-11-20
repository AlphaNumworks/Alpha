#include <ion/lock_table.h>
#include <ion/storage.h>
#include <string.h>

namespace Ion {

constexpr char LockTable::k_locktableName[];

LockTable::lockv_t LockTable::getLockv(
    InternalStorage::Record r
)
{
    char * ptr = lockvPtr(r);
    return (ptr == nullptr) ? 0 : *(lockv_t *)ptr;
}

bool LockTable::isLockable(
    InternalStorage::Record r
)
{
    return (getLockv(r) & 1u) == 1;
}

bool LockTable::makeLockable(
    InternalStorage::Record r
)
{
    char * ptr = lockvPtr(r);

    if (ptr == nullptr) {
        ptr = createLockv(r);
    }
    if (ptr == nullptr) {
        return false;
    }
    *(lockv_t *)ptr |= 1u;
    return true;
}

bool LockTable::makeNotLockable(
    InternalStorage::Record r
)
{
    if (r.isNull()) {
        return false;
    }

    char * ptr = lockvPtr(r);

    if (ptr != nullptr) {
        *(lockv_t *)ptr &= ~(lockv_t)(1u);
    }
    return true;
}

char * LockTable::lockvPtr(
    InternalStorage::Record r
)
{
    if (r.isNull()) {
        return nullptr;
    }

    InternalStorage::Record table = Ion::Storage::sharedStorage()->recordNamed(k_locktableName);

    if (table.isNull()) {
        return nullptr;
    }

    InternalStorage::Record::Data v = table.value();
    uint32_t crc                    = r.fullNameCRC32();
    char * end                      = (char *)v.buffer + v.size;

    for (char * p = (char *)v.buffer; p + k_locktableEntrySize <= end; p += k_locktableEntrySize) {
        uint32_t entryCRC;
        memcpy(&entryCRC, p, sizeof(uint32_t));
        if (entryCRC == crc) {
            return p + sizeof(uint32_t);
        }
    }
    return nullptr;
}

char * LockTable::createLockv(
    InternalStorage::Record r
)
{
    if (r.isNull()) {
        return nullptr;
    }

    Ion::Storage * storage        = Ion::Storage::sharedStorage();
    InternalStorage::Record table = storage->recordNamed(k_locktableName);

    uint8_t  entry[k_locktableEntrySize];
    uint32_t crc = r.fullNameCRC32();
    memcpy(entry, &crc, sizeof(uint32_t));
    entry[sizeof(uint32_t)] = 0;

    if (!table.isNull() && table.value().size % k_locktableEntrySize != 0) {
        table.destroy();
        table = InternalStorage::Record();
    }

    InternalStorage::Record::ErrorStatus status;

    if (table.isNull()) {
        status = storage->createRecordWithFullName(k_locktableName, entry, sizeof(entry));
    } else {
        status = storage->appendToRecord(table, entry, sizeof(entry));
    }
    if (status != InternalStorage::Record::ErrorStatus::None) {
        return nullptr;
    }
    return lockvPtr(r);
}

}
