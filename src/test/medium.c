#include <test_func.h>
#include <sem.h>
#include <xprintf.h>

/*
Memory map:
0x20000000     0x20001234   0x20001234+sizeof(hello[])
|.padding_data |.hello_data |actual .data              |.bss |system stack|
*/
volatile char __attribute__ ((section (".padding_data"))) padding[0x1234] = {0};
volatile char __attribute__ ((section (".hello_data")))   hello[]  = "HELLO WORLD";

/*
Will place process stacks to the padding area.

The padding area starts at 0x20000000, so it has proper alignment.

We may have 3 processes or less, so we don't have to worry
about the padding area boundaries as 0x200*3=0x600 < 0x1234.
*/
#define PROC_SSTART(n)  (((bgrt_stack_t *)padding) + BGRT_PROC_STACK_SIZE*(n+1) - 1)

bgrt_proc_t proc[2];

static uint16_t color = RED;

static void led_putc(char c)
{
    blink_char(c, color);
}

/*
The color of a LED will be produced in the main_color()

The main_print() will consume the color.

See https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem
*/
bgrt_sem_t produce;
bgrt_sem_t consume;

void main_color(void * arg)
{
    (void)arg;

    volatile bgrt_st_t st;

    const uint16_t cl_lut[4] =
    {
        BLUE, GREEN, ORANGE, RED
    };

    uint8_t i = 0;

    while(1)
    {
        st = bgrt_sem_lock(&produce);
        while (BGRT_ST_OK != st);/*Panic*/

        color = cl_lut[i++];

        st = bgrt_sem_free(&consume);
        while (BGRT_ST_OK != st);/*Panic*/

        i %= 4;
    }
}

void main_print(void * arg)
{
    (void)arg;

    volatile bgrt_st_t st;

    /*Bootstrap*/
    xdev_out(led_putc);

    BGRT_PROC_RUN(PR1);

    /*Work*/
    while(1)
    {
        st = bgrt_sem_lock(&consume);
        while (BGRT_ST_OK != st);/*Panic*/

        xprintf("%s", hello);

        st = bgrt_sem_free(&produce);
        while (BGRT_ST_OK != st);/*Panic*/
    }
}

int main(void)
{
    init_hardware();
    bgrt_init();

    bgrt_priv_proc_init(PR0, main_print, SVH0, RSH0, 0, PROC_SSTART(0), 1, 1, 0);
    bgrt_priv_proc_init(PR1, main_color, SVH1, RSH1, 0, PROC_SSTART(1), 2, 1, 0);

    bgrt_sem_init_cs(&produce, 1);
    bgrt_sem_init_cs(&consume, 0);

    bgrt_priv_proc_run(PR0);

    bgrt_start();
    return 0;
}
