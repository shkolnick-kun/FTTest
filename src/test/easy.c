#include <test_func.h>
#include <xprintf.h>

/*
Memory map:
0x20000000     0x20001234   0x20001234+sizeof(hello[])
|.padding_data |.hello_data |actual .data              |.bss |system stack|
*/
volatile char __attribute__ ((section (".padding_data"))) padding[0x1234] = {0};
volatile char __attribute__ ((section (".hello_data")))   hello[]  = "HELLO WORLD";

bgrt_proc_t proc[1];
/*
Will place process stacks to the padding area.

The padding area starts at 0x20000000, so it has proper alignment.

We may have 3 processes or less, so we don't have to worry
about the padding area boundaries as 0x200*3=0x600 < 0x1234.
*/
#define PROC_SSTART(n)  (((bgrt_stack_t *)padding) + BGRT_PROC_STACK_SIZE*(n+1) - 1)

static uint16_t color = RED;

/*putc for xprintf*/
static void led_putc(char c)
{
    blink_char(c, color);
}

static void main_print(void * arg)
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

    bgrt_priv_proc_init(PR0, main_print, SVH0, RSH0, 0, PROC_SSTART(0), 1, 1, 0);

    bgrt_priv_proc_run(PR0);

    bgrt_start();
    return 0;
}
