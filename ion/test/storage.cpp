#include <quiz.h>
#include <ion/storage.h>
#include <ion/lock_table.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

using namespace Ion;

Storage::Record::ErrorStatus putRecordInSharedStorage(const char * baseName, const char * extension, const char * data) {
  size_t dataSize = strlen(data);
  return Storage::sharedStorage()->createRecordWithExtension(baseName, extension, data, dataSize);
}

QUIZ_CASE(ion_storage_records_crc32) {
  const char * baseNameRecord = "ionTestStorage";
  const char * extensionRecord = "record1";
  const char * fullNameRecord = "ionTestStorage.record1";
  Storage::Record a(baseNameRecord, extensionRecord);
  Storage::Record b(fullNameRecord);
  quiz_assert(a==b);

  Storage::Record c("A.exp");
  Storage::Record d("B.exp");
  quiz_assert(c!=d);
}

QUIZ_CASE(ion_storage_store_and_destroy_record) {
  size_t initialStorageAvailableStage = Storage::sharedStorage()->availableSize();

  const char * baseNameRecord = "ionTestStorage";
  const char * extensionRecord = "record1";
  const char * dataRecord = "This is a test to ensure one can create, retrieve, modify and delete records in ion's shared storage.";

  // Put a record in the store
  Storage::Record::ErrorStatus error = putRecordInSharedStorage(baseNameRecord, extensionRecord, dataRecord);
  quiz_assert(error == Storage::Record::ErrorStatus::None);

  // Retrieve the record
  Storage::Record retrievedRecord = Storage::sharedStorage()->recordBaseNamedWithExtension(baseNameRecord, extensionRecord);
  size_t dataRecordSize = strlen(dataRecord);
  quiz_assert(retrievedRecord.value().size == dataRecordSize);
  quiz_assert(strcmp(dataRecord, static_cast<const char *>(retrievedRecord.value().buffer)) == 0);

  // Destroy it
  retrievedRecord.destroy();
  retrievedRecord = Storage::sharedStorage()->recordBaseNamedWithExtension(baseNameRecord, extensionRecord);
  quiz_assert(retrievedRecord == Storage::Record());
  quiz_assert(Storage::sharedStorage()->availableSize() == initialStorageAvailableStage);
}


QUIZ_CASE(ion_storage_put_record_twice) {
  size_t initialStorageAvailableStage = Storage::sharedStorage()->availableSize();

  const char * baseNameRecord = "ionTestStorage";
  const char * extensionRecord = "record";
  const char * dataRecord = "This is a test to ensure one can create, retrieve, modify and delete records in ion's shared storage.";

  // Put a record in the store
  Storage::Record::ErrorStatus error = putRecordInSharedStorage(baseNameRecord, extensionRecord, dataRecord);
  quiz_assert(error == Storage::Record::ErrorStatus::None);

  // Put the same record again: an error should be issued
  error = putRecordInSharedStorage(baseNameRecord, extensionRecord, dataRecord);
  quiz_assert(error == Storage::Record::ErrorStatus::NameTaken);

  // Retrieve the record
  Storage::Record retrievedRecord = Storage::sharedStorage()->recordBaseNamedWithExtension(baseNameRecord, extensionRecord);
  size_t dataRecordSize = strlen(dataRecord);
  quiz_assert(retrievedRecord.value().size == dataRecordSize);
  quiz_assert(strcmp(dataRecord, static_cast<const char *>(retrievedRecord.value().buffer)) == 0);

  // Destroy it
  retrievedRecord.destroy();
  retrievedRecord = Storage::sharedStorage()->recordBaseNamedWithExtension(baseNameRecord, extensionRecord);
  quiz_assert(retrievedRecord == Storage::Record());
  quiz_assert(Storage::sharedStorage()->availableSize() == initialStorageAvailableStage);
}

QUIZ_CASE(ion_storage_invalid_renaming) {
  size_t initialStorageAvailableStage = Storage::sharedStorage()->availableSize();

  const char * baseNameRecord = "ionTestStorage";
  const char * extensionRecord = "record1";
  const char * dataRecord = "This is a test to ensure one can create, retrieve, modify and delete records in ion's shared storage.";

  // Put a record in the store
  Storage::Record::ErrorStatus error = putRecordInSharedStorage(baseNameRecord, extensionRecord, dataRecord);
  quiz_assert(error == Storage::Record::ErrorStatus::None);

  // Retrieve the record
  Storage::Record retrievedRecord = Storage::sharedStorage()->recordBaseNamedWithExtension(baseNameRecord, extensionRecord);
  size_t dataRecordSize = strlen(dataRecord);
  quiz_assert(retrievedRecord.value().size == dataRecordSize);
  quiz_assert(strcmp(dataRecord, static_cast<const char *>(retrievedRecord.value().buffer)) == 0);

  // Rename the record with an invalid name
  const char * fullNameRecord2 = "invalidNameWithoutDot";
  error = retrievedRecord.setName(fullNameRecord2);
  quiz_assert(error == Storage::Record::ErrorStatus::NonCompliantName);

  // Destroy it
  retrievedRecord.destroy();
  retrievedRecord = Storage::sharedStorage()->recordBaseNamedWithExtension(baseNameRecord, extensionRecord);
  quiz_assert(retrievedRecord == Storage::Record());
  quiz_assert(Storage::sharedStorage()->availableSize() == initialStorageAvailableStage);
}

