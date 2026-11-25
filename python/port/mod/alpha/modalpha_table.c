#include "modalpha.h"
#include "py/obj.h"
#include "py/qstr.h"

extern const mp_obj_type_t   modalpha_app_type;
extern const mp_obj_type_t   modalpha_scene_type;
extern const mp_obj_type_t   modalpha_scenemgr_type;
extern const mp_obj_type_t   modalpha_keyboard_type;
extern const mp_obj_type_t   modalpha_event_type;
extern const mp_obj_type_t   modalpha_eventbus_type;

static MP_DEFINE_CONST_FUN_OBJ_0(modalpha_key___init___obj, modalpha___init__);

static const mp_rom_map_elem_t modalpha_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_alpha)},
    {MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&modalpha_key___init___obj)},

    {MP_ROM_QSTR(MP_QSTR_App), MP_ROM_PTR(&modalpha_app_type) },
    {MP_ROM_QSTR(MP_QSTR_Scene), MP_ROM_PTR(&modalpha_scene_type)},
    {MP_ROM_QSTR(MP_QSTR_SceneManager), MP_ROM_PTR(&modalpha_scenemgr_type)},
    {MP_ROM_QSTR(MP_QSTR_Keyboard), MP_ROM_PTR(&modalpha_keyboard_type)},
    {MP_ROM_QSTR(MP_QSTR_Event), MP_ROM_PTR(&modalpha_event_type)},
    {MP_ROM_QSTR(MP_QSTR_EventBus), MP_ROM_PTR(&modalpha_eventbus_type)},
};

static MP_DEFINE_CONST_DICT(modalpha_module_globals, modalpha_module_globals_table);

const mp_obj_module_t modalpha_module = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t*)&modalpha_module_globals
};
