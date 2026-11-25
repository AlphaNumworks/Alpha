#ifndef SPRITE_SCRIPT_STORE_H
    #define SPRITE_SCRIPT_STORE_H
    #include "apps/sprites/sprite/sprite.h"
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

            static Sprite SpriteNamed(const char * baseName) {
                return Sprite(Ion::Storage::sharedStorage()->recordBaseNamedWithExtension(baseName, k_spriteExtension));
            }

            static bool SpriteNameIsFree(const char * baseName) {
                return SpriteNamed(baseName).isNull();
            }

            static size_t NumberOfSprites(void) {
                return Ion::Storage::sharedStorage()->numberOfRecordsWithExtension(k_spriteExtension);
            }

            static void DeleteAllSprites() {
                for (size_t i = NumberOfSprites(); i > 0; --i) {
                    Ion::Storage::sharedStorage()->recordWithExtensionAtIndex(k_spriteExtension, i).destroy();
                }
            }

            static void AddSprite(const char * name)
            {
                size_t size = (strlen(name) + 1 + k_spriteExtensionLength) + sizeof(uint16_t) + (sizeof(uint8_t) * 256);

                if (size >= Ion::Storage::sharedStorage()->availableSize()) {
                    return;
                }
                // FIXME: Is there a proper method to do thisss
                uint8_t f[256] = {};
                Ion::Storage::sharedStorage()->createRecordWithExtension(name, k_spriteExtension, f, 256, true);
            }

    };

}

#endif /* SPRITE_SCRIPT_STORE_H */
