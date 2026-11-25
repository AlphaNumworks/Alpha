#ifndef SPRITE_SPRITE_H
    #define SPRITE_SPRITE_H

    #include "ion/storage.h"
    #include "kandinsky/color.h"


/**
 * 
 * Record: | Size | Name |                     Body                     |
 * Sprite: | Size | Name | RGBAColor1(uint8_t) ... RBGAColor16(uint8_t) |
 * 
 * A sprite is 16 x 16 RGBA2222 value (as uint8_t) in a row
 * So there are 2⁶ -> 64 differents colors + 2² -> 4 levels of transparancy : 0%, 33%, 67% and 100%
 * 
 * A sprite will likely take (2 + len of name + 256 bytes) -> ~270 bytes on over 65k (roughly 0.4% of shared storage)
 * So we can put at most ~200 sprites in the calculator (with the lock table).
 * 
 * However, there will be a limit of 50 sprites (~25% of shared storage)
 */

namespace Sprites
{

    class Sprite : public Ion::Storage::Record
    {
        private:
            static constexpr int k_maxNumberOfDefaultSpriteNames  = 99;
            static constexpr int k_defaultSpriteNameNumberMaxSize = 2;

            static constexpr size_t k_defaultSpriteWidth  = 16;
            static constexpr size_t k_defaultSpriteHeight = 16;

        public:
            // NOTE: 6 = strlen("sprite")
            // NOTE: 1 = '\0'
            static constexpr int k_defaultSpriteNameMaxSize = 6 + k_defaultSpriteNameNumberMaxSize + 1;

            Sprite(Ion::Storage::Record r = Ion::Storage::Record());

            const char* content(void) const;
            size_t      contentSize(void) const;

            size_t      width(void) const;
            size_t      height(void) const;

            KDColor     getColorAt(size_t x, size_t y) const;
            void        setColorAt(size_t x, size_t y, KDColor color);
    };

}

#endif /* SPRITE_SPRITE_H */
