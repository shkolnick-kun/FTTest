#include <test_func.h>
#include <sem.h>
#include <ipc.h>
#include <xprintf.h>

volatile char __attribute__ ((section (".padding_data"))) padding[0x1234] = {0};
volatile char __attribute__ ((section (".hello_data")))   hello[]  = "HELLO WORLD";

bgrt_proc_t proc[6];
bgrt_stack_t bgrt_proc_stack[6][BGRT_PROC_STACK_SIZE];

bgrt_ipc_t blink_ep;

static uint16_t color = RED;

void main_blink(void * arg)
{
    (void)arg;

    BGRT_PID_T wait_for;
    volatile bgrt_st_t st;

    while(1)
    {
        wait_for = BGRT_PID_NOTHING;

        st = bgrt_ipc_wait(&blink_ep, &wait_for, 1);
        while (BGRT_ST_OK != st);/*Panic*/

        blink_char(*(char *)blink_ep.msg, color);

        st = bgrt_ipc_reply(&blink_ep, wait_for);
        while (BGRT_ST_OK != st);/*Panic*/
    }
}

static void led_putc(char c)
{
    char buf;
    volatile bgrt_st_t st;

    buf = c;

    st = bgrt_ipc_send(&blink_ep, &buf);
    while (BGRT_ST_OK != st);/*Panic*/
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

    volatile bgrt_st_t st;

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

    xdev_out(led_putc);

    BGRT_PROC_RUN(PR1);

    BGRT_SYNC_SET_OWNER(&blink_ep, PID2);
    BGRT_PROC_RUN(PR2);

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

    bgrt_priv_proc_init(PR0, main_print,   SVH0, RSH0, 0, &bgrt_proc_stack[0][BGRT_PROC_STACK_SIZE-1], 3,      1, 0);
    bgrt_priv_proc_init(PR1, main_color,   SVH1, RSH1, 0, &bgrt_proc_stack[1][BGRT_PROC_STACK_SIZE-1], 2,      1, 0);
    bgrt_priv_proc_init(PR2, main_blink,   SVH2, RSH2, 0, &bgrt_proc_stack[2][BGRT_PROC_STACK_SIZE-1], 1,      1, 0);

    bgrt_ipc_init_cs(&blink_ep);

    bgrt_sem_init_cs(&produce, 1);
    bgrt_sem_init_cs(&consume, 0);

    bgrt_priv_proc_run(PR0);

    bgrt_start();
    return 0;
}
