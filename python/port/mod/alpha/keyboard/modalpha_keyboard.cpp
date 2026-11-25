#include "ion/keyboard.h"
extern "C" {
    #include "modalpha_keyboard.h"
    #include "py/runtime.h"
    #include "py/obj.h"
    #include "layout_keyboard.h"
    #include "mphalport.h"
}

typedef struct modalpha_keyboard_obj_s {

    mp_obj_base_t        base;
    Ion::Keyboard::State previous_scan;
    Ion::Keyboard::State current_scan;

} modalpha_keyboard_obj_t;

mp_obj_t modalpha_keyboard_make_new(
    const mp_obj_type_t *type,
    size_t               n_args,
    size_t               n_kw,
    const mp_obj_t      *args         
) {
    mp_arg_check_num(n_args, n_kw, 0, 0, false);

    modalpha_keyboard_obj_t *thus = mp_obj_malloc(modalpha_keyboard_obj_t, type);

    thus->previous_scan = Ion::Keyboard::State();
    thus->current_scan  = Ion::Keyboard::State();

    return MP_OBJ_FROM_PTR(thus);
}

// static uint64_t modalpha_keyboard_key_to_bit(
//     mp_obj_t key
// )
// {
//     mp_int_t k = mp_obj_get_int(key);
//     if (k < 0 || k > 63) {
//         return 1ULL;
//     }
//     return 1ULL << k;
// }

mp_obj_t modalpha_keyboard_release(
    mp_obj_t thus_obj,
    mp_obj_t key
)
{
    modalpha_keyboard_obj_t *thus = (modalpha_keyboard_obj_t*)MP_OBJ_TO_PTR(thus_obj);

    // bool is_release = (~(uint64_t)thus->current_scan & (uint64_t)thus->previous_scan & modalpha_keyboard_key_to_bit(key));
    bool is_release = !thus->current_scan.keyDown(static_cast<Ion::Keyboard::Key>(mp_obj_get_int(key))) && thus->previous_scan.keyDown(static_cast<Ion::Keyboard::Key>(mp_obj_get_int(key)));
    return is_release ? mp_const_true : mp_const_false;
}

mp_obj_t modalpha_keyboard_press(
    mp_obj_t thus_obj,
    mp_obj_t key
)
{
    modalpha_keyboard_obj_t *thus = (modalpha_keyboard_obj_t*)MP_OBJ_TO_PTR(thus_obj);

    // bool is_press = ((uint64_t)thus->current_scan & ~(uint64_t)thus->previous_scan & modalpha_keyboard_key_to_bit(key));
    bool is_press = thus->current_scan.keyDown(static_cast<Ion::Keyboard::Key>(mp_obj_get_int(key))) && !thus->previous_scan.keyDown(static_cast<Ion::Keyboard::Key>(mp_obj_get_int(key)));
    return is_press ? mp_const_true : mp_const_false;
}

mp_obj_t modalpha_keyboard_update(
    mp_obj_t thus_obj
)
{
    modalpha_keyboard_obj_t *thus = (modalpha_keyboard_obj_t*)MP_OBJ_TO_PTR(thus_obj);

    thus->previous_scan = thus->current_scan;
    thus->current_scan = Ion::Keyboard::scan();

    // NOTE: Do not rescan again
    if (thus->current_scan.keyDown(static_cast<Ion::Keyboard::Key>(mp_interrupt_char))) {
        mp_keyboard_interrupt();
    }

    return mp_const_none;
}
