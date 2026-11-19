#include "ion/timing.h"
#include "kandinsky/ion_context.h"
#include "port.h"
extern "C" {
    #include "mod/mu/events/bus/modmu_eventbus.h"
    #include "mod/mu/keyboard/modmu_keyboard.h"
    #include "helpers.h"
    #include "py/qstr.h"
    #include "py/misc.h"
    #include "py/gc.h"
    #include "modmu_app.h"
    #include "py/obj.h"
    #include "py/runtime.h"
    #include "mod/mu/scenemanager/modmu_scenemgr.h"
}

extern const mp_obj_type_t   modmu_scenemgr_type;
extern const mp_obj_type_t   modmu_keyboard_type;
extern const mp_obj_type_t   modmu_eventbus_type;

/**
 * @brief The App class basicaly
 */
typedef struct modmu_app_obj_s {

    mp_obj_base_t base;                  //!< The base, idk what is that for ...
    mp_obj_t      scenemgr;              //!< Scene manager bro
    mp_obj_t      keyboard;              //!< Keyboard
    mp_obj_t      events;                //!< Event bus
    bool          is_running = false;    //!< Is the app running
    bool          debug      = false;    //!< Is the app in debug mode
    int           tickrate   = 30;       //!< The desired tickrate
    float         frame_time = 1.0 / 30; //!< The tickrate

} modmu_app_obj_t;

mp_obj_t modmu_app_make_new(
    const mp_obj_type_t *type,
    size_t n_args,
    size_t n_kw,
    const mp_obj_t *args
)
{
    mp_arg_check_num(n_args, n_kw, 0, 1, false);

    modmu_app_obj_t* thus = mp_obj_malloc(modmu_app_obj_t, type);
    thus->is_running = false;
    thus->tickrate   = 30;
    thus->frame_time = 1.0 / 30;
    thus->debug     = (n_args > 0 && args[0] == mp_const_true);
    thus->scenemgr   = modmu_scenemgr_make_new(&modmu_scenemgr_type, 0, 0, NULL);
    thus->keyboard   = modmu_keyboard_make_new(&modmu_keyboard_type, 0, 0, NULL);
    thus->events     = modmu_eventbus_make_new(&modmu_eventbus_type, 0, 0, NULL);
    return MP_OBJ_FROM_PTR(thus);
}

void modmu_app_attributes(
    mp_obj_t thus_obj,
    qstr attribute,
    mp_obj_t *dest
) {
    modmu_app_obj_t *app = (modmu_app_obj_t *)MP_OBJ_TO_PTR(thus_obj);

    if (dest[0] != MP_OBJ_NULL) return;

    if (attribute == MP_QSTR_scenes) {
        dest[0] = app->scenemgr;
    } else if (attribute == MP_QSTR_keyboard) {
        dest[0] = app->keyboard;
    } else if (attribute == MP_QSTR_events) {
        dest[0] = app->events;
    } else {
        dest[1] = MP_OBJ_SENTINEL;
    }
}

mp_obj_t modmu_app_set_tickrate(
    mp_obj_t thus_obj,
    mp_obj_t tickrate
)
{
    modmu_app_obj_t *thus = (modmu_app_obj_t*)thus_obj;

    double new_tickrate = mp_obj_get_int(tickrate);

    if (new_tickrate <= 0) {
        return mp_obj_new_int(thus->tickrate);
    }

    thus->tickrate   = new_tickrate;
    thus->frame_time = 1.0 / new_tickrate;
    return mp_obj_new_int(thus->tickrate);
}

mp_obj_t modmu_app_run(
    mp_obj_t thus_obj
)
{
    modmu_app_obj_t* thus = (modmu_app_obj_t*)MP_OBJ_TO_PTR(thus_obj);
    mp_obj_t scene;
    mp_obj_t dest[3];
    double ts      = Ion::Timing::millis() / 1000.0;
    double last_ts = ts;

    thus->is_running = true;
    while (thus->is_running) {
        last_ts = ts;
        ts = Ion::Timing::millis() / 1000.0;

        modmu_keyboard_update(thus->keyboard);
        mp_handle_pending(true);

        scene = modmu_scenemgr_current(thus->scenemgr);

        if (scene == mp_const_none) break;

        mp_load_method(scene, MP_QSTR_update, dest);
        dest[2] = thus_obj;
        mp_call_method_n_kw(1, 0, dest);

        mp_load_method(scene, MP_QSTR_render, dest);
        mp_call_method_n_kw(0, 0, dest);
        
        if (thus->debug) {
            double dt = ts - last_ts;
            const KDFont *font = KDFont::SmallFont;
            VSTR_FIXED(fps_str, 24);
            vstr_printf(&fps_str, "FPS: %d / %d", dt > 0 ? (int)(1.0 / dt + 0.5) : 0, thus->tickrate);
            KDIonContext::sharedContext()->drawString(vstr_null_terminated_str(&fps_str), KDPoint(0, 0), font);

            gc_info_t gc;
            gc_info(&gc);
            VSTR_FIXED(gc_str, 32);
            vstr_printf(&gc_str, "GC: %dK / %dK (%d%%)", (int)(gc.used / 1024), (int)(gc.total / 1024), gc.total ? (int)(gc.used * 100 / gc.total) : 0);
            KDIonContext::sharedContext()->drawString(vstr_null_terminated_str(&gc_str), KDPoint(0, font->glyphSize().height()), font);

            VSTR_FIXED(current_scene_str, 32 + 8);
            vstr_printf(&current_scene_str, "Scene: %s", mp_obj_get_type_str(scene));
            KDIonContext::sharedContext()->drawString(vstr_null_terminated_str(&current_scene_str), KDPoint(0, font->glyphSize().height() * 2), font);
        }

        double elapsed = (Ion::Timing::millis() / 1000.0) - ts;
        double waiting = thus->frame_time - elapsed;
        if (waiting > 0) {
            // NOTE:: Previous function scaned again
            Ion::Timing::msleep(1000 * waiting);
        }
    }
    return mp_const_none;
}
