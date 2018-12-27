/*
 * SPI:
 * 		SYSCLK 						12 MHz
 * 		SPI Clock Frequency			2 MHz (maximum for Touch XPT2046)
 * LCD Pin I/O:
 * 		P1.0 - SPI SCK    			(digital output, push-pull)
 *		P1.2 - SPI MOSI   			(digital output, push-pull)
 * 		P1.3 - SPI DC				(0: command, 1: data)
 * 		P1.4 - SPI RST				(Low Enable)
 * 		P1.6 - SPI CS				(Low Active)
 * */

/**************************************************************************//**
* Includes
 *****************************************************************************/
#include <SI_EFM8UB2_Register_Enums.h>
#include "../ili9341/ILI9341.h"
#include "InitDevice.h"

/**************************************************************************//**
* Pin Initialization
 *****************************************************************************/
SI_SBIT (ILI9341_DC, 		p1, 3);
/* LCD Reset */
SI_SBIT (ILI9341_RST, 		p1, 4);
/* LCD CS */
SI_SBIT (ILI9341_CS, 		p1, 6);
/* LCD back light */
SI_SBIT (ILI9341_LED, 		p1, 7);

/**************************************************************************//**
* typedef struct {
*	uint16_t 	ILI9341_TFTWIDTH;		///< ILI9341 max TFT width
*	uint16_t 	ILI9341_TFTHEIGHT;		///< ILI9341 max TFT height
*	uint8_t 	DATA_COMMAND;			///< data/command parameter
* }LCD_PARAMS;
 *****************************************************************************/
LCD_PARAMS lcd_params;

//-----------------------------------------------------------------------------
// Support Routines
//-----------------------------------------------------------------------------
/**************************************************************************//**
* LCD Initialize
 *****************************************************************************/
