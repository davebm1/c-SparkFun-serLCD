#include <stdint.h>

/* Display */
#define MAX_ROWS 4
#define MAX_COLUMNS 20
/* Command Char */
#define SPECIAL_COMMAND 0xFE
#define SETTING_COMMAND 0x7C
/* Commands */
#define CLEAR_COMMAND 0x2D
#define CONTRAST_COMMAND 0x18
#define ADDRESS_COMMAND 0x19
#define SET_RGB_COMMAND 0x2B
#define ENABLE_SYSTEM_MESSAGE_DISPLAY 0x2E
#define DISABLE_SYSTEM_MESSAGE_DISPLAY 0x2F
#define ENABLE_SPLASH_DISPLAY 0x30
#define DISABLE_SPLASH_DISPLAY 0x31
#define SAVE_CURRENT_DISPLAY_AS_SPLASH 0x0A
/* Special Commands */
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_SETDDRAMADDR 0x80
/* Flags for display entry mode */
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00
/* Flags for display on/off control */
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00
/* Flags for display/cursor shift */
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

#define DEFAULT_DISPLAYCONTROL LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF
#define DEFAULT_DISPLAYMODE LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT

/* function declarations */
int i2c_connect(const char *dev_path, int addr);
int init(int fd);
int clear(int fd);
int set_backlight(int fd, uint8_t r, uint8_t g, uint8_t b);
int set_contrast(int fd, uint8_t contrast);
int enable_system_messages(int fd);
int disable_system_messages(int fd);
int cursor(int fd);
int no_cursor(int fd);
int blink(int fd);
int no_blink(int fd);
int print(int fd, const char *format, ...);
int set_cursor(int fd, int col, int row);
int create_char(int fd, uint8_t location, uint8_t char_map[]);
int write_char(int fd, uint8_t location);
int enable_splash(int fd);
int disable_splash(int fd);
