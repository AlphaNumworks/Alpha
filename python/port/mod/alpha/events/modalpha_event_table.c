#include "py/obj.h"
#include "modalpha_event.h"

static const mp_rom_map_elem_t modalpha_event_globals_table[] = {};

static MP_DEFINE_CONST_DICT(modalpha_event_globals, modalpha_event_globals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    modalpha_event_type,
    MP_QSTR_Event,
    MP_TYPE_FLAG_NONE,
    make_new,
    (const void *)modalpha_event_make_new,
    attr,
    (const void *)modalpha_event_attributes,
    locals_dict, &modalpha_event_globals
);
