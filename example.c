#include "example.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "serLCD.h"

void __perror(char *message, int errnum) {
    fprintf(stderr, "%s: [Errno %d] %s\n", message, errnum, strerror(errnum));
    exit(errnum);
}

/*
 *  M A I N
 */

int main(void) {
    /* define custom characters */
    uint8_t test_char[] = {0x15, 0x0A, 0x15, 0x0A, 0x15, 0x0A, 0x15, 0x00};
    uint8_t degree_sign[] = {0x07, 0x05, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00};

    int fd = 0;

    /* open i2c comms & set file descriptor */
    fd = i2c_connect(DEV_PATH, I2C_ADDRESS);
    if (fd < 0) __perror("i2c_connect error", errno);

    /* Initialise the system & validate the board */
    if (init(fd) < 0) __perror("initialise error", errno);

    /* configure the lcd & create custom characters */
    disable_system_messages(fd);
    disable_splash(fd);
    create_char(fd, 0, test_char);
    create_char(fd, 1, degree_sign);
    set_backlight(fd, 255, 255, 255);
    set_contrast(fd, 10);
    sleep(1);

    /* display a string */
    set_cursor(fd, 4, 0);
    print(fd, "Hello world!");

    /* display a formatted string */
    set_cursor(fd, 0, 1);
    print(fd, "Fmt  %d  %4.2f  0x%04X", 8, 3.145, 255);

    /* display custom characters with write_char */
    set_cursor(fd, 2, 2);
    write_char(fd, 0);
    write_char(fd, 0);
    write_char(fd, 0);

    /* display a custom character with octal escape code */
    set_cursor(fd, 9, 2);
    print(fd, "Temp 22\001C");

    /* run and display a clock */
    char buf[9];
    struct tm *tmNow;
    time_t now;
    for (;;) {
        now = time(NULL);
        while (time(NULL) == now) usleep(50000);

        now = time(NULL);
        tmNow = localtime(&now);
        strftime(buf, sizeof buf, "%H:%M:%S", tmNow);

        /*
         * print with error checking to test
         * longer term i2c comms stability
         */
        set_cursor(fd, 6, 3);
        if (print(fd, buf) < 0) __perror("print error", errno);
    }

    return 0;
}
