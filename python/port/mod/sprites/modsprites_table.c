#include "modsprite.h"
#include "py/obj.h"

static MP_DEFINE_CONST_FUN_OBJ_3(modsprites_draw_sprite_obj, modsprites_draw_sprite);

static const mp_rom_map_elem_t modsprites_module_globals_table[] = {
  { MP_ROM_QSTR(MP_QSTR___name__),       MP_ROM_QSTR(MP_QSTR_sprites) },
  { MP_ROM_QSTR(MP_QSTR_draw_sprite),    MP_ROM_PTR(&modsprites_draw_sprite_obj) },
};

static MP_DEFINE_CONST_DICT(modsprites_module_globals, modsprites_module_globals_table);

const mp_obj_module_t modsprites_module = {
  .base = { &mp_type_module },
  .globals = (mp_obj_dict_t*)&modsprites_module_globals,
};