QUIZ_CASE(ion_storage_valid_renaming) {
  size_t initialStorageAvailableStage = Storage::sharedStorage()->availableSize();

  const char * baseNameRecord = "ionTestStorage";
  const char * extensionRecord = "record1";
  const char * dataRecord = "This is a test to ensure one can create, retrieve, modify and delete records in ion's shared storage.";

  // Put a record in the store
  Storage::Record::ErrorStatus error = putRecordInSharedStorage(baseNameRecord, extensionRecord, dataRecord);
  quiz_assert(error == Storage::Record::ErrorStatus::None);

  // Retrieve the record
  Storage::Record retrievedRecord = Storage::sharedStorage()->recordBaseNamedWithExtension(baseNameRecord, extensionRecord);
  size_t dataRecordSize = strlen(dataRecord);
  quiz_assert(retrievedRecord.value().size == dataRecordSize);
  quiz_assert(strcmp(dataRecord, static_cast<const char *>(retrievedRecord.value().buffer)) == 0);

  // Rename the record with a valid name
  const char * newFullNameRecord = "testStorage.record2";
  error = retrievedRecord.setName(newFullNameRecord);
  quiz_assert(error == Storage::Record::ErrorStatus::None);

  // Retrieve the previous record
  Storage::Record oldRetrievedRecord = Storage::sharedStorage()->recordBaseNamedWithExtension(baseNameRecord, extensionRecord);
  quiz_assert(oldRetrievedRecord == Storage::Record());

  // Retrieve the new record
  Storage::Record newRetrievedRecord = Storage::sharedStorage()->recordNamed(newFullNameRecord);
  quiz_assert(strcmp(dataRecord, static_cast<const char *>(newRetrievedRecord.value().buffer)) == 0);

  // Destroy it
  newRetrievedRecord.destroy();
  newRetrievedRecord = Storage::sharedStorage()->recordBaseNamedWithExtension(baseNameRecord, extensionRecord);
  quiz_assert(newRetrievedRecord == Storage::Record());
  quiz_assert(Storage::sharedStorage()->availableSize() == initialStorageAvailableStage);
}

QUIZ_CASE(ion_storage_available_space_moving) {
  const char * extensionRecord = "record1";
  const char * baseNameRecord1 = "ionTestStorage1";
  const char * dataRecord1 = "This is a test to ensure one can edit a record in the shared storage - first record.";
  const char * baseNameRecord2 = "ionTestStorage2";
  const char * dataRecord2 = "This is a test to ensure one can edit a record in the shared storage - second record.";
  const char * baseNameRecord3 = "ionTestStorage3";
  const char * baseNameRecord4 = "ionTestStorage4";
  const char * dataRecord3 = R"(
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

aaaaaaaaaaaaaaaaaaaaa

aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

aaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaa

aaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaa
aaaaaaa
aaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaa
aaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

aaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaa
aaaaaaaaaaaa
aaaaaaaaaaaa
aaaaaaaaa
aaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaa
aaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaa

aaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaa

aaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

aaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaa
aaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaa
aaaa
aaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

aaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaa
aaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaa
aaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaa
aaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaa

aaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaa
aaaa
aaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaa
aaaa
aaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaa
aaaa
aaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

aaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

aaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

aaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaa
aaaaaaaaa
aaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaa
aaaaaaaaa
aaaaaaaaaaaaaaaa
aaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaa
aaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaa




aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaa

aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaa
aaaaaa)";

  // Put the records in the store
  Storage::Record::ErrorStatus error = putRecordInSharedStorage(baseNameRecord1, extensionRecord, dataRecord1);
  quiz_assert(error == Storage::Record::ErrorStatus::None);
  error = putRecordInSharedStorage(baseNameRecord2, extensionRecord, dataRecord2);
  quiz_assert(error == Storage::Record::ErrorStatus::None);
  error = putRecordInSharedStorage(baseNameRecord3, extensionRecord, dataRecord3);
  quiz_assert(error == Storage::Record::ErrorStatus::None);
  error = putRecordInSharedStorage(baseNameRecord4, extensionRecord, dataRecord3);
  quiz_assert(error == Storage::Record::ErrorStatus::None);

  // Retrieve the record
  Storage::Record retrievedRecord1 = Storage::sharedStorage()->recordBaseNamedWithExtension(baseNameRecord1, extensionRecord);
  Storage::Record retrievedRecord2 = Storage::sharedStorage()->recordBaseNamedWithExtension(baseNameRecord2, extensionRecord);
  Storage::Record retrievedRecord3 = Storage::sharedStorage()->recordBaseNamedWithExtension(baseNameRecord3, extensionRecord);
  Storage::Record retrievedRecord4 = Storage::sharedStorage()->recordBaseNamedWithExtension(baseNameRecord4, extensionRecord);

  // Put the the available space at the end of the first record and remove it
  size_t availableSpace = Storage::sharedStorage()->availableSize();
  uint32_t checksumBeforeChanges = Storage::sharedStorage()->checksum();
  Storage::sharedStorage()->putAvailableSpaceAtEndOfRecord(retrievedRecord1);
  Storage::sharedStorage()->getAvailableSpaceFromEndOfRecord(retrievedRecord1, availableSpace);
  quiz_assert(Storage::sharedStorage()->availableSize() == availableSpace);
  quiz_assert(Storage::sharedStorage()->checksum() == checksumBeforeChanges);

  // Destroy it
  retrievedRecord1.destroy();
  retrievedRecord2.destroy();
  retrievedRecord3.destroy();
  retrievedRecord4.destroy();
}

