#include <test_func.h>
#include <sem.h>
#include <ipc.h>
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

bgrt_proc_t proc[3];


/*
We have LED-server in main_blink(), clients have to
use L4-style IPC to communicate with it.

See https://en.wikipedia.org/wiki/L4_microkernel_family
*/

/*
IPC endpoint (rendezvous point).
*/
bgrt_ipc_t blink_ep;

static uint16_t color = RED;

/*
Server function.
*/
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

/*putc for xprintf (client function)*/
static void led_putc(char c)
{
    char buf;
    volatile bgrt_st_t st;

    buf = c;

    st = bgrt_ipc_send(&blink_ep, &buf);
    while (BGRT_ST_OK != st);/*Panic*/
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

    /*Bootstrap*/
    xdev_out(led_putc);

    BGRT_PROC_RUN(PR1);

    BGRT_SYNC_SET_OWNER(&blink_ep, PID2);
    BGRT_PROC_RUN(PR2);

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

    bgrt_priv_proc_init(PR0, main_print, SVH0, RSH0, 0, PROC_SSTART(0), 3, 1, 0);
    bgrt_priv_proc_init(PR1, main_color, SVH1, RSH1, 0, PROC_SSTART(1), 2, 1, 0);
    bgrt_priv_proc_init(PR2, main_blink, SVH2, RSH2, 0, PROC_SSTART(2), 1, 1, 0);

    bgrt_ipc_init_cs(&blink_ep);

    bgrt_sem_init_cs(&produce, 1);
    bgrt_sem_init_cs(&consume, 0);

    bgrt_priv_proc_run(PR0);

    bgrt_start();
    return 0;
}
