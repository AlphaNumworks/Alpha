#pragma once
#include "py/obj.h"

mp_obj_t modalpha_scenemgr_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);
mp_obj_t modalpha_scenemgr_push(size_t n_args, const mp_obj_t *args);
mp_obj_t modalpha_scenemgr_pop(size_t n_args, const mp_obj_t *args);
mp_obj_t modalpha_scenemgr_current(mp_obj_t thus_obj);
mp_obj_t modalpha_scenemgr_change(size_t n_args, const mp_obj_t *args);
