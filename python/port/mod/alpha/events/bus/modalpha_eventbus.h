#pragma once
#include "py/obj.h"

mp_obj_t modalpha_eventbus_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kwargs, const mp_obj_t *args);
mp_obj_t modalpha_eventbus_dispatch(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs);
mp_obj_t modalpha_eventbus_subscribe(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args);
mp_obj_t modalpha_sub_priority(mp_obj_t sub);
extern const mp_obj_fun_builtin_fixed_t modalpha_sub_priority_obj;
