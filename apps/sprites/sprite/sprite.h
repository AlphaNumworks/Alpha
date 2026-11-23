#ifndef SPRITE_SPRITE_H
    #define SPRITE_SPRITE_H

    #include "ion/storage.h"
    #include "kandinsky/color.h"


/**
 * 
 * Record: | Size | Name |   Body  |
 * Sprite: | Size | Name | Content |
 * 
 * A sprite is 16 x 16 KDColors in a row
 * 
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