QUIZ_CASE(ion_lock_table) {
  Storage * storage = Storage::sharedStorage();
  size_t initialAvailableSize = storage->availableSize();

  // A file that's not in the Lock Table is not lockable
  Storage::Record a("lockTestA.py");
  Storage::Record b("lockTestB.py");
  quiz_assert(!LockTable::isLockable(a));
  quiz_assert(LockTable::getLockv(a) == 0);

  // The key is the name onlyn the record doesn't have to exist
  quiz_assert(LockTable::makeLockable(a));
  quiz_assert(LockTable::isLockable(a));
  quiz_assert(!LockTable::isLockable(b));

  // A record in the storage, before and after the table keeps its content
  const char * data = "print(1)";
  quiz_assert(storage->createRecordWithFullName("lockTestB.py", data, strlen(data)) == Storage::Record::ErrorStatus::None);
  quiz_assert(LockTable::makeLockable(b));
  quiz_assert(LockTable::isLockable(a));
  quiz_assert(LockTable::isLockable(b));
  Storage::Record::Data value = storage->recordNamed("lockTestB.py").value();
  quiz_assert(value.size == strlen(data) && memcmp(value.buffer, data, value.size) == 0);

  // Changing the content of a file doesn't change its lockv
  const char * newData = "print(1+1)";
  quiz_assert(b.setValue({.buffer = newData, .size = strlen(newData)}) == Storage::Record::ErrorStatus::None);
  quiz_assert(LockTable::isLockable(b));

  quiz_assert(LockTable::makeNotLockable(a));
  quiz_assert(!LockTable::isLockable(a));
  quiz_assert(LockTable::isLockable(b));

  storage->recordNamed("lockv.cfg").destroy();
  b.destroy();
  quiz_assert(storage->availableSize() == initialAvailableSize);
}

QUIZ_CASE(ion_lock_table_grows_in_place) {
  Storage * storage = Storage::sharedStorage();
  size_t initialAvailableSize = storage->availableSize();

  // A record stored after the table must not be altered when the table grows
  const char * data = "print(1)";
  quiz_assert(storage->createRecordWithFullName("lockTestC.py", data, strlen(data)) == Storage::Record::ErrorStatus::None);

  // More than the 64 entries the temporary buffer used to allow
  constexpr int numberOfEntries = 100;
  char name[16];
  for (int i = 0; i < numberOfEntries; i++) {
    snprintf(name, sizeof(name), "lockT%d.py", i);
    quiz_assert(LockTable::makeLockable(Storage::Record(name)));
  }
  for (int i = 0; i < numberOfEntries; i++) {
    snprintf(name, sizeof(name), "lockT%d.py", i);
    quiz_assert(LockTable::isLockable(Storage::Record(name)));
  }
  quiz_assert(!LockTable::isLockable(Storage::Record("lockT100.py")));
  Storage::Record::Data value = storage->recordNamed("lockTestC.py").value();
  quiz_assert(value.size == strlen(data) && memcmp(value.buffer, data, value.size) == 0);
  quiz_assert(storage->recordNamed("lockv.cfg").value().size == numberOfEntries * LockTable::k_locktableEntrySize);

  storage->recordNamed("lockv.cfg").destroy();

  // A table with a partial entry is corrupted: a new one is started
  const char * garbage = "1234567";
  quiz_assert(storage->createRecordWithFullName("lockv.cfg", garbage, strlen(garbage)) == Storage::Record::ErrorStatus::None);
  quiz_assert(LockTable::makeLockable(Storage::Record("lockTestC.py")));
  quiz_assert(LockTable::isLockable(Storage::Record("lockTestC.py")));
  quiz_assert(storage->recordNamed("lockv.cfg").value().size == LockTable::k_locktableEntrySize);

  storage->recordNamed("lockv.cfg").destroy();
  storage->recordNamed("lockTestC.py").destroy();
  quiz_assert(storage->availableSize() == initialAvailableSize);
}

