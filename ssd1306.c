
// The ssd1306 will assume that every byte the follows a stream prefix is of
// that type.  For example, every byte from 0x40 to the stop bit is data. 
#define SSD1306_CMD_SINGLE  0x80
#define SSD1306_CMD_STREAM  0x00
#define SSD1306_DATA_STREAM 0x40

// Set contrast with following byte between 0x00 and 0xFF.
#define SSD1306_SET_CONTRAST   0x81

#define SSD1306_ENTIRE_ON_RAM    0xA4
#define SSD1306_ENTIRE_ON_NO_RAM 0xA5
#define SSD1306_NORMAL_DISP      0xA6
#define SSD1306_INVERT_DISP      0xA7
#define SSD1306_DISPLAY_SLEEP    0xAE
#define SSD1306_DISPLAY_ON       0xAF

#define SSD1306_LOW_COL_START(x)  (0x00|((x)&0x0F)
#define SSD1306_HIGH_COL_START(x) (0x01|((x)&0x0F))

// Set Addressing mode with following byte.
#define SSD1306_SET_ADDR_MODE   0x20
#define SSD1306_ADDR_MODE_HORZ  0x00
#define SSD1306_ADDR_MODE_VIRT  0x01
#define SSD1306_ADDR_MODE_PAGE  0x02

// Set column range with following two bytes.
// Values from 0x00 to 0x7f are valid.
#define SSD1306_SET_COL_ADDR    0x21

// Set page range with following two bytes.
// Values from 0x00 to 0x07
#define SSD1306_SET_PAGE_ADDR   0x22

#define SSD1306_PAGE_START(x)   (0xB0|((x)&0x07))

#define SSD1306_DISP_START_LINE(x) (0x40|((x)&0x3F))
#define SSD1306_SEG_REMAP(x)       (0xA0|((x)&0x01))

// Set the multiplex ratio with following byte
// Values range from 0x0F to 0x3F
#define SSD1306_SET_MPLEX       0xA8

#define SSD1306_COM_FORWARD     0xC0
#define SSD1306_COM_REVERSE     0xC8

// Set the display offset with the following byte
// Values range from 0x00 to 0x3F
#define SSD1306_DISP_OFFSET     0xD3

// Set the hardware comm configuration with following byte
// Values are 0x02, 0x12, 0x22, 0x32
#define SSD1306_COM_HW_CONF     0xDA

// Set the display clock ratio with following byte
// Values are 0x00 to 0xFF
#define SSD1306_CLOCK_DIVIDE   0xD5

// The charge pump is controlled with the following byte
#define SSD1306_CHARGE_PUMP    0x8D
#define SSD1306_PUMP_ON        0x14
#define SSD1306_PUMP_OFF       0x10

// Set the Vcomh deselect level with the following byte
#define SSD1306_DESELECT_LEVEL 0xDB
#define SSD1306_DESELECT_65VCC 0x00
#define SSD1306_DESELECT_77VCC 0x20
#define SSD1306_DESELECT_83VCC 0x30

unsigned char ssd1306_init_buf[] = {
	SSD1306_CMD_STREAM,
	SSD1306_SET_MPLEX,     0x3f,
	SSD1306_DISP_OFFSET,   0x00,
	SSD1306_DISP_START_LINE(0),
	SSD1306_SEG_REMAP(1),
	SSD1306_COM_REVERSE,
	SSD1306_COM_HW_CONF,   0x12,
	SSD1306_SET_CONTRAST,  0x7F,
	SSD1306_ENTIRE_ON_RAM,
	SSD1306_NORMAL_DISP,
	SSD1306_CLOCK_DIVIDE,  0x80,
	SSD1306_CHARGE_PUMP,   SSD1306_PUMP_ON,
	SSD1306_DISPLAY_ON,
	SSD1306_SET_COL_ADDR,  0x00, 0x7f,
	SSD1306_SET_PAGE_ADDR, 0x00, 0x07,
	SSD1306_SET_ADDR_MODE, SSD1306_ADDR_MODE_HORZ
};

void ssd1306_init()
{
}

