# WUT? #
This is FT test project. Let's write this "Hello world!"

# Highligths #

 * IDE - Code::Blocks
 * Toolchain - GNU ARM None Eabi
 * Drivers - Libopencm3
 * Board stm43f4 discovery.

## Dirs ##
 * lib - for binaty libs.
 * src - all sorces are here.
   * bugurtos - BuruRTOS master version.
   * libopencm3 - libopencm3 driver lib.
   * xprintf - Elm by ChaN printf for MCUs.
   * test - project specific files are here.
 * projects - IDE-related files are here.
   * **easy** just blinks one LED and stops. Main is [here](./src/test/easy.c).
   * **medium** Blinks 4 LEDs consequently. The color is produced and consumed in different threads. Main is [here](./src/test/medium.c).
   * **hard** is like **medium** but leds are controled by separate server thread. Main is [here](./src/test/hard.c).

# TODO list #
 * ~~Easy mode~~
 * ~~Medium mode~~
 * ~~Hard mode~~
 * ~~Document~~


