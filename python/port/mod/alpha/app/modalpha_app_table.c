#include "modalpha_app.h"
#include "py/obj.h"

static MP_DEFINE_CONST_FUN_OBJ_1(modalpha_app_run_obj, modalpha_app_run);
static MP_DEFINE_CONST_FUN_OBJ_2(modalpha_app_set_tickrate_obj, modalpha_app_set_tickrate);

static const mp_rom_map_elem_t modalpha_app_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR_run), MP_ROM_PTR(&modalpha_app_run_obj)},
    {MP_ROM_QSTR(MP_QSTR_set_tickrate), MP_ROM_PTR(&modalpha_app_set_tickrate_obj)}
};

static MP_DEFINE_CONST_DICT(modalpha_app_globals, modalpha_app_globals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    modalpha_app_type,
    MP_QSTR_App,
    MP_TYPE_FLAG_NONE,
    make_new,
    (const void *)modalpha_app_make_new,
    attr,
    (const void *)modalpha_app_attributes,
    locals_dict, &modalpha_app_globals
);
