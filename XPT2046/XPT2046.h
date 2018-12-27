#ifndef INC_XPT2046_H_
#define INC_XPT2046_H_

#include <SI_EFM8UB2_Register_Enums.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum CALIBRATE {
	X_MAX		= 3900,
	X_MIN		= 400,
	Y_MAX		= 3900,
	Y_MIN		= 300
} CALIBRATE;

#define	INIT_ADC 			0x80

#define	Y_DATA 				0x90
#define	X_DATA 				0xD0
#define	EMPTY 				0x00

void _XPT2046_Init();
void isTouched();
uint16_t getX();
uint16_t getY();
void read(uint8_t dt);

#endif /* INC_XPT2046_H_ */