void _ILI9341_Init() {
	ILI9341_LED = LOW_STATE;

	/*LCD Reset*/
//	ILI9341_RST = LOW_STATE;
//	for (i = 50; i > 0; i--);
	ILI9341_RST = HIGH_STATE;
//	for (i = 50; i > 0; i--);

#ifndef ILI9341_INIT
	#include "../ili9341/lcdinit.h"
#endif

	ILI9341_LED = HIGH_STATE;

}
/**************************************************************************//**
* setAddrWindow (uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
*****************************************************************************/
void setAddrWindow (uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	sendCommand(ILI9341_CASET);
	sendData(x0>>8);
	sendData(x0);
	sendData(x1>>8);
	sendData(x1);

	sendCommand(ILI9341_PASET);
	sendData(y0>>8);
	sendData(y0);
	sendData(y1>>8);
	sendData(y1);

	sendCommand(ILI9341_RAMWR);
}
/**************************************************************************//**
* fillScreen (uint16_t color)
* @param color 						-> fill color
*****************************************************************************/
void fillScreen (uint16_t color) {
    fillRect(0, 0,  lcd_params.ILI9341_TFTWIDTH, lcd_params.ILI9341_TFTHEIGHT, color);
}
/**************************************************************************//**
* fillRect (uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
* @param x, y 						-> Start coordinates
* @param w 							-> Width
* @param h 							-> Height
* @param color 						-> fill color
*****************************************************************************/
void fillRect (uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if ((x >= lcd_params.ILI9341_TFTWIDTH) || (y >= lcd_params.ILI9341_TFTHEIGHT))
        return;

    if((x + w - 1) >= lcd_params.ILI9341_TFTWIDTH)
        w = lcd_params.ILI9341_TFTWIDTH - x;

    if((y + h - 1) >= lcd_params.ILI9341_TFTHEIGHT)
        h = lcd_params.ILI9341_TFTHEIGHT - y;

    setAddrWindow(x, y, x+w-1, y+h-1);

    /*CS to 0*/
    ILI9341_CS = LOW_STATE;

    /*RS to 1 -> Data mode*/
    ILI9341_DC = HIGH_STATE;

    for(y = h; y > 0; y--) {
        for(x = w; x > 0; x--) {
            transmitBuf(color);
        }
    }

    /*CS to 1*/
    ILI9341_CS = HIGH_STATE;
}
/**************************************************************************//**
* setRotation(uint8_t orientation)
* @param orientation 				-> 0-3
*****************************************************************************/
void setRotation(uint8_t orientation) {
    uint8_t rotation = orientation % 4; // can't be higher than 3
    switch (rotation) {
    case 0:
        orientation = (MADCTL_MX | MADCTL_BGR);
        lcd_params.ILI9341_TFTWIDTH  = 240;
        lcd_params.ILI9341_TFTHEIGHT = 320;
        break;
    case 1:
    	orientation = (MADCTL_MV | MADCTL_BGR);
        lcd_params.ILI9341_TFTWIDTH  = 320;
        lcd_params.ILI9341_TFTHEIGHT = 240;
        break;
    case 2:
    	orientation = (MADCTL_MY | MADCTL_BGR);
        lcd_params.ILI9341_TFTWIDTH  = 240;
        lcd_params.ILI9341_TFTHEIGHT = 320;
        break;
    case 3:
    	orientation = (MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
        lcd_params.ILI9341_TFTWIDTH  = 320;
        lcd_params.ILI9341_TFTHEIGHT = 240;
        break;
    }

    sendCommand(ILI9341_MADCTL);
    sendData(orientation);
}
/**************************************************************************//**
 * writeChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor)
 *****************************************************************************/
void writeChar(uint16_t x, uint16_t y, char ch,
		FontDef font, uint16_t color, uint16_t bgcolor) {
    uint32_t i, b, j;

    setAddrWindow(x, y, x+font.width-1, y+font.height-1);

    /*DC to 1 -> Data mode*/
    ILI9341_DC = HIGH_STATE;

    /*CS to 0*/
    ILI9341_CS = LOW_STATE;

    for(i = 0; i < font.height; i++) {
        b = font.dt[(ch - 32) * font.height + i];
        for(j = 0; j < font.width; j++) {
            if((b << j) & 0x8000)  {
            	transmitBuf(color);
            } else {
            	transmitBuf(bgcolor);
            }
        }
    }

    /*CS to 1*/
    ILI9341_CS = HIGH_STATE;
}
/**************************************************************************//**
 * writeString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor)
 *****************************************************************************/
void writeString(uint16_t x, uint16_t y, const char* str,
		FontDef font, uint16_t color, uint16_t bgcolor) {

    while(*str) {
        if(x + font.width >= lcd_params.ILI9341_TFTWIDTH) {
            x = 0;
            y += font.height;
            if(y + font.height >= lcd_params.ILI9341_TFTHEIGHT) {
                break;
            }

            if(*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }

        writeChar(x, y, *str, font, color, bgcolor);
        x += font.width;
        str++;
    }
}
/**************************************************************************//**
* drawPixel(uint16_t x, uint16_t y, uint16_t color)
* @param x, y 						-> Pixel coordinates
* @param color 						-> pixel color
*****************************************************************************/
void drawPixel(uint16_t x, uint16_t y, uint16_t color) {
	if((x >= lcd_params.ILI9341_TFTWIDTH) || (y >= lcd_params.ILI9341_TFTHEIGHT))
        return;

    setAddrWindow(x, y, x+1, y+1);

    /*CS to 0*/
    ILI9341_CS = LOW_STATE;

    /*RS to 1 -> Data mode*/
    ILI9341_DC = HIGH_STATE;

    transmitBuf(color);

    /*CS to 1*/
    ILI9341_CS = HIGH_STATE;
}
/**************************************************************************//**
 * sendCommand(uint8_t cmd)
 * @param cmd 						-> Command
 * lcd_params.DATA_COMMAND = 0 		-> set to Command mode = 0
 *****************************************************************************/
void sendCommand(uint8_t cmd) {
	/* 0 -> Command mode */
	lcd_params.DATA_COMMAND = 0;
	transmit(cmd);
}
/**************************************************************************//**
 * sendData(uint8_t dt)
 * @param dt 						-> Data
 * lcd_params.DATA_COMMAND = 1 		-> set to Command mode = 1
 *****************************************************************************/
void sendData(uint8_t dt) {
	/* 1 -> Data mode */
	lcd_params.DATA_COMMAND = 1;
	transmit(dt);
}
/**************************************************************************//**
 * transmit(uint8_t param)
 * @param param 					-> Out
 *****************************************************************************/
void transmit(uint8_t param) {
	volatile uint32_t i;

	ILI9341_DC = lcd_params.DATA_COMMAND;
	/*CS to 0*/
	ILI9341_CS = LOW_STATE;

	SPI0DAT = param;
	while (!SPI0CN0_TXBMT);

	for (i = 1; i > 0; i--);

	/*CS to 1*/
	ILI9341_CS = HIGH_STATE;
}
/**************************************************************************//**
 * transmitBuf(uint8_t param)
 * @param color 					-> Transmit color
 *****************************************************************************/
void transmitBuf(uint16_t color) {
	volatile uint32_t i;

	uint8_t inputColor[2];
	inputColor[0] = (uint8_t)(color>>8);
	inputColor[1] = (uint8_t)(color & 0xFFUL);

	SPI0DAT = inputColor[0];
	while (!SPI0CN0_TXBMT);
	SPI0DAT = inputColor[1];

	for (i = 1; i > 0; i--);
}
