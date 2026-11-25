extern "C" {
    #include "py/runtime.h"
    #include "modalpha_scene.h"
    #include <py/obj.h>
}

typedef struct modalpha_scene_obj_s {

    mp_obj_base_t base;
    bool          is_dirty;

} modalpha_scene_obj_t;

mp_obj_t modalpha_scene_make_new(
    const mp_obj_type_t *type,
    size_t n_args,
    size_t n_kw,
    const mp_obj_t *args
)
{
    mp_arg_check_num(n_args, n_kw, 0, 0, false);

    modalpha_scene_obj_t* thus = mp_obj_malloc(modalpha_scene_obj_t, type);
    thus->is_dirty = true;
    return MP_OBJ_FROM_PTR(thus);
}

mp_obj_t modalpha_scene_make_dirty(
    mp_obj_t thus_obj
)
{
    modalpha_scene_obj_t* thus = (modalpha_scene_obj_t*)MP_OBJ_TO_PTR(thus_obj);

    thus->is_dirty = true;
    return mp_const_none;
}

mp_obj_t modalpha_scene_is_dirty(
    mp_obj_t thus_obj
)
{
    modalpha_scene_obj_t *thus = (modalpha_scene_obj_t *)MP_OBJ_TO_PTR(thus_obj);
    return mp_obj_new_bool(thus->is_dirty);
}

mp_obj_t modalpha_scene_mark_clean(
    mp_obj_t thus_obj
)
{
    modalpha_scene_obj_t* thus = (modalpha_scene_obj_t*)MP_OBJ_TO_PTR(thus_obj);

    thus->is_dirty = false;
    return mp_const_none;
}

mp_obj_t modalpha_scene_update(
    mp_obj_t thus_obj,
    mp_obj_t app_obj
)
{
    return mp_const_none;
}

mp_obj_t modalpha_scene_render(
    mp_obj_t thus_obj
)
{
    return mp_const_none;
}

mp_obj_t modalpha_scene_on_start(
    mp_obj_t thus_obj,
    mp_obj_t app_obj
)
{
    return mp_const_none;
}

mp_obj_t modalpha_scene_on_dispose(
    mp_obj_t thus_obj,
    mp_obj_t app_obj
)
{
    // clear event bus of app
    return mp_const_none;
}
