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

            static lockv_t getLockvByFullname(char * buffer, const char * fullname);

        protected:
            static char * lockvPtrByFullname(char * buffer, const char * fullname);

        private:
            // Pass
    };

}

#endif /* ION_LOCK_TABLE_H */
