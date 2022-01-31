#include "serLCD.h"

#include <fcntl.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

static int __max(int num1, int num2) { return (num1 > num2) ? num1 : num2; }
static int __min(int num1, int num2) { return (num1 > num2) ? num2 : num1; }
static void __delay_milliseconds(unsigned howLong) { usleep(howLong * 1000); }

int __print(int fd, const char *str) {
    size_t i = 0;
    int ret = -1;

    for (i = 0; i < strlen(str); i++) {
        ret = i2c_smbus_write_byte(fd, str[i]);
        __delay_milliseconds(10);
        if (ret < 0) break;
    }

    return ret;
}

/*
 *  user callable functions below here
 */

int print(int fd, const char *format, ...) {
    char buf[81];
    va_list arg;

    va_start(arg, format);
    vsprintf(buf, format, arg);
    va_end(arg);

    return __print(fd, buf);
}

int i2c_connect(const char *dev_path, int addr) {
    int fd = -1;
    int ret = -1;

    fd = open(dev_path, O_RDWR);
    if (fd < 0) return fd;

    ret = ioctl(fd, I2C_SLAVE, addr);
    if (ret < 0) {
        close(fd);
        return ret;
    }

    return fd;
}

int init(int fd) {
    int ret = -1;

    ret =
        i2c_smbus_write_byte_data(fd, SPECIAL_COMMAND, LCD_DISPLAYCONTROL | DEFAULT_DISPLAYCONTROL);
    __delay_milliseconds(50);
    if (ret < 0) return ret;
    ret = i2c_smbus_write_byte_data(fd, SPECIAL_COMMAND, LCD_ENTRYMODESET | DEFAULT_DISPLAYMODE);
    __delay_milliseconds(50);
    if (ret < 0) return ret;

    return clear(fd);
}

int clear(int fd) {
    int ret = -1;

    ret = i2c_smbus_write_byte_data(fd, SETTING_COMMAND, CLEAR_COMMAND);
    __delay_milliseconds(10);

    return ret;
}

int set_backlight(int fd, uint8_t r, uint8_t g, uint8_t b) {
    int ret = -1;

    uint8_t values[4] = {SET_RGB_COMMAND, r, g, b};
    ret = i2c_smbus_write_i2c_block_data(fd, SETTING_COMMAND, sizeof(values), values);
    __delay_milliseconds(10);

    return ret;
}

int set_contrast(int fd, uint8_t contrast) {
    int ret = -1;

    uint8_t values[2] = {CONTRAST_COMMAND, contrast};
    ret = i2c_smbus_write_i2c_block_data(fd, SETTING_COMMAND, sizeof(values), values);
    __delay_milliseconds(10);

    return ret;
}

int enable_system_messages(int fd) {
    int ret = -1;

    ret = i2c_smbus_write_byte_data(fd, SETTING_COMMAND, ENABLE_SYSTEM_MESSAGE_DISPLAY);
    __delay_milliseconds(50);

    return ret;
}

int disable_system_messages(int fd) {
    int ret = -1;

    ret = i2c_smbus_write_byte_data(fd, SETTING_COMMAND, DISABLE_SYSTEM_MESSAGE_DISPLAY);
    __delay_milliseconds(50);

    return ret;
}

int cursor(int fd) {
    int ret = -1;
    uint8_t displayControl = DEFAULT_DISPLAYCONTROL;

    displayControl |= LCD_CURSORON;
    ret = i2c_smbus_write_byte_data(fd, SPECIAL_COMMAND, LCD_DISPLAYCONTROL | displayControl);
    __delay_milliseconds(50);

    return ret;
}

int no_cursor(int fd) {
    int ret = -1;
    uint8_t displayControl = DEFAULT_DISPLAYCONTROL;

    displayControl &= ~LCD_CURSORON;
    ret = i2c_smbus_write_byte_data(fd, SPECIAL_COMMAND, LCD_DISPLAYCONTROL | displayControl);
    __delay_milliseconds(50);

    return ret;
}

int blink(int fd) {
    int ret = -1;
    uint8_t displayControl = DEFAULT_DISPLAYCONTROL;

    displayControl |= LCD_BLINKON;
    ret = i2c_smbus_write_byte_data(fd, SPECIAL_COMMAND, LCD_DISPLAYCONTROL | displayControl);
    __delay_milliseconds(50);

    return ret;
}

int no_blink(int fd) {
    int ret = -1;
    uint8_t displayControl = DEFAULT_DISPLAYCONTROL;

    displayControl &= ~LCD_BLINKON;
    ret = i2c_smbus_write_byte_data(fd, SPECIAL_COMMAND, LCD_DISPLAYCONTROL | displayControl);
    __delay_milliseconds(50);

    return ret;
}

int set_cursor(int fd, int col, int row) {
    int ret = -1;

    uint8_t row_offsets[4] = {0x00, 0x40, 0x14, 0x54};
    row = __max(0, row);
    row = __min(row, MAX_ROWS - 1);
    ret =
        i2c_smbus_write_byte_data(fd, SPECIAL_COMMAND, LCD_SETDDRAMADDR | (col + row_offsets[row]));
    __delay_milliseconds(50);

    return ret;
}

int create_char(int fd, uint8_t location, uint8_t char_map[]) {
    size_t i = 0;
    int ret = -1;
    uint8_t arr[9];

    location &= 0x7;
    arr[0] = 27 + location;

    for (i = 1; i < 9; i++) {
        arr[i] = char_map[i - 1];
    }

    ret = i2c_smbus_write_i2c_block_data(fd, SETTING_COMMAND, 9, arr);
    __delay_milliseconds(10);

    return ret;
}

int write_char(int fd, uint8_t location) {
    int ret = -1;
    location &= 0x7;

    ret = i2c_smbus_write_byte_data(fd, SETTING_COMMAND, 35 + location);
    __delay_milliseconds(10);

    return ret;
}

int enable_splash(int fd) {
    int ret = -1;

    ret = i2c_smbus_write_byte_data(fd, SETTING_COMMAND, ENABLE_SPLASH_DISPLAY);
    __delay_milliseconds(50);

    return ret;
}

int disable_splash(int fd) {
    int ret = -1;

    ret = i2c_smbus_write_byte_data(fd, SETTING_COMMAND, DISABLE_SPLASH_DISPLAY);
    __delay_milliseconds(50);

    return ret;
}
