#include "modalpha_eventbus.h"
#include "py/obj.h"
#include "py/qstr.h"

static MP_DEFINE_CONST_FUN_OBJ_KW(modalpha_eventbus_dispatch_obj, 2, modalpha_eventbus_dispatch);
static MP_DEFINE_CONST_FUN_OBJ_KW(modalpha_eventbus_subscribe_obj, 2, modalpha_eventbus_subscribe);

MP_DEFINE_CONST_FUN_OBJ_1(modalpha_sub_priority_obj, modalpha_sub_priority);

static const mp_rom_map_elem_t modalpha_eventbus_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR_dispatch), MP_ROM_PTR(&modalpha_eventbus_dispatch_obj)},
    {MP_ROM_QSTR(MP_QSTR_subscribe), MP_ROM_PTR(&modalpha_eventbus_subscribe_obj)},
};

static MP_DEFINE_CONST_DICT(modalpha_eventbus_globals, modalpha_eventbus_globals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    modalpha_eventbus_type,
    MP_QSTR_EventBus,
    MP_TYPE_FLAG_NONE,
    make_new,
    (const void *)modalpha_eventbus_make_new,
    locals_dict, &modalpha_eventbus_globals
);
