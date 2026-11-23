#ifndef SPRITE_SCRIPT_STORE_H
    #define SPRITE_SCRIPT_STORE_H
    #include <cstddef>

namespace Sprites
{

    class SpriteStore
    {

        public:
            static constexpr char k_spriteExtension[] = "spr";
            static constexpr size_t k_spriteExtensionLength = 3;

            static bool SpriteNameIsFree(const char * baseName);

            SpriteStore();
            // Sprite

    };

}

#endif /* SPRITE_SCRIPT_STORE_H */
