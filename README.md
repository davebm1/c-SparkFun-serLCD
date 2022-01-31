# SparkFun SerLCD - RGB Backlight (Qwiic)

## C language i2c demonstration code for the Raspberry Pi

Tested with: Raspberry Pi 4B / Raspbian GNU/Linux 11 (bullseye) with the [SparkFun 20x4 serLCD](https://www.sparkfun.com/products/16398).

This code is ported with reference to this [python script](https://github.com/shigeru-kawaguchi/Python-SparkFun-SerLCD) and this [Arduino library](https://github.com/sparkfun/SparkFun_SerLCD_Arduino_Library).

---

### Quick Start

1. edit DEV_PATH and I2C_ADDRESS in example.h as necessary
2. make
3. ./example
4. ctrl-c to terminate

---
This is C language demonstration code for the above liquid crystal display module. The included control functions are a minimum set to :-

1. display a plain string.
2. display a formatted string.
3. create and display custom characters.
4. provide a subset of basic control commands (function declarations in serLCD.h).

It is not a comprehensive port of the above repositories and connection is via i2c only. Displayed strings should be a maximum of 20 characters in length (for the 20x4 lcd) as overflow to the next line can be inconsistent. Control functions return 0 on success or a negative error number on failure.

No fancy scrolling, I'm afraid! I found the various scrolling functions too unpredictable, even with the official python driver.

__Please read the next two sections to avoid a couple of gotchas!__

---
Settings prerequisites:

1. sudo raspi-config --> interfacing options --> enable i2c
2. sudo apt install libi2c-dev i2c-tools
3. determine i2c busses with i2cdetect -l
4. determine SerLCD device location and id with i2cdetect -y \<bus number\>

Hardware prerequisites:

1. You may see "[Errno 121] Remote I/O error" with the default Raspberry Pi i2c baud rate settings. This can be resolved by running at a lower baud rate (10k), as opposed to the default baud rate (100k). If you need higher baud rates for other i2c devices, these can be connected to a separate i2c bus. e.g. /boot/config.txt settings of :-

    ```bash
    dtparam=i2c_arm=on,i2c_arm_baudrate=10000
    dtoverlay=i2c3,baudrate=400000
    ```

    with the serLCD on i2c-1 and other devices on i2c-3.
2. Put a 10k ohm pull down resistor on either one of the serLCD's RXI pins, to avoid random characters appearing on the display as a result of electrical noise pickup.