QUIZ_CASE(ion_lock_table_follows_records) {
  Storage * s = Storage::sharedStorage();
  const char * d = "print(1)";

  // The second creation makes the table grow before the new record.
  quiz_assert(s->createRecordWithFullName("lockU1.py", d, strlen(d), true) == Storage::Record::ErrorStatus::None);
  quiz_assert(s->createRecordWithFullName("lockU2.py", d, strlen(d), true) == Storage::Record::ErrorStatus::None);
  Storage::Record::Data v = s->recordNamed("lockU2.py").value();
  quiz_assert(v.size == strlen(d) && memcmp(v.buffer, d, v.size) == 0);
  quiz_assert(LockTable::isLockable(Storage::Record("lockU1.py")));
  quiz_assert(LockTable::isLockable(Storage::Record("lockU2.py")));

  // Renaming keeps the lock state.
  quiz_assert(s->recordNamed("lockU1.py").setBaseNameWithExtension("lockU3", "py") == Storage::Record::ErrorStatus::None);
  quiz_assert(LockTable::isLockable(Storage::Record("lockU3.py")));
  quiz_assert(!LockTable::isLockable(Storage::Record("lockU1.py")));

  // A destroyed record is forgotten: a new record with the same name is not lockable.
  s->recordNamed("lockU2.py").destroy();
  quiz_assert(s->createRecordWithFullName("lockU2.py", d, strlen(d)) == Storage::Record::ErrorStatus::None);
  quiz_assert(!LockTable::isLockable(Storage::Record("lockU2.py")));
  quiz_assert(LockTable::isLockable(Storage::Record("lockU3.py")));

  s->destroyAllRecords();
}

QUIZ_CASE(ion_lock_table_lock_records) {
  Storage * s = Storage::sharedStorage();
  const char * d = "print(1)";
  const char * e = "print(22)";
  quiz_assert(s->createRecordWithFullName("keepA.py", d, strlen(d), true) == Storage::Record::ErrorStatus::None);
  quiz_assert(s->createRecordWithFullName("drop.eq", d, strlen(d)) == Storage::Record::ErrorStatus::None);
  quiz_assert(s->createRecordWithFullName("keepC.py", e, strlen(e), true) == Storage::Record::ErrorStatus::None);

  s->lockRecords();
  // The lockable records (and the table) are hidden, the others are destroyed
  quiz_assert(s->numberOfRecords() == 0);
  quiz_assert(s->recordNamed("keepA.py").isNull());

  // The table is read-only in exam mode
  quiz_assert(!LockTable::makeLockable(Storage::Record("other.py")));

  // A record created in exam mode is visible and not lockable
  quiz_assert(s->createRecordWithFullName("exam.py", d, strlen(d), true) == Storage::Record::ErrorStatus::None);
  quiz_assert(s->numberOfRecords() == 1);
  quiz_assert(!LockTable::isLockable(Storage::Record("exam.py")));

  // Locking again must not touch the locked records
  s->lockRecords();
  quiz_assert(s->numberOfRecords() == 1);

  s->unlockRecords();
  quiz_assert(s->numberOfRecords() == 4);
  Storage::Record::Data a = s->recordNamed("keepA.py").value();
  quiz_assert(a.size == strlen(d) && memcmp(a.buffer, d, a.size) == 0);
  Storage::Record::Data c = s->recordNamed("keepC.py").value();
  quiz_assert(c.size == strlen(e) && memcmp(c.buffer, e, c.size) == 0);
  quiz_assert(s->recordNamed("drop.eq").isNull());
  quiz_assert(LockTable::isLockable(Storage::Record("keepA.py")));
  quiz_assert(LockTable::isLockable(Storage::Record("keepC.py")));

  s->destroyAllRecords();
}
