#include <test_func.h>
#include <xprintf.h>

volatile char __attribute__ ((section (".padding_data"))) padding[0x1234] = {0};
volatile char __attribute__ ((section (".hello_data")))   hello[]  = "HELLO WORLD";

bgrt_proc_t proc[6];
bgrt_stack_t bgrt_proc_stack[6][BGRT_PROC_STACK_SIZE];

static uint16_t color = RED;

static void led_putc(char c)
{
    blink_char(c, color);
}

void main_with_return(void * arg)
{
    (void)arg;

    xdev_out(led_putc);
    xprintf("%s", hello);

    LED_ON(ORANGE);
    bgrt_wait_time(1000);
    LED_ON(GREEN);
    bgrt_wait_time(1000);
    LED_ON(BLUE);
    bgrt_wait_time(1000);
    LED_ON(RED);
}

int main(void)
{
    init_hardware();
    bgrt_init();

    bgrt_priv_proc_init(PR0, main_with_return,   SVH0, RSH0, 0, &bgrt_proc_stack[0][BGRT_PROC_STACK_SIZE-1], 1,      1, 0);

    bgrt_priv_proc_run(PR0);

    bgrt_start();
    return 0;
}
