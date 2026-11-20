#ifndef ION_LOCK_TABLE_H
    #define ION_LOCK_TABLE_H

#include <cstdint>

namespace Ion
{

    class LockTable
    {
        
        public:
            typedef uint8_t lockv_t;
            static constexpr int k_locktableEntrySize = sizeof(lockv_t) + sizeof(uint32_t);

            static lockv_t getLockvByFullname(const char * fullname);
            static bool isLockableByFullname(const char * fullname);

            static lockv_t getLockvByNameAndExtension(const char * name, const char * extension);
            static bool isLockableByNameAndExtension(const char * name, const char * extension);

        protected:
            static char * lockvPtrByFullname(const char * fullname);
            static char * lockvPtrByNameAndExtension(const char * fullname, const char * extension);

        private:
            // Pass
    };

}

#endif /* ION_LOCK_TABLE_H */
