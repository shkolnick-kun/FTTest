# WUT? #
This is FT test project. Let's write this "Hello world!"

# Highligths #

 * IDE - [Code::Blocks](http://www.codeblocks.org/)
 * Toolchain - [GNU ARM None Eabi](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
 * Drivers - [libopencm3](https://github.com/libopencm3/libopencm3)
 * Board - [stm32f4discovery](https://www.st.com/en/evaluation-tools/stm32f4discovery.html).

## Dirs ##
 * [lib](./lib) - for binary libs.
 * [src](./src) - all sorces are here.
   * [bugurtos](./src/bugurtos) - BuruRTOS master version.
   * [libopencm3](./src/libopencm3) - libopencm3 driver lib.
   * [xprintf](./src/xprintf) - Elm by ChaN printf for MCUs.
   * [test](./src/test) - project specific files are here.
 * [projects](./projects) - IDE-related files are here.
   * **easy** just blinks one LED and stops. Main is [here](./src/test/easy.c).
   * **medium** Blinks 4 LEDs consequently. The color is produced and consumed in different threads. Main is [here](./src/test/medium.c).
   * **hard** is like **medium** but leds are controled by separate server thread. Main is [here](./src/test/hard.c).

# TODO list #
 * ~~Easy mode~~
 * ~~Medium mode~~
 * ~~Hard mode~~
 * ~~Document~~


