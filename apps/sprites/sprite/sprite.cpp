#include "sprite.h"
#include "apps/code/script.h"
#include "apps/sprites/store/sprite_store.hpp"

namespace Sprites
{

    static inline void intToText(int i, char * buffer, int bufferSize) {
        // We only support integers from 0 to 99.
        assert(i >= 0);
        assert(i < 100);
        assert(bufferSize >= 3);
        if (i/10 == 0) {
            buffer[0] = i+'0';
            buffer[1] = 0;
            return;
        }
        buffer[0] = i/10+'0';
        buffer[1] = i-10*(i/10)+'0';
        buffer[2] = 0;
    }

    bool Sprite::DefaultName(char buffer[], size_t bufferSize)
    {
        assert(bufferSize >= k_defaultSpriteNameMaxSize);
        static constexpr char defaultScriptName[] = "sprite";
        static constexpr int defaultScriptNameLength = 6;
        strlcpy(buffer, defaultScriptName, bufferSize);

        int currentScriptNumber = 1;
        while (currentScriptNumber <= k_maxNumberOfDefaultSpriteNames) {
            intToText(currentScriptNumber, &buffer[defaultScriptNameLength], bufferSize - defaultScriptNameLength );
            if (SpriteStore::SpriteNameIsFree(buffer)) {
                return true;
            }
            currentScriptNumber++;
        }
        return false;
    }

    bool Sprite::nameCompliant(const char * name)
    {
        /* We allow here the empty script name ".py", because it is the name used to
        * create a new empty script. When naming or renaming a script, we check
        * elsewhere that the name is no longer empty.
        * The name format is ([a-z_][a-z0-9_]*)*\.py
        *
        * We do not allow upper cases in the script names because script names are
        * used in the URLs of the NumWorks workshop website and we do not want
        * problems with case sensitivity. */
        UTF8Decoder decoder(name);
        CodePoint c = decoder.nextCodePoint();
        if (c == UCodePointNull || !(c.isLatinSmallLetter() || c == '_' || c == '.')) {
            /* The name cannot be empty. Its first letter must be in [a-z_] or the
            * extension dot. */
            return false;
        }
        while (c != UCodePointNull) {
            if (c == '.' && strcmp(decoder.stringPosition(), SpriteStore::k_spriteExtension) == 0) {
                return true;
            }
            if (!(c.isLatinSmallLetter() || c == '_' || c.isDecimalDigit())) {
                return false;
            }
            c = decoder.nextCodePoint();
        }
        return false;
    }

    Sprite::Sprite(
        Ion::Storage::Record r
    ) : Record(r)
    {
        // Init aha how funny
    }

    const char * Sprite::content(void) const
    {
        Data d = this->value();

        // NOTE: No status for now
        return (const char *)d.buffer;
    }

    size_t Sprite::contentSize(void) const
    {
        return this->value().size;
    }

    size_t Sprite::width(void) const
    {
        return k_defaultSpriteWidth;
    }

    size_t Sprite::height(void) const
    {
        return k_defaultSpriteHeight;
    }

    uint8_t Sprite::getColorAt(size_t x, size_t y) const
    {
        Data d = this->value();

        size_t idx = (y * this->width() + x) * sizeof(uint8_t);

        if (idx >= d.size) {
            return KDColor();
        }
        return *(uint8_t *)((char *)d.buffer + idx);
    }

    void Sprite::setColorAt(size_t x, size_t y, uint8_t color)
    {
        // NOTE: No need to append things to record, the space should already be reserved uppon creation 
        size_t idx = (y * this->width() + x) * sizeof(uint8_t);
        Data d = this->value();

        if (idx >= d.size) {
            return;
        }

        *(uint8_t *)((char *)d.buffer + idx) = color;
    }

}
