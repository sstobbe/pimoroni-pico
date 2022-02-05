#include "picopower.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// BreakoutRTC Class
////////////////////////////////////////////////////////////////////////////////////////////////////

/***** Methods *****/

MP_DEFINE_CONST_FUN_OBJ_1(PicoPower_enable_timer_obj, PicoPower_enable_timer);
MP_DEFINE_CONST_FUN_OBJ_1(PicoPower_sleep_obj, PicoPower_sleep);

/***** Binding of Methods *****/
STATIC const mp_rom_map_elem_t PicoPower_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_enable_timer), MP_ROM_PTR(&PicoPower_enable_timer_obj) },
    { MP_ROM_QSTR(MP_QSTR_sleep), MP_ROM_PTR(&PicoPower_sleep_obj) }
};
STATIC MP_DEFINE_CONST_DICT(PicoPower_locals_dict, PicoPower_locals_dict_table);

/***** Class Definition *****/
const mp_obj_type_t picopower_PicoPower_type = {
    { &mp_type_type },
    .name = MP_QSTR_picopower,
    .print = PicoPower_print,
    .make_new = PicoPower_make_new,
    .locals_dict = (mp_obj_dict_t*)&PicoPower_locals_dict,
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// breakout_rtc Module
////////////////////////////////////////////////////////////////////////////////////////////////////

/***** Globals Table *****/
STATIC const mp_map_elem_t picopower_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_picopower) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PicoPower), (mp_obj_t)&picopower_PicoPower_type },
};
STATIC MP_DEFINE_CONST_DICT(mp_module_picopower_globals, picopower_globals_table);

/***** Module Definition *****/
const mp_obj_module_t picopower_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_picopower_globals,
};

////////////////////////////////////////////////////////////////////////////////////////////////////
MP_REGISTER_MODULE(MP_QSTR_picopower, picopower_user_cmodule, MODULE_PICOPOWER_ENABLED);
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
