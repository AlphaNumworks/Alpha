#ifndef SPRITE_SCRIPT_STORE_H
    #define SPRITE_SCRIPT_STORE_H
    #include "apps/sprites/sprite/sprite.h"
#include <cassert>
    #include <cstddef>
#include <cstring>

namespace Sprites
{

    class SpriteStore
    {

        public:
            static constexpr char k_spriteExtension[] = "spr";
            static constexpr size_t k_spriteExtensionLength = 3;

            SpriteStore();

            static Sprite spriteAtIdx(int idx)
            {
                return Sprite(Ion::Storage::sharedStorage()->recordWithExtensionAtIndex(k_spriteExtension, idx));
            }

            static Sprite SpriteNamed(const char * baseName) {
                return Sprite(Ion::Storage::sharedStorage()->recordBaseNamedWithExtension(baseName, k_spriteExtension));
            }

            static bool SpriteNameIsFree(const char * baseName) {
                return SpriteNamed(baseName).isNull();
            }

            static int NumberOfSprites(void) {
                return Ion::Storage::sharedStorage()->numberOfRecordsWithExtension(k_spriteExtension);
            }

            void DeleteAllSprites() {
                for (size_t i = NumberOfSprites(); i > 0; --i) {
                    Ion::Storage::sharedStorage()->recordWithExtensionAtIndex(k_spriteExtension, i).destroy();
                }
            }

            static Ion::Storage::Record::ErrorStatus AddSprite(const char * name)
            {
                uint8_t f[256] = {};
                if (!Sprite::nameCompliant(name)) {
                    return Ion::Storage::Record::ErrorStatus::NonCompliantName;
                }
                return Ion::Storage::sharedStorage()->createRecordWithExtension(name, k_spriteExtension, f, 256, true);
            }

            static Ion::Storage::Record::ErrorStatus AddNewSprite()
            {
                uint8_t f[256] = {};
                // NOTE:                 0b11111100 -> R(4)G(4)B(4)A(0)
                memset(f, (uint8_t)(0xFC), 256);
                return Ion::Storage::sharedStorage()->createRecordWithFullName(".spr", f, 256, true);
            }

            bool isFull(void)
            {
                return Ion::Storage::sharedStorage()->availableSize() < k_fullFreeSpaceSizeLimit;
            }

        private:
            static constexpr int k_fullFreeSpaceSizeLimit = sizeof(Ion::Storage::record_size_t)+Sprite::k_defaultSpriteNameMaxSize+k_spriteExtensionLength+256;

    };

}

#endif /* SPRITE_SCRIPT_STORE_H */
