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
                memset(f, (uint8_t)(0xFF), 256);
                return Ion::Storage::sharedStorage()->createRecordWithFullName(".spr", f, 256, true);
            }

    };

}

#endif /* SPRITE_SCRIPT_STORE_H */
