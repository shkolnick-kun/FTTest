#include <test_func.h>
#include <sem.h>
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

bgrt_sem_t produce;
bgrt_sem_t consume;

void main_color(void * arg)
{
    (void)arg;

    const uint16_t cl_lut[4] =
    {
        BLUE, GREEN, ORANGE, RED
    };

    uint8_t i = 0;

    while(1)
    {
        bgrt_sem_lock(&produce);
        color = cl_lut[i++];
        bgrt_sem_free(&consume);
        i %= 4;
    }
}

void main_print(void * arg)
{
    (void)arg;

    xdev_out(led_putc);

    BGRT_PROC_RUN(PR1);

    while(1)
    {
        bgrt_sem_lock(&consume);
        xprintf("%s", hello);
        bgrt_sem_free(&produce);
    }
}

int main(void)
{
    init_hardware();
    bgrt_init();

    bgrt_priv_proc_init(PR0, main_print,   SVH0, RSH0, 0, &bgrt_proc_stack[0][BGRT_PROC_STACK_SIZE-1], 1,      1, 0);
    bgrt_priv_proc_init(PR1, main_color,   SVH1, RSH1, 0, &bgrt_proc_stack[1][BGRT_PROC_STACK_SIZE-1], 2,      1, 0);

    bgrt_sem_init_cs(&produce, 1);
    bgrt_sem_init_cs(&consume, 0);

    bgrt_priv_proc_run(PR0);

    bgrt_start();
    return 0;
}
