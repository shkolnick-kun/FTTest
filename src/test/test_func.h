#include <bugurt.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

/* stm32f4-discovery LEDS */
#define GREEN  GPIO12
#define ORANGE GPIO13
#define RED    GPIO14
#define BLUE   GPIO15

#define LED_ON(CL)  gpio_set(GPIOD, CL)
#define LED_OFF(CL) gpio_clear(GPIOD, CL)

void init_hardware(void);

#define TM_UNIT (200)

#define TM_DOT     (TM_UNIT)
#define TM_DASH    (TM_UNIT*3)
#define TM_LETTER  (TM_UNIT)
#define TM_WORD    (TM_UNIT*2) /*Actually 3, but we have 1 at the end of dot/dash*/
#define TM_SPACE   (TM_UNIT*4) /*Actually 7, but we have 3 at the end of letter*/

void blink_letter(const char * sym, uint16_t cl);
void blink_char(char c, uint16_t cl);
