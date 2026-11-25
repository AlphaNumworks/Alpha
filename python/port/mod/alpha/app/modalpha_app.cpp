#include "ion/timing.h"
#include "kandinsky/ion_context.h"
#include "port.h"
extern "C" {
    #include "mod/alpha/events/bus/modalpha_eventbus.h"
    #include "mod/alpha/keyboard/modalpha_keyboard.h"
    #include "helpers.h"
    #include "py/qstr.h"
    #include "py/misc.h"
    #include "py/gc.h"
    #include "modalpha_app.h"
    #include "py/obj.h"
    #include "py/runtime.h"
    #include "mod/alpha/scenemanager/modalpha_scenemgr.h"
}

extern const mp_obj_type_t   modalpha_scenemgr_type;
extern const mp_obj_type_t   modalpha_keyboard_type;
extern const mp_obj_type_t   modalpha_eventbus_type;

/**
 * @brief The App class basicaly
 */
typedef struct modalpha_app_obj_s {

    mp_obj_base_t base;                  //!< The base, idk what is that for ...
    mp_obj_t      scenemgr;              //!< Scene manager bro
    mp_obj_t      keyboard;              //!< Keyboard
    mp_obj_t      events;                //!< Event bus
    bool          is_running = false;    //!< Is the app running
    bool          debug      = false;    //!< Is the app in debug mode
    int           tickrate   = 30;       //!< The desired tickrate
    float         frame_time = 1.0 / 30; //!< The tickrate

} modalpha_app_obj_t;

mp_obj_t modalpha_app_make_new(
    const mp_obj_type_t *type,
    size_t n_args,
    size_t n_kw,
    const mp_obj_t *args
)
{
    mp_arg_check_num(n_args, n_kw, 0, 1, false);

    modalpha_app_obj_t* thus = mp_obj_malloc(modalpha_app_obj_t, type);
    thus->is_running = false;
    thus->tickrate   = 30;
    thus->frame_time = 1.0 / 30;
    thus->debug     = (n_args > 0 && args[0] == mp_const_true);
    thus->scenemgr   = modalpha_scenemgr_make_new(&modalpha_scenemgr_type, 0, 0, NULL);
    thus->keyboard   = modalpha_keyboard_make_new(&modalpha_keyboard_type, 0, 0, NULL);
    thus->events     = modalpha_eventbus_make_new(&modalpha_eventbus_type, 0, 0, NULL);
    return MP_OBJ_FROM_PTR(thus);
}

void modalpha_app_attributes(
    mp_obj_t thus_obj,
    qstr attribute,
    mp_obj_t *dest
) {
    modalpha_app_obj_t *app = (modalpha_app_obj_t *)MP_OBJ_TO_PTR(thus_obj);

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

mp_obj_t modalpha_app_set_tickrate(
    mp_obj_t thus_obj,
    mp_obj_t tickrate
)
{
    modalpha_app_obj_t *thus = (modalpha_app_obj_t*)thus_obj;

    double new_tickrate = mp_obj_get_int(tickrate);

    if (new_tickrate <= 0) {
        return mp_obj_new_int(thus->tickrate);
    }

    thus->tickrate   = new_tickrate;
    thus->frame_time = 1.0 / new_tickrate;
    return mp_obj_new_int(thus->tickrate);
}

mp_obj_t modalpha_app_run(
    mp_obj_t thus_obj
)
{
    modalpha_app_obj_t* thus = (modalpha_app_obj_t*)MP_OBJ_TO_PTR(thus_obj);
    mp_obj_t scene;
    mp_obj_t dest[3];
    double ts      = Ion::Timing::millis() / 1000.0;
    double last_ts = ts;
    uint64_t t_update = 0, t_render = 0, worst = 0, worst_shown = 0;
    int frames = 0;

    thus->is_running = true;
    while (thus->is_running) {
        last_ts = ts;
        ts = Ion::Timing::millis() / 1000.0;

        modalpha_keyboard_update(thus->keyboard);
        mp_handle_pending(true);

        scene = modalpha_scenemgr_current(thus->scenemgr);

        if (scene == mp_const_none) break;

        uint64_t t0 = Ion::Timing::millis();
        mp_load_method(scene, MP_QSTR_update, dest);
        dest[2] = thus_obj;
        mp_call_method_n_kw(1, 0, dest);
        uint64_t t1 = Ion::Timing::millis();

        mp_load_method(scene, MP_QSTR_render, dest);
        mp_call_method_n_kw(0, 0, dest);
        uint64_t t2 = Ion::Timing::millis();

        if (thus->debug) {
            // NOTE: Compute times for render(), update() (current scene) and show worst frame
            t_update = t1 - t0;
            t_render = t2 - t1;
            if (t2 - t0 > worst) {
                worst = t2 - t0;
            }
            ++frames;
            if (frames >= 60) {
                worst_shown = worst;
                worst = 0;
                frames = 0;
            }

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
            vstr_printf(&current_scene_str, "Scene: %.*s", 32, mp_obj_get_type_str(scene));
            KDIonContext::sharedContext()->drawString(vstr_null_terminated_str(&current_scene_str), KDPoint(0, font->glyphSize().height() * 2), font);

            VSTR_FIXED(t_str, 32);
            vstr_printf(&t_str, "U:%dms R:%dms W:%dms", (int)t_update, (int)t_render, (int)worst_shown);
            KDIonContext::sharedContext()->drawString(vstr_null_terminated_str(&t_str), KDPoint(0, font->glyphSize().height() * 3), font);
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
