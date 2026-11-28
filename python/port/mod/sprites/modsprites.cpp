#include "apps/sprites/store/sprite_store.hpp"
#include "ion/display.h"
#include "kandinsky/rect.h"
extern "C" {
    #include "modsprite.h"
    #include <py/obj.h>
    #include <py/runtime.h>
}

#include <ion/storage.h>

mp_obj_t modsprites_draw_sprite(mp_obj_t nameo, mp_obj_t xo, mp_obj_t yo)
{
    size_t len;
    const char * name = mp_obj_str_get_data(nameo, &len);
    Sprites::Sprite s = Sprites::SpriteStore::SpriteNamed(name);

    if (s.isNull()) {
        mp_raise_OSError(2);
    }

    int x = mp_obj_get_int(xo);
    int y = mp_obj_get_int(yo);

    // TODO: Draw the sprite 16x16
    KDColor pixels[256];
    KDRect  rect(x, y, 16, 16);

    Ion::Display::pullRect(rect, pixels);

    // for (size_t i = 0; i < 256; ++i) {
        
    // }

    memcpy(pixels, s.value().buffer, 256);

    Ion::Display::pushRect(rect, pixels);

    return mp_const_none;
}
