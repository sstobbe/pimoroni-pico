
#include <string>
#include <cstring>

#define MP_OBJ_TO_PTR2(o, t) ((t *)(uintptr_t)(o))




extern "C" {
#include "picopower.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/xosc.h"
#include "hardware/gpio.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"
#include "hardware/structs/xosc.h"
#include "hardware/structs/rosc.h"
#include "hardware/structs/adc.h"
#include "hardware/structs/usb.h"
#include "hardware/structs/pads_qspi.h"
#include "hardware/structs/vreg_and_chip_reset.h"
#include "hardware/structs/psm.h"
#include "hardware/structs/syscfg.h"
#include "hardware/vreg.h"
#include "hardware/structs/sio.h"
static long long unsigned int alrm;

void Timer0ISR(uint alarm_num)
{
  alrm += 1000000;
  hardware_alarm_set_target(0, alrm);

  //sio_hw->gpio_togl = 1 << 25;
}


/***** Variables Struct *****/
typedef struct _picopower_PicoPower_obj_t {
    mp_obj_base_t base;
    int *breakout;
} picopower_PicoPower_obj_t;

/***** Print *****/
void PicoPower_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind; //Unused input parameter

    mp_print_str(print, ")");
}

/***** Constructor *****/
mp_obj_t PicoPower_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    picopower_PicoPower_obj_t *self = nullptr;
    self = m_new_obj(picopower_PicoPower_obj_t);
    self->base.type = &picopower_PicoPower_type;
    
  hardware_alarm_set_callback(0, Timer0ISR);  
  alrm = time_us_64() + 100000;
  hardware_alarm_set_target(0, alrm);
  
    hw_clear_bits(&(vreg_and_chip_reset_hw->bod),1);
  for(int i=0;i<=6;i++){
    hw_clear_bits(pads_qspi_hw->io+i, 0x0F);
  }

    return MP_OBJ_FROM_PTR(self);
}




mp_obj_t PicoPower_enable_timer(mp_obj_t self_in) {


    if( gpio_get(24) == 0) {
      
      }

    return mp_const_none;
}

mp_obj_t PicoPower_sleep(mp_obj_t self_in) {

  // only clock sleep with no usb conection
  // should eventually look at CDC status
  if( gpio_get(24) == 0) {
      clock_configure(clk_sys,CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_XOSC_CLKSRC  ,12 * MHZ,12 * MHZ);
      rosc_hw->ctrl = 0xfabfa5;
      pll_deinit(pll_sys);
      pll_deinit(pll_usb);
      clock_configure(clk_peri,0,CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,12 * MHZ,12 * MHZ);
      clock_configure(clk_adc,0,CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS,12 * MHZ,12 * MHZ);
      clocks_hw->sleep_en0 = 3<<21;
      clocks_hw->sleep_en1 = 1<<5;
      vreg_set_voltage(VREG_VOLTAGE_0_85);
      
      // set alaram 950 ms from now
      alrm = time_us_64() + 950000;
      hardware_alarm_set_target(0, alrm);
      // goto sleep
      __asm("wfi");
      
      vreg_set_voltage(VREG_VOLTAGE_1_20);
      clocks_hw->sleep_en0 = ~0;
      clocks_hw->sleep_en1 = ~0;
      
      clocks_init();
      /*
      pll_init(pll_sys, 1, 1500 * MHZ, 6, 2);
      pll_init(pll_usb, 1, 480 * MHZ, 5, 2);
    
    clock_configure(clk_ref,
                    CLOCKS_CLK_REF_CTRL_SRC_VALUE_XOSC_CLKSRC,
                    0, // No aux mux
                    12 * MHZ,
                    12 * MHZ);
    
    clock_configure(clk_usb,
                    0, // No GLMUX
                    CLOCKS_CLK_USB_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB,
                    48 * MHZ,
                    48 * MHZ);
    
    
    // CLK ADC = PLL USB (48MHZ) / 1 = 48MHz
    clock_configure(clk_adc,
                    0, // No GLMUX
                    CLOCKS_CLK_ADC_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB,
                    48 * MHZ,
                    48 * MHZ);
                    
    clock_configure(clk_sys,
                    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
                    CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
                    125 * MHZ,
                    125 * MHZ);
                    */
  }
  return mp_const_none;
}

}
