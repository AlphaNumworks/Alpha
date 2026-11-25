#include "py/obj.h"
#include "modalpha_keyboard.h"

static MP_DEFINE_CONST_FUN_OBJ_2(modalpha_keyboard_press_obj, modalpha_keyboard_press);
static MP_DEFINE_CONST_FUN_OBJ_2(modalpha_keyboard_release_obj, modalpha_keyboard_release);
static MP_DEFINE_CONST_FUN_OBJ_1(modalpha_keyboard_update_obj, modalpha_keyboard_update);

static const mp_rom_map_elem_t modalpha_keyboard_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR_press), MP_ROM_PTR(&modalpha_keyboard_press_obj)},
    {MP_ROM_QSTR(MP_QSTR_release), MP_ROM_PTR(&modalpha_keyboard_release_obj)},
    {MP_ROM_QSTR(MP_QSTR_update), MP_ROM_PTR(&modalpha_keyboard_update_obj)},
};

static MP_DEFINE_CONST_DICT(modalpha_keyboard_globals, modalpha_keyboard_globals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    modalpha_keyboard_type,
    MP_QSTR_Keyboard,
    MP_TYPE_FLAG_NONE,
    make_new,
    modalpha_keyboard_make_new,
    locals_dict, &modalpha_keyboard_globals
);
