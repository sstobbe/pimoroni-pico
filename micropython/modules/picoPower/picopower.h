// Include MicroPython API.
#include "py/runtime.h"
//#include "py/objstr.h"

/***** Constants *****/

/***** Extern of Class Definition *****/
extern const mp_obj_type_t picopower_PicoPower_type;

/***** Extern of Class Methods *****/
extern mp_obj_t PicoPower_enable_timer(mp_obj_t self_in);
extern mp_obj_t PicoPower_sleep(mp_obj_t self_in);
extern mp_obj_t PicoPower_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args);
extern void PicoPower_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind);
