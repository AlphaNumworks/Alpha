#include "sprite.h"
#include "apps/code/script.h"

namespace Sprites
{

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

    KDColor Sprite::getColorAt(size_t x, size_t y) const
    {
        Data d = this->value();

        size_t idx = (y * this->width() + x) * sizeof(uint16_t);

        if (idx >= d.size) {
            return KDColor();
        }
        return KDColor::RGB16(Ion::StorageHelper::unalignedShort((char *)d.buffer + idx));
    }

    void Sprite::setColorAt(size_t x, size_t y, KDColor color)
    {
        // NOTE: No need to append things to record, the space should already be reserved uppon creation 
        size_t idx = (y * this->width() + x) * sizeof(uint16_t);
        Data d = this->value();

        if (idx >= d.size) {
            // TODO: Return a status
            return;
        }

        uint16_t c = color;

        Ion::StorageHelper::writeUnalignedShort(c, (char *)d.buffer + idx);
    }

}
