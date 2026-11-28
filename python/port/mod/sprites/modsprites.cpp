extern "C" {
    #include "modsprite.h"
    #include <py/runtime.h>
}

#include <ion/storage.h>

mp_obj_t modsprites_draw_sprite(mp_obj_t nameo, mp_obj_t xo, mp_obj_t yo)
{
    size_t len;
    const char * name = mp_obj_str_get_data(nameo, &len);
    Ion::Storage::Record r = Ion::Storage::sharedStorage()->recordNamed(name);

    if (r.isNull()) {
        mp_raise_OSError(2);
    }

    // TODO: Draw the sprite

    return mp_const_none;
}
