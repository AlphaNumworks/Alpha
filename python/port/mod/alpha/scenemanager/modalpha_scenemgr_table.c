#include "modalpha_scenemgr.h"
#include "py/obj.h"

static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(modalpha_scenemgr_push_obj, 2, 3, modalpha_scenemgr_push);
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(modalpha_scenemgr_change_obj, 2, 3, modalpha_scenemgr_change);
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(modalpha_scenemgr_pop_obj, 1, 2, modalpha_scenemgr_pop);
static MP_DEFINE_CONST_FUN_OBJ_1(modalpha_scenemgr_current_obj, modalpha_scenemgr_current);

static const mp_rom_map_elem_t modalpha_scenemgr_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR_push), MP_ROM_PTR(&modalpha_scenemgr_push_obj)},
        {MP_ROM_QSTR(MP_QSTR_pop), MP_ROM_PTR(&modalpha_scenemgr_pop_obj)},
    {MP_ROM_QSTR(MP_QSTR_current), MP_ROM_PTR(&modalpha_scenemgr_current_obj)},
    {MP_ROM_QSTR(MP_QSTR_change), MP_ROM_PTR(&modalpha_scenemgr_change_obj)},
};

static MP_DEFINE_CONST_DICT(modalpha_scenemgr_globals, modalpha_scenemgr_globals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    modalpha_scenemgr_type,
    MP_QSTR_SceneManager,
    MP_TYPE_FLAG_NONE,
    make_new,
    (const void *)modalpha_scenemgr_make_new,
    locals_dict, &modalpha_scenemgr_globals
);
