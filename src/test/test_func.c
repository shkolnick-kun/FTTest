#include <test_func.h>

void HardFault_Handler(void)
{
    while (1)
    {

    }
}

void init_hardware(void)
{
    __asm__ __volatile__ ("cpsid i \n\t");
    rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
    /* Enable GPIOC clock. */
    rcc_periph_clock_enable(RCC_GPIOD);
    /* Set GPIO12 (in GPIO port C) to 'output push-pull'. */
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GREEN | RED | BLUE | ORANGE);
    gpio_clear(GPIOD, GREEN);
    gpio_clear(GPIOD, RED);
    gpio_clear(GPIOD, BLUE);
    gpio_clear(GPIOD, ORANGE);
}

static inline void blink(bgrt_tmr_t tm, uint16_t cl)
{
    LED_ON(cl);
    bgrt_wait_time(tm);
    LED_OFF(cl);
}

void blink_letter(const char * sym, uint16_t cl)
{
    do {
        switch(*sym)
        {
        case '.':
        {
            blink(TM_DOT, cl);
            break;
        }
        case '-':
        {
            blink(TM_DASH, cl);
            break;
        }
        case 0:
        {
            break;
        }
        default:
        {
            while(1);/*WHOOOOOPSS!!!*/
            return;
        }
        }
        bgrt_wait_time(TM_LETTER);
    } while (*sym++);
    bgrt_wait_time(TM_WORD);
}

void blink_char(char c, uint16_t cl)
{
    switch(c)
    {
    case 'A':
    case 'a':
    {
        blink_letter(".-", cl);
        break;
    }
    case 'B':
    case 'b':
    {
        blink_letter("-...", cl);
        break;
    }
    case 'C':
    case 'c':
    {
        blink_letter("-.-.", cl);
        break;
    }
    case 'D':
    case 'd':
    {
        blink_letter("-..", cl);
        break;
    }
    case 'E':
    case 'e':
    {
        blink_letter(".", cl);
        break;
    }
    case 'F':
    case 'f':
    {
        blink_letter("..-.", cl);
        break;
    }
    case 'G':
    case 'g':
    {
        blink_letter("--.", cl);
        break;
    }
    case 'H':
    case 'h':
    {
        blink_letter("....", cl);
        break;
    }
    case 'I':
    case 'i':
    {
        blink_letter("..", cl);
        break;
    }
    case 'J':
    case 'j':
    {
        blink_letter(".---", cl);
        break;
    }
    case 'K':
    case 'k':
    {
        blink_letter("-.-", cl);
        break;
    }
    case 'L':
    case 'l':
    {
        blink_letter(".-..", cl);
        break;
    }
    case 'M':
    case 'm':
    {
        blink_letter("--", cl);
        break;
    }
    case 'N':
    case 'n':
    {
        blink_letter("-.", cl);
        break;
    }
    case 'O':
    case 'o':
    {
        blink_letter("---", cl);
        break;
    }
    case 'P':
    case 'p':
    {
        blink_letter(".--.", cl);
        break;
    }
    case 'Q':
    case 'q':
    {
        blink_letter("--.-", cl);
        break;
    }
    case 'R':
    case 'r':
    {
        blink_letter(".-.", cl);
        break;
    }
    case 'S':
    case 's':
    {
        blink_letter("...", cl);
        break;
    }
    case 'T':
    case 't':
    {
        blink_letter("-", cl);
        break;
    }
    case 'U':
    case 'u':
    {
        blink_letter("..-", cl);
        break;
    }
    case 'V':
    case 'v':
    {
        blink_letter("...-", cl);
        break;
    }
    case 'W':
    case 'w':
    {
        blink_letter(".--", cl);
        break;
    }
    case 'X':
    case 'x':
    {
        blink_letter("-..-", cl);
        break;
    }
    case 'Y':
    case 'y':
    {
        blink_letter("-.--", cl);
        break;
    }
    case 'Z':
    case 'z':
    {
        blink_letter("--..", cl);
        break;
    }
    case '1':
    {
        blink_letter(".----", cl);
        break;
    }
    case '2':
    {
        blink_letter("..---", cl);
        break;
    }
    case '3':
    {
        blink_letter("...--", cl);
        break;
    }
    case '4':
    {
        blink_letter("....-", cl);
        break;
    }
    case '5':
    {
        blink_letter(".....", cl);
        break;
    }
    case '6':
    {
        blink_letter("-....", cl);
        break;
    }
    case '7':
    {
        blink_letter("--...", cl);
        break;
    }
    case '8':
    {
        blink_letter("---..", cl);
        break;
    }
    case '9':
    {
        blink_letter("----.", cl);
        break;
    }
    case '0':
    {
        blink_letter("-----", cl);
        break;
    }
    case ' ':
    {
        bgrt_wait_time(TM_SPACE);
        break;
    }
    default:
    {
        while(1);/*WHOOOOOPSS!!!*/
        return;
    }
    }
}
