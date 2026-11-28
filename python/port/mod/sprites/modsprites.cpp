#include "apps/sprites/store/sprite_store.hpp"
#include "apps/sprites/views/CanvaView.hpp"
#include "ion/display.h"
#include "kandinsky/ion_context.h"
#include "kandinsky/rect.h"
#include "port.h"
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
    // NOTE: memcpy into a KDColor get a 2 sprites cause sizeof(uint16_t) == 2
    Sprites::Sprite::Data d = s.value();

    // FIXME: One set px per pixel is a little bit slowww
    // FIXME: Constants and scale ??
    MicroPython::ExecutionEnvironment::currentExecutionEnvironment()->displaySandbox();
    for (size_t i = 0; i < 256; ++i) {
        KDIonContext::sharedContext()->setPixel(KDPoint(x + (i % 16), y + (i / 16)), Sprites::CanvaView::toKDColor(((uint8_t *)d.buffer)[i]));
    }

    return mp_const_none;
}
