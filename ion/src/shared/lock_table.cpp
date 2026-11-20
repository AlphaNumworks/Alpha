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
    if (isTable(r)) {
        return true;
    }
    return (getLockv(r) & 1u) == 1;
}

bool LockTable::makeLockable(
    InternalStorage::Record r
)
{
    if (isTable(r)) return true;
    if (isLocked()) return false;

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
    if (isTable(r) || isLocked()) return false;

    char * ptr = lockvPtr(r);

    if (ptr != nullptr) {
        *(lockv_t *)ptr &= ~(lockv_t)(1u);
    }
    return true;
}

void LockTable::remove(
    InternalStorage::Record r
)
{
    if (r.isNull() || isLocked()) {
        return;
    }

    char * entry = entryPtr(tableValue(), r.fullNameCRC32());

    if (entry != nullptr) {
        memset(entry, 0, k_locktableEntrySize);
    }
}

void LockTable::rename(
    InternalStorage::Record oldRecord,
    InternalStorage::Record newRecord
)
{
    if (oldRecord.isNull() || newRecord.isNull() || oldRecord == newRecord || isTable(oldRecord) || isTable(newRecord) || isLocked()) {
        return;
    }

    remove(newRecord);

    char * entry = entryPtr(tableValue(), oldRecord.fullNameCRC32());

    if (entry != nullptr) {
        uint32_t crc = newRecord.fullNameCRC32();
        memcpy(entry, &crc, sizeof(uint32_t));
    }
}

bool LockTable::isTable(
    InternalStorage::Record r
)
{
    return r == InternalStorage::Record(k_locktableName);
}

InternalStorage::Record::Data LockTable::tableValue()
{
    InternalStorage::Record table = Ion::Storage::sharedStorage()->recordNamed(k_locktableName);

    if (table.isNull()) {
        return {.buffer = nullptr, .size = 0};
    }
    return table.value();
}

bool LockTable::isLockableIn(
    InternalStorage::Record::Data table,
    InternalStorage::Record r
)
{
    if (r.isNull()) {
        return false;
    }

    char * entry = entryPtr(table, r.fullNameCRC32());

    return entry != nullptr && (*(lockv_t *)(entry + sizeof(uint32_t)) & 1u) == 1;
}

bool LockTable::isLocked()
{
    return Ion::Storage::sharedStorage()->m_lockOffset != 0;
}

char * LockTable::entryPtr(
    InternalStorage::Record::Data table,
    uint32_t crc
)
{
    if (table.buffer == nullptr) return nullptr;

    char * end = (char *)table.buffer + table.size;

    for (char * p = (char *)table.buffer; p + k_locktableEntrySize <= end; p += k_locktableEntrySize) {
        uint32_t entryCRC;
        memcpy(&entryCRC, p, sizeof(uint32_t));
        if (entryCRC == crc) {
            return p;
        }
    }
    return nullptr;
}

char * LockTable::lockvPtr(
    InternalStorage::Record r
)
{
    if (r.isNull()) {
        return nullptr;
    }

    char * entry = entryPtr(tableValue(), r.fullNameCRC32());

    return (entry == nullptr) ? nullptr : entry + sizeof(uint32_t);
}

char * LockTable::createLockv(
    InternalStorage::Record r
)
{
    if (r.isNull()) {
        return nullptr;
    }

    uint32_t crc = r.fullNameCRC32();

    // NOTE: Reusing a old entry (CRC = 0)
    char * slot = entryPtr(tableValue(), 0);

    if (slot != nullptr) {
        memcpy(slot, &crc, sizeof(uint32_t));
        slot[sizeof(uint32_t)] = 0;
        return slot + sizeof(uint32_t);
    }

    Ion::Storage * storage        = Ion::Storage::sharedStorage();
    InternalStorage::Record table = storage->recordNamed(k_locktableName);

    uint8_t entry[k_locktableEntrySize];
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
