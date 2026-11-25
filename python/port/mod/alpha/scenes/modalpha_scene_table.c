#include "modalpha_scene.h"
#include "py/obj.h"

static MP_DEFINE_CONST_FUN_OBJ_1(modalpha_scene_make_dirty_obj, modalpha_scene_make_dirty);
static MP_DEFINE_CONST_FUN_OBJ_1(modalpha_scene_mark_clean_obj, modalpha_scene_mark_clean);
static MP_DEFINE_CONST_FUN_OBJ_2(modalpha_scene_on_start_obj, modalpha_scene_on_start);
static MP_DEFINE_CONST_FUN_OBJ_2(modalpha_scene_on_dispose_obj, modalpha_scene_on_dispose);
static MP_DEFINE_CONST_FUN_OBJ_2(modalpha_scene_update_obj, modalpha_scene_update);
static MP_DEFINE_CONST_FUN_OBJ_1(modalpha_scene_render_obj, modalpha_scene_render);
static MP_DEFINE_CONST_FUN_OBJ_1(modalpha_scene_is_dirty_obj, modalpha_scene_is_dirty);

static const mp_rom_map_elem_t modalpha_scene_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR_mark_dirty), MP_ROM_PTR(&modalpha_scene_make_dirty_obj)},
    {MP_ROM_QSTR(MP_QSTR_mark_clean), MP_ROM_PTR(&modalpha_scene_mark_clean_obj)},
    {MP_ROM_QSTR(MP_QSTR_on_start), MP_ROM_PTR(&modalpha_scene_on_start_obj)},
    {MP_ROM_QSTR(MP_QSTR_on_dispose), MP_ROM_PTR(&modalpha_scene_on_dispose_obj)},
    {MP_ROM_QSTR(MP_QSTR_update), MP_ROM_PTR(&modalpha_scene_update_obj)},
    {MP_ROM_QSTR(MP_QSTR_render), MP_ROM_PTR(&modalpha_scene_render_obj)},
    {MP_ROM_QSTR(MP_QSTR_is_dirty), MP_ROM_PTR(&modalpha_scene_is_dirty_obj)},
};

static MP_DEFINE_CONST_DICT(modalpha_scene_globals, modalpha_scene_globals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    modalpha_scene_type,
    MP_QSTR_Scene,
    MP_TYPE_FLAG_NONE,
    make_new,
    modalpha_scene_make_new,
    locals_dict, &modalpha_scene_globals
);
