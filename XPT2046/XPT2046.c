/*
 * SPI:
 * 		SYSCLK 						12 MHz
 * 		SPI Clock Frequency			2 MHz (maximum for Touch XPT2046)
 * LCD Pin I/O:
 * 		P1.0 - SPI SCK    			(digital output, push-pull)
 * 		P1.2 - SPI MOSI   			(digital output, push-pull)
 * 		P1.3 - SPI DC				(0: command, 1: data)
 * 		P1.4 - SPI RST				(Low Enable)
 * 		P1.6 - SPI CS				(Low Active)
 * 		P1.7 - BACK LED   			(LCD Back light)
 * Touch Pin I/O:
 * 		P2.1 - SPI T_IRQ			(Low Active)
 * 		P2.2 - SPI T_CS				(Low Active)
 * 		P1.1 - SPI T_DO				(MISO)
 * 		P1.2 - SPI T_DIN			(MOSI)
 * 		P1.0 - SPI T_CLK			(digital output, push-pull)
 * */

/**************************************************************************//**
* Includes
 *****************************************************************************/
#include <SI_EFM8UB2_Register_Enums.h>
#include "InitDevice.h"
#include "XPT2046.h"

/**************************************************************************//**
* Pin Initialization
 *****************************************************************************/
/*LCD IRQ*/
SI_SBIT (XPT2046_T_IRQ, 	SFR_P2, 1);
/*LCD CS*/
SI_SBIT (XPT2046_T_CS, 		SFR_P2, 2);

//-----------------------------------------------------------------------------
// Support Routines
//-----------------------------------------------------------------------------
/**************************************************************************//**
* Touch Initialize
 *****************************************************************************/
void _XPT2046_Init() {
	volatile uint32_t i;

	/*CS to 0*/
	XPT2046_T_CS = 0;

	SPI0DAT = INIT_ADC;
	while (!SPI0CN0_SPIF);
	SPI0DAT = 0x00;
	while (!SPI0CN0_SPIF);
	SPI0DAT = 0x00;

	for (i = 1; i > 0; i--);
	/*CS to 1*/
	XPT2046_T_CS = 1;
}

void isTouched() {
//	if(!XPT2046_T_IRQ) {
//		getTouchXY();
//	}
}
/**************************************************************************//**
* fillScreen (uint16_t color)
* @return touch_x 						-> coordinate with calibrate
*****************************************************************************/
uint16_t getX() {
	xdata uint16_t touch_x = 0;

	read(X_DATA);  				//Read X

	read(EMPTY); 				// High 8 bit
	touch_x = SPI0DAT;
	touch_x <<= 8;

	read(EMPTY); 				// Low 4 bit
	touch_x |= SPI0DAT;
	touch_x >>= 3;

	touch_x -= 300;
	touch_x = 240 - touch_x/((X_MAX - X_MIN) / 240);

	return touch_x;
}
/**************************************************************************//**
* fillScreen (uint16_t color)
* @return touch_y 						-> coordinate with calibrate
*****************************************************************************/
uint16_t getY() {
	xdata uint16_t touch_y = 0;

	read(Y_DATA);  				// Read Y

	read(EMPTY);  				// High 8 bit
	touch_y = SPI0DAT;
	touch_y <<= 8;

	read(EMPTY);  				// Low 4 bit
	touch_y |= SPI0DAT;
	touch_y >>= 3;

	touch_y -= 350;
	touch_y = 320 - touch_y/((Y_MAX - Y_MIN) / 320);

	return touch_y;
}
/**************************************************************************//**
* fillScreen (uint16_t color)
* @param dt 							-> Transmit data
*****************************************************************************/
void read(uint8_t dt) {
	volatile uint32_t i;

	/*CS to 0*/
	XPT2046_T_CS = 0;

	SPI0DAT = dt;
	while (!SPI0CN0_SPIF);

	for (i = 1; i > 0; i--);
	/*CS to 1*/
	XPT2046_T_CS = 1;
}
